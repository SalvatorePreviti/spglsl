precision highp float;
// NEW CODE
in vec2 FC;
uniform vec2 iR;
uniform vec3 iP;
uniform vec4 iD;
uniform vec4 iA;
uniform vec4 iB;
uniform vec4 iC;
uniform vec4 iS;
uniform vec4 iX;
uniform mat3 iM;
uniform lowp int iF;
out vec4 oC;
uniform highp sampler2D tN;
uniform highp sampler2D tH;
uniform highp sampler2D tP;
uniform highp sampler2D tS;
float u;
float n(float rn) {
  return clamp(rn, 0., 1.);
}
float X(float ur, float et, float c3) {
  float t = max(c3 - abs(ur - et), 0.) / c3;
  return min(ur, et) - t * t * c3 / 4.;
}
vec4 rn(float et) {
  vec4 M = n(et) * vec4(.999998, 254.99948, 65024.867, 160581056);
  M = fract(M);
  M -= M.yzww * vec4(.003921569, .003921569, .003921569, 0);
  return M;
}
float ur(vec4 c3) {
  return dot(c3, vec4(1, .003921569, 1.53787e-5, 6.2273724e-9));
}
vec3 et(vec2 M) {
  vec4 b = texelFetch(tN, ivec2(floor(M)) & 511, 0);
  float c = b.y - b.x, Q = b.z - b.x, e = b.w - c - b.z;
  vec2 H = fract(M), w = H * H, T = w * (3. - 2. * H);
  return vec3(b.x + c * T.x + Q * T.y + e * T.x * T.y, 30. * w * (H * (H - 2.) + 1.) * (vec2(c, Q) + e * T.yx));
}
int c3 = 0;
float M = 150.;
void t(int c, float Q) {
  if (Q < u && Q != M) {
    c3 = c;
    M = Q;
  }
}
float b(vec3 Q, float e) {
  return length(Q) - e;
}
float c(vec3 e, vec3 H) {
  vec3 T = abs(e) - H;
  return min(max(T.x, max(T.y, T.z)), 0.) + length(max(T, 0.));
}
float Q(vec3 H, float w) {
  vec3 G = abs(H) - w;
  return min(max(G.x, max(G.y, G.z)), 0.) + length(max(G, 0.));
}
float e(vec3 w, float T, float G) {
  float v = length(w.xy) - T;
  return max(v, abs(w.z) - G);
}
float H(vec3 T, vec2 G) {
  return length(vec2(length(T.xz) - G.x, T.y)) - G.y;
}
float w(float G, float L, float v, float xz) {
  float at = L * .5, D = floor((G + at) / L), K = mod(G + at, L) - at;
  return D > xz ? K + L * (D - xz) : D < v ? K + L * (D - v) : K;
}
vec2 T(vec2 L, float v) {
  float lo = 3.1415927 / v, at = mod(atan(L.y, L.x) + lo, lo * 2.) - lo;
  return vec2(cos(at), sin(at)) * length(L);
}
vec2 G(vec2 v, float xz, float lo) {
  float D = 3.1415927 / xz, K = mod(atan(v.y, v.x) + D + lo, D * 2.) - D;
  return vec2(cos(K), sin(K)) * length(v);
}
float L(float xz, float lo) {
  return abs(xz) - lo;
}
mat2 v(float lo) {
  float D = cos(lo), K = sin(lo);
  return mat2(D, K, -K, D);
}
vec3 xz(vec3 at) {
  return vec3(at.xy, -at.z);
}
float lo(vec3 D) {
  float ma = length(D) - .3;
  if (ma > .3)
    return ma;
  D.xz *= v(-1.2);
  D.yz *= v(-.2);
  return min(e(D, .025, .1), max(b(D - vec3(0, 0, .12), .05), D.z - .12));
}
float at(vec3 K) {
  float l = length(K) - .3;
  if (l > .3)
    return l;
  float N = e(K, .01, .06);
  N = min(N, e(K.yzx + vec3(0, .1, 0), .04, .005));
  N = min(N, c(K - vec3(0, -.01, .04), vec3(.002, .02, .02)));
  return N;
}
float D(vec3 ma) {
  float N = c(ma - vec3(.03, 0, 0), vec3(.03, .006, .03));
  t(1, N);
  return min(c(ma, vec3(.06, .005, .06)), N);
}
float K(vec3 l, float N, float V) {
  float s = length(l) - N * .6;
  if (s > 4.)
    return s;
  l.y += cos(l.z * V / N);
  l.x = abs(l.x);
  float i = c(l - vec3(.2, 0, 0), vec3(.1, .03, N * .55)), C = e(l - vec3(.5, 1, 0), .02, N * .55);
  l.z = w(l.z, .55, -N, N);
  C = min(C, e(l.xzy - vec3(.5, 0, .5), .02, .5));
  t(1, C);
  i = min(i, c(l, vec3(.5, .05, .2)));
  t(-1, i);
  return min(i, C);
}
float ma(vec3 N) {
  float k = length(N) - 2.;
  if (k > 1.)
    return k;
  vec3 s = N;
  s.xy *= v(-.25);
  float i = c(s + vec3(.2, .25, 0), vec3(.25, .5, .5)) - .01;
  s -= vec3(-.13, .25, 0);
  s.z = w(s.z, .04, -10., 10.);
  s.x = w(s.x, .04, -3., 3.);
  float C = Q(s, .01) - .005;
  t(1, C);
  i = min(min(i, C), c(N - vec3(-.45, .2, 0), vec3(.2, .8, .5)) - .01);
  return i;
}
float l(vec3 V) {
  V.zy *= v(.06);
  V.y += cos(V.z / 20.) * 3.;
  return e(V, .01, 27.5);
}
float N(vec3 k) {
  float i = length(k) - 3.;
  if (i > .5)
    return i;
  k.xz -= vec2(1, -.05);
  k.zx *= v(iA.y * -2.5);
  k.xz += vec2(1, -.05);
  float C = e(k, .99, .05);
  vec3 fl = vec3(T(k.xy, 8.), k.z);
  return max(C, -min(c(fl - vec3(.5, 0, .1), vec3(.02, .1, .1)), e(fl - vec3(0, 0, .1), .02, .1)));
}
float V(vec3 s, float i) {
  float fl = length(s) - 1.;
  if (fl > 1.)
    return fl;
  float x = c(s, vec3(.2, .5, .05));
  x = max(x, -c(s, vec3(.03, .2, 1)));
  s.yz *= v(-1.5707964 * i + .7853982);
  s.z += .2;
  x = min(x, e(s, .02, .2));
  s.z += .2;
  x = min(x, e(s, .03, .05));
  return x;
}
float k(vec3 i) {
  float fl = length(i) - 18.;
  if (fl > 5.)
    return fl;
  i.y -= 9.;
  vec3 x = i;
  x.xz *= v(iB.y);
  x.xy *= v(.5);
  x.y -= 9.;
  float a = b(x, 9.), U = max(L(a, .01), x.y + 4.5);
  U = min(U, e(x.xzy + vec3(0, 0, 4.5), .1, 4.5));
  U = min(U, b(x, .3));
  i.y += 6.75;
  float F = c(i, vec3(2.25, 3.6, 4.5));
  F = min(F,
      min(max(L(e(i.xzy - vec3(2.25, 0, 0), 4.4, 3.5), .1),
              -min(e(i.zyx - vec3(0, 1.8, 0), 1., 100.), e(i - vec3(4.5, 2.3, 0), .4, 100.))),
          e(i.xzy - vec3(2.25, 0, -2.2), 4.4, 2.9)));
  float q = ma(i - vec3(3, 1.5, 2)), f = N(i.zyx - vec3(0, 1.8, 6.5)),
        m = V(xz(i - vec3(3.7, 2, -4)), clamp(iA.w, 0., 1.));
  i.y -= 2.25;
  F = max(min(F, e(i.xzy, .45000002, 4.7699995)), -a);
  i -= vec3(7, -2.85, 0);
  i.xy *= v(-.5);
  F = min(F, c(i, vec3(1, 1, .8)) - .01);
  float S = min(U, f);
  t(2, m);
  t(3, q);
  t(5, S);
  return min(min(q, F), min(S, m));
}
float s(vec3 C) {
  float x = length(C.xz) - 2.;
  if (x > 3.)
    return x;
  float a = e(C.xzy, .2, .5), U = C.y + iA.z * 4.;
  if ((iF & 8) != 0) {
    float q = at(vec3(C.x, U, C.z) + vec3(1.05, -5.05, 1.05));
    if (q < a) {
      t(2, q);
      a = q;
    }
  }
  float F = c(vec3(T(C.xz, 8.), U).xzy - vec3(1.5, 0, 0), vec3(.1, 5, .2));
  if (F < a) {
    t(4, F);
    a = F;
  }
  float q = e(C.xzy + vec3(0, 0, clamp(iA.z, 0., .02)), .05, .53);
  if (q < a) {
    t(1, q);
    a = q;
  }
  return a;
}
float i(vec3 fl) {
  vec3 a = fl.zyx - vec3(11, 1.25, -44);
  float U = length(a) - 8.;
  if (U > 5.)
    return U;
  a.y -= 2.;
  float F = c(a - vec3(-2.7, -1, -1.3), vec3(.35, .5, .5)),
        q = max(L(c(a, vec3(4, 1.6, 2)), .23),
            -min(e(a - vec3(0, .5, 0), .8, 100.), c(a - vec3(4, -.37, 1), vec3(2, 1, .53))));
  vec3 f = a - vec3(4, -.77, .5);
  f.xz *= v(-iA.x * 3.1415927 / 2.);
  float m = c(f - vec3(0, .4, .5), vec3(.05, .99, .52));
  a.x = w(a.x, .3, -10., 10.);
  a.z = abs(a.z);
  float S = e(a.xzy - vec3(0, 2, .5), .01, 1.), E = min(F, m);
  t(1, S);
  t(-1, E);
  float re = min(S, min(q, E)), A = 150.;
  if ((iF & 4) != 0)
    A = lo(fl - vec3(-42, 3, 11.2));
  if ((iF & 2) != 0)
    A = min(A, at(fl.yzx - vec3(2, 7.4, -45.5)));
  if (A < re) {
    t(2, A);
    return A;
  }
  return re;
}
float C(vec3 x) {
  float U = length(x) - 9.;
  if (U > 1.)
    return U;
  x.xz *= v(-.7853982);
  float F = c(x - vec3(-1.5, 1, 5), vec3(1, .2, 3));
  x.y -= iB.w;
  vec3 q = x.xzy - vec3(-2, 0, 2);
  float f = X(b(x - clamp(x, vec3(-6, 0, 0), vec3(6, 0, 0)), 1.7),
      min(e(q - clamp(q, vec3(-.5, 0, 0), vec3(.5, 0, 0)), .4, .5),
          min(c(x - vec3(7.5, 0, 0), vec3(.3, 2, .05)) - .05, c(x - vec3(7.5, 0, 0), vec3(.3, .05, 2)) - .05)),
      .3);
  t(3, f);
  return min(F, f);
}
float fl(vec3 a) {
  float F = length(a) - 13.;
  if (F > 3.)
    return F;
  vec3 q = abs(a), f = vec3(q.x, abs(a.y - 4.58), q.z);
  float m = V(xz(a.xzy - vec3(1.9, -1.5, .2)) * .5, min(1., (6. - iX.x) * .2)) / .5,
        S = max(c(vec3(a.x, abs(a.y - 3.5) - 3.5, a.z), vec3(6, .2, 6)) - .05,
            max(-Q(a - vec3(2, 7, 2), 1.5), -Q(a - vec3(5.7, 0, 4), .52)));
  vec3 E = a - vec3(5, 7.6, -2);
  E.xy *= v(.3);
  vec3 re = vec3(a.xy, abs(a.z + 2.));
  m = min(m,
      min(min(e(re.xzy - vec3(-6, 1.1, 8.7), 1., 1.75), e(re.xzy - vec3(-6.5, 1.1, 0), .2, 8.)),
          e(vec3(a.z, abs(a.y - 7.6), a.x) - vec3(-3, .2, 0), .1, 5.)));
  float A = min(min(min(e(vec3(q.xz, a.y) - vec3(5, 5, 0), .5, 8.3), e(f.zyx - vec3(5.3, 3.5, 0), .05, 5.3)),
                    max(e(f - vec3(5.3, 3.5, 0), .05, 5.3), -Q(a - vec3(5, .7, 4), .8))),
      c(E, vec3(.5, .6, 1.5)) - .05);
  t(1, A);
  vec3 Y = a - vec3(2, 3.59, -.1);
  Y.zy *= v(-.7853982);
  Y.y -= iX.x;
  m = min(m, c(Y, vec3(1, 5.1, .02)) - .05);
  t(5, m);
  float oa = min(min(S, m), A);
  vec3 j = E - vec3(0, .8, 0);
  if (length(j) - 1. < 2.) {
    float R = min(min(H(j, vec2(.5, .02)), e(j.xzy + vec3(0, 0, .5), .02, .5)),
        e(vec3(G(j.xz, 5., iB.x), j.y).zyx - vec3(0, 0, .25), .01, .25));
    if (R < oa) {
      t(2, R);
      oa = R;
    }
  }
  return oa;
}
float x(vec3 U) {
  vec3 q = U.zyx - vec3(4, -1, 17);
  q.zy *= v(-.19);
  q.z -= 19. - iA.w;
  return min(K(q, 21., 0.), e(q.xzy + vec3(0, 10.5, 6), .15, 5.));
}
float a(vec3 F) {
  vec3 f = F - vec3(8.7, 9.3, 37);
  float m = length(f.xz) - 5.;
  if (m > 4.)
    return m;
  vec3 S = vec3(T(f.xz, 6.), f.y).xzy, E = vec3(S.x, w(f.y, 1.5, -3., 7.), S.z);
  float re = max(
      max(min(e(f.xzy, 1.1, 12.), max(L(e(f.xzy - vec3(0, 0, 14), 4., 2.), .2), -c(S - vec3(4, 14, 0), vec3(1, 1, 2)))),
          -min(e(f.xzy, 1., 13.), c(E - vec3(1, 0, 0), vec3(.2, .3, .13)))),
      -c(f + vec3(0, 7, 1), vec3(.8, 1.2, .8)));
  vec3 A = vec3(f.x, f.y - iB.z, f.z);
  float Y = e(A.xzy, 1., 11.);
  A.y = w(A.y, 1.5, -7., 7.);
  Y = max(Y, -H(A, vec2(1, .01)));
  vec3 oa = vec3(f.x - .8, w(f.y - 12.7, 20.5, -1., 0.), f.z + .9);
  float j = b(oa - vec3(0, .5, 0), .06),
        mi = min(e(oa.xzy, .05, .5), min(c(oa - vec3(0, .5, 0), vec3(.05, .1, .1)), j));
  t(2, j);
  t(1, mi);
  t(5, Y);
  float R = min(min(re, min(mi, Y)), c(f + vec3(0, 10.3, 3), vec3(1.1, 2, 3)));
  if ((iF & 16) != 0) {
    float I = D(F - vec3(12.15, 22.31, 38.65));
    if (I < R) {
      t(2, I);
      return I;
    }
  }
  return R;
}
vec2 U;
float F(vec3 f, vec3 m, vec2 S) {
  f -= m;
  float re = length(f) - 2.;
  if (re > .5)
    return re;
  f.xz *= v(1.5707964);
  U = (S - f.xy) / (S * 2.);
  float A = c(f, vec3(S, .01));
  return A;
}
float q(vec3 m) {
  vec3 E = abs(vec3(m.x, m.y + 3., m.z)) - vec3(60, 0, 39);
  if (E.x < 0. && E.z < 0.)
    E.y -= ur(textureLod(tH, m.xz / vec2(120, 78) + .5, 0.)) * 19.;
  return min(E.y, 0.) + length(max(E, 0.));
}
float f(vec3 S) {
  float re = K(S - vec3(45, 1.7, 22.4), 10., 2.), A = k(S - vec3(2, 10, 2)), Y = s(S - vec3(47.5, 3.5, 30.5)),
        oa = i(S);
  vec3 j = S - vec3(26, 5, -58);
  j.xz *= v(1.9707963);
  float mi = fl(j), R = x(j), Z = l(j.zyx - vec3(-2, 9.7, 32.5)), I = a(S), y = C(S - vec3(-46, -.5, -30)),
        z = min(min(min(re, A), min(Y, oa)), min(mi, min(R, min(I, y))));
  if (Z < z) {
    t(1, Z);
    return Z;
  }
  t(0, z);
  return z;
}
int m = 0;
float S(vec3 re) {
  float Y = q(re);
  if (Y <= u) {
    m = 1;
    return Y;
  }
  float oa = f(re), j = F(re, vec3(4.76, 14.42, 4), vec2(.45, .29)), mi = min(j, oa);
  if (Y < mi) {
    m = 1;
    return Y;
  }
  m = j <= oa ? 3 : 2;
  return mi;
}
vec3 E(vec3 A) {
  return normalize(vec3(f(A + vec3(.001, 0, 0)), f(A + vec3(0, .001, 0)), f(A + vec3(0, 0, .001))) - f(A));
}
vec3 re(vec3 Y, float oa) {
  vec2 mi = vec2(mix(.03, .001, min(oa / 120., 1.)), 0);
  return normalize(vec3(q(Y + mi.xyy), q(Y + mi.yxy), q(Y + mi.yyx)) - q(Y));
}
float A(vec3 oa, vec3 j, float mi, float R) {
  float I = mi;
  for (int z = 0;; z++) {
    vec3 P = oa + j * R;
    u = mi * max(R, 1.);
    if (P.y <= -2.9995 || R >= 150.) {
      float o = (-3. - oa.y) / j.y;
      if (o >= 0. && o < 500.) {
        m = 1;
        return o;
      }
      break;
    }
    if (P.y > 45.)
      break;
    float d = S(P);
    if (d < 0.) {
      R -= I;
      d = I / 2.;
    }
    R += d;
    if (d <= u || z >= 100)
      return R;
    I = d;
  }
  m = 0;
  return 500.;
}
float Y(vec3 j, float mi, vec3 R, float Z) {
  float y = clamp(mi * .005, .01, .1);
  j = j + R * y;
  float z = iS.w - u * 2.;
  for (float P = 1.; P < 50.; P++) {
    vec3 o = j + iS.xyz * y;
    if (y >= 80. || o.y > 45. || o.y < z || length(j) >= 150.)
      break;
    float h = f(o), O = max(u, .01 * min(1., y) + P * 2e-4);
    if (h <= O)
      return 0.;
    Z = min(Z, 85. * h / y);
    if (Z < .078)
      return 0.;
    y += h + u;
  }
  return Z;
}
float oa(vec3 mi, vec3 R) {
  float I = (iS.w - mi.y) / R.y;
  return min(I >= 0. ? I : 500., 500.);
}
vec3 j(vec2 R) {
  vec3 I = vec3(0);
  float y = 0., z = 1., th = 0., P = 1. - length(iP.xz - R) / 500., d = 5. * P * P;
  for (float h = 0.; h < d; ++h) {
    R += iD.w * .5;
    th *= -.75;
    vec3 r = et(R + sin(R.yx * .5 + iD.w * .5) * .5);
    I += r * z;
    R += r.yz * .43;
    R *= 2.;
    y += z;
    z *= .75;
  }
  return I / y;
}
vec3 mi(vec3 Z, float I, vec3 y) {
  float th = min(I / 500., 1.), P = n(pow(th, 3.5) + 1. - exp(-I * .005)), d = max(dot(y, iS.xyz), 0.);
  vec3 o = mix(vec3(.4, .8, 1), vec3(1.065, .95, .85), pow(d, 10.));
  return mix(Z, o, P);
}
vec3 R(vec3 I, vec3 y) {
  vec4 packed = texelFetch(tP, ivec2(FC * 256. / iR), 0);
  float th = uintBitsToFloat(uint(packed.x * 255.) << 24 | uint(packed.y * 255.) << 16 | uint(packed.z * 255.) << 8 |
            uint(packed.w * 255.)),
        P = A(I, y, .001, th), d = oa(I, y);
  vec3 o, h = vec3(0, 1, 0);
  float O = P;
  if (m == 3)
    return iB.y > 0. ? texture(tS, U).xyz : vec3(0);
  vec3 r = I + y * P;
  bool J = d < 500. && d < P;
  vec3 c2;
  float B = 0.;
  if (J) {
    B = mix(.2, 1., n((P - d) / 3.));
    vec3 le = I + y * d, W = mix(vec3(0), j(le.xz * (.7 - iS.w * .02)) * (1. - length(le) / 450.), B);
    h = normalize(vec3(W.y, 1., W.x));
    d -= abs(W.z) * B * .6;
    O = d;
    c2 = mix(vec3(.25, .52, .73), vec3(.15, .62, .83), n(abs(W.z) - B));
  }
  int tu = m, le = c3;
  if (m == 0)
    o = vec3(.4, .8, 1);
  else {
    vec3 p;
    if (r.y <= -2.9995) {
      p = vec3(0, 1, 0);
      o = vec3(1);
    } else {
      o = vec3(.8);
      switch (tu) {
        case 1:
          p = re(r, P);
          o = mix(vec3(.93, .8, .64),
                  mix(vec3(.69 + texture(tN, r.xz * 1e-4).x, .67, .65), vec3(.38, .52, .23), dot(p, vec3(0, 1, 0))),
                  n(r.y * .5 - 1.)) +
              texture(tN, r.xz * .15).x * .1 + texture(tN, r.xz * .01).y * .1;
          break;
        case 2:
          p = E(r);
          switch (le) {
            case 1: o = vec3(1); break;
            case 2: o = vec3(1, 0, 0); break;
            case 3: o = vec3(.5, 0, 0); break;
            case 4: o = vec3(.2, .1, .2); break;
            case 5: o = vec3(1, .95, .8); break;
            case -1: o = vec3(.64000005, .48000002, .32000002); break;
            default:
              vec4 ec =
                  texture(tN, r.xy * .35) * p.z + texture(tN, r.yz * .35) * p.x + texture(tN, r.xz * .35) * p.y - .5;
              o += .125 * (ec.x - ec.y + ec.z - ec.w);
              break;
          }
      }
      h = normalize(mix(p, h, B));
    }
  }
  float W = J || tu == 2 && le > 0 ? pow(n(dot(iS.xyz, reflect(y, h))), 50.) : 0., p = n(dot(iS.xyz, h)),
        ax = n(dot(iS.xyz * vec3(-1, 1, -1), h)), g = p + ax * .15;
  if (tu == 1)
    g = pow(g * mix(.9, 1.02, p * p), 1. + p * .6);
  g = mix(g, p, B);
  float ve = 1.;
  if (m != 0)
    ve = Y(I + y * O, O, h, 1.);
  if ((iF & 1) != 0 && P < 20.) {
    float zy = pow(clamp(dot(iD.xyz, y), 0., 1.), 32.) * smoothstep(10., 0., P);
    g += zy * max(dot(h, -y), 0.) * (1. - g);
    ve += zy * (1. - ve);
  }
  o = mix(o, c2, B);
  o = (o * vec3(1.065, .95, .85) * g + W) * mix(.38 + (1. - g) * .2, 1., ve);
  return mi(o, O, y);
}
void main_c() {
  vec3 y = vec3(0, 0, 1);
  y.xz *= v(FC.x * .049087387 + 3.1415927);
  oC = rn(.2 - S(vec3(iP.x, iP.y + (FC.y / 64. - 1.) - .8, iP.z) + normalize(y) * .15));
}
void main_p() {
  vec2 y = FC / 128. - 1. + .001953125;
  vec3 z = normalize(iM * vec3(y.x * -1.5, y.y, 2.4142134));
  float th = A(iP, z, .0046875, .15);
  uint packed = floatBitsToUint(th >= 150. ? 150. : th - u);
  oC = vec4(float(packed >> 24 & 255u) / 255., float(packed >> 16 & 255u) / 255., float(packed >> 8 & 255u) / 255.,
      float(packed & 255u) / 255.);
}
void main_m() {
  vec2 y = FC / (iR * .5) - 1.;
  vec3 z = normalize(iM * vec3(y.x * -1.5, y.y, 2.4142134));
  oC = vec4(R(iP, z), 1);
}
float Z(vec2 y, float z, float th, float P, float d) {
  vec2 h = y - vec2(z, th);
  return n(1. - smoothstep(P - P * d, P, dot(h, h) * 4.));
}
void main_h() {
  vec2 z = FC / (iR * .5) - 1., th = vec2(1.3, 1), P = vec2(0);
  float d = 0., o = 1., h = 1., O = 0., r = 1.;
  while (r < 11.) {
    vec3 B = et(21.1 + z * th * v(r++ * 2.4));
    P += B.yz;
    d += h * (1. - B.x) / (1. + dot(P, P));
    o += h * (.5 - B.x);
    O += h;
    h *= .5;
    th *= 1.8;
  }
  d /= O;
  o *= .5;
  float J = length(z * (1.2 - o + d)) * n(o + .55 - .5 * d * z.x * (1. - z.y * .5)),
        c2 = Z(z, -.45, -.52, 1., 2.3) + Z(z, -.6, -.1, 1., 3.3) + Z(z, .6, -.7, 1., 5.) + Z(z, .84, .84, d, o * 5.);
  J = n(1. - X(J, 1. - mix(0., d * 2., c2), .05 + o * .5));
  vec2 B = 1. - abs(z) + d * .04;
  J = X(J, X(B.x, B.y, .3) * 2., .1);
  oC = rn(X(d, J, .01) * 1.33 - .045);
}
