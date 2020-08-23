#version 310 es
layout(binding = 3) buffer buf {
  writeonly int b1;
  writeonly buffer int b2;
};
void main() {
}

// BufferVariables BufferVariablesTest, BufferVariablesWithMemoryQualifier