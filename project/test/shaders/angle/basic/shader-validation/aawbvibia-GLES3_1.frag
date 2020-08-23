#version 310 es

layout(std140) buffer bufferName {
  uint u1;
}
instanceName[1];

void main() {
  atomicAdd(instanceName[0].u1, 2u);
}

// ShaderValidation FragmentShaderValidationTest, AtomicAddWithBufferVariableInBlockInstanceArray