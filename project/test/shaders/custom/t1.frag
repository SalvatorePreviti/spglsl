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

float e(float e) {
  return clamp(e, 0., 1.);
}

float t(float t, float c, float e) {
  float v = max(e - abs(t - c), 0.) / e;
  return min(t, c) - v * v * e / 4.;
}

vec4 t(float c) {
  vec4 t = e(c) * vec4(.999998, 254.99948, 65024.867, 160581056);
  t = fract(t), t -= t.yzww * vec4(.003921569, .003921569, .003921569, 0);
  return t;
}

float t(vec4 e) {
  return dot(e, vec4(1, .003921569, 1.53787e-5, 6.2273724e-9));
}

vec3 e(vec2 a) {
  vec4 e = texelFetch(tN, ivec2(floor(a)) & 511, 0);
  float v = e.y - e.x, i = e.z - e.x, o = e.w - v - e.z;
  vec2 t = fract(a), r = t * t, c = r * (3. - 2. * t);
  return vec3(e.x + v * c.x + i * c.y + o * c.x * c.y, 30. * r * (t * (t - 2.) + 1.) * (vec2(v, i) + o * c.yx));
}

int r = 0;
float f = 150.;

void t(int t, float e) {
  if (e < a && e != f) {
    r = t, f = e;
  }
}

float i(vec3 e, float t) {
  return length(e) - t;
}

float e(vec3 t, vec3 c) {
  vec3 e = abs(t) - c;
  return min(max(e.x, max(e.y, e.z)), 0.) + length(max(e, 0.));
}

float c(vec3 t, float c) {
  vec3 e = abs(t) - c;
  return min(max(e.x, max(e.y, e.z)), 0.) + length(max(e, 0.));
}

float e(vec3 e, float t, float c) {
  float v = length(e.xy) - t;
  return max(v, abs(e.z) - c);
}

float t(vec3 e, vec2 t) {
  return length(vec2(length(e.xz) - t.x, e.y)) - t.y;
}

float t(float a, float e, float i, float o) {
  float c = e * .5, t = floor((a + c) / e), v = mod(a + c, e) - c;
  return t > o ? v + e * (t - o) : t < i ? v + e * (t - i) : v;
}

vec2 t(vec2 e, float v) {
  float t = 3.1415927 / v, c = mod(atan(e.y, e.x) + t, t * 2.) - t;
  return vec2(cos(c), sin(c)) * length(e);
}

vec2 t(vec2 e, float v, float a) {
  float t = 3.1415927 / v, c = mod(atan(e.y, e.x) + t + a, t * 2.) - t;
  return vec2(cos(c), sin(c)) * length(e);
}

float t(float e, float t) {
  return abs(e) - t;
}

mat2 c(float e) {
  float t = cos(e), c = sin(e);
  return mat2(t, c, -c, t);
}

vec3 l(vec3 e) {
  return vec3(e.xy, -e.z);
}

float x(vec3 t) {
  float v = length(t) - .3;
  if (v > .3) {
    return v;
  }

  t.xz *= c(-1.2), t.yz *= c(-.2);
  return min(e(t, .025, .1), max(i(t - vec3(0, 0, .12), .05), t.z - .12));
}

float n(vec3 c) {
  float v = length(c) - .3;
  if (v > .3) {
    return v;
  }

  float t = e(c, .01, .06);
  t = min(t, e(c.yzx + vec3(0, .1, 0), .04, .005)), t = min(t, e(c - vec3(0, -.01, .04), vec3(.002, .02, .02)));
  return t;
}

float m(vec3 c) {
  float v = e(c - vec3(.03, 0, 0), vec3(.03, .006, .03));
  t(1, v);
  return min(e(c, vec3(.06, .005, .06)), v);
}

float t(vec3 c, float v, float r) {
  float o = length(c) - v * .6;
  if (o > 4.) {
    return o;
  }

  c.y += cos(c.z * r / v), c.x = abs(c.x);
  float a = e(c - vec3(.2, 0, 0), vec3(.1, .03, v * .55)), i = e(c - vec3(.5, 1, 0), .02, v * .55);
  c.z = t(c.z, .55, -v, v), i = min(i, e(c.xzy - vec3(.5, 0, .5), .02, .5)), t(1, i),
  a = min(a, e(c, vec3(.5, .05, .2))), t(-1, a);
  return min(a, i);
}

float y(vec3 a) {
  float o = length(a) - 2.;
  if (o > 1.) {
    return o;
  }

  vec3 v = a;
  v.xy *= c(-.25);
  float i = e(v + vec3(.2, .25, 0), vec3(.25, .5, .5)) - .01;
  v -= vec3(-.13, .25, 0), v.z = t(v.z, .04, -10., 10.), v.x = t(v.x, .04, -3., 3.);
  float r = c(v, .01) - .005;
  t(1, r), i = min(min(i, r), e(a - vec3(-.45, .2, 0), vec3(.2, .8, .5)) - .01);
  return i;
}

float z(vec3 t) {
  t.zy *= c(.06), t.y += cos(t.z / 20.) * 3.;
  return e(t, .01, 27.5);
}

float u(vec3 v) {
  float a = length(v) - 3.;
  if (a > .5) {
    return a;
  }

  v.xz -= vec2(1, -.05), v.zx *= c(iA.y * -2.5), v.xz += vec2(1, -.05);
  float o = e(v, .99, .05);
  vec3 i = vec3(t(v.xy, 8.), v.z);
  return max(o, -min(e(i - vec3(.5, 0, .1), vec3(.02, .1, .1)), e(i - vec3(0, 0, .1), .02, .1)));
}

float e(vec3 t, float i) {
  float a = length(t) - 1.;
  if (a > 1.) {
    return a;
  }

  float v = e(t, vec3(.2, .5, .05));
  v = max(v, -e(t, vec3(.03, .2, 1))), t.yz *= c(-1.5707964 * i + .7853982), t.z += .2, v = min(v, e(t, .02, .2)),
  t.z += .2, v = min(v, e(t, .03, .05));
  return v;
}

float s(vec3 v) {
  float n = length(v) - 18.;
  if (n > 5.) {
    return n;
  }

  v.y -= 9.;
  vec3 o = v;
  o.xz *= c(iB.y), o.xy *= c(.5), o.y -= 9.;
  float f, r, a;
  f = i(o, 9.), r = max(t(f, .01), o.y + 4.5), r = min(r, e(o.xzy + vec3(0, 0, 4.5), .1, 4.5)), r = min(r, i(o, .3)),
  v.y += 6.75, a = e(v, vec3(2.25, 3.6, 4.5)),
  a = min(a,
      min(max(t(e(v.xzy - vec3(2.25, 0, 0), 4.4, 3.5), .1),
              -min(e(v.zyx - vec3(0, 1.8, 0), 1., 100.), e(v - vec3(4.5, 2.3, 0), .4, 100.))),
          e(v.xzy - vec3(2.25, 0, -2.2), 4.4, 2.9)));
  float b = y(v - vec3(3, 1.5, 2)), z = u(v.zyx - vec3(0, 1.8, 6.5)),
        x = e(l(v - vec3(3.7, 2, -4)), clamp(iA.w, 0., 1.));
  v.y -= 2.25, a = max(min(a, e(v.xzy, .45000002, 4.7699995)), -f), v -= vec3(7, -2.85, 0), v.xy *= c(-.5),
               a = min(a, e(v, vec3(1, 1, .8)) - .01);
  float m = min(r, z);
  t(2, x), t(3, b), t(5, m);
  return min(min(b, a), min(m, x));
}

float h(vec3 c) {
  float o = length(c.xz) - 2.;
  if (o > 3.) {
    return o;
  }

  float v = e(c.xzy, .2, .5), r = c.y + iA.z * 4.;
  if ((iF & 8) != 0) {
    float e = n(vec3(c.x, r, c.z) + vec3(1.05, -5.05, 1.05));
    if (e < v) {
      t(2, e), v = e;
    }
  }

  float a = e(vec3(t(c.xz, 8.), r).xzy - vec3(1.5, 0, 0), vec3(.1, 5, .2));
  if (a < v) {
    t(4, a), v = a;
  }

  float i = e(c.xzy + vec3(0, 0, clamp(iA.z, 0., .02)), .05, .53);
  if (i < v) {
    t(1, i), v = i;
  }

  return v;
}

float g(vec3 i) {
  vec3 v = i.zyx - vec3(11, 1.25, -44);
  float o = length(v) - 8.;
  if (o > 5.) {
    return o;
  }

  v.y -= 2.;
  float m = e(v - vec3(-2.7, -1, -1.3), vec3(.35, .5, .5)),
        y = max(t(e(v, vec3(4, 1.6, 2)), .23),
            -min(e(v - vec3(0, .5, 0), .8, 100.), e(v - vec3(4, -.37, 1), vec3(2, 1, .53))));
  vec3 r = v - vec3(4, -.77, .5);
  r.xz *= c(-iA.x * 3.1415927 / 2.);
  float z = e(r - vec3(0, .4, .5), vec3(.05, .99, .52));
  v.x = t(v.x, .3, -10., 10.), v.z = abs(v.z);
  float f = e(v.xzy - vec3(0, 2, .5), .01, 1.), l = min(m, z);
  t(1, f), t(-1, l);
  float b = min(f, min(y, l)), a = 150.;
  if ((iF & 4) != 0) {
    a = x(i - vec3(-42, 3, 11.2));
  }

  if ((iF & 2) != 0) {
    a = min(a, n(i.yzx - vec3(2, 7.4, -45.5)));
  }

  if (a < b) {
    t(2, a);
    return a;
  }

  return b;
}

float i(vec3 v) {
  float a = length(v) - 9.;
  if (a > 1.) {
    return a;
  }

  v.xz *= c(-.7853982);
  float f = e(v - vec3(-1.5, 1, 5), vec3(1, .2, 3));
  v.y -= iB.w;
  vec3 o = v.xzy - vec3(-2, 0, 2);
  float r = t(i(v - clamp(v, vec3(-6, 0, 0), vec3(6, 0, 0)), 1.7),
      min(e(o - clamp(o, vec3(-.5, 0, 0), vec3(.5, 0, 0)), .4, .5),
          min(e(v - vec3(7.5, 0, 0), vec3(.3, 2, .05)) - .05, e(v - vec3(7.5, 0, 0), vec3(.3, .05, 2)) - .05)),
      .3);
  t(3, r);
  return min(f, r);
}

float c(vec3 v) {
  float b = length(v) - 13.;
  if (b > 3.) {
    return b;
  }

  vec3 o = abs(v), x = vec3(o.x, abs(v.y - 4.58), o.z);
  float a = e(l(v.xzy - vec3(1.9, -1.5, .2)) * .5, min(1., (6. - iX.x) * .2)) / .5,
        z = max(e(vec3(v.x, abs(v.y - 3.5) - 3.5, v.z), vec3(6, .2, 6)) - .05,
            max(-c(v - vec3(2, 7, 2), 1.5), -c(v - vec3(5.7, 0, 4), .52)));
  vec3 r = v - vec3(5, 7.6, -2);
  r.xy *= c(.3);
  vec3 m = vec3(v.xy, abs(v.z + 2.));
  a = min(a,
      min(min(e(m.xzy - vec3(-6, 1.1, 8.7), 1., 1.75), e(m.xzy - vec3(-6.5, 1.1, 0), .2, 8.)),
          e(vec3(v.z, abs(v.y - 7.6), v.x) - vec3(-3, .2, 0), .1, 5.)));
  float y = min(min(min(e(vec3(o.xz, v.y) - vec3(5, 5, 0), .5, 8.3), e(x.zyx - vec3(5.3, 3.5, 0), .05, 5.3)),
                    max(e(x - vec3(5.3, 3.5, 0), .05, 5.3), -c(v - vec3(5, .7, 4), .8))),
      e(r, vec3(.5, .6, 1.5)) - .05);
  t(1, y);
  vec3 f = v - vec3(2, 3.59, -.1);
  f.zy *= c(-.7853982), f.y -= iX.x, a = min(a, e(f, vec3(1, 5.1, .02)) - .05), t(5, a);
  float n = min(min(z, a), y);
  vec3 i = r - vec3(0, .8, 0);
  if (length(i) - 1. < 2.) {
    float c = min(min(t(i, vec2(.5, .02)), e(i.xzy + vec3(0, 0, .5), .02, .5)),
        e(vec3(t(i.xz, 5., iB.x), i.y).zyx - vec3(0, 0, .25), .01, .25));
    if (c < n) {
      t(2, c), n = c;
    }
  }

  return n;
}

float w(vec3 a) {
  vec3 v = a.zyx - vec3(4, -1, 17);
  v.zy *= c(-.19), v.z -= 19. - iA.w;
  return min(t(v, 21., 0.), e(v.xzy + vec3(0, 10.5, 6), .15, 5.));
}

float d(vec3 f) {
  vec3 c = f - vec3(8.7, 9.3, 37);
  float l = length(c.xz) - 5.;
  if (l > 4.) {
    return l;
  }

  vec3 o = vec3(t(c.xz, 6.), c.y).xzy, y = vec3(o.x, t(c.y, 1.5, -3., 7.), o.z);
  float z = max(
      max(min(e(c.xzy, 1.1, 12.), max(t(e(c.xzy - vec3(0, 0, 14), 4., 2.), .2), -e(o - vec3(4, 14, 0), vec3(1, 1, 2)))),
          -min(e(c.xzy, 1., 13.), e(y - vec3(1, 0, 0), vec3(.2, .3, .13)))),
      -e(c + vec3(0, 7, 1), vec3(.8, 1.2, .8)));
  vec3 v = vec3(c.x, c.y - iB.z, c.z);
  float a = e(v.xzy, 1., 11.);
  v.y = t(v.y, 1.5, -7., 7.), a = max(a, -t(v, vec2(1, .01)));
  vec3 r = vec3(c.x - .8, t(c.y - 12.7, 20.5, -1., 0.), c.z + .9);
  float n = i(r - vec3(0, .5, 0), .06), b = min(e(r.xzy, .05, .5), min(e(r - vec3(0, .5, 0), vec3(.05, .1, .1)), n));
  t(2, n), t(1, b), t(5, a);
  float x = min(min(z, min(b, a)), e(c + vec3(0, 10.3, 3), vec3(1.1, 2, 3)));
  if ((iF & 16) != 0) {
    float e = m(f - vec3(12.15, 22.31, 38.65));
    if (e < x) {
      t(2, e);
      return e;
    }
  }

  return x;
}

vec2 b;

float t(vec3 t, vec3 i, vec2 v) {
  t -= i;
  float a = length(t) - 2.;
  if (a > .5) {
    return a;
  }

  t.xz *= c(1.5707964), b = (v - t.xy) / (v * 2.);
  float o = e(t, vec3(v, .01));
  return o;
}

float e(vec3 c) {
  vec3 e = abs(vec3(c.x, c.y + 3., c.z)) - vec3(60, 0, 39);
  if (e.x < 0. && e.z < 0.) {
    e.y -= t(textureLod(tH, c.xz / vec2(120, 78) + .5, 0.)) * 19.;
  }

  return min(e.y, 0.) + length(max(e, 0.));
}

float o(vec3 e) {
  float r = t(e - vec3(45, 1.7, 22.4), 10., 2.), f = s(e - vec3(2, 10, 2)), l = h(e - vec3(47.5, 3.5, 30.5)), n = g(e);
  vec3 v = e - vec3(26, 5, -58);
  v.xz *= c(1.9707963);
  float b = c(v), x = w(v), a = z(v.zyx - vec3(-2, 9.7, 32.5)), m = d(e), y = i(e - vec3(-46, -.5, -30)),
        o = min(min(min(r, f), min(l, n)), min(b, min(x, min(m, y))));
  if (a < o) {
    t(1, a);
    return a;
  }

  t(0, o);
  return o;
}

int v = 0;

float t(vec3 i) {
  float c = e(i);
  if (c <= a) {
    v = 1;
    return c;
  }

  float r = o(i), f = t(i, vec3(4.76, 14.42, 4), vec2(.45, .29)), l = min(f, r);
  if (c < l) {
    v = 1;
    return c;
  }

  v = f <= r ? 3 : 2;
  return l;
}

vec3 p(vec3 e) {
  return normalize(vec3(o(e + vec3(.001, 0, 0)), o(e + vec3(0, .001, 0)), o(e + vec3(0, 0, .001))) - o(e));
}

vec3 t(vec3 t, float v) {
  vec2 c = vec2(mix(.03, .001, min(v / 120., 1.)), 0);
  return normalize(vec3(e(t + c.xyy), e(t + c.yxy), e(t + c.yyx)) - e(t));
}

float t(vec3 i, vec3 o, float r, float e) {
  float c = r;
  for (int n = 0;; n++) {
    vec3 l = i + o * e;
    a = r * max(e, 1.);
    if (l.y <= -2.9995 || e >= 150.) {
      float e = (-3. - i.y) / o.y;
      if (e >= 0. && e < 500.) {
        v = 1;
        return e;
      }

      break;
    }

    if (l.y > 45.) {
      break;
    }

    float f = t(l);
    if (f < 0.) {
      e -= c, f = c / 2.;
    }

    e += f;
    if (f <= a || n >= 100) {
      return e;
    }

    c = f;
  }

  v = 0;
  return 500.;
}

float t(vec3 t, float v, vec3 i, float c) {
  float e = clamp(v * .005, .01, .1);
  t = t + i * e;
  float r = iS.w - a * 2.;
  for (float v = 1.; v < 50.; v++) {
    vec3 i = t + iS.xyz * e;
    if (e >= 80. || i.y > 45. || i.y < r || length(t) >= 150.) {
      break;
    }

    float f = o(i), l = max(a, .01 * min(1., e) + v * 2e-4);
    if (f <= l) {
      return 0.;
    }

    c = min(c, 85. * f / e);
    if (c < .078) {
      return 0.;
    }

    e += f + a;
  }

  return c;
}

float t(vec3 t, vec3 c) {
  float e = (iS.w - t.y) / c.y;
  return min(e >= 0. ? e : 500., 500.);
}

vec3 t(vec2 t) {
  vec3 v = vec3(0);
  float a = 0., c = 1., o = 0., i = 1. - length(iP.xz - t) / 500., r = 5. * i * i;
  for (float i = 0.; i < r; ++i) {
    t += iD.w * .5, o *= -.75;
    vec3 f = e(t + sin(t.yx * .5 + iD.w * .5) * .5);
    v += f * c, t += f.yz * .43, t *= 2., a += c, c *= .75;
  }

  return v / a;
}

vec3 t(vec3 c, float t, vec3 v) {
  float a = min(t / 500., 1.), i = e(pow(a, 3.5) + 1. - exp(-t * .005)), o = max(dot(v, iS.xyz), 0.);
  vec3 r = mix(vec3(.4, .8, 1), vec3(1.065, .95, .85), pow(o, 10.));
  return mix(c, r, i);
}

vec3 c(vec3 m, vec3 i) {
  vec4 z = texelFetch(tP, ivec2(FC * 256. / iR), 0);
  float F = uintBitsToFloat(uint(z.x * 255.) << 24 | uint(z.y * 255.) << 16 | uint(z.z * 255.) << 8 | uint(z.w * 255.)),
        l = t(m, i, .001, F), x = t(m, i);
  vec3 c, o = vec3(0, 1, 0);
  float u = l;
  if (v == 3) {
    return iB.y > 0. ? texture(tS, b).xyz : vec3(0);
  }

  vec3 a = m + i * l;
  bool g = x < 500. && x < l;
  vec3 w;
  float n = 0.;
  if (g) {
    n = mix(.2, 1., e((l - x) / 3.));
    vec3 v = m + i * x, c = mix(vec3(0), t(v.xz * (.7 - iS.w * .02)) * (1. - length(v) / 450.), n);
    o = normalize(vec3(c.y, 1., c.x)), x -= abs(c.z) * n * .6, u = x,
    w = mix(vec3(.25, .52, .73), vec3(.15, .62, .83), e(abs(c.z) - n));
  }

  int h = v, d = r;
  if (v == 0) {
    c = vec3(.4, .8, 1);
  } else {
    vec3 v;
    if (a.y <= -2.9995) {
      v = vec3(0, 1, 0), c = vec3(1);
    } else {
      c = vec3(.8);
      switch (h) {
        case 1:
          v = t(a, l),
          c = mix(vec3(.93, .8, .64),
                  mix(vec3(.69 + texture(tN, a.xz * 1e-4).x, .67, .65), vec3(.38, .52, .23), dot(v, vec3(0, 1, 0))),
                  e(a.y * .5 - 1.)) +
              texture(tN, a.xz * .15).x * .1 + texture(tN, a.xz * .01).y * .1;

          break;
        case 2:
          v = p(a);

          switch (d) {
            case 1: c = vec3(1); break;
            case 2: c = vec3(1, 0, 0); break;
            case 3: c = vec3(.5, 0, 0); break;
            case 4: c = vec3(.2, .1, .2); break;
            case 5: c = vec3(1, .95, .8); break;
            case -1: c = vec3(.64000005, .48000002, .32000002); break;
            default:
              vec4 e =
                  texture(tN, a.xy * .35) * v.z + texture(tN, a.yz * .35) * v.x + texture(tN, a.xz * .35) * v.y - .5;
              c += .125 * (e.x - e.y + e.z - e.w);
              break;
          }
      }

      o = normalize(mix(v, o, n));
    }
  }

  float C = g || h == 2 && d > 0 ? pow(e(dot(iS.xyz, reflect(i, o))), 50.) : 0., y = e(dot(iS.xyz, o)),
        k = e(dot(iS.xyz * vec3(-1, 1, -1), o)), f = y + k * .15;
  if (h == 1) {
    f = pow(f * mix(.9, 1.02, y * y), 1. + y * .6);
  }

  f = mix(f, y, n);
  float s = 1.;
  if (v != 0) {
    s = t(m + i * u, u, o, 1.);
  }

  if ((iF & 1) != 0 && l < 20.) {
    float e = pow(clamp(dot(iD.xyz, i), 0., 1.), 32.) * smoothstep(10., 0., l);
    f += e * max(dot(o, -i), 0.) * (1. - f), s += e * (1. - s);
  }

  c = mix(c, w, n), c = (c * vec3(1.065, .95, .85) * f + C) * mix(.38 + (1. - f) * .2, 1., s);
  return t(c, u, i);
}

void main_c() {
  vec3 e = vec3(0, 0, 1);
  e.xz *= c(FC.x * .049087387 + 3.1415927),
      oC = t(.2 - t(vec3(iP.x, iP.y + (FC.y / 64. - 1.) - .8, iP.z) + normalize(e) * .15));
}

void main_p() {
  vec2 c = FC / 128. - 1. + .001953125;
  vec3 i = normalize(iM * vec3(c.x * -1.5, c.y, 2.4142134));
  float v = t(iP, i, .0046875, .15);
  uint e = floatBitsToUint(v >= 150. ? 150. : v - a);
  oC = vec4(
      float(e >> 24 & 255u) / 255., float(e >> 16 & 255u) / 255., float(e >> 8 & 255u) / 255., float(e & 255u) / 255.);
}

void main_m() {
  vec2 e = FC / (iR * .5) - 1.;
  vec3 t = normalize(iM * vec3(e.x * -1.5, e.y, 2.4142134));
  oC = vec4(c(iP, t), 1);
}

float t(vec2 v, float a, float i, float t, float o) {
  vec2 c = v - vec2(a, i);
  return e(1. - smoothstep(t - t * o, t, dot(c, c) * 4.));
}

void main_h() {
  vec2 v = FC / (iR * .5) - 1., l = vec2(1.3, 1), f = vec2(0);
  float a = 0., i = 1., r = 1., n = 0., b = 1.;
  while (b < 11.) {
    vec3 t = e(21.1 + v * l * c(b++ * 2.4));
    f += t.yz, a += r * (1. - t.x) / (1. + dot(f, f)), i += r * (.5 - t.x), n += r, r *= .5, l *= 1.8;
  }

  a /= n, i *= .5;
  float o = length(v * (1.2 - i + a)) * e(i + .55 - .5 * a * v.x * (1. - v.y * .5)),
        m = t(v, -.45, -.52, 1., 2.3) + t(v, -.6, -.1, 1., 3.3) + t(v, .6, -.7, 1., 5.) + t(v, .84, .84, a, i * 5.);
  o = e(1. - t(o, 1. - mix(0., a * 2., m), .05 + i * .5));
  vec2 x = 1. - abs(v) + a * .04;
  o = t(o, t(x.x, x.y, .3) * 2., .1), oC = t(t(a, o, .01) * 1.33 - .045);
}
