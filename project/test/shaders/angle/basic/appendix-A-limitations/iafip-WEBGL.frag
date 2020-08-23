#version 310 es
precision mediump float;
out vec4 my_FragColor;
void fun(int b, inout int a) {
  a += b;
}
void main() {
  for (int i = 0; i < 2; ++i) {
    int a = 1;
    fun(i, a);
  }
  my_FragColor = vec4(0.0);
}

// AppendixALimitations AppendixALimitationsTest, IndexAsFunctionInParameter