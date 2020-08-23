#version 310 es

layout(std140) buffer bufferName {
  uvec4 u1[2];
}
instanceName[3];

void main() {
  atomicAdd(instanceName[2].u1[1].y, 2u);
}

// ShaderValidation FragmentShaderValidationTest, AtomicAddWithSwizzle