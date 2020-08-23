#version 300 es

precision highp float;

#include "included.glsl"

in float vertexCounter;
uniform int uVertZero;
uniform int uReference;
out vec4 fragColor;

void main() {
  fragColor = vec4(0.0, 0.0, 0.0, 1.0);
  fragColor.r = float(counter(uVertZero, uVertZero) == uReference);
  fragColor.g = float(int(vertexCounter) == uReference);
}