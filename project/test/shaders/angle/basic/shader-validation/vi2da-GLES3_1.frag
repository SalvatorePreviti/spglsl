#version 310 es
precision mediump float;
precision mediump image2DArray;
in vec4 myInput;
layout(rgba32f) uniform readonly image2DArray myImage;
void main() {
  imageLoad(myImage, ivec3(0));
}

// ShaderValidation FragmentShaderValidationTest, ValidImage2DArray