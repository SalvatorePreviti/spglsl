#version 310 es
precision mediump float;
precision mediump image3D;
in vec4 myInput;
layout(rgba32f) uniform readonly image3D myImage;
void main() {
  imageLoad(myImage, ivec3(0));
}

// ShaderValidation FragmentShaderValidationTest, ValidImage3D