#version 310 es

layout(std140) buffer blockName {
  uint data[2];
}
instanceName;

void main() {
  atomicAdd(instanceName.data[0], 2u);
}

// ShaderValidation FragmentShaderValidationTest, AtomicAddWithElementOfArrayInBlockInstance