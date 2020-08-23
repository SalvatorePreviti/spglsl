#version 300 es

uniform mediump vec4 a;
out mediump vec4 b;

void main() {
  b = vec4(a.x + -a.y, -a.z + a.w, -(a.z + a.x) + a.w, (a.z + a.w) - a.x);
  // expect: vec4(a.x - a.y, a.w - a.z, a.w - (a.z + a.x), a.z + a.w - a.x)
}
