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
float f;
float i(float e) {
  return clamp(e, 0., 1.);
}
float r(float t, float c, float e) {
  float v = max(e - abs(t - c), 0.) / e;
  return min(t, c) - v * v * e / 4.;
}
vec4 h(float t) {
  vec4 e = i(t) * vec4(.999998, 254.99948, 65024.867, 160581056);
  e = fract(e);
  e -= e.yzww * vec4(.003921569, .003921569, .003921569, 0);
  return e;
}
float N(vec4 e) {
  return dot(e, vec4(1, .003921569, 1.53787e-5, 6.2273724e-9));
}
vec3 b(vec2 a) {
  vec4 e = texelFetch(tN, ivec2(floor(a)) & 511, 0);
  float v = e.y - e.x, a = e.z - e.x, e = e.w - v - e.z;
  vec2 t = fract(a), v = t * t, c = v * (3. - 2. * t);
  return vec3(e.x + v * c.x + a * c.y + e * c.x * c.y, 30. * v * (t * (t - 2.) + 1.) * (vec2(v, a) + e * c.yx));
}
int g = 0;
float w = 150.;
void c(int t, float e) {
  if (e < f && e != w) {
    g = t;
    w = e;
  }
}
float x(vec3 e, float t) {
  return length(e) - t;
}
float t(vec3 t, vec3 c) {
  vec3 e = abs(t) - c;
  return min(max(e.x, max(e.y, e.z)), 0.) + length(max(e, 0.));
}
float y(vec3 t, float c) {
  vec3 e = abs(t) - c;
  return min(max(e.x, max(e.y, e.z)), 0.) + length(max(e, 0.));
}
float e(vec3 e, float t, float c) {
  float v = length(e.xy) - t;
  return max(v, abs(e.z) - c);
}
float d(vec3 e, vec2 t) {
  return length(vec2(length(e.xz) - t.x, e.y)) - t.y;
}
float l(float a, float e, float a, float e) {
  float c = e * .5, t = floor((a + c) / e), v = mod(a + c, e) - c;
  return t > e ? v + e * (t - e) : t < a ? v + e * (t - a) : v;
}
vec2 s(vec2 e, float v) {
  float t = 3.1415927 / v, c = mod(atan(e.y, e.x) + t, t * 2.) - t;
  return vec2(cos(c), sin(c)) * length(e);
}
vec2 A(vec2 e, float v, float a) {
  float t = 3.1415927 / v, c = mod(atan(e.y, e.x) + t + a, t * 2.) - t;
  return vec2(cos(c), sin(c)) * length(e);
}
float z(float e, float t) {
  return abs(e) - t;
}
mat2 v(float e) {
  float t = cos(e), c = sin(e);
  return mat2(t, c, -c, t);
}
vec3 p(vec3 e) {
  return vec3(e.xy, -e.z);
}
float D(vec3 t) {
  float c = length(t) - .3;
  if (c > .3)
    return c;
  t.xz *= v(-1.2);
  t.yz *= v(-.2);
  return min(e(t, .025, .1), max(x(t - vec3(0, 0, .12), .05), t.z - .12));
}
float F(vec3 v) {
  float a = length(v) - .3;
  if (a > .3)
    return a;
  float c = e(v, .01, .06);
  c = min(c, e(v.yzx + vec3(0, .1, 0), .04, .005));
  c = min(c, t(v - vec3(0, -.01, .04), vec3(.002, .02, .02)));
  return c;
}
float R(vec3 e) {
  float v = t(e - vec3(.03, 0, 0), vec3(.03, .006, .03));
  c(1, v);
  return min(t(e, vec3(.06, .005, .06)), v);
}
float C(vec3 v, float a, float a) {
  float v = length(v) - a * .6;
  if (v > 4.)
    return v;
  v.y += cos(v.z * a / a);
  v.x = abs(v.x);
  float a = t(v - vec3(.2, 0, 0), vec3(.1, .03, a * .55)), e = e(v - vec3(.5, 1, 0), .02, a * .55);
  v.z = l(v.z, .55, -a, a);
  e = min(e, e(v.xzy - vec3(.5, 0, .5), .02, .5));
  c(1, e);
  a = min(a, t(v, vec3(.5, .05, .2)));
  c(-1, a);
  return min(a, e);
}
float X(vec3 a) {
  float e = length(a) - 2.;
  if (e > 1.)
    return e;
  vec3 e = a;
  e.xy *= v(-.25);
  float a = t(e + vec3(.2, .25, 0), vec3(.25, .5, .5)) - .01;
  e -= vec3(-.13, .25, 0);
  e.z = l(e.z, .04, -10., 10.);
  e.x = l(e.x, .04, -3., 3.);
  float v = y(e, .01) - .005;
  c(1, v);
  a = min(min(a, v), t(a - vec3(-.45, .2, 0), vec3(.2, .8, .5)) - .01);
  return a;
}
float M(vec3 t) {
  t.zy *= v(.06);
  t.y += cos(t.z / 20.) * 3.;
  return e(t, .01, 27.5);
}
float H(vec3 c) {
  float a = length(c) - 3.;
  if (a > .5)
    return a;
  c.xz -= vec2(1, -.05);
  c.zx *= v(iA.y * -2.5);
  c.xz += vec2(1, -.05);
  float e = e(c, .99, .05);
  vec3 a = vec3(s(c.xy, 8.), c.z);
  return max(e, -min(t(a - vec3(.5, 0, .1), vec3(.02, .1, .1)), e(a - vec3(0, 0, .1), .02, .1)));
}
float k(vec3 c, float e) {
  float a = length(c) - 1.;
  if (a > 1.)
    return a;
  float a = t(c, vec3(.2, .5, .05));
  a = max(a, -t(c, vec3(.03, .2, 1)));
  c.yz *= v(-1.5707964 * e + .7853982);
  c.z += .2;
  a = min(a, e(c, .02, .2));
  c.z += .2;
  a = min(a, e(c, .03, .05));
  return a;
}
float T(vec3 a) {
  float t = length(a) - 18.;
  if (t > 5.)
    return t;
  a.y -= 9.;
  vec3 v = a;
  v.xz *= v(iB.y);
  v.xy *= v(.5);
  v.y -= 9.;
  float i, a, e;
  i = x(v, 9.);
  a = max(z(i, .01), v.y + 4.5);
  a = min(a, e(v.xzy + vec3(0, 0, 4.5), .1, 4.5));
  a = min(a, x(v, .3));
  a.y += 6.75;
  e = t(a, vec3(2.25, 3.6, 4.5));
  e = min(e,
      min(max(z(e(a.xzy - vec3(2.25, 0, 0), 4.4, 3.5), .1),
              -min(e(a.zyx - vec3(0, 1.8, 0), 1., 100.), e(a - vec3(4.5, 2.3, 0), .4, 100.))),
          e(a.xzy - vec3(2.25, 0, -2.2), 4.4, 2.9)));
  float a = X(a - vec3(3, 1.5, 2)), e = H(a.zyx - vec3(0, 1.8, 6.5)),
        o = k(p(a - vec3(3.7, 2, -4)), clamp(iA.w, 0., 1.));
  a.y -= 2.25;
  e = max(min(e, e(a.xzy, .45000002, 4.7699995)), -i);
  a -= vec3(7, -2.85, 0);
  a.xy *= v(-.5);
  e = min(e, t(a, vec3(1, 1, .8)) - .01);
  float c = min(a, e);
  c(2, o);
  c(3, a);
  c(5, c);
  return min(min(a, e), min(c, o));
}
float L(vec3 v) {
  float a = length(v.xz) - 2.;
  if (a > 3.)
    return a;
  float a = e(v.xzy, .2, .5), i = v.y + iA.z * 4.;
  if ((iF & 8) != 0) {
    float e = F(vec3(v.x, i, v.z) + vec3(1.05, -5.05, 1.05));
    if (e < a) {
      c(2, e);
      a = e;
    }
  }
  float e = t(vec3(s(v.xz, 8.), i).xzy - vec3(1.5, 0, 0), vec3(.1, 5, .2));
  if (e < a) {
    c(4, e);
    a = e;
  }
  float v = e(v.xzy + vec3(0, 0, clamp(iA.z, 0., .02)), .05, .53);
  if (v < a) {
    c(1, v);
    a = v;
  }
  return a;
}
float U(vec3 v) {
  vec3 a = v.zyx - vec3(11, 1.25, -44);
  float i = length(a) - 8.;
  if (i > 5.)
    return i;
  a.y -= 2.;
  float e = t(a - vec3(-2.7, -1, -1.3), vec3(.35, .5, .5)),
        f = max(z(t(a, vec3(4, 1.6, 2)), .23),
            -min(e(a - vec3(0, .5, 0), .8, 100.), t(a - vec3(4, -.37, 1), vec3(2, 1, .53))));
  vec3 t = a - vec3(4, -.77, .5);
  t.xz *= v(-iA.x * 3.1415927 / 2.);
  float v = t(t - vec3(0, .4, .5), vec3(.05, .99, .52));
  a.x = l(a.x, .3, -10., 10.);
  a.z = abs(a.z);
  float a = e(a.xzy - vec3(0, 2, .5), .01, 1.), o = min(e, v);
  c(1, a);
  c(-1, o);
  float c = min(a, min(f, o)), e = 150.;
  if ((iF & 4) != 0)
    e = D(v - vec3(-42, 3, 11.2));
  if ((iF & 2) != 0)
    e = min(e, F(v.yzx - vec3(2, 7.4, -45.5)));
  if (e < c) {
    c(2, e);
    return e;
  }
  return c;
}
float j(vec3 a) {
  float e = length(a) - 9.;
  if (e > 1.)
    return e;
  a.xz *= v(-.7853982);
  float t = t(a - vec3(-1.5, 1, 5), vec3(1, .2, 3));
  a.y -= iB.w;
  vec3 v = a.xzy - vec3(-2, 0, 2);
  float i = r(x(a - clamp(a, vec3(-6, 0, 0), vec3(6, 0, 0)), 1.7),
      min(e(v - clamp(v, vec3(-.5, 0, 0), vec3(.5, 0, 0)), .4, .5),
          min(t(a - vec3(7.5, 0, 0), vec3(.3, 2, .05)) - .05, t(a - vec3(7.5, 0, 0), vec3(.3, .05, 2)) - .05)),
      .3);
  c(3, i);
  return min(t, i);
}
float q(vec3 a) {
  float e = length(a) - 13.;
  if (e > 3.)
    return e;
  vec3 i = abs(a), f = vec3(i.x, abs(a.y - 4.58), i.z);
  float e = k(p(a.xzy - vec3(1.9, -1.5, .2)) * .5, min(1., (6. - iX.x) * .2)) / .5,
        a = max(t(vec3(a.x, abs(a.y - 3.5) - 3.5, a.z), vec3(6, .2, 6)) - .05,
            max(-y(a - vec3(2, 7, 2), 1.5), -y(a - vec3(5.7, 0, 4), .52)));
  vec3 t = a - vec3(5, 7.6, -2);
  t.xy *= v(.3);
  vec3 v = vec3(a.xy, abs(a.z + 2.));
  e = min(e,
      min(min(e(v.xzy - vec3(-6, 1.1, 8.7), 1., 1.75), e(v.xzy - vec3(-6.5, 1.1, 0), .2, 8.)),
          e(vec3(a.z, abs(a.y - 7.6), a.x) - vec3(-3, .2, 0), .1, 5.)));
  float c = min(min(min(e(vec3(i.xz, a.y) - vec3(5, 5, 0), .5, 8.3), e(f.zyx - vec3(5.3, 3.5, 0), .05, 5.3)),
                    max(e(f - vec3(5.3, 3.5, 0), .05, 5.3), -y(a - vec3(5, .7, 4), .8))),
      t(t, vec3(.5, .6, 1.5)) - .05);
  c(1, c);
  vec3 o = a - vec3(2, 3.59, -.1);
  o.zy *= v(-.7853982);
  o.y -= iX.x;
  e = min(e, t(o, vec3(1, 5.1, .02)) - .05);
  c(5, e);
  float c = min(min(a, e), c);
  vec3 v = t - vec3(0, .8, 0);
  if (length(v) - 1. < 2.) {
    float t = min(min(d(v, vec2(.5, .02)), e(v.xzy + vec3(0, 0, .5), .02, .5)),
        e(vec3(A(v.xz, 5., iB.x), v.y).zyx - vec3(0, 0, .25), .01, .25));
    if (t < c) {
      c(2, t);
      c = t;
    }
  }
  return c;
}
float E(vec3 c) {
  vec3 t = c.zyx - vec3(4, -1, 17);
  t.zy *= v(-.19);
  t.z -= 19. - iA.w;
  return min(C(t, 21., 0.), e(t.xzy + vec3(0, 10.5, 6), .15, 5.));
}
float I(vec3 o) {
  vec3 v = o - vec3(8.7, 9.3, 37);
  float c = length(v.xz) - 5.;
  if (c > 4.)
    return c;
  vec3 i = vec3(s(v.xz, 6.), v.y).xzy, c = vec3(i.x, l(v.y, 1.5, -3., 7.), i.z);
  float a = max(
      max(min(e(v.xzy, 1.1, 12.), max(z(e(v.xzy - vec3(0, 0, 14), 4., 2.), .2), -t(i - vec3(4, 14, 0), vec3(1, 1, 2)))),
          -min(e(v.xzy, 1., 13.), t(c - vec3(1, 0, 0), vec3(.2, .3, .13)))),
      -t(v + vec3(0, 7, 1), vec3(.8, 1.2, .8)));
  vec3 a = vec3(v.x, v.y - iB.z, v.z);
  float e = e(a.xzy, 1., 11.);
  a.y = l(a.y, 1.5, -7., 7.);
  e = max(e, -d(a, vec2(1, .01)));
  vec3 t = vec3(v.x - .8, l(v.y - 12.7, 20.5, -1., 0.), v.z + .9);
  float e = x(t - vec3(0, .5, 0), .06), f = min(e(t.xzy, .05, .5), min(t(t - vec3(0, .5, 0), vec3(.05, .1, .1)), e));
  c(2, e);
  c(1, f);
  c(5, e);
  float v = min(min(a, min(f, e)), t(v + vec3(0, 10.3, 3), vec3(1.1, 2, 3)));
  if ((iF & 16) != 0) {
    float e = R(o - vec3(12.15, 22.31, 38.65));
    if (e < v) {
      c(2, e);
      return e;
    }
  }
  return v;
}
vec2 S;
float J(vec3 e, vec3 e, vec2 c) {
  e -= e;
  float a = length(e) - 2.;
  if (a > .5)
    return a;
  e.xz *= v(1.5707964);
  S = (c - e.xy) / (c * 2.);
  float i = t(e, vec3(c, .01));
  return i;
}
float m(vec3 t) {
  vec3 e = abs(vec3(t.x, t.y + 3., t.z)) - vec3(60, 0, 39);
  if (e.x < 0. && e.z < 0.)
    e.y -= N(textureLod(tH, t.xz / vec2(120, 78) + .5, 0.)) * 19.;
  return min(e.y, 0.) + length(max(e, 0.));
}
float n(vec3 e) {
  float i = C(e - vec3(45, 1.7, 22.4), 10., 2.), t = T(e - vec3(2, 10, 2)), o = L(e - vec3(47.5, 3.5, 30.5)), c = U(e);
  vec3 t = e - vec3(26, 5, -58);
  t.xz *= v(1.9707963);
  float e = q(t), f = E(t), a = M(t.zyx - vec3(-2, 9.7, 32.5)), v = I(e), c = j(e - vec3(-46, -.5, -30)),
        e = min(min(min(i, t), min(o, c)), min(e, min(f, min(v, c))));
  if (a < e) {
    c(1, a);
    return a;
  }
  c(0, e);
  return e;
}
int o = 0;
float P(vec3 t) {
  float e = m(t);
  if (e <= f) {
    o = 1;
    return e;
  }
  float c = n(t), v = J(t, vec3(4.76, 14.42, 4), vec2(.45, .29)), a = min(v, c);
  if (e < a) {
    o = 1;
    return e;
  }
  o = v <= c ? 3 : 2;
  return a;
}
vec3 Q(vec3 e) {
  return normalize(vec3(n(e + vec3(.001, 0, 0)), n(e + vec3(0, .001, 0)), n(e + vec3(0, 0, .001))) - n(e));
}
vec3 G(vec3 e, float c) {
  vec2 t = vec2(mix(.03, .001, min(c / 120., 1.)), 0);
  return normalize(vec3(m(e + t.xyy), m(e + t.yxy), m(e + t.yyx)) - m(e));
}
float B(vec3 c, vec3 v, float a, float e) {
  float t = a;
  for (int t = 0;; t++) {
    vec3 i = c + v * e;
    f = a * max(e, 1.);
    if (i.y <= -2.9995 || e >= 150.) {
      float e = (-3. - c.y) / v.y;
      if (e >= 0. && e < 500.) {
        o = 1;
        return e;
      }
      break;
    }
    if (i.y > 45.)
      break;
    float e = P(i);
    if (e < 0.) {
      e -= t;
      e = t / 2.;
    }
    e += e;
    if (e <= f || t >= 100)
      return e;
    t = e;
  }
  o = 0;
  return 500.;
}
float K(vec3 t, float v, vec3 a, float c) {
  float e = clamp(v * .005, .01, .1);
  t = t + a * e;
  float e = iS.w - f * 2.;
  for (float v = 1.; v < 50.; v++) {
    vec3 a = t + iS.xyz * e;
    if (e >= 80. || a.y > 45. || a.y < e || length(t) >= 150.)
      break;
    float i = n(a), t = max(f, .01 * min(1., e) + v * 2e-4);
    if (i <= t)
      return 0.;
    c = min(c, 85. * i / e);
    if (c < .078)
      return 0.;
    e += i + f;
  }
  return c;
}
float V(vec3 t, vec3 c) {
  float e = (iS.w - t.y) / c.y;
  return min(e >= 0. ? e : 500., 500.);
}
vec3 Y(vec2 e) {
  vec3 c = vec3(0);
  float v = 0., t = 1., e = 0., a = 1. - length(iP.xz - e) / 500., i = 5. * a * a;
  for (float a = 0.; a < i; ++a) {
    e += iD.w * .5;
    e *= -.75;
    vec3 t = b(e + sin(e.yx * .5 + iD.w * .5) * .5);
    c += t * t;
    e += t.yz * .43;
    e *= 2.;
    v += t;
    t *= .75;
  }
  return c / v;
}
vec3 Z(vec3 t, float e, vec3 c) {
  float v = min(e / 500., 1.), a = i(pow(v, 3.5) + 1. - exp(-e * .005)), e = max(dot(c, iS.xyz), 0.);
  vec3 t = mix(vec3(.4, .8, 1), vec3(1.065, .95, .85), pow(e, 10.));
  return mix(t, t, a);
}
vec3 O(vec3 l, vec3 c) {
  vec4 c = texelFetch(tP, ivec2(FC * 256. / iR), 0);
  float e = uintBitsToFloat(uint(c.x * 255.) << 24 | uint(c.y * 255.) << 16 | uint(c.z * 255.) << 8 | uint(c.w * 255.)),
        e = B(l, c, .001, e), t = V(l, c);
  vec3 e, v = vec3(0, 1, 0);
  float r = e;
  if (o == 3)
    return iB.y > 0. ? texture(tS, S).xyz : vec3(0);
  vec3 t = l + c * e;
  bool a = t < 500. && t < e;
  vec3 y;
  float f = 0.;
  if (a) {
    f = mix(.2, 1., i((e - t) / 3.));
    vec3 t = l + c * t, e = mix(vec3(0), Y(t.xz * (.7 - iS.w * .02)) * (1. - length(t) / 450.), f);
    v = normalize(vec3(e.y, 1., e.x));
    t -= abs(e.z) * f * .6;
    r = t;
    y = mix(vec3(.25, .52, .73), vec3(.15, .62, .83), i(abs(e.z) - f));
  }
  int m = o, z = g;
  if (o == 0)
    e = vec3(.4, .8, 1);
  else {
    vec3 c;
    if (t.y <= -2.9995) {
      c = vec3(0, 1, 0);
      e = vec3(1);
    } else {
      e = vec3(.8);
      switch (m) {
        case 1:
          c = G(t, e);
          e = mix(vec3(.93, .8, .64),
                  mix(vec3(.69 + texture(tN, t.xz * 1e-4).x, .67, .65), vec3(.38, .52, .23), dot(c, vec3(0, 1, 0))),
                  i(t.y * .5 - 1.)) +
              texture(tN, t.xz * .15).x * .1 + texture(tN, t.xz * .01).y * .1;
          break;
        case 2:
          c = Q(t);
          switch (z) {
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
      v = normalize(mix(c, v, f));
    }
  }
  float u = a || m == 2 && z > 0 ? pow(i(dot(iS.xyz, reflect(c, v))), 50.) : 0., n = i(dot(iS.xyz, v)),
        o = i(dot(iS.xyz * vec3(-1, 1, -1), v)), a = n + o * .15;
  if (m == 1)
    a = pow(a * mix(.9, 1.02, n * n), 1. + n * .6);
  a = mix(a, n, f);
  float x = 1.;
  if (o != 0)
    x = K(l + c * r, r, v, 1.);
  if ((iF & 1) != 0 && e < 20.) {
    float e = pow(clamp(dot(iD.xyz, c), 0., 1.), 32.) * smoothstep(10., 0., e);
    a += e * max(dot(v, -c), 0.) * (1. - a);
    x += e * (1. - x);
  }
  e = mix(e, y, f);
  e = (e * vec3(1.065, .95, .85) * a + u) * mix(.38 + (1. - a) * .2, 1., x);
  return Z(e, r, c);
}
void main_c() {
  vec3 e = vec3(0, 0, 1);
  e.xz *= v(FC.x * .049087387 + 3.1415927);
  oC = h(.2 - P(vec3(iP.x, iP.y + (FC.y / 64. - 1.) - .8, iP.z) + normalize(e) * .15));
}
void main_p() {
  vec2 t = FC / 128. - 1. + .001953125;
  vec3 v = normalize(iM * vec3(t.x * -1.5, t.y, 2.4142134));
  float c = B(iP, v, .0046875, .15);
  uint e = floatBitsToUint(c >= 150. ? 150. : c - f);
  oC = vec4(
      float(e >> 24 & 255u) / 255., float(e >> 16 & 255u) / 255., float(e >> 8 & 255u) / 255., float(e & 255u) / 255.);
}
void main_m() {
  vec2 e = FC / (iR * .5) - 1.;
  vec3 t = normalize(iM * vec3(e.x * -1.5, e.y, 2.4142134));
  oC = vec4(O(iP, t), 1);
}
float u(vec2 c, float v, float a, float e, float o) {
  vec2 t = c - vec2(v, a);
  return i(1. - smoothstep(e - e * o, e, dot(t, t) * 4.));
}
void main_h() {
  vec2 e = FC / (iR * .5) - 1., l = vec2(1.3, 1), f = vec2(0);
  float t = 0., c = 1., o = 1., n = 0., x = 1.;
  while (x < 11.) {
    vec3 a = b(21.1 + e * l * v(x++ * 2.4));
    f += a.yz;
    t += o * (1. - a.x) / (1. + dot(f, f));
    c += o * (.5 - a.x);
    n += o;
    o *= .5;
    l *= 1.8;
  }
  t /= n;
  c *= .5;
  float a = length(e * (1.2 - c + t)) * i(c + .55 - .5 * t * e.x * (1. - e.y * .5)),
        y = u(e, -.45, -.52, 1., 2.3) + u(e, -.6, -.1, 1., 3.3) + u(e, .6, -.7, 1., 5.) + u(e, .84, .84, t, c * 5.);
  a = i(1. - r(a, 1. - mix(0., t * 2., y), .05 + c * .5));
  vec2 m = 1. - abs(e) + t * .04;
  a = r(a, r(m.x, m.y, .3) * 2., .1);
  oC = h(r(t, a, .01) * 1.33 - .045);
}
