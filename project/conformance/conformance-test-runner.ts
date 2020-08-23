if (require.main === module) {
  require('tsconfig-paths').register()
}

import { conformanceTests } from './conformance-test-list'
import { serverStarted, startServer, stopServer } from './conformance-test-server'
import puppeteer = require('puppeteer')
import chalk = require('chalk')

const noop = () => undefined

export async function runConformanceTests({ headless = true, testTimeout = 12000 } = {}): Promise<boolean> {
  const ownsServer = !serverStarted()
  let browser: puppeteer.Browser | null = null
  try {
    if (ownsServer) {
      await startServer()
    }

    browser = await puppeteer.launch({ headless: false, args: headless ? ['--mute-audio'] : undefined })

    const pagesWeakMap = new WeakMap<puppeteer.Page, { testEnded?: () => void }>()

    for (const test of conformanceTests) {
      let page = (await browser.pages())[0]
      if (page) {
        await page.goto('about:blank')
      } else {
        page = await browser.newPage()
      }

      let wm = pagesWeakMap.get(page)
      if (!wm) {
        wm = { testEnded: noop }
        pagesWeakMap.set(page, wm)
        await page.exposeFunction('__sp_automated_test_end', () => {
          if (wm && wm.testEnded) {
            wm.testEnded()
          }
        })
      }

      try {
        const endedPromise = new Promise((resolve, reject) => {
          const timeoutTimer = setTimeout(() => {
            reject(new Error('Test timed out'))
          }, testTimeout || 12000)
          wm!.testEnded = () => {
            clearTimeout(timeoutTimer)
            resolve()
          }
        })

        await page.goto(test.url)

        await endedPromise
      } catch (error) {
        console.error(chalk.redBright(test.url), error)
        const errorMsg = `${error}`
        if (!test.errors.includes(errorMsg)) {
          test.errors.push(errorMsg)
        }
        await page.close().catch(() => {})
      }
    }

    for (let repeat = 0; repeat < 20; ++repeat) {
      let allFinished = true
      for (const suite of conformanceTests) {
        if (suite.served && !suite.finished) {
          allFinished = false
          break
        }
      }
      if (allFinished) {
        break
      }
      await new Promise((resolve) => {
        setTimeout(resolve, 25)
      })
    }

    let failedSuites = 0
    for (const suite of conformanceTests) {
      if (!suite.finished || suite.errors.length > 0) {
        ++failedSuites
      }
    }
    console.log()
    if (failedSuites) {
      console.log(
        chalk.redBright(
          `CONFORMANCE TEST FAILED WITH ${failedSuites} ERROR${failedSuites === 1 ? '' : 'S'} OVER ${
            conformanceTests.length
          }`
        )
      )
      return false
    }

    console.log(chalk.greenBright(`Conformance test PASSED :)`))

    return true
  } finally {
    if (ownsServer) {
      await Promise.all([stopServer(), browser ? browser.close().catch(() => {}) : null])
    }
  }
}

if (require.main === module) {
  console.log('Running conformance tests automatically. Run conformance-test-server separately if you need to debug.')
  console.time('conformance-test-runner')
  runConformanceTests({ headless: !process.argv.includes('--show') })
    .then((ok) => {
      if (!ok && !process.exitCode) {
        process.exitCode = 1
      }
      console.timeEnd('conformance-test-runner')
    })
    .catch((e) => {
      if (!process.exitCode) {
        process.exitCode = 2
      }
      console.error(e)
    })
}
