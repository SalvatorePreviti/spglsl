#version 310 es
precision mediump float;
precision mediump image2D;
layout(r32f) uniform image2D myImage;
void myFunc(in coherent volatile image2D someImage) {
}
void main() {
  myFunc(myImage);
}

// ShaderValidation FragmentShaderValidationTest, OverqualifyingImageParameter