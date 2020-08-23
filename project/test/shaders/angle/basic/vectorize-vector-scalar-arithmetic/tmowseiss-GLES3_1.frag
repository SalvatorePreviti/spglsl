#version 310 es
precision highp float;

out vec4 res;
uniform float uf;

void main() {
  res = vec4(0.0);
  float f = uf;
  res += f *= f, res += f *= f;
}

// VectorizeVectorScalarArithmetic VectorizeVectorScalarArithmeticTest, TwoMutatedOpsWithSideEffectsInsideSameStatement