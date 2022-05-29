#version 300 es

precision mediump float;

out vec4 color;
int a = 0;

float[2] t() {
  ++a;
  return float[2](float(a), 1.);
}

struct MyStruct {
  bool a[2];
}

;
uniform UBOData {
  float UBORed;
  float UBOGreen;
  float UBOBlue;
}

;
uniform UBOD {
  float UBOR;
  float UBOG;
  float UBOB;
};

void main() {
  float l;
  for (int a = 1; bool(l = t()[0]); ++a) {
    if (a >= 3) {
      break;
    }
  }

  MyStruct o;
  o.a[0] = true, o.a[1] = false;
  MyStruct e;
  e.a[0] = true, e.a[1] = false, color = o == e ? vec4(0, 1, 0, 1) : vec4(1, 0, 0, 1);
  vec4 B = vec4(UBORed * UBOR, UBOGreen * UBOG, UBOBlue * UBOB, 1.);
  color = B + (o == e ? vec4(0, 1, 0, 1) : vec4(1, 0, 0, 1)) +
      (abs(l - 3.) < .01 && a == 3 ? vec4(0, 1, 0, 1) : vec4(1, 0, 0, 1));
}
