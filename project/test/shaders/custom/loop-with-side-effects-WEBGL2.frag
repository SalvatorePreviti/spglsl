#version 310 es
precision highp float;
in float vertexCounter;
uniform int uVertZero;
uniform int uReference;
out vec4 fragColor;
int counter(int n, int i) {
  bool s0 = true;
  while (true) {
    if (!(s0 || (i++ < 3))) {
      break;
    }
    s0 = false;
    n++;
  }
  return n;
}

void main() {
  fragColor = vec4(0.0, 0.0, 0.0, 1.0);
  fragColor.r = float(counter(uVertZero, uVertZero) == uReference);
  fragColor.g = float(int(vertexCounter) == uReference);
}