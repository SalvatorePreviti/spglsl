#version 310 es
layout(binding = 3) buffer buf {
  int b1;
  buffer int b2;
};
out vec4 my;
void main() {
  // my.x = float(b1);
}

// BufferVariables BufferVariablesTest, BasicShaderStorageBlockDeclaration