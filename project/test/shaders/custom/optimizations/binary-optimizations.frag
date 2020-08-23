#version 300 es

precision highp int;

uniform ivec4 A1;
uniform ivec4 A2;
uniform bvec4 B1;
uniform bvec4 B2;

layout(location = 0) out ivec4 O1;
layout(location = 1) out ivec4 O2;
layout(location = 2) out ivec4 O3;

int f(bool b) {
  return b ? 1 : 0;
}

int v(bvec4 b) {
  return b.x ? 1 : 0;
}

void main() {
  O1.x = f(!B1.x == !B2.x);
  O1.y = f(!B1.y != !B2.y);
  O1.z = f(!B1.z && !B2.z);
  O1.w = f(!B1.w || !B2.w);

  O2.x = f(~A1.x == ~A2.x);
  O2.y = f(~A1.x != ~A2.x);
  O2.z = ~A1.x ^ ~A2.x;
  O2.w = ~A1.x & ~A2.x;

  O3.x = ~A1.x | ~A2.x;
}
