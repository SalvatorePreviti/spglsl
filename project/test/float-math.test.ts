import { expect } from 'chai'

import { floatToGlsl, glslToFloat, FLOAT_MIN, FLOAT_MAX, FLOAT_EPSILON } from 'spglsl'

describe('float-math', () => {
  describe('floatToGlsl', () => {
    it('returns 0. for 0', () => {
      expect(floatToGlsl(0)).to.equal('0.')
      expect(floatToGlsl(+0)).to.equal('0.')
      expect(floatToGlsl(-0)).to.equal('0.')
      expect(floatToGlsl('0')).to.equal('0.')
      expect(floatToGlsl('0.')).to.equal('0.')
      expect(floatToGlsl('0.0')).to.equal('0.')
      expect(floatToGlsl('+0.0')).to.equal('0.')
      expect(floatToGlsl('+0.')).to.equal('0.')
      expect(floatToGlsl('-0.')).to.equal('0.')
      expect(floatToGlsl('-0')).to.equal('0.')
      expect(floatToGlsl('+0')).to.equal('0.')
      expect(floatToGlsl(false)).to.equal('0.')
    })

    it('works for NaN', () => {
      expect(floatToGlsl(NaN)).to.equal('0./0.')
      expect(floatToGlsl('NaN')).to.equal('0./0.')
      expect(floatToGlsl('0/0')).to.equal('0./0.')
      expect(floatToGlsl('0./0.')).to.equal('0./0.')
    })

    it('works for positive infinity', () => {
      expect(floatToGlsl(Infinity)).to.equal('1./0.')
      expect(floatToGlsl('Infinity')).to.equal('1./0.')
      expect(floatToGlsl('+Infinity')).to.equal('1./0.')
      expect(floatToGlsl('1/0')).to.equal('1./0.')
      expect(floatToGlsl('1./0')).to.equal('1./0.')
    })

    it('works for negative infinity', () => {
      expect(floatToGlsl(-Infinity)).to.equal('-1./0.')
      expect(floatToGlsl('-Infinity')).to.equal('-1./0.')
      expect(floatToGlsl('-1/0')).to.equal('-1./0.')
      expect(floatToGlsl('-1./0')).to.equal('-1./0.')
    })

    it('works for positive integer numbers', () => {
      expect(floatToGlsl(1)).to.equal('1.')
      expect(floatToGlsl(10)).to.equal('10.')
      expect(floatToGlsl(12345)).to.equal('12345.')
    })

    it('works for negative integer numbers', () => {
      expect(floatToGlsl(-1)).to.equal('-1.')
      expect(floatToGlsl(-10)).to.equal('-10.')
      expect(floatToGlsl(-12345)).to.equal('-12345.')
    })
  })

  describe('float->string->float', () => {
    const commonValues = [
      0.1,
      0.2,
      0.25,
      0.125,
      0.0625,
      0.3,
      0.4,
      0.5,
      0.6,
      0.7,
      0.8,
      0.9,
      0.12,
      0.23,
      0.34,
      0.45,
      0.56,
      0.67,
      0.78,
      0.89,
      0.91,
      1.1,
      1.2,
      1.3,
      1.4,
      1.5,
      1.6,
      1.7,
      1.8,
      1.9,
      321.231,
      145.2,
      1121.344,
      144.45,
      113.543,
      155.6,
      2231.744,
      551.6684,
      31.19,
      Math.PI,
      Math.E,
      Math.LOG10E,
      Math.LOG2E,
      Math.SQRT1_2,
      Math.LN10,
      Math.LN2,
      Math.SQRT2,
      Math.sin(Math.PI / 3),
      Math.sin(Math.PI / 4),
      Math.sin(1),
      Math.sin(2),
      Math.sin(3),
      Math.sin(4),
      Math.cos(1),
      Math.cos(2),
      Math.cos(3),
      Math.cos(4),
      FLOAT_MIN,
      FLOAT_MAX,
      FLOAT_EPSILON
    ]

    it('works for integer values', () => {
      for (let v = -50; v < 500; ++v) {
        expect(glslToFloat(floatToGlsl(v))).to.equal(Math.fround(v))
      }
    })

    it('works for Infinity and NaN without parentheses', () => {
      const a = glslToFloat(floatToGlsl(Infinity, false))
      const b = glslToFloat(floatToGlsl(-Infinity, false))
      const c = glslToFloat(floatToGlsl(NaN, false))
      expect(a).to.equal(Infinity)
      expect(b).to.equal(-Infinity)
      expect(isNaN(c)).to.equal(true)
    })

    it('works for Infinity and NaN with parentheses', () => {
      const a = glslToFloat(floatToGlsl(Infinity, true))
      const b = glslToFloat(floatToGlsl(-Infinity, true))
      const c = glslToFloat(floatToGlsl(NaN, true))
      expect(a).to.equal(Infinity)
      expect(b).to.equal(-Infinity)
      expect(isNaN(c)).to.equal(true)
    })

    it('works for common floating point positive values', () => {
      for (const v of commonValues) {
        expect(glslToFloat(floatToGlsl(v))).to.equal(Math.fround(v))
      }
    })

    it('works for common floating point positive values', () => {
      for (const v of commonValues) {
        expect(glslToFloat(floatToGlsl(v))).to.equal(Math.fround(v))
      }
    })

    it('works for common floating point negative values', () => {
      for (const positive of commonValues) {
        const v = -positive
        expect(glslToFloat(floatToGlsl(v))).to.equal(Math.fround(v))
      }
    })
  })
})
