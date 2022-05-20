#version 300 es
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
float t;
float t(float t) {
  return clamp(t, 0., 1.);
}
float e(float v, float c, float i) {
  float t = max(i - abs(v - c), 0.) / i;
  return min(v, c) - t * t * i / 4.;
}
vec4 e(float v) {
  vec4 t = t(v) * vec4(.999998, 254.99948, 65024.867, 160581056);
  t = fract(t);
  t -= t.yzww * vec4(.003921569, .003921569, .003921569, 0);
  return t;
}
float e(vec4 v) {
  return dot(v, vec4(1, .003921569, 1.53787e-5, 6.2273724e-9));
}
vec3 t(vec2 e) {
  vec4 t = texelFetch(tN, ivec2(floor(e)) & 511, 0);
  float t = t.y - t.x, t = t.z - t.x, t = t.w - t - t.z;
  vec2 t = fract(e), t = t * t, t = t * (3. - 2. * t);
  return vec3(t.x + t * t.x + t * t.y + t * t.x * t.y, 30. * t * (t * (t - 2.) + 1.) * (vec2(t, t) + t * t.yx));
}
int e = 0;
float v = 150.;
void c(int i, float a) {
  if (a < t && a != v) {
    e = i;
    v = a;
  }
}
float t(vec3 e, float v) {
  return length(e) - v;
}
float t(vec3 e, vec3 v) {
  vec3 t = abs(e) - v;
  return min(max(t.x, max(t.y, t.z)), 0.) + length(max(t, 0.));
}
float e(vec3 v, float c) {
  vec3 t = abs(v) - c;
  return min(max(t.x, max(t.y, t.z)), 0.) + length(max(t, 0.));
}
float e(vec3 v, float c, float i) {
  float t = length(v.xy) - c;
  return max(t, abs(v.z) - i);
}
float e(vec3 v, vec2 c) {
  return length(vec2(length(v.xz) - c.x, v.y)) - c.y;
}
float t(float e, float v, float c, float i) {
  float t = v * .5, t = floor((e + t) / v), t = mod(e + t, v) - t;
  return t > i ? t + v * (t - i) : t < c ? t + v * (t - c) : t;
}
vec2 e(vec2 v, float c) {
  float t = 3.1415927 / c, t = mod(atan(v.y, v.x) + t, t * 2.) - t;
  return vec2(cos(t), sin(t)) * length(v);
}
vec2 e(vec2 v, float c, float i) {
  float t = 3.1415927 / c, t = mod(atan(v.y, v.x) + t + i, t * 2.) - t;
  return vec2(cos(t), sin(t)) * length(v);
}
float e(float v, float c) {
  return abs(v) - c;
}
mat2 t(float e) {
  float t = cos(e), t = sin(e);
  return mat2(t, t, -t, t);
}
vec3 e(vec3 v) {
  return vec3(v.xy, -v.z);
}
float t(vec3 e) {
  float t = length(e) - .3;
  if (t > .3)
    return t;
  e.xz *= t(-1.2);
  e.yz *= t(-.2);
  return min(e(e, .025, .1), max(t(e - vec3(0, 0, .12), .05), e.z - .12));
}
float e(vec3 v) {
  float t = length(v) - .3;
  if (t > .3)
    return t;
  float t = e(v, .01, .06);
  t = min(t, e(v.yzx + vec3(0, .1, 0), .04, .005));
  t = min(t, t(v - vec3(0, -.01, .04), vec3(.002, .02, .02)));
  return t;
}
float e(vec3 v) {
  float t = t(v - vec3(.03, 0, 0), vec3(.03, .006, .03));
  c(1, t);
  return min(t(v, vec3(.06, .005, .06)), t);
}
float e(vec3 v, float c, float i) {
  float t = length(v) - c * .6;
  if (t > 4.)
    return t;
  v.y += cos(v.z * i / c);
  v.x = abs(v.x);
  float t = t(v - vec3(.2, 0, 0), vec3(.1, .03, c * .55)), t = e(v - vec3(.5, 1, 0), .02, c * .55);
  v.z = t(v.z, .55, -c, c);
  t = min(t, e(v.xzy - vec3(.5, 0, .5), .02, .5));
  c(1, t);
  t = min(t, t(v, vec3(.5, .05, .2)));
  c(-1, t);
  return min(t, t);
}
float e(vec3 v) {
  float t = length(v) - 2.;
  if (t > 1.)
    return t;
  vec3 t = v;
  t.xy *= t(-.25);
  float t = t(t + vec3(.2, .25, 0), vec3(.25, .5, .5)) - .01;
  t -= vec3(-.13, .25, 0);
  t.z = t(t.z, .04, -10., 10.);
  t.x = t(t.x, .04, -3., 3.);
  float t = e(t, .01) - .005;
  c(1, t);
  t = min(min(t, t), t(v - vec3(-.45, .2, 0), vec3(.2, .8, .5)) - .01);
  return t;
}
float e(vec3 v) {
  v.zy *= t(.06);
  v.y += cos(v.z / 20.) * 3.;
  return e(v, .01, 27.5);
}
float t(vec3 e) {
  float t = length(e) - 3.;
  if (t > .5)
    return t;
  e.xz -= vec2(1, -.05);
  e.zx *= t(iA.y * -2.5);
  e.xz += vec2(1, -.05);
  float t = e(e, .99, .05);
  vec3 t = vec3(e(e.xy, 8.), e.z);
  return max(t, -min(t(t - vec3(.5, 0, .1), vec3(.02, .1, .1)), e(t - vec3(0, 0, .1), .02, .1)));
}
float e(vec3 v, float c) {
  float t = length(v) - 1.;
  if (t > 1.)
    return t;
  float t = t(v, vec3(.2, .5, .05));
  t = max(t, -t(v, vec3(.03, .2, 1)));
  v.yz *= t(-1.5707964 * c + .7853982);
  v.z += .2;
  t = min(t, e(v, .02, .2));
  v.z += .2;
  t = min(t, e(v, .03, .05));
  return t;
}
float e(vec3 v) {
  float t = length(v) - 18.;
  if (t > 5.)
    return t;
  v.y -= 9.;
  vec3 t = v;
  t.xz *= t(iB.y);
  t.xy *= t(.5);
  t.y -= 9.;
  float t = t(t, 9.), t = max(e(t, .01), t.y + 4.5);
  t = min(t, e(t.xzy + vec3(0, 0, 4.5), .1, 4.5));
  t = min(t, t(t, .3));
  v.y += 6.75;
  float t = t(v, vec3(2.25, 3.6, 4.5));
  t = min(t,
      min(max(e(e(v.xzy - vec3(2.25, 0, 0), 4.4, 3.5), .1),
              -min(e(v.zyx - vec3(0, 1.8, 0), 1., 100.), e(v - vec3(4.5, 2.3, 0), .4, 100.))),
          e(v.xzy - vec3(2.25, 0, -2.2), 4.4, 2.9)));
  float t = e(v - vec3(3, 1.5, 2)), t = t(v.zyx - vec3(0, 1.8, 6.5)),
        t = e(e(v - vec3(3.7, 2, -4)), clamp(iA.w, 0., 1.));
  v.y -= 2.25;
  t = max(min(t, e(v.xzy, .45000002, 4.7699995)), -t);
  v -= vec3(7, -2.85, 0);
  v.xy *= t(-.5);
  t = min(t, t(v, vec3(1, 1, .8)) - .01);
  float t = min(t, t);
  c(2, t);
  c(3, t);
  c(5, t);
  return min(min(t, t), min(t, t));
}
float e(vec3 v) {
  float t = length(v.xz) - 2.;
  if (t > 3.)
    return t;
  float t = e(v.xzy, .2, .5), t = v.y + iA.z * 4.;
  if ((iF & 8) != 0) {
    float t = e(vec3(v.x, t, v.z) + vec3(1.05, -5.05, 1.05));
    if (t < t) {
      c(2, t);
      t = t;
    }
  }
  float t = t(vec3(e(v.xz, 8.), t).xzy - vec3(1.5, 0, 0), vec3(.1, 5, .2));
  if (t < t) {
    c(4, t);
    t = t;
  }
  float t = e(v.xzy + vec3(0, 0, clamp(iA.z, 0., .02)), .05, .53);
  if (t < t) {
    c(1, t);
    t = t;
  }
  return t;
}
float e(vec3 v) {
  vec3 t = v.zyx - vec3(11, 1.25, -44);
  float t = length(t) - 8.;
  if (t > 5.)
    return t;
  t.y -= 2.;
  float t = t(t - vec3(-2.7, -1, -1.3), vec3(.35, .5, .5)),
        t = max(e(t(t, vec3(4, 1.6, 2)), .23),
            -min(e(t - vec3(0, .5, 0), .8, 100.), t(t - vec3(4, -.37, 1), vec3(2, 1, .53))));
  vec3 t = t - vec3(4, -.77, .5);
  t.xz *= t(-iA.x * 3.1415927 / 2.);
  float t = t(t - vec3(0, .4, .5), vec3(.05, .99, .52));
  t.x = t(t.x, .3, -10., 10.);
  t.z = abs(t.z);
  float t = e(t.xzy - vec3(0, 2, .5), .01, 1.), t = min(t, t);
  c(1, t);
  c(-1, t);
  float t = min(t, min(t, t)), t = 150.;
  if ((iF & 4) != 0)
    t = t(v - vec3(-42, 3, 11.2));
  if ((iF & 2) != 0)
    t = min(t, e(v.yzx - vec3(2, 7.4, -45.5)));
  if (t < t) {
    c(2, t);
    return t;
  }
  return t;
}
float e(vec3 v) {
  float t = length(v) - 9.;
  if (t > 1.)
    return t;
  v.xz *= t(-.7853982);
  float t = t(v - vec3(-1.5, 1, 5), vec3(1, .2, 3));
  v.y -= iB.w;
  vec3 t = v.xzy - vec3(-2, 0, 2);
  float t = e(t(v - clamp(v, vec3(-6, 0, 0), vec3(6, 0, 0)), 1.7),
      min(e(t - clamp(t, vec3(-.5, 0, 0), vec3(.5, 0, 0)), .4, .5),
          min(t(v - vec3(7.5, 0, 0), vec3(.3, 2, .05)) - .05, t(v - vec3(7.5, 0, 0), vec3(.3, .05, 2)) - .05)),
      .3);
  c(3, t);
  return min(t, t);
}
float e(vec3 v) {
  float t = length(v) - 13.;
  if (t > 3.)
    return t;
  vec3 t = abs(v), t = vec3(t.x, abs(v.y - 4.58), t.z);
  float t = e(e(v.xzy - vec3(1.9, -1.5, .2)) * .5, min(1., (6. - iX.x) * .2)) / .5,
        t = max(t(vec3(v.x, abs(v.y - 3.5) - 3.5, v.z), vec3(6, .2, 6)) - .05,
            max(-e(v - vec3(2, 7, 2), 1.5), -e(v - vec3(5.7, 0, 4), .52)));
  vec3 t = v - vec3(5, 7.6, -2);
  t.xy *= t(.3);
  vec3 t = vec3(v.xy, abs(v.z + 2.));
  t = min(t,
      min(min(e(t.xzy - vec3(-6, 1.1, 8.7), 1., 1.75), e(t.xzy - vec3(-6.5, 1.1, 0), .2, 8.)),
          e(vec3(v.z, abs(v.y - 7.6), v.x) - vec3(-3, .2, 0), .1, 5.)));
  float t = min(min(min(e(vec3(t.xz, v.y) - vec3(5, 5, 0), .5, 8.3), e(t.zyx - vec3(5.3, 3.5, 0), .05, 5.3)),
                    max(e(t - vec3(5.3, 3.5, 0), .05, 5.3), -e(v - vec3(5, .7, 4), .8))),
      t(t, vec3(.5, .6, 1.5)) - .05);
  c(1, t);
  vec3 t = v - vec3(2, 3.59, -.1);
  t.zy *= t(-.7853982);
  t.y -= iX.x;
  t = min(t, t(t, vec3(1, 5.1, .02)) - .05);
  c(5, t);
  float t = min(min(t, t), t);
  vec3 t = t - vec3(0, .8, 0);
  if (length(t) - 1. < 2.) {
    float t = min(min(e(t, vec2(.5, .02)), e(t.xzy + vec3(0, 0, .5), .02, .5)),
        e(vec3(e(t.xz, 5., iB.x), t.y).zyx - vec3(0, 0, .25), .01, .25));
    if (t < t) {
      c(2, t);
      t = t;
    }
  }
  return t;
}
float e(vec3 v) {
  vec3 t = v.zyx - vec3(4, -1, 17);
  t.zy *= t(-.19);
  t.z -= 19. - iA.w;
  return min(e(t, 21., 0.), e(t.xzy + vec3(0, 10.5, 6), .15, 5.));
}
float e(vec3 v) {
  vec3 t = v - vec3(8.7, 9.3, 37);
  float t = length(t.xz) - 5.;
  if (t > 4.)
    return t;
  vec3 t = vec3(e(t.xz, 6.), t.y).xzy, t = vec3(t.x, t(t.y, 1.5, -3., 7.), t.z);
  float t = max(
      max(min(e(t.xzy, 1.1, 12.), max(e(e(t.xzy - vec3(0, 0, 14), 4., 2.), .2), -t(t - vec3(4, 14, 0), vec3(1, 1, 2)))),
          -min(e(t.xzy, 1., 13.), t(t - vec3(1, 0, 0), vec3(.2, .3, .13)))),
      -t(t + vec3(0, 7, 1), vec3(.8, 1.2, .8)));
  vec3 t = vec3(t.x, t.y - iB.z, t.z);
  float t = e(t.xzy, 1., 11.);
  t.y = t(t.y, 1.5, -7., 7.);
  t = max(t, -e(t, vec2(1, .01)));
  vec3 t = vec3(t.x - .8, t(t.y - 12.7, 20.5, -1., 0.), t.z + .9);
  float t = t(t - vec3(0, .5, 0), .06), t = min(e(t.xzy, .05, .5), min(t(t - vec3(0, .5, 0), vec3(.05, .1, .1)), t));
  c(2, t);
  c(1, t);
  c(5, t);
  float t = min(min(t, min(t, t)), t(t + vec3(0, 10.3, 3), vec3(1.1, 2, 3)));
  if ((iF & 16) != 0) {
    float t = e(v - vec3(12.15, 22.31, 38.65));
    if (t < t) {
      c(2, t);
      return t;
    }
  }
  return t;
}
vec2 e;
float v(vec3 c, vec3 i, vec2 a) {
  c -= i;
  float t = length(c) - 2.;
  if (t > .5)
    return t;
  c.xz *= t(1.5707964);
  e = (a - c.xy) / (a * 2.);
  float t = t(c, vec3(a, .01));
  return t;
}
float e(vec3 v) {
  vec3 t = abs(vec3(v.x, v.y + 3., v.z)) - vec3(60, 0, 39);
  if (t.x < 0. && t.z < 0.)
    t.y -= e(textureLod(tH, v.xz / vec2(120, 78) + .5, 0.)) * 19.;
  return min(t.y, 0.) + length(max(t, 0.));
}
float e(vec3 v) {
  float t = e(v - vec3(45, 1.7, 22.4), 10., 2.), t = e(v - vec3(2, 10, 2)), t = e(v - vec3(47.5, 3.5, 30.5)), t = e(v);
  vec3 t = v - vec3(26, 5, -58);
  t.xz *= t(1.9707963);
  float t = e(t), t = e(t), t = e(t.zyx - vec3(-2, 9.7, 32.5)), t = e(v), t = e(v - vec3(-46, -.5, -30)),
        t = min(min(min(t, t), min(t, t)), min(t, min(t, min(t, t))));
  if (t < t) {
    c(1, t);
    return t;
  }
  c(0, t);
  return t;
}
int e = 0;
float v(vec3 c) {
  float t = e(c);
  if (t <= t) {
    e = 1;
    return t;
  }
  float t = e(c), t = v(c, vec3(4.76, 14.42, 4), vec2(.45, .29)), t = min(t, t);
  if (t < t) {
    e = 1;
    return t;
  }
  e = t <= t ? 3 : 2;
  return t;
}
vec3 e(vec3 v) {
  return normalize(vec3(e(v + vec3(.001, 0, 0)), e(v + vec3(0, .001, 0)), e(v + vec3(0, 0, .001))) - e(v));
}
vec3 t(vec3 e, float v) {
  vec2 t = vec2(mix(.03, .001, min(v / 120., 1.)), 0);
  return normalize(vec3(e(e + t.xyy), e(e + t.yxy), e(e + t.yyx)) - e(e));
}
float e(vec3 v, vec3 c, float i, float a) {
  float t = i;
  for (int t = 0;; t++) {
    vec3 t = v + c * a;
    t = i * max(a, 1.);
    if (t.y <= -2.9995 || a >= 150.) {
      float t = (-3. - v.y) / c.y;
      if (t >= 0. && t < 500.) {
        e = 1;
        return t;
      }
      break;
    }
    if (t.y > 45.)
      break;
    float t = v(t);
    if (t < 0.) {
      a -= t;
      t = t / 2.;
    }
    a += t;
    if (t <= t || t >= 100)
      return a;
    t = t;
  }
  e = 0;
  return 500.;
}
float e(vec3 v, float c, vec3 i, float a) {
  float t = clamp(c * .005, .01, .1);
  v = v + i * t;
  float t = iS.w - t * 2.;
  for (float t = 1.; t < 50.; t++) {
    vec3 t = v + iS.xyz * t;
    if (t >= 80. || t.y > 45. || t.y < t || length(v) >= 150.)
      break;
    float t = e(t), t = max(t, .01 * min(1., t) + t * 2e-4);
    if (t <= t)
      return 0.;
    a = min(a, 85. * t / t);
    if (a < .078)
      return 0.;
    t += t + t;
  }
  return a;
}
float t(vec3 t, vec3 t) {
  float t = (iS.w - t.y) / t.y;
  return min(t >= 0. ? t : 500., 500.);
}
vec3 t(vec2 t) {
  vec3 t = vec3(0);
  float t = 0., t = 1., t = 0., t = 1. - length(iP.xz - t) / 500., t = 5. * t * t;
  for (float t = 0.; t < t; ++t) {
    t += iD.w * .5;
    t *= -.75;
    vec3 t = t(t + sin(t.yx * .5 + iD.w * .5) * .5);
    t += t * t;
    t += t.yz * .43;
    t *= 2.;
    t += t;
    t *= .75;
  }
  return t / t;
}
vec3 t(vec3 t, float t, vec3 t) {
  float t = min(t / 500., 1.), t = t(pow(t, 3.5) + 1. - exp(-t * .005)), t = max(dot(t, iS.xyz), 0.);
  vec3 t = mix(vec3(.4, .8, 1), vec3(1.065, .95, .85), pow(t, 10.));
  return mix(t, t, t);
}
vec3 t(vec3 t, vec3 t) {
  vec4 t = texelFetch(tP, ivec2(FC * 256. / iR), 0);
  float t = uintBitsToFloat(uint(t.x * 255.) << 24 | uint(t.y * 255.) << 16 | uint(t.z * 255.) << 8 | uint(t.w * 255.)),
        t = e(t, t, .001, t), t = t(t, t);
  vec3 t, t = vec3(0, 1, 0);
  float t = t;
  if (e == 3)
    return iB.y > 0. ? texture(tS, e).xyz : vec3(0);
  vec3 t = t + t * t;
  bool t = t < 500. && t < t;
  vec3 t;
  float t = 0.;
  if (t) {
    t = mix(.2, 1., t((t - t) / 3.));
    vec3 t = t + t * t, t = mix(vec3(0), t(t.xz * (.7 - iS.w * .02)) * (1. - length(t) / 450.), t);
    t = normalize(vec3(t.y, 1., t.x));
    t -= abs(t.z) * t * .6;
    t = t;
    t = mix(vec3(.25, .52, .73), vec3(.15, .62, .83), t(abs(t.z) - t));
  }
  int t = e, t = e;
  if (e == 0)
    t = vec3(.4, .8, 1);
  else {
    vec3 t;
    if (t.y <= -2.9995) {
      t = vec3(0, 1, 0);
      t = vec3(1);
    } else {
      t = vec3(.8);
      switch (t) {
        case 1:
          t = t(t, t);
          t = mix(vec3(.93, .8, .64),
                  mix(vec3(.69 + texture(tN, t.xz * 1e-4).x, .67, .65), vec3(.38, .52, .23), dot(t, vec3(0, 1, 0))),
                  t(t.y * .5 - 1.)) +
              texture(tN, t.xz * .15).x * .1 + texture(tN, t.xz * .01).y * .1;
          break;
        case 2:
          t = e(t);
          switch (t) {
            case 1: t = vec3(1); break;
            case 2: t = vec3(1, 0, 0); break;
            case 3: t = vec3(.5, 0, 0); break;
            case 4: t = vec3(.2, .1, .2); break;
            case 5: t = vec3(1, .95, .8); break;
            case -1: t = vec3(.64000005, .48000002, .32000002); break;
            default:
              vec4 t =
                  texture(tN, t.xy * .35) * t.z + texture(tN, t.yz * .35) * t.x + texture(tN, t.xz * .35) * t.y - .5;
              t += .125 * (t.x - t.y + t.z - t.w);
              break;
          }
      }
      t = normalize(mix(t, t, t));
    }
  }
  float t = t || t == 2 && t > 0 ? pow(t(dot(iS.xyz, reflect(t, t))), 50.) : 0., t = t(dot(iS.xyz, t)),
        t = t(dot(iS.xyz * vec3(-1, 1, -1), t)), t = t + t * .15;
  if (t == 1)
    t = pow(t * mix(.9, 1.02, t * t), 1. + t * .6);
  t = mix(t, t, t);
  float t = 1.;
  if (e != 0)
    t = e(t + t * t, t, t, 1.);
  if ((iF & 1) != 0 && t < 20.) {
    float t = pow(clamp(dot(iD.xyz, t), 0., 1.), 32.) * smoothstep(10., 0., t);
    t += t * max(dot(t, -t), 0.) * (1. - t);
    t += t * (1. - t);
  }
  t = mix(t, t, t);
  t = (t * vec3(1.065, .95, .85) * t + t) * mix(.38 + (1. - t) * .2, 1., t);
  return t(t, t, t);
}
void main_c() {
  vec3 t = vec3(0, 0, 1);
  t.xz *= t(FC.x * .049087387 + 3.1415927);
  oC = e(.2 - v(vec3(iP.x, iP.y + (FC.y / 64. - 1.) - .8, iP.z) + normalize(t) * .15));
}
void main_p() {
  vec2 t = FC / 128. - 1. + .001953125;
  vec3 t = normalize(iM * vec3(t.x * -1.5, t.y, 2.4142134));
  float t = e(iP, t, .0046875, .15);
  uint t = floatBitsToUint(t >= 150. ? 150. : t - t);
  oC = vec4(
      float(t >> 24 & 255u) / 255., float(t >> 16 & 255u) / 255., float(t >> 8 & 255u) / 255., float(t & 255u) / 255.);
}
void main_m() {
  vec2 t = FC / (iR * .5) - 1.;
  vec3 t = normalize(iM * vec3(t.x * -1.5, t.y, 2.4142134));
  oC = vec4(t(iP, t), 1);
}
float t(vec2 t, float t, float t, float t, float t) {
  vec2 t = t - vec2(t, t);
  return t(1. - smoothstep(t - t * t, t, dot(t, t) * 4.));
}
void main_h() {
  vec2 t = FC / (iR * .5) - 1., t = vec2(1.3, 1), t = vec2(0);
  float t = 0., t = 1., t = 1., t = 0., t = 1.;
  while (t < 11.) {
    vec3 t = t(21.1 + t * t * t(t++ * 2.4));
    t += t.yz;
    t += t * (1. - t.x) / (1. + dot(t, t));
    t += t * (.5 - t.x);
    t += t;
    t *= .5;
    t *= 1.8;
  }
  t /= t;
  t *= .5;
  float t = length(t * (1.2 - t + t)) * t(t + .55 - .5 * t * t.x * (1. - t.y * .5)),
        t = t(t, -.45, -.52, 1., 2.3) + t(t, -.6, -.1, 1., 3.3) + t(t, .6, -.7, 1., 5.) + t(t, .84, .84, t, t * 5.);
  t = t(1. - e(t, 1. - mix(0., t * 2., t), .05 + t * .5));
  vec2 t = 1. - abs(t) + t * .04;
  t = e(t, e(t.x, t.y, .3) * 2., .1);
  oC = e(e(t, t, .01) * 1.33 - .045);
}
