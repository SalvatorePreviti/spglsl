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
float a;
float e(float c) {
  return clamp(c, 0., 1.);
}
float c(float e, float t, float c) {
  float v = max(c - abs(e - t), 0.) / c;
  return min(e, t) - v * v * c / 4.;
}
vec4 t(float t) {
  vec4 c = e(t) * vec4(.999998, 254.99948, 65024.867, 160581056);
  c = fract(c);
  c -= c.yzww * vec4(.003921569, .003921569, .003921569, 0);
  return c;
}
float c(vec4 c) {
  return dot(c, vec4(1, .003921569, 1.53787e-5, 6.2273724e-9));
}
vec3 e(vec2 a) {
  vec4 c = texelFetch(tN, ivec2(floor(a)) & 511, 0);
  float v = c.y - c.x, i = c.z - c.x, o = c.w - v - c.z;
  vec2 e = fract(a), r = e * e, t = r * (3. - 2. * e);
  return vec3(c.x + v * t.x + i * t.y + o * t.x * t.y, 30. * r * (e * (e - 2.) + 1.) * (vec2(v, i) + o * t.yx));
}
int r = 0;
float f = 150.;
void c(int e, float c) {
  if (c < a && c != f) {
    r = e;
    f = c;
  }
}
float i(vec3 c, float e) {
  return length(c) - e;
}
float t(vec3 e, vec3 t) {
  vec3 c = abs(e) - t;
  return min(max(c.x, max(c.y, c.z)), 0.) + length(max(c, 0.));
}
float t(vec3 e, float t) {
  vec3 c = abs(e) - t;
  return min(max(c.x, max(c.y, c.z)), 0.) + length(max(c, 0.));
}
float e(vec3 c, float e, float t) {
  float v = length(c.xy) - e;
  return max(v, abs(c.z) - t);
}
float c(vec3 c, vec2 e) {
  return length(vec2(length(c.xz) - e.x, c.y)) - e.y;
}
float c(float a, float c, float i, float o) {
  float t = c * .5, e = floor((a + t) / c), v = mod(a + t, c) - t;
  return e > o ? v + c * (e - o) : e < i ? v + c * (e - i) : v;
}
vec2 c(vec2 c, float v) {
  float e = 3.1415927 / v, t = mod(atan(c.y, c.x) + e, e * 2.) - e;
  return vec2(cos(t), sin(t)) * length(c);
}
vec2 c(vec2 c, float v, float a) {
  float e = 3.1415927 / v, t = mod(atan(c.y, c.x) + e + a, e * 2.) - e;
  return vec2(cos(t), sin(t)) * length(c);
}
float c(float c, float e) {
  return abs(c) - e;
}
mat2 c(float c) {
  float e = cos(c), t = sin(c);
  return mat2(e, t, -t, e);
}
vec3 l(vec3 c) {
  return vec3(c.xy, -c.z);
}
float x(vec3 t) {
  float v = length(t) - .3;
  if (v > .3)
    return v;
  t.xz *= c(-1.2);
  t.yz *= c(-.2);
  return min(e(t, .025, .1), max(i(t - vec3(0, 0, .12), .05), t.z - .12));
}
float n(vec3 v) {
  float a = length(v) - .3;
  if (a > .3)
    return a;
  float c = e(v, .01, .06);
  c = min(c, e(v.yzx + vec3(0, .1, 0), .04, .005));
  c = min(c, t(v - vec3(0, -.01, .04), vec3(.002, .02, .02)));
  return c;
}
float m(vec3 e) {
  float v = t(e - vec3(.03, 0, 0), vec3(.03, .006, .03));
  c(1, v);
  return min(t(e, vec3(.06, .005, .06)), v);
}
float c(vec3 v, float a, float f) {
  float r = length(v) - a * .6;
  if (r > 4.)
    return r;
  v.y += cos(v.z * f / a);
  v.x = abs(v.x);
  float i = t(v - vec3(.2, 0, 0), vec3(.1, .03, a * .55)), o = e(v - vec3(.5, 1, 0), .02, a * .55);
  v.z = c(v.z, .55, -a, a);
  o = min(o, e(v.xzy - vec3(.5, 0, .5), .02, .5));
  c(1, o);
  i = min(i, t(v, vec3(.5, .05, .2)));
  c(-1, i);
  return min(i, o);
}
float y(vec3 v) {
  float i = length(v) - 2.;
  if (i > 1.)
    return i;
  vec3 e = v;
  e.xy *= c(-.25);
  float a = t(e + vec3(.2, .25, 0), vec3(.25, .5, .5)) - .01;
  e -= vec3(-.13, .25, 0);
  e.z = c(e.z, .04, -10., 10.);
  e.x = c(e.x, .04, -3., 3.);
  float o = t(e, .01) - .005;
  c(1, o);
  a = min(min(a, o), t(v - vec3(-.45, .2, 0), vec3(.2, .8, .5)) - .01);
  return a;
}
float z(vec3 t) {
  t.zy *= c(.06);
  t.y += cos(t.z / 20.) * 3.;
  return e(t, .01, 27.5);
}
float u(vec3 v) {
  float a = length(v) - 3.;
  if (a > .5)
    return a;
  v.xz -= vec2(1, -.05);
  v.zx *= c(iA.y * -2.5);
  v.xz += vec2(1, -.05);
  float o = e(v, .99, .05);
  vec3 i = vec3(c(v.xy, 8.), v.z);
  return max(o, -min(t(i - vec3(.5, 0, .1), vec3(.02, .1, .1)), e(i - vec3(0, 0, .1), .02, .1)));
}
float c(vec3 v, float o) {
  float i = length(v) - 1.;
  if (i > 1.)
    return i;
  float a = t(v, vec3(.2, .5, .05));
  a = max(a, -t(v, vec3(.03, .2, 1)));
  v.yz *= c(-1.5707964 * o + .7853982);
  v.z += .2;
  a = min(a, e(v, .02, .2));
  v.z += .2;
  a = min(a, e(v, .03, .05));
  return a;
}
float s(vec3 v) {
  float n = length(v) - 18.;
  if (n > 5.)
    return n;
  v.y -= 9.;
  vec3 o = v;
  o.xz *= c(iB.y);
  o.xy *= c(.5);
  o.y -= 9.;
  float f, r, a;
  f = i(o, 9.);
  r = max(c(f, .01), o.y + 4.5);
  r = min(r, e(o.xzy + vec3(0, 0, 4.5), .1, 4.5));
  r = min(r, i(o, .3));
  v.y += 6.75;
  a = t(v, vec3(2.25, 3.6, 4.5));
  a = min(a,
      min(max(c(e(v.xzy - vec3(2.25, 0, 0), 4.4, 3.5), .1),
              -min(e(v.zyx - vec3(0, 1.8, 0), 1., 100.), e(v - vec3(4.5, 2.3, 0), .4, 100.))),
          e(v.xzy - vec3(2.25, 0, -2.2), 4.4, 2.9)));
  float b = y(v - vec3(3, 1.5, 2)), z = u(v.zyx - vec3(0, 1.8, 6.5)),
        x = c(l(v - vec3(3.7, 2, -4)), clamp(iA.w, 0., 1.));
  v.y -= 2.25;
  a = max(min(a, e(v.xzy, .45000002, 4.7699995)), -f);
  v -= vec3(7, -2.85, 0);
  v.xy *= c(-.5);
  a = min(a, t(v, vec3(1, 1, .8)) - .01);
  float m = min(r, z);
  c(2, x);
  c(3, b);
  c(5, m);
  return min(min(b, a), min(m, x));
}
float h(vec3 v) {
  float r = length(v.xz) - 2.;
  if (r > 3.)
    return r;
  float a = e(v.xzy, .2, .5), f = v.y + iA.z * 4.;
  if ((iF & 8) != 0) {
    float e = n(vec3(v.x, f, v.z) + vec3(1.05, -5.05, 1.05));
    if (e < a) {
      c(2, e);
      a = e;
    }
  }
  float i = t(vec3(c(v.xz, 8.), f).xzy - vec3(1.5, 0, 0), vec3(.1, 5, .2));
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
float g(vec3 i) {
  vec3 v = i.zyx - vec3(11, 1.25, -44);
  float o = length(v) - 8.;
  if (o > 5.)
    return o;
  v.y -= 2.;
  float m = t(v - vec3(-2.7, -1, -1.3), vec3(.35, .5, .5)),
        y = max(c(t(v, vec3(4, 1.6, 2)), .23),
            -min(e(v - vec3(0, .5, 0), .8, 100.), t(v - vec3(4, -.37, 1), vec3(2, 1, .53))));
  vec3 r = v - vec3(4, -.77, .5);
  r.xz *= c(-iA.x * 3.1415927 / 2.);
  float z = t(r - vec3(0, .4, .5), vec3(.05, .99, .52));
  v.x = c(v.x, .3, -10., 10.);
  v.z = abs(v.z);
  float f = e(v.xzy - vec3(0, 2, .5), .01, 1.), l = min(m, z);
  c(1, f);
  c(-1, l);
  float b = min(f, min(y, l)), a = 150.;
  if ((iF & 4) != 0)
    a = x(i - vec3(-42, 3, 11.2));
  if ((iF & 2) != 0)
    a = min(a, n(i.yzx - vec3(2, 7.4, -45.5)));
  if (a < b) {
    c(2, a);
    return a;
  }
  return b;
}
float w(vec3 v) {
  float a = length(v) - 9.;
  if (a > 1.)
    return a;
  v.xz *= c(-.7853982);
  float f = t(v - vec3(-1.5, 1, 5), vec3(1, .2, 3));
  v.y -= iB.w;
  vec3 o = v.xzy - vec3(-2, 0, 2);
  float r = c(i(v - clamp(v, vec3(-6, 0, 0), vec3(6, 0, 0)), 1.7),
      min(e(o - clamp(o, vec3(-.5, 0, 0), vec3(.5, 0, 0)), .4, .5),
          min(t(v - vec3(7.5, 0, 0), vec3(.3, 2, .05)) - .05, t(v - vec3(7.5, 0, 0), vec3(.3, .05, 2)) - .05)),
      .3);
  c(3, r);
  return min(f, r);
}
float d(vec3 v) {
  float b = length(v) - 13.;
  if (b > 3.)
    return b;
  vec3 o = abs(v), x = vec3(o.x, abs(v.y - 4.58), o.z);
  float a = c(l(v.xzy - vec3(1.9, -1.5, .2)) * .5, min(1., (6. - iX.x) * .2)) / .5,
        z = max(t(vec3(v.x, abs(v.y - 3.5) - 3.5, v.z), vec3(6, .2, 6)) - .05,
            max(-t(v - vec3(2, 7, 2), 1.5), -t(v - vec3(5.7, 0, 4), .52)));
  vec3 r = v - vec3(5, 7.6, -2);
  r.xy *= c(.3);
  vec3 m = vec3(v.xy, abs(v.z + 2.));
  a = min(a,
      min(min(e(m.xzy - vec3(-6, 1.1, 8.7), 1., 1.75), e(m.xzy - vec3(-6.5, 1.1, 0), .2, 8.)),
          e(vec3(v.z, abs(v.y - 7.6), v.x) - vec3(-3, .2, 0), .1, 5.)));
  float y = min(min(min(e(vec3(o.xz, v.y) - vec3(5, 5, 0), .5, 8.3), e(x.zyx - vec3(5.3, 3.5, 0), .05, 5.3)),
                    max(e(x - vec3(5.3, 3.5, 0), .05, 5.3), -t(v - vec3(5, .7, 4), .8))),
      t(r, vec3(.5, .6, 1.5)) - .05);
  c(1, y);
  vec3 f = v - vec3(2, 3.59, -.1);
  f.zy *= c(-.7853982);
  f.y -= iX.x;
  a = min(a, t(f, vec3(1, 5.1, .02)) - .05);
  c(5, a);
  float n = min(min(z, a), y);
  vec3 i = r - vec3(0, .8, 0);
  if (length(i) - 1. < 2.) {
    float t = min(min(c(i, vec2(.5, .02)), e(i.xzy + vec3(0, 0, .5), .02, .5)),
        e(vec3(c(i.xz, 5., iB.x), i.y).zyx - vec3(0, 0, .25), .01, .25));
    if (t < n) {
      c(2, t);
      n = t;
    }
  }
  return n;
}
float p(vec3 v) {
  vec3 t = v.zyx - vec3(4, -1, 17);
  t.zy *= c(-.19);
  t.z -= 19. - iA.w;
  return min(c(t, 21., 0.), e(t.xzy + vec3(0, 10.5, 6), .15, 5.));
}
float i(vec3 l) {
  vec3 v = l - vec3(8.7, 9.3, 37);
  float n = length(v.xz) - 5.;
  if (n > 4.)
    return n;
  vec3 r = vec3(c(v.xz, 6.), v.y).xzy, z = vec3(r.x, c(v.y, 1.5, -3., 7.), r.z);
  float u = max(
      max(min(e(v.xzy, 1.1, 12.), max(c(e(v.xzy - vec3(0, 0, 14), 4., 2.), .2), -t(r - vec3(4, 14, 0), vec3(1, 1, 2)))),
          -min(e(v.xzy, 1., 13.), t(z - vec3(1, 0, 0), vec3(.2, .3, .13)))),
      -t(v + vec3(0, 7, 1), vec3(.8, 1.2, .8)));
  vec3 a = vec3(v.x, v.y - iB.z, v.z);
  float o = e(a.xzy, 1., 11.);
  a.y = c(a.y, 1.5, -7., 7.);
  o = max(o, -c(a, vec2(1, .01)));
  vec3 f = vec3(v.x - .8, c(v.y - 12.7, 20.5, -1., 0.), v.z + .9);
  float b = i(f - vec3(0, .5, 0), .06), x = min(e(f.xzy, .05, .5), min(t(f - vec3(0, .5, 0), vec3(.05, .1, .1)), b));
  c(2, b);
  c(1, x);
  c(5, o);
  float y = min(min(u, min(x, o)), t(v + vec3(0, 10.3, 3), vec3(1.1, 2, 3)));
  if ((iF & 16) != 0) {
    float e = m(l - vec3(12.15, 22.31, 38.65));
    if (e < y) {
      c(2, e);
      return e;
    }
  }
  return y;
}
vec2 b;
float c(vec3 e, vec3 i, vec2 v) {
  e -= i;
  float a = length(e) - 2.;
  if (a > .5)
    return a;
  e.xz *= c(1.5707964);
  b = (v - e.xy) / (v * 2.);
  float o = t(e, vec3(v, .01));
  return o;
}
float t(vec3 t) {
  vec3 e = abs(vec3(t.x, t.y + 3., t.z)) - vec3(60, 0, 39);
  if (e.x < 0. && e.z < 0.)
    e.y -= c(textureLod(tH, t.xz / vec2(120, 78) + .5, 0.)) * 19.;
  return min(e.y, 0.) + length(max(e, 0.));
}
float o(vec3 e) {
  float o = c(e - vec3(45, 1.7, 22.4), 10., 2.), r = s(e - vec3(2, 10, 2)), f = h(e - vec3(47.5, 3.5, 30.5)), l = g(e);
  vec3 t = e - vec3(26, 5, -58);
  t.xz *= c(1.9707963);
  float n = d(t), b = p(t), v = z(t.zyx - vec3(-2, 9.7, 32.5)), x = i(e), m = w(e - vec3(-46, -.5, -30)),
        a = min(min(min(o, r), min(f, l)), min(n, min(b, min(x, m))));
  if (v < a) {
    c(1, v);
    return v;
  }
  c(0, a);
  return a;
}
int v = 0;
float e(vec3 i) {
  float e = t(i);
  if (e <= a) {
    v = 1;
    return e;
  }
  float r = o(i), f = c(i, vec3(4.76, 14.42, 4), vec2(.45, .29)), l = min(f, r);
  if (e < l) {
    v = 1;
    return e;
  }
  v = f <= r ? 3 : 2;
  return l;
}
vec3 c(vec3 c) {
  return normalize(vec3(o(c + vec3(.001, 0, 0)), o(c + vec3(0, .001, 0)), o(c + vec3(0, 0, .001))) - o(c));
}
vec3 e(vec3 c, float v) {
  vec2 e = vec2(mix(.03, .001, min(v / 120., 1.)), 0);
  return normalize(vec3(t(c + e.xyy), t(c + e.yxy), t(c + e.yyx)) - t(c));
}
float c(vec3 i, vec3 o, float r, float c) {
  float t = r;
  for (int n = 0;; n++) {
    vec3 l = i + o * c;
    a = r * max(c, 1.);
    if (l.y <= -2.9995 || c >= 150.) {
      float c = (-3. - i.y) / o.y;
      if (c >= 0. && c < 500.) {
        v = 1;
        return c;
      }
      break;
    }
    if (l.y > 45.)
      break;
    float f = e(l);
    if (f < 0.) {
      c -= t;
      f = t / 2.;
    }
    c += f;
    if (f <= a || n >= 100)
      return c;
    t = f;
  }
  v = 0;
  return 500.;
}
float c(vec3 e, float v, vec3 i, float t) {
  float c = clamp(v * .005, .01, .1);
  e = e + i * c;
  float r = iS.w - a * 2.;
  for (float v = 1.; v < 50.; v++) {
    vec3 i = e + iS.xyz * c;
    if (c >= 80. || i.y > 45. || i.y < r || length(e) >= 150.)
      break;
    float f = o(i), l = max(a, .01 * min(1., c) + v * 2e-4);
    if (f <= l)
      return 0.;
    t = min(t, 85. * f / c);
    if (t < .078)
      return 0.;
    c += f + a;
  }
  return t;
}
float e(vec3 e, vec3 t) {
  float c = (iS.w - e.y) / t.y;
  return min(c >= 0. ? c : 500., 500.);
}
vec3 c(vec2 c) {
  vec3 v = vec3(0);
  float a = 0., t = 1., o = 0., i = 1. - length(iP.xz - c) / 500., r = 5. * i * i;
  for (float i = 0.; i < r; ++i) {
    c += iD.w * .5;
    o *= -.75;
    vec3 f = e(c + sin(c.yx * .5 + iD.w * .5) * .5);
    v += f * t;
    c += f.yz * .43;
    c *= 2.;
    a += t;
    t *= .75;
  }
  return v / a;
}
vec3 c(vec3 t, float c, vec3 v) {
  float a = min(c / 500., 1.), i = e(pow(a, 3.5) + 1. - exp(-c * .005)), o = max(dot(v, iS.xyz), 0.);
  vec3 r = mix(vec3(.4, .8, 1), vec3(1.065, .95, .85), pow(o, 10.));
  return mix(t, r, i);
}
vec3 c(vec3 m, vec3 i) {
  vec4 z = texelFetch(tP, ivec2(FC * 256. / iR), 0);
  float p = uintBitsToFloat(uint(z.x * 255.) << 24 | uint(z.y * 255.) << 16 | uint(z.z * 255.) << 8 | uint(z.w * 255.)),
        l = c(m, i, .001, p), x = e(m, i);
  vec3 t, o = vec3(0, 1, 0);
  float u = l;
  if (v == 3)
    return iB.y > 0. ? texture(tS, b).xyz : vec3(0);
  vec3 a = m + i * l;
  bool g = x < 500. && x < l;
  vec3 w;
  float n = 0.;
  if (g) {
    n = mix(.2, 1., e((l - x) / 3.));
    vec3 v = m + i * x, t = mix(vec3(0), c(v.xz * (.7 - iS.w * .02)) * (1. - length(v) / 450.), n);
    o = normalize(vec3(t.y, 1., t.x));
    x -= abs(t.z) * n * .6;
    u = x;
    w = mix(vec3(.25, .52, .73), vec3(.15, .62, .83), e(abs(t.z) - n));
  }
  int h = v, d = r;
  if (v == 0)
    t = vec3(.4, .8, 1);
  else {
    vec3 v;
    if (a.y <= -2.9995) {
      v = vec3(0, 1, 0);
      t = vec3(1);
    } else {
      t = vec3(.8);
      switch (h) {
        case 1:
          v = e(a, l);
          t = mix(vec3(.93, .8, .64),
                  mix(vec3(.69 + texture(tN, a.xz * 1e-4).x, .67, .65), vec3(.38, .52, .23), dot(v, vec3(0, 1, 0))),
                  e(a.y * .5 - 1.)) +
              texture(tN, a.xz * .15).x * .1 + texture(tN, a.xz * .01).y * .1;
          break;
        case 2:
          v = c(a);
          switch (d) {
            case 1: t = vec3(1); break;
            case 2: t = vec3(1, 0, 0); break;
            case 3: t = vec3(.5, 0, 0); break;
            case 4: t = vec3(.2, .1, .2); break;
            case 5: t = vec3(1, .95, .8); break;
            case -1: t = vec3(.64000005, .48000002, .32000002); break;
            default:
              vec4 c =
                  texture(tN, a.xy * .35) * v.z + texture(tN, a.yz * .35) * v.x + texture(tN, a.xz * .35) * v.y - .5;
              t += .125 * (c.x - c.y + c.z - c.w);
              break;
          }
      }
      o = normalize(mix(v, o, n));
    }
  }
  float F = g || h == 2 && d > 0 ? pow(e(dot(iS.xyz, reflect(i, o))), 50.) : 0., y = e(dot(iS.xyz, o)),
        C = e(dot(iS.xyz * vec3(-1, 1, -1), o)), f = y + C * .15;
  if (h == 1)
    f = pow(f * mix(.9, 1.02, y * y), 1. + y * .6);
  f = mix(f, y, n);
  float s = 1.;
  if (v != 0)
    s = c(m + i * u, u, o, 1.);
  if ((iF & 1) != 0 && l < 20.) {
    float c = pow(clamp(dot(iD.xyz, i), 0., 1.), 32.) * smoothstep(10., 0., l);
    f += c * max(dot(o, -i), 0.) * (1. - f);
    s += c * (1. - s);
  }
  t = mix(t, w, n);
  t = (t * vec3(1.065, .95, .85) * f + F) * mix(.38 + (1. - f) * .2, 1., s);
  return c(t, u, i);
}
void main_c() {
  vec3 v = vec3(0, 0, 1);
  v.xz *= c(FC.x * .049087387 + 3.1415927);
  oC = t(.2 - e(vec3(iP.x, iP.y + (FC.y / 64. - 1.) - .8, iP.z) + normalize(v) * .15));
}
void main_p() {
  vec2 t = FC / 128. - 1. + .001953125;
  vec3 i = normalize(iM * vec3(t.x * -1.5, t.y, 2.4142134));
  float v = c(iP, i, .0046875, .15);
  uint e = floatBitsToUint(v >= 150. ? 150. : v - a);
  oC = vec4(
      float(e >> 24 & 255u) / 255., float(e >> 16 & 255u) / 255., float(e >> 8 & 255u) / 255., float(e & 255u) / 255.);
}
void main_m() {
  vec2 e = FC / (iR * .5) - 1.;
  vec3 t = normalize(iM * vec3(e.x * -1.5, e.y, 2.4142134));
  oC = vec4(c(iP, t), 1);
}
float c(vec2 v, float a, float i, float c, float o) {
  vec2 t = v - vec2(a, i);
  return e(1. - smoothstep(c - c * o, c, dot(t, t) * 4.));
}
void main_h() {
  vec2 v = FC / (iR * .5) - 1., l = vec2(1.3, 1), f = vec2(0);
  float a = 0., i = 1., r = 1., n = 0., b = 1.;
  while (b < 11.) {
    vec3 t = e(21.1 + v * l * c(b++ * 2.4));
    f += t.yz;
    a += r * (1. - t.x) / (1. + dot(f, f));
    i += r * (.5 - t.x);
    n += r;
    r *= .5;
    l *= 1.8;
  }
  a /= n;
  i *= .5;
  float o = length(v * (1.2 - i + a)) * e(i + .55 - .5 * a * v.x * (1. - v.y * .5)),
        m = c(v, -.45, -.52, 1., 2.3) + c(v, -.6, -.1, 1., 3.3) + c(v, .6, -.7, 1., 5.) + c(v, .84, .84, a, i * 5.);
  o = e(1. - c(o, 1. - mix(0., a * 2., m), .05 + i * .5));
  vec2 x = 1. - abs(v) + a * .04;
  o = c(o, c(x.x, x.y, .3) * 2., .1);
  oC = t(c(a, o, .01) * 1.33 - .045);
}
