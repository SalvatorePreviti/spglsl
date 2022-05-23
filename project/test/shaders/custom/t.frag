#version 300 es
precision highp float;
in vec2 FC;
uniform vec2 iR;
uniform vec3 iP;
uniform vec4 iD, iA, iB, iC, iS, iX;
uniform mat3 iM;
uniform lowp int iF;
out vec4 oC;
uniform highp sampler2D tN, tH, tP, tS;
float o;
float a(float e) {
  return clamp(e, 0., 1.);
}
float n(float t, float c, float e) {
  float v = max(e - abs(t - c), 0.) / e;
  return min(t, c) - v * v * e / 4.;
}
vec4 s(float t) {
  vec4 e = a(t) * vec4(.999998, 254.99948, 65024.867, 160581056);
  e = fract(e);
  e -= e.yzww * vec4(.003921569, .003921569, .003921569, 0);
  return e;
}
float B(vec4 e) {
  return dot(e, vec4(1, .003921569, 1.53787e-5, 6.2273724e-9));
}
vec3 h(vec2 a) {
  vec4 e = texelFetch(tN, ivec2(floor(a)) & 511, 0);
  float v = e.y - e.x, i = e.z - e.x, o = e.w - v - e.z;
  vec2 t = fract(a), f = t * t, c = f * (3. - 2. * t);
  return vec3(e.x + v * c.x + i * c.y + o * c.x * c.y, 30. * f * (t * (t - 2.) + 1.) * (vec2(v, i) + o * c.yx));
}
int b = 0;
float g = 150.;
void c(int t, float e) {
  if (e < o && e != g) {
    b = t;
    g = e;
  }
}
float r(vec3 e, float t) {
  return length(e) - t;
}
float t(vec3 t, vec3 c) {
  vec3 e = abs(t) - c;
  return min(max(e.x, max(e.y, e.z)), 0.) + length(max(e, 0.));
}
float m(vec3 t, float c) {
  vec3 e = abs(t) - c;
  return min(max(e.x, max(e.y, e.z)), 0.) + length(max(e, 0.));
}
float e(vec3 e, float t, float c) {
  float v = length(e.xy) - t;
  return max(v, abs(e.z) - c);
}
float w(vec3 e, vec2 t) {
  return length(vec2(length(e.xz) - t.x, e.y)) - t.y;
}
float f(float a, float e, float i, float o) {
  float c = e * .5, t = floor((a + c) / e), v = mod(a + c, e) - c;
  return t > o ? v + e * (t - o) : t < i ? v + e * (t - i) : v;
}
vec2 u(vec2 e, float v) {
  float t = 3.1415927 / v, c = mod(atan(e.y, e.x) + t, t * 2.) - t;
  return vec2(cos(c), sin(c)) * length(e);
}
vec2 N(vec2 e, float v, float a) {
  float t = 3.1415927 / v, c = mod(atan(e.y, e.x) + t + a, t * 2.) - t;
  return vec2(cos(c), sin(c)) * length(e);
}
float y(float e, float t) {
  return abs(e) - t;
}
mat2 v(float e) {
  float t = cos(e), c = sin(e);
  return mat2(t, c, -c, t);
}
vec3 d(vec3 e) {
  return vec3(e.xy, -e.z);
}
float A(vec3 t) {
  float c = length(t) - .3;
  if (c > .3)
    return c;
  t.xz *= v(-1.2);
  t.yz *= v(-.2);
  return min(e(t, .025, .1), max(r(t - vec3(0, 0, .12), .05), t.z - .12));
}
float p(vec3 v) {
  float a = length(v) - .3;
  if (a > .3)
    return a;
  float c = e(v, .01, .06);
  c = min(c, e(v.yzx + vec3(0, .1, 0), .04, .005));
  c = min(c, t(v - vec3(0, -.01, .04), vec3(.002, .02, .02)));
  return c;
}
float D(vec3 e) {
  float v = t(e - vec3(.03, 0, 0), vec3(.03, .006, .03));
  c(1, v);
  return min(t(e, vec3(.06, .005, .06)), v);
}
float F(vec3 v, float a, float n) {
  float l = length(v) - a * .6;
  if (l > 4.)
    return l;
  v.y += cos(v.z * n / a);
  v.x = abs(v.x);
  float i = t(v - vec3(.2, 0, 0), vec3(.1, .03, a * .55)), o = e(v - vec3(.5, 1, 0), .02, a * .55);
  v.z = f(v.z, .55, -a, a);
  o = min(o, e(v.xzy - vec3(.5, 0, .5), .02, .5));
  c(1, o);
  i = min(i, t(v, vec3(.5, .05, .2)));
  c(-1, i);
  return min(i, o);
}
float R(vec3 a) {
  float o = length(a) - 2.;
  if (o > 1.)
    return o;
  vec3 e = a;
  e.xy *= v(-.25);
  float i = t(e + vec3(.2, .25, 0), vec3(.25, .5, .5)) - .01;
  e -= vec3(-.13, .25, 0);
  e.z = f(e.z, .04, -10., 10.);
  e.x = f(e.x, .04, -3., 3.);
  float l = m(e, .01) - .005;
  c(1, l);
  i = min(min(i, l), t(a - vec3(-.45, .2, 0), vec3(.2, .8, .5)) - .01);
  return i;
}
float X(vec3 t) {
  t.zy *= v(.06);
  t.y += cos(t.z / 20.) * 3.;
  return e(t, .01, 27.5);
}
float M(vec3 c) {
  float a = length(c) - 3.;
  if (a > .5)
    return a;
  c.xz -= vec2(1, -.05);
  c.zx *= v(iA.y * -2.5);
  c.xz += vec2(1, -.05);
  float o = e(c, .99, .05);
  vec3 i = vec3(u(c.xy, 8.), c.z);
  return max(o, -min(t(i - vec3(.5, 0, .1), vec3(.02, .1, .1)), e(i - vec3(0, 0, .1), .02, .1)));
}
float C(vec3 c, float o) {
  float i = length(c) - 1.;
  if (i > 1.)
    return i;
  float a = t(c, vec3(.2, .5, .05));
  a = max(a, -t(c, vec3(.03, .2, 1)));
  c.yz *= v(-1.5707964 * o + .7853982);
  c.z += .2;
  a = min(a, e(c, .02, .2));
  c.z += .2;
  a = min(a, e(c, .03, .05));
  return a;
}
float H(vec3 a) {
  float n = length(a) - 18.;
  if (n > 5.)
    return n;
  a.y -= 9.;
  vec3 o = a;
  o.xz *= v(iB.y);
  o.xy *= v(.5);
  o.y -= 9.;
  float l, f, i;
  l = r(o, 9.);
  f = max(y(l, .01), o.y + 4.5);
  f = min(f, e(o.xzy + vec3(0, 0, 4.5), .1, 4.5));
  f = min(f, r(o, .3));
  a.y += 6.75;
  i = t(a, vec3(2.25, 3.6, 4.5));
  i = min(i,
      min(max(y(e(a.xzy - vec3(2.25, 0, 0), 4.4, 3.5), .1),
              -min(e(a.zyx - vec3(0, 1.8, 0), 1., 100.), e(a - vec3(4.5, 2.3, 0), .4, 100.))),
          e(a.xzy - vec3(2.25, 0, -2.2), 4.4, 2.9)));
  float x = R(a - vec3(3, 1.5, 2)), u = M(a.zyx - vec3(0, 1.8, 6.5)),
        m = C(d(a - vec3(3.7, 2, -4)), clamp(iA.w, 0., 1.));
  a.y -= 2.25;
  i = max(min(i, e(a.xzy, .45000002, 4.7699995)), -l);
  a -= vec3(7, -2.85, 0);
  a.xy *= v(-.5);
  i = min(i, t(a, vec3(1, 1, .8)) - .01);
  float z = min(f, u);
  c(2, m);
  c(3, x);
  c(5, z);
  return min(min(x, i), min(z, m));
}
float T(vec3 v) {
  float f = length(v.xz) - 2.;
  if (f > 3.)
    return f;
  float a = e(v.xzy, .2, .5), l = v.y + iA.z * 4.;
  if ((iF & 8) != 0) {
    float e = p(vec3(v.x, l, v.z) + vec3(1.05, -5.05, 1.05));
    if (e < a) {
      c(2, e);
      a = e;
    }
  }
  float i = t(vec3(u(v.xz, 8.), l).xzy - vec3(1.5, 0, 0), vec3(.1, 5, .2));
  if (i < a) {
    c(4, i);
    a = i;
  }
  float o = e(v.xzy + vec3(0, 0, clamp(iA.z, 0., .02)), .05, .53);
  if (o < a) {
    c(1, o);
    a = o;
  }
  return a;
}
float L(vec3 o) {
  vec3 a = o.zyx - vec3(11, 1.25, -44);
  float l = length(a) - 8.;
  if (l > 5.)
    return l;
  a.y -= 2.;
  float z = t(a - vec3(-2.7, -1, -1.3), vec3(.35, .5, .5)),
        u = max(y(t(a, vec3(4, 1.6, 2)), .23),
            -min(e(a - vec3(0, .5, 0), .8, 100.), t(a - vec3(4, -.37, 1), vec3(2, 1, .53))));
  vec3 n = a - vec3(4, -.77, .5);
  n.xz *= v(-iA.x * 3.1415927 / 2.);
  float s = t(n - vec3(0, .4, .5), vec3(.05, .99, .52));
  a.x = f(a.x, .3, -10., 10.);
  a.z = abs(a.z);
  float r = e(a.xzy - vec3(0, 2, .5), .01, 1.), x = min(z, s);
  c(1, r);
  c(-1, x);
  float m = min(r, min(u, x)), i = 150.;
  if ((iF & 4) != 0)
    i = A(o - vec3(-42, 3, 11.2));
  if ((iF & 2) != 0)
    i = min(i, p(o.yzx - vec3(2, 7.4, -45.5)));
  if (i < m) {
    c(2, i);
    return i;
  }
  return m;
}
float U(vec3 a) {
  float i = length(a) - 9.;
  if (i > 1.)
    return i;
  a.xz *= v(-.7853982);
  float l = t(a - vec3(-1.5, 1, 5), vec3(1, .2, 3));
  a.y -= iB.w;
  vec3 o = a.xzy - vec3(-2, 0, 2);
  float f = n(r(a - clamp(a, vec3(-6, 0, 0), vec3(6, 0, 0)), 1.7),
      min(e(o - clamp(o, vec3(-.5, 0, 0), vec3(.5, 0, 0)), .4, .5),
          min(t(a - vec3(7.5, 0, 0), vec3(.3, 2, .05)) - .05, t(a - vec3(7.5, 0, 0), vec3(.3, .05, 2)) - .05)),
      .3);
  c(3, f);
  return min(l, f);
}
float j(vec3 a) {
  float x = length(a) - 13.;
  if (x > 3.)
    return x;
  vec3 f = abs(a), y = vec3(f.x, abs(a.y - 4.58), f.z);
  float i = C(d(a.xzy - vec3(1.9, -1.5, .2)) * .5, min(1., (6. - iX.x) * .2)) / .5,
        s = max(t(vec3(a.x, abs(a.y - 3.5) - 3.5, a.z), vec3(6, .2, 6)) - .05,
            max(-m(a - vec3(2, 7, 2), 1.5), -m(a - vec3(5.7, 0, 4), .52)));
  vec3 l = a - vec3(5, 7.6, -2);
  l.xy *= v(.3);
  vec3 z = vec3(a.xy, abs(a.z + 2.));
  i = min(i,
      min(min(e(z.xzy - vec3(-6, 1.1, 8.7), 1., 1.75), e(z.xzy - vec3(-6.5, 1.1, 0), .2, 8.)),
          e(vec3(a.z, abs(a.y - 7.6), a.x) - vec3(-3, .2, 0), .1, 5.)));
  float u = min(min(min(e(vec3(f.xz, a.y) - vec3(5, 5, 0), .5, 8.3), e(y.zyx - vec3(5.3, 3.5, 0), .05, 5.3)),
                    max(e(y - vec3(5.3, 3.5, 0), .05, 5.3), -m(a - vec3(5, .7, 4), .8))),
      t(l, vec3(.5, .6, 1.5)) - .05);
  c(1, u);
  vec3 n = a - vec3(2, 3.59, -.1);
  n.zy *= v(-.7853982);
  n.y -= iX.x;
  i = min(i, t(n, vec3(1, 5.1, .02)) - .05);
  c(5, i);
  float r = min(min(s, i), u);
  vec3 o = l - vec3(0, .8, 0);
  if (length(o) - 1. < 2.) {
    float t = min(min(w(o, vec2(.5, .02)), e(o.xzy + vec3(0, 0, .5), .02, .5)),
        e(vec3(N(o.xz, 5., iB.x), o.y).zyx - vec3(0, 0, .25), .01, .25));
    if (t < r) {
      c(2, t);
      r = t;
    }
  }
  return r;
}
float q(vec3 c) {
  vec3 t = c.zyx - vec3(4, -1, 17);
  t.zy *= v(-.19);
  t.z -= 19. - iA.w;
  return min(F(t, 21., 0.), e(t.xzy + vec3(0, 10.5, 6), .15, 5.));
}
float E(vec3 n) {
  vec3 v = n - vec3(8.7, 9.3, 37);
  float x = length(v.xz) - 5.;
  if (x > 4.)
    return x;
  vec3 o = vec3(u(v.xz, 6.), v.y).xzy, h = vec3(o.x, f(v.y, 1.5, -3., 7.), o.z);
  float b = max(
      max(min(e(v.xzy, 1.1, 12.), max(y(e(v.xzy - vec3(0, 0, 14), 4., 2.), .2), -t(o - vec3(4, 14, 0), vec3(1, 1, 2)))),
          -min(e(v.xzy, 1., 13.), t(h - vec3(1, 0, 0), vec3(.2, .3, .13)))),
      -t(v + vec3(0, 7, 1), vec3(.8, 1.2, .8)));
  vec3 a = vec3(v.x, v.y - iB.z, v.z);
  float i = e(a.xzy, 1., 11.);
  a.y = f(a.y, 1.5, -7., 7.);
  i = max(i, -w(a, vec2(1, .01)));
  vec3 l = vec3(v.x - .8, f(v.y - 12.7, 20.5, -1., 0.), v.z + .9);
  float m = r(l - vec3(0, .5, 0), .06), z = min(e(l.xzy, .05, .5), min(t(l - vec3(0, .5, 0), vec3(.05, .1, .1)), m));
  c(2, m);
  c(1, z);
  c(5, i);
  float s = min(min(b, min(z, i)), t(v + vec3(0, 10.3, 3), vec3(1.1, 2, 3)));
  if ((iF & 16) != 0) {
    float e = D(n - vec3(12.15, 22.31, 38.65));
    if (e < s) {
      c(2, e);
      return e;
    }
  }
  return s;
}
vec2 k;
float I(vec3 e, vec3 i, vec2 c) {
  e -= i;
  float a = length(e) - 2.;
  if (a > .5)
    return a;
  e.xz *= v(1.5707964);
  k = (c - e.xy) / (c * 2.);
  float o = t(e, vec3(c, .01));
  return o;
}
float x(vec3 t) {
  vec3 e = abs(vec3(t.x, t.y + 3., t.z)) - vec3(60, 0, 39);
  if (e.x < 0. && e.z < 0.)
    e.y -= B(textureLod(tH, t.xz / vec2(120, 78) + .5, 0.)) * 19.;
  return min(e.y, 0.) + length(max(e, 0.));
}
float l(vec3 e) {
  float o = F(e - vec3(45, 1.7, 22.4), 10., 2.), f = H(e - vec3(2, 10, 2)), l = T(e - vec3(47.5, 3.5, 30.5)), n = L(e);
  vec3 t = e - vec3(26, 5, -58);
  t.xz *= v(1.9707963);
  float r = j(t), x = q(t), a = X(t.zyx - vec3(-2, 9.7, 32.5)), m = E(e), y = U(e - vec3(-46, -.5, -30)),
        i = min(min(min(o, f), min(l, n)), min(r, min(x, min(m, y))));
  if (a < i) {
    c(1, a);
    return a;
  }
  c(0, i);
  return i;
}
int i = 0;
float S(vec3 t) {
  float e = x(t);
  if (e <= o) {
    i = 1;
    return e;
  }
  float c = l(t), v = I(t, vec3(4.76, 14.42, 4), vec2(.45, .29)), a = min(v, c);
  if (e < a) {
    i = 1;
    return e;
  }
  i = v <= c ? 3 : 2;
  return a;
}
vec3 J(vec3 e) {
  return normalize(vec3(l(e + vec3(.001, 0, 0)), l(e + vec3(0, .001, 0)), l(e + vec3(0, 0, .001))) - l(e));
}
vec3 Q(vec3 e, float c) {
  vec2 t = vec2(mix(.03, .001, min(c / 120., 1.)), 0);
  return normalize(vec3(x(e + t.xyy), x(e + t.yxy), x(e + t.yyx)) - x(e));
}
float P(vec3 c, vec3 v, float a, float e) {
  float t = a;
  for (int n = 0;; n++) {
    vec3 l = c + v * e;
    o = a * max(e, 1.);
    if (l.y <= -2.9995 || e >= 150.) {
      float e = (-3. - c.y) / v.y;
      if (e >= 0. && e < 500.) {
        i = 1;
        return e;
      }
      break;
    }
    if (l.y > 45.)
      break;
    float f = S(l);
    if (f < 0.) {
      e -= t;
      f = t / 2.;
    }
    e += f;
    if (f <= o || n >= 100)
      return e;
    t = f;
  }
  i = 0;
  return 500.;
}
float G(vec3 t, float v, vec3 a, float c) {
  float e = clamp(v * .005, .01, .1);
  t = t + a * e;
  float i = iS.w - o * 2.;
  for (float v = 1.; v < 50.; v++) {
    vec3 a = t + iS.xyz * e;
    if (e >= 80. || a.y > 45. || a.y < i || length(t) >= 150.)
      break;
    float f = l(a), n = max(o, .01 * min(1., e) + v * 2e-4);
    if (f <= n)
      return 0.;
    c = min(c, 85. * f / e);
    if (c < .078)
      return 0.;
    e += f + o;
  }
  return c;
}
float K(vec3 t, vec3 c) {
  float e = (iS.w - t.y) / c.y;
  return min(e >= 0. ? e : 500., 500.);
}
vec3 V(vec2 e) {
  vec3 c = vec3(0);
  float v = 0., t = 1., i = 0., a = 1. - length(iP.xz - e) / 500., o = 5. * a * a;
  for (float a = 0.; a < o; ++a) {
    e += iD.w * .5;
    i *= -.75;
    vec3 f = h(e + sin(e.yx * .5 + iD.w * .5) * .5);
    c += f * t;
    e += f.yz * .43;
    e *= 2.;
    v += t;
    t *= .75;
  }
  return c / v;
}
vec3 Y(vec3 t, float e, vec3 c) {
  float v = min(e / 500., 1.), i = a(pow(v, 3.5) + 1. - exp(-e * .005)), o = max(dot(c, iS.xyz), 0.);
  vec3 f = mix(vec3(.4, .8, 1), vec3(1.065, .95, .85), pow(o, 10.));
  return mix(t, f, i);
}
vec3 Z(vec3 r, vec3 c) {
  vec4 m = texelFetch(tP, ivec2(FC * 256. / iR), 0);
  float w = uintBitsToFloat(uint(m.x * 255.) << 24 | uint(m.y * 255.) << 16 | uint(m.z * 255.) << 8 | uint(m.w * 255.)),
        f = P(r, c, .001, w), n = K(r, c);
  vec3 e, v = vec3(0, 1, 0);
  float y = f;
  if (i == 3)
    return iB.y > 0. ? texture(tS, k).xyz : vec3(0);
  vec3 t = r + c * f;
  bool s = n < 500. && n < f;
  vec3 h;
  float l = 0.;
  if (s) {
    l = mix(.2, 1., a((f - n) / 3.));
    vec3 t = r + c * n, e = mix(vec3(0), V(t.xz * (.7 - iS.w * .02)) * (1. - length(t) / 450.), l);
    v = normalize(vec3(e.y, 1., e.x));
    n -= abs(e.z) * l * .6;
    y = n;
    h = mix(vec3(.25, .52, .73), vec3(.15, .62, .83), a(abs(e.z) - l));
  }
  int u = i, g = b;
  if (i == 0)
    e = vec3(.4, .8, 1);
  else {
    vec3 c;
    if (t.y <= -2.9995) {
      c = vec3(0, 1, 0);
      e = vec3(1);
    } else {
      e = vec3(.8);
      switch (u) {
        case 1:
          c = Q(t, f);
          e = mix(vec3(.93, .8, .64),
                  mix(vec3(.69 + texture(tN, t.xz * 1e-4).x, .67, .65), vec3(.38, .52, .23), dot(c, vec3(0, 1, 0))),
                  a(t.y * .5 - 1.)) +
              texture(tN, t.xz * .15).x * .1 + texture(tN, t.xz * .01).y * .1;
          break;
        case 2:
          c = J(t);
          switch (g) {
            case 1: e = vec3(1); break;
            case 2: e = vec3(1, 0, 0); break;
            case 3: e = vec3(.5, 0, 0); break;
            case 4: e = vec3(.2, .1, .2); break;
            case 5: e = vec3(1, .95, .8); break;
            case -1: e = vec3(.64000005, .48000002, .32000002); break;
            default:
              vec4 v =
                  texture(tN, t.xy * .35) * c.z + texture(tN, t.yz * .35) * c.x + texture(tN, t.xz * .35) * c.y - .5;
              e += .125 * (v.x - v.y + v.z - v.w);
              break;
          }
      }
      v = normalize(mix(c, v, l));
    }
  }
  float d = s || u == 2 && g > 0 ? pow(a(dot(iS.xyz, reflect(c, v))), 50.) : 0., x = a(dot(iS.xyz, v)),
        p = a(dot(iS.xyz * vec3(-1, 1, -1), v)), o = x + p * .15;
  if (u == 1)
    o = pow(o * mix(.9, 1.02, x * x), 1. + x * .6);
  o = mix(o, x, l);
  float z = 1.;
  if (i != 0)
    z = G(r + c * y, y, v, 1.);
  if ((iF & 1) != 0 && f < 20.) {
    float e = pow(clamp(dot(iD.xyz, c), 0., 1.), 32.) * smoothstep(10., 0., f);
    o += e * max(dot(v, -c), 0.) * (1. - o);
    z += e * (1. - z);
  }
  e = mix(e, h, l);
  e = (e * vec3(1.065, .95, .85) * o + d) * mix(.38 + (1. - o) * .2, 1., z);
  return Y(e, y, c);
}
void main_c() {
  vec3 e = vec3(0, 0, 1);
  e.xz *= v(FC.x * .049087387 + 3.1415927);
  oC = s(.2 - S(vec3(iP.x, iP.y + (FC.y / 64. - 1.) - .8, iP.z) + normalize(e) * .15));
}
void main_p() {
  vec2 t = FC / 128. - 1. + .001953125;
  vec3 v = normalize(iM * vec3(t.x * -1.5, t.y, 2.4142134));
  float c = P(iP, v, .0046875, .15);
  uint e = floatBitsToUint(c >= 150. ? 150. : c - o);
  oC = vec4(
      float(e >> 24 & 255u) / 255., float(e >> 16 & 255u) / 255., float(e >> 8 & 255u) / 255., float(e & 255u) / 255.);
}
void main_m() {
  vec2 e = FC / (iR * .5) - 1.;
  vec3 t = normalize(iM * vec3(e.x * -1.5, e.y, 2.4142134));
  oC = vec4(Z(iP, t), 1);
}
float z(vec2 c, float v, float i, float e, float o) {
  vec2 t = c - vec2(v, i);
  return a(1. - smoothstep(e - e * o, e, dot(t, t) * 4.));
}
void main_h() {
  vec2 e = FC / (iR * .5) - 1., l = vec2(1.3, 1), f = vec2(0);
  float t = 0., c = 1., o = 1., r = 0., x = 1.;
  while (x < 11.) {
    vec3 a = h(21.1 + e * l * v(x++ * 2.4));
    f += a.yz;
    t += o * (1. - a.x) / (1. + dot(f, f));
    c += o * (.5 - a.x);
    r += o;
    o *= .5;
    l *= 1.8;
  }
  t /= r;
  c *= .5;
  float i = length(e * (1.2 - c + t)) * a(c + .55 - .5 * t * e.x * (1. - e.y * .5)),
        y = z(e, -.45, -.52, 1., 2.3) + z(e, -.6, -.1, 1., 3.3) + z(e, .6, -.7, 1., 5.) + z(e, .84, .84, t, c * 5.);
  i = a(1. - n(i, 1. - mix(0., t * 2., y), .05 + c * .5));
  vec2 m = 1. - abs(e) + t * .04;
  i = n(i, n(m.x, m.y, .3) * 2., .1);
  oC = s(n(t, i, .01) * 1.33 - .045);
}
