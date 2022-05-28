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
float v;
float c(float e) {
  return clamp(e, 0., 1.);
}
float e(float c, float t, float e) {
  float v = max(e - abs(c - t), 0.) / e;
  return min(c, t) - v * v * e / 4.;
}
vec4 a(float t) {
  vec4 e = c(t) * vec4(.999998, 254.99948, 65024.867, 160581056);
  e = fract(e), e -= e.yzww * vec4(.003921569, .003921569, .003921569, 0);
  return e;
}
float e(vec4 e) {
  return dot(e, vec4(1, .003921569, 1.53787e-5, 6.2273724e-9));
}
vec3 e(vec2 a) {
  vec4 e = texelFetch(tN, ivec2(floor(a)) & 511, 0);
  float v = e.y - e.x, r = e.z - e.x, i = e.w - v - e.z;
  vec2 c = fract(a), m = c * c, t = m * (3. - 2. * c);
  return vec3(e.x + v * t.x + r * t.y + i * t.x * t.y, 30. * m * (c * (c - 2.) + 1.) * (vec2(v, r) + i * t.yx));
}
int r = 0;
float i = 150.;
void e(int c, float e) {
  if (e < v && e != i)
    r = c, i = e;
}
float e(vec3 e, float c) {
  return length(e) - c;
}
float e(vec3 c, vec3 t) {
  vec3 e = abs(c) - t;
  return min(max(e.x, max(e.y, e.z)), 0.) + length(max(e, 0.));
}
float c(vec3 c, float t) {
  vec3 e = abs(c) - t;
  return min(max(e.x, max(e.y, e.z)), 0.) + length(max(e, 0.));
}
float e(vec3 e, float c, float t) {
  float v = length(e.xy) - c;
  return max(v, abs(e.z) - t);
}
float e(vec3 e, vec2 c) {
  return length(vec2(length(e.xz) - c.x, e.y)) - c.y;
}
float e(float a, float e, float r, float i) {
  float t = e * .5, c = floor((a + t) / e), v = mod(a + t, e) - t;
  return c > i ? v + e * (c - i) : c < r ? v + e * (c - r) : v;
}
vec2 e(vec2 e, float v) {
  float c = 3.1415927 / v, t = mod(atan(e.y, e.x) + c, c * 2.) - c;
  return vec2(cos(t), sin(t)) * length(e);
}
vec2 e(vec2 e, float v, float a) {
  float c = 3.1415927 / v, t = mod(atan(e.y, e.x) + c + a, c * 2.) - c;
  return vec2(cos(t), sin(t)) * length(e);
}
float e(float e, float c) {
  return abs(e) - c;
}
mat2 e(float e) {
  float c = cos(e), t = sin(e);
  return mat2(c, t, -t, c);
}
vec3 a(vec3 e) {
  return vec3(e.xy, -e.z);
}
float f(vec3 c) {
  float t = length(c) - .3;
  if (t > .3)
    return t;
  c.xz *= e(-1.2), c.yz *= e(-.2);
  return min(e(c, .025, .1), max(e(c - vec3(0, 0, .12), .05), c.z - .12));
}
float m(vec3 t) {
  float v = length(t) - .3;
  if (v > .3)
    return v;
  float c = e(t, .01, .06);
  c = min(c, e(t.yzx + vec3(0, .1, 0), .04, .005)), c = min(c, e(t - vec3(0, -.01, .04), vec3(.002, .02, .02)));
  return c;
}
float l(vec3 c) {
  float t = e(c - vec3(.03, 0, 0), vec3(.03, .006, .03));
  e(1, t);
  return min(e(c, vec3(.06, .005, .06)), t);
}
float c(vec3 c, float t, float i) {
  float r = length(c) - t * .6;
  if (r > 4.)
    return r;
  c.y += cos(c.z * i / t), c.x = abs(c.x);
  float v = e(c - vec3(.2, 0, 0), vec3(.1, .03, t * .55)), a = e(c - vec3(.5, 1, 0), .02, t * .55);
  c.z = e(c.z, .55, -t, t), a = min(a, e(c.xzy - vec3(.5, 0, .5), .02, .5));
  e(1, a);
  v = min(v, e(c, vec3(.5, .05, .2)));
  e(-1, v);
  return min(v, a);
}
float n(vec3 v) {
  float r = length(v) - 2.;
  if (r > 1.)
    return r;
  vec3 t = v;
  t.xy *= e(-.25);
  float a = e(t + vec3(.2, .25, 0), vec3(.25, .5, .5)) - .01;
  t -= vec3(-.13, .25, 0), t.z = e(t.z, .04, -10., 10.), t.x = e(t.x, .04, -3., 3.);
  float i = c(t, .01) - .005;
  e(1, i);
  a = min(min(a, i), e(v - vec3(-.45, .2, 0), vec3(.2, .8, .5)) - .01);
  return a;
}
float x(vec3 c) {
  c.zy *= e(.06), c.y += cos(c.z / 20.) * 3.;
  return e(c, .01, 27.5);
}
float y(vec3 c) {
  float t = length(c) - 3.;
  if (t > .5)
    return t;
  c.xz -= vec2(1, -.05), c.zx *= e(iA.y * -2.5), c.xz += vec2(1, -.05);
  float a = e(c, .99, .05);
  vec3 v = vec3(e(c.xy, 8.), c.z);
  return max(a, -min(e(v - vec3(.5, 0, .1), vec3(.02, .1, .1)), e(v - vec3(0, 0, .1), .02, .1)));
}
float a(vec3 c, float a) {
  float v = length(c) - 1.;
  if (v > 1.)
    return v;
  float t = e(c, vec3(.2, .5, .05));
  t = max(t, -e(c, vec3(.03, .2, 1))), c.yz *= e(-1.5707964 * a + .7853982), c.z += .2, t = min(t, e(c, .02, .2)),
  c.z += .2, t = min(t, e(c, .03, .05));
  return t;
}
float z(vec3 c) {
  float m = length(c) - 18.;
  if (m > 5.)
    return m;
  c.y -= 9.;
  vec3 v = c;
  v.xz *= e(iB.y), v.xy *= e(.5), v.y -= 9.;
  float i, r, t;
  i = e(v, 9.), r = max(e(i, .01), v.y + 4.5), r = min(r, e(v.xzy + vec3(0, 0, 4.5), .1, 4.5)), r = min(r, e(v, .3)),
  c.y += 6.75, t = e(c, vec3(2.25, 3.6, 4.5)),
  t = min(t,
      min(max(e(e(c.xzy - vec3(2.25, 0, 0), 4.4, 3.5), .1),
              -min(e(c.zyx - vec3(0, 1.8, 0), 1., 100.), e(c - vec3(4.5, 2.3, 0), .4, 100.))),
          e(c.xzy - vec3(2.25, 0, -2.2), 4.4, 2.9)));
  float b = n(c - vec3(3, 1.5, 2)), l = y(c.zyx - vec3(0, 1.8, 6.5)),
        o = a(a(c - vec3(3.7, 2, -4)), clamp(iA.w, 0., 1.));
  c.y -= 2.25, t = max(min(t, e(c.xzy, .45000002, 4.7699995)), -i), c -= vec3(7, -2.85, 0), c.xy *= e(-.5),
               t = min(t, e(c, vec3(1, 1, .8)) - .01);
  float f = min(r, l);
  e(2, o);
  e(3, b);
  e(5, f);
  return min(min(b, t), min(f, o));
}
float u(vec3 c) {
  float r = length(c.xz) - 2.;
  if (r > 3.)
    return r;
  float t = e(c.xzy, .2, .5), i = c.y + iA.z * 4.;
  if ((iF & 8) != 0) {
    float v = m(vec3(c.x, i, c.z) + vec3(1.05, -5.05, 1.05));
    if (v < t) {
      e(2, v);
      t = v;
    }
  }
  float v = e(vec3(e(c.xz, 8.), i).xzy - vec3(1.5, 0, 0), vec3(.1, 5, .2));
  if (v < t) {
    e(4, v);
    t = v;
  }
  float a = e(c.xzy + vec3(0, 0, clamp(iA.z, 0., .02)), .05, .53);
  if (a < t) {
    e(1, a);
    t = a;
  }
  return t;
}
float s(vec3 v) {
  vec3 c = v.zyx - vec3(11, 1.25, -44);
  float a = length(c) - 8.;
  if (a > 5.)
    return a;
  c.y -= 2.;
  float l = e(c - vec3(-2.7, -1, -1.3), vec3(.35, .5, .5)),
        n = max(e(e(c, vec3(4, 1.6, 2)), .23),
            -min(e(c - vec3(0, .5, 0), .8, 100.), e(c - vec3(4, -.37, 1), vec3(2, 1, .53))));
  vec3 r = c - vec3(4, -.77, .5);
  r.xz *= e(-iA.x * 3.1415927 / 2.);
  float x = e(r - vec3(0, .4, .5), vec3(.05, .99, .52));
  c.x = e(c.x, .3, -10., 10.), c.z = abs(c.z);
  float i = e(c.xzy - vec3(0, 2, .5), .01, 1.), b = min(l, x);
  e(1, i);
  e(-1, b);
  float o = min(i, min(n, b)), t = 150.;
  if ((iF & 4) != 0)
    t = f(v - vec3(-42, 3, 11.2));
  if ((iF & 2) != 0)
    t = min(t, m(v.yzx - vec3(2, 7.4, -45.5)));
  if (t < o) {
    e(2, t);
    return t;
  }
  return o;
}
float h(vec3 c) {
  float t = length(c) - 9.;
  if (t > 1.)
    return t;
  c.xz *= e(-.7853982);
  float r = e(c - vec3(-1.5, 1, 5), vec3(1, .2, 3));
  c.y -= iB.w;
  vec3 v = c.xzy - vec3(-2, 0, 2);
  float a = e(e(c - clamp(c, vec3(-6, 0, 0), vec3(6, 0, 0)), 1.7),
      min(e(v - clamp(v, vec3(-.5, 0, 0), vec3(.5, 0, 0)), .4, .5),
          min(e(c - vec3(7.5, 0, 0), vec3(.3, 2, .05)) - .05, e(c - vec3(7.5, 0, 0), vec3(.3, .05, 2)) - .05)),
      .3);
  e(3, a);
  return min(r, a);
}
float g(vec3 t) {
  float f = length(t) - 13.;
  if (f > 3.)
    return f;
  vec3 i = abs(t), l = vec3(i.x, abs(t.y - 4.58), i.z);
  float v = a(a(t.xzy - vec3(1.9, -1.5, .2)) * .5, min(1., (6. - iX.x) * .2)) / .5,
        y = max(e(vec3(t.x, abs(t.y - 3.5) - 3.5, t.z), vec3(6, .2, 6)) - .05,
            max(-c(t - vec3(2, 7, 2), 1.5), -c(t - vec3(5.7, 0, 4), .52)));
  vec3 m = t - vec3(5, 7.6, -2);
  m.xy *= e(.3);
  vec3 n = vec3(t.xy, abs(t.z + 2.));
  v = min(v,
      min(min(e(n.xzy - vec3(-6, 1.1, 8.7), 1., 1.75), e(n.xzy - vec3(-6.5, 1.1, 0), .2, 8.)),
          e(vec3(t.z, abs(t.y - 7.6), t.x) - vec3(-3, .2, 0), .1, 5.)));
  float x = min(min(min(e(vec3(i.xz, t.y) - vec3(5, 5, 0), .5, 8.3), e(l.zyx - vec3(5.3, 3.5, 0), .05, 5.3)),
                    max(e(l - vec3(5.3, 3.5, 0), .05, 5.3), -c(t - vec3(5, .7, 4), .8))),
      e(m, vec3(.5, .6, 1.5)) - .05);
  e(1, x);
  vec3 b = t - vec3(2, 3.59, -.1);
  b.zy *= e(-.7853982), b.y -= iX.x, v = min(v, e(b, vec3(1, 5.1, .02)) - .05);
  e(5, v);
  float o = min(min(y, v), x);
  vec3 r = m - vec3(0, .8, 0);
  if (length(r) - 1. < 2.) {
    float c = min(min(e(r, vec2(.5, .02)), e(r.xzy + vec3(0, 0, .5), .02, .5)),
        e(vec3(e(r.xz, 5., iB.x), r.y).zyx - vec3(0, 0, .25), .01, .25));
    if (c < o) {
      e(2, c);
      o = c;
    }
  }
  return o;
}
float w(vec3 v) {
  vec3 t = v.zyx - vec3(4, -1, 17);
  t.zy *= e(-.19), t.z -= 19. - iA.w;
  return min(c(t, 21., 0.), e(t.xzy + vec3(0, 10.5, 6), .15, 5.));
}
float d(vec3 i) {
  vec3 c = i - vec3(8.7, 9.3, 37);
  float m = length(c.xz) - 5.;
  if (m > 4.)
    return m;
  vec3 a = vec3(e(c.xz, 6.), c.y).xzy, n = vec3(a.x, e(c.y, 1.5, -3., 7.), a.z);
  float x = max(
      max(min(e(c.xzy, 1.1, 12.), max(e(e(c.xzy - vec3(0, 0, 14), 4., 2.), .2), -e(a - vec3(4, 14, 0), vec3(1, 1, 2)))),
          -min(e(c.xzy, 1., 13.), e(n - vec3(1, 0, 0), vec3(.2, .3, .13)))),
      -e(c + vec3(0, 7, 1), vec3(.8, 1.2, .8)));
  vec3 t = vec3(c.x, c.y - iB.z, c.z);
  float v = e(t.xzy, 1., 11.);
  t.y = e(t.y, 1.5, -7., 7.), v = max(v, -e(t, vec2(1, .01)));
  vec3 r = vec3(c.x - .8, e(c.y - 12.7, 20.5, -1., 0.), c.z + .9);
  float b = e(r - vec3(0, .5, 0), .06), o = min(e(r.xzy, .05, .5), min(e(r - vec3(0, .5, 0), vec3(.05, .1, .1)), b));
  e(2, b);
  e(1, o);
  e(5, v);
  float f = min(min(x, min(o, v)), e(c + vec3(0, 10.3, 3), vec3(1.1, 2, 3)));
  if ((iF & 16) != 0) {
    float c = l(i - vec3(12.15, 22.31, 38.65));
    if (c < f) {
      e(2, c);
      return c;
    }
  }
  return f;
}
vec2 b;
float e(vec3 c, vec3 a, vec2 t) {
  c -= a;
  float v = length(c) - 2.;
  if (v > .5)
    return v;
  c.xz *= e(1.5707964), b = (t - c.xy) / (t * 2.);
  float r = e(c, vec3(t, .01));
  return r;
}
float c(vec3 t) {
  vec3 c = abs(vec3(t.x, t.y + 3., t.z)) - vec3(60, 0, 39);
  if (c.x < 0. && c.z < 0.)
    c.y -= e(textureLod(tH, t.xz / vec2(120, 78) + .5, 0.)) * 19.;
  return min(c.y, 0.) + length(max(c, 0.));
}
float e(vec3 t) {
  float i = c(t - vec3(45, 1.7, 22.4), 10., 2.), m = z(t - vec3(2, 10, 2)), b = u(t - vec3(47.5, 3.5, 30.5)), o = s(t);
  vec3 v = t - vec3(26, 5, -58);
  v.xz *= e(1.9707963);
  float f = g(v), l = w(v), a = x(v.zyx - vec3(-2, 9.7, 32.5)), n = d(t), y = h(t - vec3(-46, -.5, -30)),
        r = min(min(min(i, m), min(b, o)), min(f, min(l, min(n, y))));
  if (a < r) {
    e(1, a);
    return a;
  }
  e(0, r);
  return r;
}
int t = 0;
float o(vec3 r) {
  float a = c(r);
  if (a <= v) {
    t = 1;
    return a;
  }
  float i = e(r), m = e(r, vec3(4.76, 14.42, 4), vec2(.45, .29)), b = min(m, i);
  if (a < b) {
    t = 1;
    return a;
  }
  t = m <= i ? 3 : 2;
  return b;
}
vec3 F(vec3 c) {
  return normalize(vec3(e(c + vec3(.001, 0, 0)), e(c + vec3(0, .001, 0)), e(c + vec3(0, 0, .001))) - e(c));
}
vec3 m(vec3 e, float v) {
  vec2 t = vec2(mix(.03, .001, min(v / 120., 1.)), 0);
  return normalize(vec3(c(e + t.xyy), c(e + t.yxy), c(e + t.yyx)) - c(e));
}
float e(vec3 a, vec3 r, float i, float e) {
  float c = i;
  for (int f = 0;; f++) {
    vec3 b = a + r * e;
    v = i * max(e, 1.);
    if (b.y <= -2.9995 || e >= 150.) {
      float e = (-3. - a.y) / r.y;
      if (e >= 0. && e < 500.) {
        t = 1;
        return e;
      }
      break;
    }
    if (b.y > 45.)
      break;
    float m = o(b);
    if (m < 0.)
      e -= c, m = c / 2.;
    e += m;
    if (m <= v || f >= 100)
      return e;
    c = m;
  }
  t = 0;
  return 500.;
}
float e(vec3 t, float r, vec3 i, float a) {
  float c = clamp(r * .005, .01, .1);
  t = t + i * c;
  float m = iS.w - v * 2.;
  for (float r = 1.; r < 50.; r++) {
    vec3 i = t + iS.xyz * c;
    if (c >= 80. || i.y > 45. || i.y < m || length(t) >= 150.)
      break;
    float b = e(i), o = max(v, .01 * min(1., c) + r * 2e-4);
    if (b <= o)
      return 0.;
    a = min(a, 85. * b / c);
    if (a < .078)
      return 0.;
    c += b + v;
  }
  return a;
}
float c(vec3 c, vec3 t) {
  float e = (iS.w - c.y) / t.y;
  return min(e >= 0. ? e : 500., 500.);
}
vec3 c(vec2 c) {
  vec3 v = vec3(0);
  float a = 0., t = 1., i = 0., r = 1. - length(iP.xz - c) / 500., m = 5. * r * r;
  for (float r = 0.; r < m; ++r) {
    c += iD.w * .5, i *= -.75;
    vec3 b = e(c + sin(c.yx * .5 + iD.w * .5) * .5);
    v += b * t, c += b.yz * .43, c *= 2., a += t, t *= .75;
  }
  return v / a;
}
vec3 e(vec3 t, float e, vec3 v) {
  float a = min(e / 500., 1.), r = c(pow(a, 3.5) + 1. - exp(-e * .005)), i = max(dot(v, iS.xyz), 0.);
  vec3 m = mix(vec3(.4, .8, 1), vec3(1.065, .95, .85), pow(i, 10.));
  return mix(t, m, r);
}
vec3 a(vec3 y, vec3 i) {
  vec4 u = texelFetch(tP, ivec2(FC * 256. / iR), 0);
  float C = uintBitsToFloat(uint(u.x * 255.) << 24 | uint(u.y * 255.) << 16 | uint(u.z * 255.) << 8 | uint(u.w * 255.)),
        l = e(y, i, .001, C), x = c(y, i);
  vec3 v, o = vec3(0, 1, 0);
  float s = l;
  if (t == 3)
    return iB.y > 0. ? texture(tS, b).xyz : vec3(0);
  vec3 a = y + i * l;
  bool w = x < 500. && x < l;
  vec3 d;
  float n = 0.;
  if (w) {
    n = mix(.2, 1., c((l - x) / 3.));
    vec3 t = y + i * x, e = mix(vec3(0), c(t.xz * (.7 - iS.w * .02)) * (1. - length(t) / 450.), n);
    o = normalize(vec3(e.y, 1., e.x)), x -= abs(e.z) * n * .6, s = x,
    d = mix(vec3(.25, .52, .73), vec3(.15, .62, .83), c(abs(e.z) - n));
  }
  int g = t, p = r;
  if (t == 0)
    v = vec3(.4, .8, 1);
  else {
    vec3 e;
    if (a.y <= -2.9995)
      e = vec3(0, 1, 0), v = vec3(1);
    else {
      v = vec3(.8);
      switch (g) {
        case 1:
          e = m(a, l),
          v = mix(vec3(.93, .8, .64),
                  mix(vec3(.69 + texture(tN, a.xz * 1e-4).x, .67, .65), vec3(.38, .52, .23), dot(e, vec3(0, 1, 0))),
                  c(a.y * .5 - 1.)) +
              texture(tN, a.xz * .15).x * .1 + texture(tN, a.xz * .01).y * .1;
          break;
        case 2:
          e = F(a);
          switch (p) {
            case 1: v = vec3(1); break;
            case 2: v = vec3(1, 0, 0); break;
            case 3: v = vec3(.5, 0, 0); break;
            case 4: v = vec3(.2, .1, .2); break;
            case 5: v = vec3(1, .95, .8); break;
            case -1: v = vec3(.64000005, .48000002, .32000002); break;
            default:
              vec4 c =
                  texture(tN, a.xy * .35) * e.z + texture(tN, a.yz * .35) * e.x + texture(tN, a.xz * .35) * e.y - .5;
              v += .125 * (c.x - c.y + c.z - c.w);
              break;
          }
      }
      o = normalize(mix(e, o, n));
    }
  }
  float k = w || g == 2 && p > 0 ? pow(c(dot(iS.xyz, reflect(i, o))), 50.) : 0., z = c(dot(iS.xyz, o)),
        S = c(dot(iS.xyz * vec3(-1, 1, -1), o)), f = z + S * .15;
  if (g == 1)
    f = pow(f * mix(.9, 1.02, z * z), 1. + z * .6);
  f = mix(f, z, n);
  float h = 1.;
  if (t != 0)
    h = e(y + i * s, s, o, 1.);
  if ((iF & 1) != 0 && l < 20.) {
    float e = pow(clamp(dot(iD.xyz, i), 0., 1.), 32.) * smoothstep(10., 0., l);
    f += e * max(dot(o, -i), 0.) * (1. - f), h += e * (1. - h);
  }
  v = mix(v, d, n), v = (v * vec3(1.065, .95, .85) * f + k) * mix(.38 + (1. - f) * .2, 1., h);
  return e(v, s, i);
}
void main_c() {
  vec3 c = vec3(0, 0, 1);
  c.xz *= e(FC.x * .049087387 + 3.1415927),
      oC = a(.2 - o(vec3(iP.x, iP.y + (FC.y / 64. - 1.) - .8, iP.z) + normalize(c) * .15));
}
void main_p() {
  vec2 t = FC / 128. - 1. + .001953125;
  vec3 r = normalize(iM * vec3(t.x * -1.5, t.y, 2.4142134));
  float a = e(iP, r, .0046875, .15);
  uint c = floatBitsToUint(a >= 150. ? 150. : a - v);
  oC = vec4(
      float(c >> 24 & 255u) / 255., float(c >> 16 & 255u) / 255., float(c >> 8 & 255u) / 255., float(c & 255u) / 255.);
}
void main_m() {
  vec2 e = FC / (iR * .5) - 1.;
  vec3 c = normalize(iM * vec3(e.x * -1.5, e.y, 2.4142134));
  oC = vec4(a(iP, c), 1);
}
float e(vec2 v, float a, float r, float e, float i) {
  vec2 t = v - vec2(a, r);
  return c(1. - smoothstep(e - e * i, e, dot(t, t) * 4.));
}
void main_h() {
  vec2 t = FC / (iR * .5) - 1., o = vec2(1.3, 1), b = vec2(0);
  float v = 0., r = 1., m = 1., f = 0., l = 1.;
  while (l < 11.) {
    vec3 c = e(21.1 + t * o * e(l++ * 2.4));
    b += c.yz, v += m * (1. - c.x) / (1. + dot(b, b)), r += m * (.5 - c.x), f += m, m *= .5, o *= 1.8;
  }
  v /= f, r *= .5;
  float i = length(t * (1.2 - r + v)) * c(r + .55 - .5 * v * t.x * (1. - t.y * .5)),
        x = e(t, -.45, -.52, 1., 2.3) + e(t, -.6, -.1, 1., 3.3) + e(t, .6, -.7, 1., 5.) + e(t, .84, .84, v, r * 5.);
  i = c(1. - e(i, 1. - mix(0., v * 2., x), .05 + r * .5));
  vec2 n = 1. - abs(t) + v * .04;
  i = e(i, e(n.x, n.y, .3) * 2., .1), oC = a(e(v, i, .01) * 1.33 - .045);
}
