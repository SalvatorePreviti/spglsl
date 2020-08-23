import { expect } from 'chai'

/*
import { GlslInfoLogArray } from 'spglsl'

describe('GlslInfoLogArray', () => {
  const someErrorsText =
    "ERROR: 0:4: '\n" +
    "' : #version directive must occur on the first line of the shader\n" +
    "ERROR: 1:5: 'xxxyz' : syntax error\n" +
    "ERROR: 2:5: 'xxxyz' : syntax error\nxxx"

  const someErrorsAndWarningText =
    "ERROR: 0:4: '\n" +
    "' : #version directive must occur on the first line of the shader\n" +
    "WARNING: 0:4: 'hey' : syntax warning\n" +
    "ERROR: 0:5: 'xxxyz' : syntax error\nxxx"

  const getFilenameReplacer = (fileId: number) => {
    if (fileId === 0) {
      return 'hello.frag'
    }
    if (fileId === 1) {
      return 'ciao.frag'
    }
    return undefined
  }

  describe('static parse', () => {
    it('returns an empty array for an empty string', () => {
      expect(GlslInfoLogArray.parse('')).to.deep.equal([])
    })

    it('returns an empty array for a list of empty strings', () => {
      expect(GlslInfoLogArray.parse('   \n   \n  \n\n  ')).to.deep.equal([])
    })

    it('parses correctly some errors', () => {
      const parsed = GlslInfoLogArray.parse(someErrorsText)
      expect(JSON.parse(JSON.stringify(parsed))).to.deep.equal([
        {
          type: 'ERROR',
          fileId: 0,
          line: 4,
          source: '\n',
          message: '#version directive must occur on the first line of the shader'
        },
        { type: 'ERROR', fileId: 1, line: 5, source: 'xxxyz', message: 'syntax error' },
        { type: 'ERROR', fileId: 2, line: 5, source: 'xxxyz', message: 'syntax error\nxxx' }
      ])
    })

    it('parses correctly some errors, replacing filenames with replacer', () => {
      const parsed = GlslInfoLogArray.parse(someErrorsText, { getFilename: getFilenameReplacer })
      expect(JSON.parse(JSON.stringify(parsed))).to.deep.equal([
        {
          type: 'ERROR',
          fileId: 0,
          line: 4,
          source: '\n',
          message: '#version directive must occur on the first line of the shader',
          filePath: 'hello.frag'
        },
        { type: 'ERROR', fileId: 1, line: 5, source: 'xxxyz', message: 'syntax error', filePath: 'ciao.frag' },
        { type: 'ERROR', fileId: 2, line: 5, source: 'xxxyz', message: 'syntax error\nxxx' }
      ])
    })
  })

  describe('toString', () => {
    it('returns a valid string with errors and warnings', () => {
      const parsed = GlslInfoLogArray.parse(someErrorsAndWarningText)
      expect(parsed.toString()).to.equal(
        '2 errors 1 warning\n' +
          "  ERROR: 0:4 '': #version directive must occur on the first line of the shader\n" +
          "  WARNING: 0:4 'hey': syntax warning\n" +
          "  ERROR: 0:5 'xxxyz': syntax error xxx\n"
      )
    })

    it('returns a valid string with errors, warnings and filenames', () => {
      const parsed = GlslInfoLogArray.parse(someErrorsText, { getFilename: getFilenameReplacer })
      expect(parsed.toString()).to.equal(
        '3 errors 0 warnings\n' +
          "  ERROR: hello.frag:4 '': #version directive must occur on the first line of the shader\n" +
          "  ERROR: ciao.frag:5 'xxxyz': syntax error\n" +
          "  ERROR: 2:5 'xxxyz': syntax error xxx\n"
      )
    })
  })
})
*/
