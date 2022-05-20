#version 300 es
precision highp float;
// NEW CODE
precision highp float;
in vec2 FC;
uniform vec2 iR;
uniform vec3 iP;
uniform vec4 iD, iA, iB, iC, iS, iX;
uniform mat3 iM;
uniform lowp int iF;
out vec4 oC;
uniform highp sampler2D tN, tH, tP, tS;
float s;
float n(float wi) {
  return clamp(wi, 0., 1.);
}

float j(float fo, float am, float ax) {
  float as = max(ax - abs(fo - am), 0.) / ax;
  return min(fo, am) - as * as * ax / 4.;
}
vec4 ca(float au) {
  vec4 q = n(au) * vec4(.999998, 254.99948, 65024.867, 160581056);
  q = fract(q);
  q -= q.yzww * vec4(.003921569, .003921569, .003921569, 0);
  return q;
}
float q(vec4 bo) {
  return dot(bo, vec4(1, .003921569, 1.53787e-5, 6.2273724e-9));
}
vec3 xt(vec2 mp) {
  vec4 p = texelFetch(tN, ivec2(floor(mp)) & 511, 0);
  float en = p.y - p.x, c = p.z - p.x, al = p.w - en - p.z;
  vec2 E = fract(mp), cl = E * E, I = cl * (3. - 2. * E);
  return vec3(p.x + en * I.x + c * I.y + al * I.x * I.y, 30. * cl * (E * (E - 2.) + 1.) * (vec2(en, c) + al * I.yx));
}
int iz = 0;
float p = 150.;
void c(int al, float E) {
  if (E < s && E != p) {
    iz = al;
    p = E;
  }
}
float I(vec3 de, float hp) {
  return length(de) - hp;
}
float t(vec3 pl, vec3 er) {
  vec3 ec = abs(pl) - er;
  return min(max(ec.x, max(ec.y, ec.z)), 0.) + length(max(ec, 0.));
}
float ve(vec3 eL, float fa) {
  vec3 e = abs(eL) - fa;
  return min(max(e.x, max(e.y, e.z)), 0.) + length(max(e, 0.));
}
float e(vec3 la, float il, float ig) {
  float li = length(la.xy) - il;
  return max(li, abs(la.z) - ig);
}
float li(vec3 no, vec2 ze) {
  return length(vec2(length(no.xz) - ze.x, no.y)) - ze.y;
}
float F(float ni, float J, float ui, float un) {
  float gt = J * .5, c3 = floor((ni + gt) / J), ng = mod(ni + gt, J) - gt;
  return c3 > un ? ng + J * (c3 - un) : c3 < ui ? ng + J * (c3 - ui) : ng;
}
vec2 gt(vec2 c3, float ng) {
  float xy = 3.1415927 / ng, ab = mod(atan(c3.y, c3.x) + xy, xy * 2.) - xy;
  return vec2(cos(ab), sin(ab)) * length(c3);
}
vec2 xy(vec2 ab, float lt, float r2) {
  float at = 3.1415927 / lt, ow = mod(atan(ab.y, ab.x) + at + r2, at * 2.) - at;
  return vec2(cos(ow), sin(ow)) * length(ab);
}
float at(float ow, float t3) {
  return abs(ow) - t3;
}
mat2 v(float po) {
  float co = cos(po), id = sin(po);
  return mat2(co, id, -id, co);
}
vec3 co(vec3 id) {
  return vec3(id.xy, -id.z);
}
float xp(vec3 A) {
  float mo = length(A) - .3;
  if (mo > .3)
    return mo;
  A.xz *= v(-1.2);
  A.yz *= v(-.2);
  return min(e(A, .025, .1), max(I(A - vec3(0, 0, .12), .05), A.z - .12));
}
float mo(vec3 lo) {
  float vo = length(lo) - .3;
  if (vo > .3)
    return vo;
  float Q = e(lo, .01, .06);
  Q = min(Q, e(lo.yzx + vec3(0, .1, 0), .04, .005));
  Q = min(Q, t(lo - vec3(0, -.01, .04), vec3(.002, .02, .02)));
  return Q;
}
float vo(vec3 Q) {
  float oi = t(Q - vec3(.03, 0, 0), vec3(.03, .006, .03));
  c(1, oi);
  return min(t(Q, vec3(.06, .005, .06)), oi);
}
float oi(vec3 r, float D, float gh) {
  float os = length(r) - D * .6;
  if (os > 4.)
    return os;
  r.y += cos(r.z * gh / D);
  r.x = abs(r.x);
  float fl = t(r - vec3(.2, 0, 0), vec3(.1, .03, D * .55)), oa = e(r - vec3(.5, 1, 0), .02, D * .55);
  r.z = F(r.z, .55, -D, D);
  oa = min(oa, e(r.xzy - vec3(.5, 0, .5), .02, .5));
  c(1, oa);
  fl = min(fl, t(r, vec3(.5, .05, .2)));
  c(-1, fl);
  return min(fl, oa);
}
float os(vec3 fl) {
  float oo = length(fl) - 2.;
  if (oo > 1.)
    return oo;
  vec3 h = fl;
  h.xy *= v(-.25);
  float bs = t(h + vec3(.2, .25, 0), vec3(.25, .5, .5)) - .01;
  h -= vec3(-.13, .25, 0);
  h.z = F(h.z, .04, -10., 10.);
  h.x = F(h.x, .04, -3., 3.);
  float ai = ve(h, .01) - .005;
  c(1, ai);
  bs = min(min(bs, ai), t(fl - vec3(-.45, .2, 0), vec3(.2, .8, .5)) - .01);
  return bs;
}
float oo(vec3 h) {
  h.zy *= v(.06);
  h.y += cos(h.z / 20.) * 3.;
  return e(h, .01, 27.5);
}
float ai(vec3 C) {
  float ch = length(C) - 3.;
  if (ch > .5)
    return ch;
  C.xz -= vec2(1, -.05);
  C.zx *= v(iA.y * -2.5);
  C.xz += vec2(1, -.05);
  float b = e(C, .99, .05);
  vec3 el = vec3(gt(C.xy, 8.), C.z);
  return max(b, -min(t(el - vec3(.5, 0, .1), vec3(.02, .1, .1)), e(el - vec3(0, 0, .1), .02, .1)));
}
float ch(vec3 b, float el) {
  float it = length(b) - 1.;
  if (it > 1.)
    return it;
  float i = t(b, vec3(.2, .5, .05));
  i = max(i, -t(b, vec3(.03, .2, 1)));
  b.yz *= v(-1.5707964 * el + .7853982);
  b.z += .2;
  i = min(i, e(b, .02, .2));
  b.z += .2;
  i = min(i, e(b, .03, .05));
  return i;
}
float it(vec3 i) {
  float tc = length(i) - 18.;
  if (tc > 5.)
    return tc;
  i.y -= 9.;
  vec3 k = i;
  k.xz *= v(iB.y);
  k.xy *= v(.5);
  k.y -= 9.;
  float si = I(k, 9.), G = max(at(si, .01), k.y + 4.5);
  G = min(G, e(k.xzy + vec3(0, 0, 4.5), .1, 4.5));
  G = min(G, I(k, .3));
  i.y += 6.75;
  float S = t(i, vec3(2.25, 3.6, 4.5));
  S = min(S,
      min(max(at(e(i.xzy - vec3(2.25, 0, 0), 4.4, 3.5), .1),
              -min(e(i.zyx - vec3(0, 1.8, 0), 1., 100.), e(i - vec3(4.5, 2.3, 0), .4, 100.))),
          e(i.xzy - vec3(2.25, 0, -2.2), 4.4, 2.9)));
  float od = os(i - vec3(3, 1.5, 2)), l = ai(i.zyx - vec3(0, 1.8, 6.5)),
        m = ch(co(i - vec3(3.7, 2, -4)), clamp(iA.w, 0., 1.));
  i.y -= 2.25;
  S = max(min(S, e(i.xzy, .45000002, 4.7699995)), -si);
  i -= vec3(7, -2.85, 0);
  i.xy *= v(-.5);
  S = min(S, t(i, vec3(1, 1, .8)) - .01);
  float ct = min(G, l);
  c(2, m);
  c(3, od);
  c(5, ct);
  return min(min(od, S), min(ct, m));
}
float tc(vec3 k) {
  float G = length(k.xz) - 2.;
  if (G > 3.)
    return G;
  float S = e(k.xzy, .2, .5), od = k.y + iA.z * 4.;
  if ((iF & 8) != 0) {
    float m = mo(vec3(k.x, od, k.z) + vec3(1.05, -5.05, 1.05));
    if (m < S) {
      c(2, m);
      S = m;
    }
  }
  float l = t(vec3(gt(k.xz, 8.), od).xzy - vec3(1.5, 0, 0), vec3(.1, 5, .2));
  if (l < S) {
    c(4, l);
    S = l;
  }
  float m = e(k.xzy + vec3(0, 0, clamp(iA.z, 0., .02)), .05, .53);
  if (m < S) {
    c(1, m);
    S = m;
  }
  return S;
}
float G(vec3 S) {
  vec3 l = S.zyx - vec3(11, 1.25, -44);
  float m = length(l) - 8.;
  if (m > 5.)
    return m;
  l.y -= 2.;
  float ct = t(l - vec3(-2.7, -1, -1.3), vec3(.35, .5, .5)),
        an = max(at(t(l, vec3(4, 1.6, 2)), .23),
            -min(e(l - vec3(0, .5, 0), .8, 100.), t(l - vec3(4, -.37, 1), vec3(2, 1, .53))));
  vec3 a = l - vec3(4, -.77, .5);
  a.xz *= v(-iA.x * 3.1415927 / 2.);
  float hi = t(a - vec3(0, .4, .5), vec3(.05, .99, .52));
  l.x = F(l.x, .3, -10., 10.);
  l.z = abs(l.z);
  float zx = e(l.xzy - vec3(0, 2, .5), .01, 1.), yz = min(ct, hi);
  c(1, zx);
  c(-1, yz);
  float ac = min(zx, min(an, yz)), R = 150.;
  if ((iF & 4) != 0)
    R = xp(S - vec3(-42, 3, 11.2));
  if ((iF & 2) != 0)
    R = min(R, mo(S.yzx - vec3(2, 7.4, -45.5)));
  if (R < ac) {
    c(2, R);
    return R;
  }
  return ac;
}
float l(vec3 m) {
  float an = length(m) - 9.;
  if (an > 1.)
    return an;
  m.xz *= v(-.7853982);
  float a = t(m - vec3(-1.5, 1, 5), vec3(1, .2, 3));
  m.y -= iB.w;
  vec3 hi = m.xzy - vec3(-2, 0, 2);
  float zx = j(I(m - clamp(m, vec3(-6, 0, 0), vec3(6, 0, 0)), 1.7),
      min(e(hi - clamp(hi, vec3(-.5, 0, 0), vec3(.5, 0, 0)), .4, .5),
          min(t(m - vec3(7.5, 0, 0), vec3(.3, 2, .05)) - .05, t(m - vec3(7.5, 0, 0), vec3(.3, .05, 2)) - .05)),
      .3);
  c(3, zx);
  return min(a, zx);
}
float an(vec3 a) {
  float zx = length(a) - 13.;
  if (zx > 3.)
    return zx;
  vec3 yz = abs(a), ac = vec3(yz.x, abs(a.y - 4.58), yz.z);
  float R = ch(co(a.xzy - vec3(1.9, -1.5, .2)) * .5, min(1., (6. - iX.x) * .2)) / .5,
        ep = max(t(vec3(a.x, abs(a.y - 3.5) - 3.5, a.z), vec3(6, .2, 6)) - .05,
            max(-ve(a - vec3(2, 7, 2), 1.5), -ve(a - vec3(5.7, 0, 4), .52)));
  vec3 ix = a - vec3(5, 7.6, -2);
  ix.xy *= v(.3);
  vec3 f = vec3(a.xy, abs(a.z + 2.));
  R = min(R,
      min(min(e(f.xzy - vec3(-6, 1.1, 8.7), 1., 1.75), e(f.xzy - vec3(-6.5, 1.1, 0), .2, 8.)),
          e(vec3(a.z, abs(a.y - 7.6), a.x) - vec3(-3, .2, 0), .1, 5.)));
  float hs = min(min(min(e(vec3(yz.xz, a.y) - vec3(5, 5, 0), .5, 8.3), e(ac.zyx - vec3(5.3, 3.5, 0), .05, 5.3)),
                     max(e(ac - vec3(5.3, 3.5, 0), .05, 5.3), -ve(a - vec3(5, .7, 4), .8))),
      t(ix, vec3(.5, .6, 1.5)) - .05);
  c(1, hs);
  vec3 V = a - vec3(2, 3.59, -.1);
  V.zy *= v(-.7853982);
  V.y -= iX.x;
  R = min(R, t(V, vec3(1, 5.1, .02)) - .05);
  c(5, R);
  float c4 = min(min(ep, R), hs);
  vec3 K = ix - vec3(0, .8, 0);
  if (length(K) - 1. < 2.) {
    float Y = min(min(li(K, vec2(.5, .02)), e(K.xzy + vec3(0, 0, .5), .02, .5)),
        e(vec3(xy(K.xz, 5., iB.x), K.y).zyx - vec3(0, 0, .25), .01, .25));
    if (Y < c4) {
      c(2, Y);
      c4 = Y;
    }
  }
  return c4;
}
float zx(vec3 yz) {
  vec3 R = yz.zyx - vec3(4, -1, 17);
  R.zy *= v(-.19);
  R.z -= 19. - iA.w;
  return min(oi(R, 21., 0.), e(R.xzy + vec3(0, 10.5, 6), .15, 5.));
}
float R(vec3 ep) {
  vec3 f = ep - vec3(8.7, 9.3, 37);
  float hs = length(f.xz) - 5.;
  if (hs > 4.)
    return hs;
  vec3 V = vec3(gt(f.xz, 6.), f.y).xzy, c4 = vec3(V.x, F(f.y, 1.5, -3., 7.), V.z);
  float K = max(max(min(e(f.xzy, 1.1, 12.),
                        max(at(e(f.xzy - vec3(0, 0, 14), 4., 2.), .2), -t(V - vec3(4, 14, 0), vec3(1, 1, 2)))),
                    -min(e(f.xzy, 1., 13.), t(c4 - vec3(1, 0, 0), vec3(.2, .3, .13)))),
      -t(f + vec3(0, 7, 1), vec3(.8, 1.2, .8)));
  vec3 re = vec3(f.x, f.y - iB.z, f.z);
  float Y = e(re.xzy, 1., 11.);
  re.y = F(re.y, 1.5, -7., 7.);
  Y = max(Y, -li(re, vec2(1, .01)));
  vec3 mi = vec3(f.x - .8, F(f.y - 12.7, 20.5, -1., 0.), f.z + .9);
  float ef = I(mi - vec3(0, .5, 0), .06),
        X = min(e(mi.xzy, .05, .5), min(t(mi - vec3(0, .5, 0), vec3(.05, .1, .1)), ef));
  c(2, ef);
  c(1, X);
  c(5, Y);
  float P = min(min(K, min(X, Y)), t(f + vec3(0, 10.3, 3), vec3(1.1, 2, 3)));
  if ((iF & 16) != 0) {
    float rm = vo(ep - vec3(12.15, 22.31, 38.65));
    if (rm < P) {
      c(2, rm);
      return rm;
    }
  }
  return P;
}
vec2 ix;
float hs(vec3 V, vec3 c4, vec2 K) {
  V -= c4;
  float Y = length(V) - 2.;
  if (Y > .5)
    return Y;
  V.xz *= v(1.5707964);
  ix = (K - V.xy) / (K * 2.);
  float mi = t(V, vec3(K, .01));
  return mi;
}
float Y(vec3 mi) {
  vec3 X = abs(vec3(mi.x, mi.y + 3., mi.z)) - vec3(60, 0, 39);
  if (X.x < 0. && X.z < 0.)
    X.y -= q(textureLod(tH, mi.xz / vec2(120, 78) + .5, 0.)) * 19.;
  return min(X.y, 0.) + length(max(X, 0.));
}
float X(vec3 P) {
  float rm = oi(P - vec3(45, 1.7, 22.4), 10., 2.), fr = it(P - vec3(2, 10, 2)), ot = tc(P - vec3(47.5, 3.5, 30.5)),
        sa = G(P);
  vec3 tu = P - vec3(26, 5, -58);
  tu.xz *= v(1.9707963);
  float ur = an(tu), iv = zx(tu), nt = oo(tu.zyx - vec3(-2, 9.7, 32.5)), et = R(P), ul = l(P - vec3(-46, -.5, -30)),
        te = min(min(min(rm, fr), min(ot, sa)), min(ur, min(iv, min(et, ul))));
  if (nt < te) {
    c(1, nt);
    return nt;
  }
  c(0, te);
  return te;
}
int y = 0;
float fr(vec3 ot) {
  float tu = Y(ot);
  if (tu <= s) {
    y = 1;
    return tu;
  }
  float ur = X(ot), iv = hs(ot, vec3(4.76, 14.42, 4), vec2(.45, .29)), nt = min(iv, ur);
  if (tu < nt) {
    y = 1;
    return tu;
  }
  y = iv <= ur ? 3 : 2;
  return nt;
}
vec3 tu(vec3 ur) {
  return normalize(vec3(X(ur + vec3(.001, 0, 0)), X(ur + vec3(0, .001, 0)), X(ur + vec3(0, 0, .001))) - X(ur));
}
vec3 nt(vec3 et, float ul) {
  vec2 ak = vec2(mix(.03, .001, min(ul / 120., 1.)), 0);
  return normalize(vec3(Y(et + ak.xyy), Y(et + ak.yxy), Y(et + ak.yyx)) - Y(et));
}
float ak(vec3 ls, vec3 lF, float ra, float M) {
  float br = ra;
  for (int ta = 0;; ta++) {
    vec3 xz = ls + lF * M;
    s = ra * max(M, 1.);
    if (xz.y <= -2.9995 || M >= 150.) {
      float H = (-3. - ls.y) / lF.y;
      if (H >= 0. && H < 500.) {
        y = 1;
        return H;
      }
      break;
    }
    if (xz.y > 45.)
      break;
    float Z = fr(xz);
    if (Z < 0.) {
      M -= br;
      Z = br / 2.;
    }
    M += Z;
    if (Z <= s || ta >= 100)
      return M;
    br = Z;
  }
  y = 0;
  return 500.;
}
float br(vec3 rn, float ta, vec3 ut, float xz) {
  float H = clamp(ta * .005, .01, .1);
  rn = rn + ut * H;
  float zw = iS.w - s * 2.;
  for (float ea = 1.; ea < 50.; ea++) {
    vec3 T = rn + iS.xyz * H;
    if (H >= 80. || T.y > 45. || T.y < zw || length(rn) >= 150.)
      break;
    float ex = X(T), tB = max(s, .01 * min(1., H) + ea * 2e-4);
    if (ex <= tB)
      return 0.;
    xz = min(xz, 85. * ex / H);
    if (xz < .078)
      return 0.;
    H += ex + s;
  }
  return xz;
}
float H(vec3 zw, vec3 ol) {
  float ts = (iS.w - zw.y) / ol.y;
  return min(ts >= 0. ? ts : 500., 500.);
}
vec3 ts(vec2 T) {
  vec3 tB = vec3(0);
  float t2 = 0., yx = 1., ou = 0., xe = 1. - length(iP.xz - T) / 500., oF = 5. * xe * xe;
  for (float z = 0.; z < oF; ++z) {
    T += iD.w * .5;
    ou *= -.75;
    vec3 ma = xt(T + sin(T.yx * .5 + iD.w * .5) * .5);
    tB += ma * yx;
    T += ma.yz * .43;
    T *= 2.;
    t2 += yx;
    yx *= .75;
  }
  return tB / t2;
}
vec3 tB(vec3 t2, float yx, vec3 ou) {
  float oF = min(yx / 500., 1.), O = n(pow(oF, 3.5) + 1. - exp(-yx * .005)), z = max(dot(ou, iS.xyz), 0.);
  vec3 oU = mix(vec3(.4, .8, 1), vec3(1.065, .95, .85), pow(z, 10.));
  return mix(t2, oU, O);
}
vec3 oF(vec3 O, vec3 z) {
  vec4 ma = texelFetch(tP, ivec2(FC * 256. / iR), 0);
  float sT = uintBitsToFloat(
            uint(ma.x * 255.) << 24 | uint(ma.y * 255.) << 16 | uint(ma.z * 255.) << 8 | uint(ma.w * 255.)),
        B = ak(O, z, .001, sT), L = H(O, z);
  vec3 o, g = vec3(0, 1, 0);
  float c2 = B;
  if (y == 3)
    return iB.y > 0. ? texture(tS, ix).xyz : vec3(0);
  vec3 x = O + z * B;
  bool st = L < 500. && L < B;
  vec3 sm;
  float N = 0.;
  if (st) {
    N = mix(.2, 1., n((B - L) / 3.));
    vec3 sw = O + z * L, zy = mix(vec3(0), ts(sw.xz * (.7 - iS.w * .02)) * (1. - length(sw) / 450.), N);
    g = normalize(vec3(zy.y, 1., zy.x));
    L -= abs(zy.z) * N * .6;
    c2 = L;
    sm = mix(vec3(.25, .52, .73), vec3(.15, .62, .83), n(abs(zy.z) - N));
  }
  int u = y, sw = iz;
  if (y == 0)
    o = vec3(.4, .8, 1);
  else {
    vec3 w;
    if (x.y <= -2.9995) {
      w = vec3(0, 1, 0);
      o = vec3(1);
    } else {
      o = vec3(.8);
      switch (u) {
        case 1:
          w = nt(x, B);
          o = mix(vec3(.93, .8, .64),
                  mix(vec3(.69 + texture(tN, x.xz * 1e-4).x, .67, .65), vec3(.38, .52, .23), dot(w, vec3(0, 1, 0))),
                  n(x.y * .5 - 1.)) +
              texture(tN, x.xz * .15).x * .1 + texture(tN, x.xz * .01).y * .1;
          break;
        case 2:
          w = tu(x);
          switch (sw) {
            case 1: o = vec3(1); break;
            case 2: o = vec3(1, 0, 0); break;
            case 3: o = vec3(.5, 0, 0); break;
            case 4: o = vec3(.2, .1, .2); break;
            case 5: o = vec3(1, .95, .8); break;
            case -1: o = vec3(.64000005, .48000002, .32000002); break;
            default:
              vec4 le =
                  texture(tN, x.xy * .35) * w.z + texture(tN, x.yz * .35) * w.x + texture(tN, x.xz * .35) * w.y - .5;
              o += .125 * (le.x - le.y + le.z - le.w);
              break;
          }
      }
      g = normalize(mix(w, g, N));
    }
  }
  float zy = st || u == 2 && sw > 0 ? pow(n(dot(iS.xyz, reflect(z, g))), 50.) : 0., w = n(dot(iS.xyz, g)),
        U = n(dot(iS.xyz * vec3(-1, 1, -1), g)), d = w + U * .15;
  if (u == 1)
    d = pow(d * mix(.9, 1.02, w * w), 1. + w * .6);
  d = mix(d, w, N);
  float th = 1.;
  if (y != 0)
    th = br(O + z * c2, c2, g, 1.);
  if ((iF & 1) != 0 && B < 20.) {
    float W = pow(clamp(dot(iD.xyz, z), 0., 1.), 32.) * smoothstep(10., 0., B);
    d += W * max(dot(g, -z), 0.) * (1. - d);
    th += W * (1. - th);
  }
  o = mix(o, sm, N);
  o = (o * vec3(1.065, .95, .85) * d + zy) * mix(.38 + (1. - d) * .2, 1., th);
  return tB(o, c2, z);
}
void main_c() {
  vec3 sT = vec3(0, 0, 1);
  sT.xz *= v(FC.x * .049087387 + 3.1415927);
  oC = ca(.2 - fr(vec3(iP.x, iP.y + (FC.y / 64. - 1.) - .8, iP.z) + normalize(sT) * .15));
}
void main_p() {
  vec2 B = FC / 128. - 1. + .001953125;
  vec3 L = normalize(iM * vec3(B.x * -1.5, B.y, 2.4142134));
  float o = ak(iP, L, .0046875, .15);
  uint g = floatBitsToUint(o >= 150. ? 150. : o - s);
  oC = vec4(
      float(g >> 24 & 255u) / 255., float(g >> 16 & 255u) / 255., float(g >> 8 & 255u) / 255., float(g & 255u) / 255.);
}
void main_m() {
  vec2 L = FC / (iR * .5) - 1.;
  vec3 o = normalize(iM * vec3(L.x * -1.5, L.y, 2.4142134));
  oC = vec4(oF(iP, o), 1);
}
float L(vec2 o, float g, float c2, float x, float st) {
  vec2 N = o - vec2(g, c2);
  return n(1. - smoothstep(x - x * st, x, dot(N, N) * 4.));
}
void main_h() {
  vec2 u = FC / (iR * .5) - 1., sw = vec2(1.3, 1), zy = vec2(0);
  float w = 0., U = 1., d = 1., th = 0., le = 1.;
  while (le < 11.) {
    vec3 se = xt(21.1 + u * sw * v(le++ * 2.4));
    zy += se.yz;
    w += d * (1. - se.x) / (1. + dot(zy, zy));
    U += d * (.5 - se.x);
    th += d;
    d *= .5;
    sw *= 1.8;
  }
  w /= th;
  U *= .5;
  float W = length(u * (1.2 - U + w)) * n(U + .55 - .5 * w * u.x * (1. - u.y * .5)),
        se = L(u, -.45, -.52, 1., 2.3) + L(u, -.6, -.1, 1., 3.3) + L(u, .6, -.7, 1., 5.) + L(u, .84, .84, w, U * 5.);
  W = n(1. - j(W, 1. - mix(0., w * 2., se), .05 + U * .5));
  vec2 yy = 1. - abs(u) + w * .04;
  W = j(W, j(yy.x, yy.y, .3) * 2., .1);
  oC = ca(j(w, W, .01) * 1.33 - .045);
}
