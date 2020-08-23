#version 300 es

precision highp int;

uniform ivec4 A1;
uniform bvec4 B1;
uniform bvec4 B2;

layout(location = 0) out ivec4 O1;
layout(location = 1) out ivec4 O2;
layout(location = 2) out ivec4 O3;
layout(location = 4) out ivec4 O5;

int f(bool b) {
  return b ? 1 : 0;
}

void main() {
  O1.x = f(B1.x ? true : B2.y);
  O1.y = f(B1.y ? false : B2.y);
  O1.z = f(B1.z ? B2.y : true);
  O1.w = f(B1.w ? B2.y : false);

  O2.x = f(B1.x ? true : false);
  O2.y = f(B1.y ? false : true);
  O2.z = f(B1.z ? true : true);
  O2.w = f(B1.w ? false : false);

  O3.x = !B1.x ? A1.x : A1.w;
  O3.y = (!B1.y ? A1.y : A1.z);
  O3.z = (!!B1.z ? A1.z : A1.y);
  O3.w = (!!!B1.w ? A1.w : A1.x);

  O3.x = f(B1.x ? true : B1.y ? true : B1.z ? B1.w : false);
  O3.y = f(B1.x ? false : B1.y ? false : B1.z ? B1.w : true);
  O3.z = f(!B1.x ? false : !B1.y ? false : !B1.z ? B1.w : true);
  O3.w = f(B1.x ? (B2.x ? true : B1.x) : (B1.y ? false : B1.z) ? B1.w : true);
}
