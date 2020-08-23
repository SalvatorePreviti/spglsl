#version 310 es
precision mediump float;
precision mediump image2D;
layout(r32f) uniform restrict image2D myImage;
void myFunc(in restrict image2D someImage) {
}
void main() {
  myFunc(myImage);
}

// ShaderValidation FragmentShaderValidationTest, RestrictQualifierDiscardedInFunctionArgument