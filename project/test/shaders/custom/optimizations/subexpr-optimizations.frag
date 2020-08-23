#version 300 es

precision highp int;

uniform ivec4 A1;
uniform ivec4 A2;
uniform bvec4 B1;
uniform bvec4 B2;

int globalVar;

layout(location = 0) out ivec4 O1;
layout(location = 1) out ivec4 O2;
layout(location = 2) out ivec4 O3;
layout(location = 3) out ivec4 O4;
layout(location = 4) out ivec4 O5;
layout(location = 5) out ivec4 O6;
layout(location = 6) out ivec4 O7;
layout(location = 7) out ivec4 O8;
layout(location = 8) out ivec4 O9;

int f(bool b) {
  return b ? 1 : 0;
}

int v(bvec4 b) {
  return b.x ? 1 : 0;
}

void main() {
  O1.x = (A1.x) & (A1.x);
  O1.y = (A1.y + 2) | (A1.y + 2);
  O1.z = f((B1.z || B1.z) && (B1.z || B1.z));
  O1.w = f((B1.w && B1.w) || (B1.w));

  O2.x = f((!B1.x) && (B1.x));
  O2.y = (A1.y + 2) | ~(A1.y + 2);
  O2.z = f((B1.z || B1.z) && !(B1.z || B1.z));
  O2.w = f((!(B1.w && B1.w)) || (B1.w));

  O2 = O2;

  O3.x = A1.x ^ A1.x;
  O3.y = A1.x ^ ~A1.x;
  O3.z = ~A1.x ^ A1.x;
  O3.w = ~A1.x ^ ~A1.x;

  O4.x = -A1.x + A1.y;
  O4.y = A1.x + -A1.y;
  O4.z = A1.x + 0;
  O4.z = 0 + A1.x;
  O4 += 0;

  O5.x = A1.x - -A1.y;
  O5.y = -A1.x - -A1.y;
  O5.z = A1.x - 0;
  O5.z = 0 - A1.x;
  O5 += 0;

  O6.x = A1.x - (A1.y + A1.z);
  O6.y = A1.x - (A1.y - A1.z);
  O6.z = A1.x + (A1.y - A1.z);
  O6.w = (A1.x - A1.y) + A1.z;

  O7.x = (A1.x + 6) * 7;
  O7.y = (A1.y - 6) * 7;
  O7.z = (A1.z + 6) / 7;
  O7.w = (A1.w * 2) + 3;

  O8.x = (A1.x + 6) + 7;
  O8.y = (A1.y - 6) + 7;
  O8.z = (6 + A1.z) - 7;
  O8.w = (8 * A1.w) / 4;

  O9.x = (4 / A1.w) * 2;

  /*  O9.x = (6 + A1.x) + (A2.x + 7);
    O9.y = (A1.y - 6) + (A2.x + 7);
    O9.z = (A1.z + 6) - (A2.x - 7);
    O9.w = (A1.w * 8) / (A2.x * 2);

    /*O6.y = (A1.x + 6) + (A1.y + 7);
    O6.z = (A1.x - 6) + (A1.y + 7);
    O6.w = (A1.x * 6) * (A1.y * 7);

    O7.x = A1.x - (A1.y + A1.z);
    O7.y = (A1.x - 6) * (A1.y - 7);
    O7.z = (A1.x - 6) / (A1.y + 7);
    O7.w = (A1.x * 6) - (A1.y * 7);

    // O4.x = A1.x + A1.x;
    // O4.y = A1.x + -A1.x;
    // O4.z = -A1.x + A1.x;
    // O4.w = A1.x - A1.x;*/
}
