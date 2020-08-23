#version 310 es

precision highp float;
precision highp sampler2D;
precision highp isampler2D;
precision highp int;

float globalF;

uniform ivec4 uivec;
uniform int ui;

struct SS {
  int iField;
  float fField;
  vec2 f2Field;
  sampler2D sField;
  isampler2D isField;
};
uniform SS us;

out vec4 my_FragColor;

float[3] sideEffectArray() {
  globalF += 1.0;
  return float[3](globalF, globalF * 2.0, globalF * 3.0);
}

// This struct is unused and can be pruned.
struct SUnused {
  vec2 fField;
};

void main() {
  struct S2 {
    float fField;
  } s2;
  vec4 foo = vec4(ui);
  mat4 fooM = mat4(foo.x);

  // Some unused variables that can be pruned.
  float fUnused, fUnused2;
  ivec4 iUnused, iUnused2;

  globalF = us.fField;
  s2.fField = us.fField;

  float[3] fa = sideEffectArray();

  globalF -= us.fField;
  if (fa == sideEffectArray()) {
    globalF += us.fField * sin(2.0);
  }

  // Switch with fall-through.
  switch (ui) {
    case 0:
      // Sequence operator and matrix and vector dynamic indexing.
      (globalF += 1.0, fooM[ui][ui] += fooM[ui - 1][uivec[ui] + 1]);
    case 1:
      // Built-in emulation.
      foo[3] = tanh(foo[1]);
    default:
      // Sequence operator and length of an array expression with side effects.
      foo[2] += (globalF -= 1.0, float((sideEffectArray()).length() * 2));
  }
  int i = 0;
  do {
    s2.fField = us.fField * us.f2Field.x;
    // Sequence operator and short-circuiting operator with side effects on the right hand side.
  } while ((++i, i < int(us.fField) && ++i <= ui || ++i < ui * 2 - 3));
  // Samplers in structures and integer texture sampling.
  foo += texture(us.sField, us.f2Field) + intBitsToFloat(texture(us.isField, us.f2Field + 4.0));
  my_FragColor = foo * s2.fField * globalF + fooM[ui];
}