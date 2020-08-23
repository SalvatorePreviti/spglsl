#version 300 es

precision highp float;

//#define hash11 tan

float hash11(float p) {
  vec2 p2 = fract(p * vec2(.53, .82));
  p2 += dot(p2 + sin(p * p2.x), p2.yx + 33.33);
  return fract(p2.x + p2.y);
}

vec3 SmoothNoise_DXY(vec2 o) {
  vec2 _30 = floor(o.xy);
  vec4 _36 = vec4(_30.x + _30.y * 98.) + vec4(0., 1., 98., 99.);

  const vec2 O = vec2(.231, .824);

  vec2 _39 = fract(O * _36.x);
  vec2 _42 = fract(O * _36.y);
  vec2 _45 = fract(O * _36.z);
  vec2 _48 = fract(O * _36.w);

  vec4 S = sin(_36 * vec4(_39.x, _42.x, _45.x, _48.x));

  float k0 = dot(_39 + S.x, _39.yx + 33.33);
  float k1 = dot(_42 + S.y, _42.yx + 33.33);
  float k2 = dot(_45 + S.z, _45.yx + 33.33);
  float k3 = dot(_48 + S.w, _48.yx + 33.33);

  float _92 = fract(_39.x + _39.y + k0 * 2.);
  float _96 = fract(_42.x + _42.y + k1 * 2.);
  float _100 = fract(_45.x + _45.y + k2 * 2.);
  float _200 = fract(_48.x + _48.y + k3 * 2.);

  vec2 _101 = fract(o.xy);
  vec2 _102 = _101 * _101;
  vec2 _106 = _102 * 3. - _102 * _101 * 2.;
  vec2 _108 = (_101 - _102) * 6.;
  float _109 = _96 - _92;
  float _116 = _200 - _100 - _109;
  float _120 = _100 - _92 + _116 * _106.x;
  return vec3((_109 + _116 * _106.y) * _108.x, _120 * _108.y, _92 + dot(vec2(_109, _120), _106));
}

uniform vec3 position;
out vec4 oColor;

void main() {
  oColor.xyz = SmoothNoise_DXY(position.xy);
}

/*uniform float iTime;
#define time iTime

float rand(vec2 n) {
  return fract(sin(dot(n, vec2(12.98, 4.14))) * 43758.54);
}

float noise(vec2 n) {
  vec2 r = ceil(n), t = floor(n), f = fract(n);
  vec2 s = (3. - 2. * f) * f * f;
  vec2 hc = mix(vec2(rand(t), rand(vec2(t.x, r.y))), vec2(rand(vec2(r.x, t.y)), rand(r)), s.x);
  return mix(hc.x, hc.y, s.y);
}

float water(vec2 pos) {
  vec2 p = pos + time * 0.1;

  float a = 0.51 * noise(p + vec2(time * 0.7, time * 0.6));
  float b = 0.35 * noise(p * 8.0 + time);
  float c = 0.11 * noise(p * 16.0 + vec2(0.0, time * 0.5));
  float d = 0.05 * noise(p * 24.0);

  return a + b + c + d;
}

uniform vec3 position;
out vec4 oColor;
void main() {
  oColor.x = water(position.xy);
}*/