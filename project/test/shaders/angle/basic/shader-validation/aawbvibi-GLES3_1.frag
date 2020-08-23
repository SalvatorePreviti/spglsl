#version 310 es

layout(std140) buffer bufferName {
  uint u1;
}
instanceName;

void main() {
  atomicAdd(instanceName.u1, 2u);
}

// ShaderValidation FragmentShaderValidationTest, AtomicAddWithBufferVariableInBlockInstance