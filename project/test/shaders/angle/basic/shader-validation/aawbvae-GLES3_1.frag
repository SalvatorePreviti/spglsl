#version 310 es

layout(std140) buffer bufferName1 {
  uint u1[2];
};

void main() {
  atomicAdd(u1[0], 2u);
}

// ShaderValidation FragmentShaderValidationTest, AtomicAddWithBufferVariableArrayElement