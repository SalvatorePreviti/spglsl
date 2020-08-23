#version 310 es
layout(binding = 3) buffer buf {
  readonly float f;
};
void foo(float a) {
}
void main() {
  foo(f);
}

// BufferVariables BufferVariablesTest, UserDefinedFunctionWithReadonlyBufferVariableInArgument