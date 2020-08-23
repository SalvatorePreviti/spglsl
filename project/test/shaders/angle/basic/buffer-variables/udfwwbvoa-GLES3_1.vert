#version 310 es
layout(binding = 3) buffer buf {
  writeonly float f;
};
void foo(out float a) {
}
void main() {
  foo(f);
}

// BufferVariables BufferVariablesTest, UserDefinedFunctionWithWriteonlyBufferVariableOutArgument