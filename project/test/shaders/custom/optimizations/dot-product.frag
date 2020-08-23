#version 300 es

uniform mediump vec4 v;
out mediump vec4 r;

void main() {
  r = vec4((v.x + v.y) + (v.z + v.w));  // dot(v, 1.0)
}