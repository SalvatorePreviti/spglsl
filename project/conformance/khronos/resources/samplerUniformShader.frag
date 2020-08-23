#version 300 es
precision mediump float;
uniform sampler2D s2D;
uniform samplerCube sCube;
out vec4 my_FragColor;
void main() {
  my_FragColor = texture(s2D, vec2(0.5, 0.5)) + texture(sCube, vec3(0.5, 0.5, 0.5));
}
