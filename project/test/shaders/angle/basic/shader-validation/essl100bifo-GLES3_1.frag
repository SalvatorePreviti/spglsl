#version 310 es
precision mediump float;
out vec4 my_FragColor;
float isin(float x) {
  return sin(float(x));
}
int isin(int x) {
  return int(isin(float(x)));
}
void main() {
  my_FragColor = vec4(isin(1));
}
