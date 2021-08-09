import express from 'express'
import path from 'path'
import fs from 'fs'
import util from 'util'
import cheerio from 'cheerio'
import { conformanceTests, ConformanceTest } from './conformance-test-list'
import type { IncomingMessage, ServerResponse } from 'http'
import { spglslAngleCompile } from 'spglsl'
import chalk from 'chalk'

import { isMainModule } from '@balsamic/esrun'

export const PORT = 3000

const LOG_REQUESTS = false
const SPGLSL_COMPILE_ENABLED = true

export const app = express()

app.use(nocache)
app.use(express.json())

let _server: ReturnType<typeof app.listen> | null = null
let _startServerPromise: Promise<ReturnType<typeof app.listen>> | null = null
let _serverReady: boolean

export function serverReady(): boolean {
  return _serverReady
}

export function serverStarted(): boolean {
  return !!_startServerPromise
}

export function startServer(): Promise<ReturnType<typeof app.listen>> {
  return _startServerPromise || (_startServerPromise = _startServer())
}

function _serveIndex() {
  const $ = cheerio.load('<html><head></head><body></body></html>')
  const body = $('body')
  for (const test of conformanceTests) {
    const link = $('<a></a>')
    link.attr('href', test.url)
    link.text(test.urlpath)
    body.append(link)
    body.append('&nbsp;')
    if (test.finished) {
      body.append(
        test.errors.length > 0
          ? ` <span style="background-color:maroon;color:yellow;">&nbsp;FAILED&nbsp;</span> <span style="color:maroon">${
              test.errors.length
            } errors</span> ${Math.round(test.getDurationMs())}ms`
          : ` <span style="background-color:green;color:lime;">&nbsp;PASSED&nbsp;</span> ${Math.round(
              test.getDurationMs()
            )}ms`
      )
      if (!test.usesGlsl) {
        body.append(' <span style="color:gray">no-glsl</span> ')
      }
    } else if (test.served) {
      body.append('<span style="background-color:#ddd">&nbsp;running&nbsp;</span>')
    }
    body.append($('<br/>'))
  }
  return $.html()
}

async function _serveTestFile(test: ConformanceTest): Promise<string> {
  const $ = cheerio.load(await fs.promises.readFile(test.filepath, 'utf8'))

  let scriptInjected = false
  const scripts = $('script')
  for (let i = 0; i < scripts.length; ++i) {
    const script = $(scripts[i])
    if ((script.attr('src') || '').includes('/js-test-pre.js')) {
      $('<script src="/client/harness.js"></script>').insertAfter(script)
      scriptInjected = true
      break
    }
  }

  if (!scriptInjected) {
    $('body').prepend($('<script src="/client/harness.js"></script>'))
  }

  $('head').prepend($(`<script>__sp_test_file=${JSON.stringify(test.filepath)};</script>`))
  return $.html()
}

function _makeHtmlHandler(handler: () => Promise<string> | string): express.RequestHandler {
  return async (req, res) => {
    if (LOG_REQUESTS) {
      console.log(chalk.gray(req.url))
    }
    try {
      let result = handler()
      if (typeof result === 'object' && result !== null && typeof result.then === 'function') {
        result = await result
      }
      res.set('Content-Type', 'text/html')
      res.send(result)
    } catch (error) {
      console.error('Error serving', req.url, error)
      res.set('Content-Type', 'text/plain')
      res.status(400).send(util.inspect(error, { colors: false }))
    }
  }
}

async function _setupServer() {
  console.log(`Serving ${conformanceTests.length} tests`)

  app.use('/client', express.static(path.resolve(__dirname, './client')))
  app.use('/khronos/js', express.static(path.resolve(__dirname, './khronos/js')))
  app.use('/khronos/resources', express.static(path.resolve(__dirname, './khronos/resources')))

  app.post('/harness/error', (req, res) => {
    res.status(200).send('')
    const test = ConformanceTest.get(req.body.__sp_test_file)
    if (test) {
      test.errors.push(req.body.message)
    }
  })

  app.post('/harness/begin', (req, res) => {
    res.status(200).send('')
    const { __sp_test_file } = req.body
    if (LOG_REQUESTS) {
      console.log(chalk.gray(req.url), chalk.gray(__sp_test_file))
    }
    const test = ConformanceTest.get(__sp_test_file)
    if (test) {
      test.start()
    }
  })

  app.post('/spglsl/glsl', async (req, res) => {
    const { __sp_test_file, sourceCode, shaderType } = req.body
    if (LOG_REQUESTS) {
      console.log(chalk.gray(req.url), chalk.gray(__sp_test_file), chalk.gray(shaderType))
    }
    const test = ConformanceTest.get(req.body.__sp_test_file)
    try {
      if (test) {
        test.usesGlsl = true
      }

      if (!SPGLSL_COMPILE_ENABLED) {
        res.json({
          valid: true,
          infolog: '',
          out: sourceCode
        })
        return
      }

      const result = await spglslAngleCompile({
        mainFilePath: '0',
        mainSourceCode: sourceCode,
        compileMode: 'Optimize',
        language: shaderType,
        minify: true
      })

      console.log('\n\n', chalk.green(sourceCode), '\n\n', result.output, '\n\n')

      res.json({
        valid: result.valid,
        infolog: result.infoLog.original(),
        out: result.output || ''
      })
    } catch (err) {
      const error = (err instanceof Error ? err : new Error(err)) as any
      error.__url = req.url
      error.__sp_test_file = __sp_test_file
      res.json({
        _error: error.name || Object.getPrototypeOf(error).constructor.name || 'Error',
        message: error.message,
        serverStack: error.stack,
        ...error
      })
      console.error('/spglsl/glsl', __sp_test_file, error)
      if (test) {
        test.errors.push(`${error}`)
      }
    }
  })

  app.post('/harness/end', (req, res) => {
    res.status(200).send('')
    const { __sp_test_file } = req.body
    if (LOG_REQUESTS) {
      console.log(chalk.gray(req.url), chalk.gray(__sp_test_file))
    }
    const test = ConformanceTest.get(__sp_test_file)
    if (test) {
      test.finish()
    }
  })

  app.get('/', _makeHtmlHandler(_serveIndex))

  for (const test of conformanceTests) {
    app.get(
      `/${test.urlpath}`,
      _makeHtmlHandler(() => _serveTestFile(test))
    )
  }
}

function _startServer(): Promise<ReturnType<typeof app.listen>> {
  return new Promise<ReturnType<typeof app.listen>>((resolve, reject) => {
    const handleError = (e: any) => reject(e instanceof Error ? e : new Error(e))
    app.once('error', handleError)
    _server = app.listen(PORT, () => {
      _setupServer()
        .then(() => {
          console.log(`Server listening at http://localhost:${PORT}`)
          _serverReady = true
          app.off('error', handleError)
          _server?.on('close', () => {
            stopServer().catch(() => {})
          })
          resolve(_server!)
        })
        .catch((e: any) => {
          app.off('error', handleError)
          handleError(e)
        })
    })
  })
}

export async function stopServer() {
  if (_startServerPromise) {
    try {
      await _startServerPromise
    } catch (_error) {}
    _startServerPromise = null
  }
  const server = _server
  if (server) {
    _server = null
    await new Promise<void>((resolve, reject) => {
      server.close((e) => {
        console.log('Server closed.')
        if (e) {
          reject(e)
        } else {
          resolve()
        }
      })
    }).catch((e) => {
      console.warn('Error closing server', e)
    })
  }
}

function nocache(_req: IncomingMessage, res: ServerResponse, next: () => void) {
  res.setHeader('Surrogate-Control', 'no-store')
  res.setHeader('Cache-Control', 'no-store, no-cache, must-revalidate, proxy-revalidate')
  res.setHeader('Pragma', 'no-cache')
  res.setHeader('Expires', '0')
  next()
}

if (isMainModule(import.meta)) {
  startServer().catch((e) => {
    if (!process.exitCode) {
      process.exitCode = 1
    }
    console.error(e)
  })
}
