#version 300 es
precision mediump float;
layout(location = 0) out vec4 oColor;

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
  oColor = vec4(UBORed * UBOR, UBOGreen * UBOG, UBOBlue * UBOB, 1.0);
}