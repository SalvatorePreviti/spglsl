#version 300 es
precision mediump float;
out vec4 color;
int sideEffectCounter = 0;
float[2] functionReturnArray() {
  ++sideEffectCounter;
  return float[2](float(sideEffectCounter), 1.0);
}
struct MyStruct {
  bool a[2];
};

uniform UBOData {
  float UBORed;
  float UBOGreen;
  float UBOBlue;
};

uniform UBOD {
  float UBOR;
  float UBOG;
  float UBOB;
};

void main() {
  float c = 0.0;
  for (int i = 1; bool(c = functionReturnArray()[0]); ++i) {
    if (i >= 3) {
      break;
    }
  }
  MyStruct b;
  b.a[0] = true;
  b.a[1] = false;

  MyStruct d;
  d.a[0] = true;
  d.a[1] = false;

  color = b == d ? vec4(0, 1.0, 0, 1.0) : vec4(1.0, 0, 0, 1.0);

  vec4 t = vec4(UBORed * UBOR, UBOGreen * UBOG, UBOBlue * UBOB, 1.0);

  color = t + (b == d ? vec4(0, 1.0, 0, 1.0) : vec4(1.0, 0, 0, 1.0)) +
      ((abs(c - 3.0) < 0.01 && sideEffectCounter == 3) ? vec4(0, 1.0, 0, 1.0) : vec4(1.0, 0, 0, 1.0));
}
