#version 310 es

layout(std430) buffer buf {
  int arr[];
};

void main() {
  arr[100];
}

// BufferVariables BufferVariablesTest, IndexRuntimeSizedArray