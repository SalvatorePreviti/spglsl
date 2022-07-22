#version 300 es

precision highp float;

uniform vec2 uniformA;
uniform vec2 uniformB;
uniform vec2 uniformC;

in vec2 inputA;
in vec2 inputB;
in vec2 inputC;

layout(location = 0) out vec2 outputA;
layout(location = 1) out vec2 outputB;
layout(location = 2) out vec2 outputC;

void main() {
  outputA = inputA * uniformA;
  outputB = inputB * uniformB;
  outputC = inputC * uniformC;
}
