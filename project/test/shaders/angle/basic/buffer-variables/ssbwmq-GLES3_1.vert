#version 310 es
layout(binding = 3) writeonly buffer buf {
  int b1;
  buffer int b2;
};
void main() {
}

// BufferVariables BufferVariablesTest, ShaderStorageBlockWithMemoryQualifier