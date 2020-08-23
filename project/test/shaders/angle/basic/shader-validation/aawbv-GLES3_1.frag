#version 310 es

layout(std140) buffer bufferName1 {
  uint u1;
};

void main() {
  atomicAdd(u1, 2u);
}

// ShaderValidation FragmentShaderValidationTest, AtomicAddWithBufferVariable