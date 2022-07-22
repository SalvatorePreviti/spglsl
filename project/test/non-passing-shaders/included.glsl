#version 300 es
int counter(int n, int i) {
  bool s0 = true;
  while (true) {
    if (!(s0 || (i++ < 3))) {
      break;
    }
    s0 = false;
    n++;
  }
  return n;
}
