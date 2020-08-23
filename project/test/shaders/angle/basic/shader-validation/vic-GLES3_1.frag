#version 310 es
precision mediump float;
precision mediump imageCube;
in vec4 myInput;
layout(rgba32f) uniform readonly imageCube myImage;
void main() {
  imageLoad(myImage, ivec3(0));
}

// ShaderValidation FragmentShaderValidationTest, ValidImageCube