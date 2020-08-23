#version 300 es

precision highp int;

uniform ivec4 A1;
uniform bvec4 B1;
uniform bvec4 B2;

layout(location = 0) out ivec4 O1;
layout(location = 1) out ivec4 O2;
layout(location = 2) out ivec4 O3;
layout(location = 3) out ivec4 O4;
layout(location = 4) out ivec4 O5;
layout(location = 5) out ivec4 O6;

int f(bool b) {
  return b ? 1 : 0;
}

void main() {
  O1.x = +A1.x;
  O1.y = +(+A1.x);
  O1.z = +(+(+A1.x));
  O1.w = +(+(+(+A1.x)));

  O2.x = -A1.x;
  O2.y = -(-A1.x);
  O2.z = -+(-(-A1.x));
  O2.w = -(-(-(-A1.x)));

  O3.x = ~A1.x;
  O3.y = ~(~A1.x);
  O3.z = ~+(~(~A1.x));
  O3.w = ~(~(~(~A1.x)));

  O4.x = B1.x ? 1 : -5;
  O4.y = !B1.y ? -2 : 6;
  O4.z = !!B1.z ? 3 : -7;
  O4.w = !!!B1.w ? -4 : 8;

  O5.x = !(A1.x == A1.w) ? 9 : -10;
  O5.y = !(A1.y != A1.z) ? 11 : -12;
  O5.z = !(A1.z < A1.y) ? 13 : -14;
  O5.w = !(A1.w > A1.x) ? 15 : -16;

  O6.x = !(A1.x <= A1.w) ? 17 : -18;
  O6.y = !(A1.y >= A1.z) ? 19 : -20;
  O6.w = !!(A1.w == A1.w) ? 21 : -22;
  O6.z = !!(A1.z != A1.z) ? 23 : -24;
}
