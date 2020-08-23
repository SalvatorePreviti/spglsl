#version 310 es
precision mediump float;
uniform int u;
out vec4 my_FragColor;
void main() {
  int foo = 1;
  do {
    foo *= u;
  } while (foo < 8);
  my_FragColor = vec4(foo) * 0.1;
}

// RewriteDoWhile RewriteDoWhileCrashTest, RunsSuccessfully