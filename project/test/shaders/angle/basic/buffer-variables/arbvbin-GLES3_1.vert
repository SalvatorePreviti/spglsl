#version 310 es
layout(binding = 3) buffer buf {
  readonly float f;
}
instanceBuffer;
void main() {
  gl_Position.x = instanceBuffer.f;
}

// BufferVariables BufferVariablesTest, AccessReadonlyBufferVariableByInstanceName