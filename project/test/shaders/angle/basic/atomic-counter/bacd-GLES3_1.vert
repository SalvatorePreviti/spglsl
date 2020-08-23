#version 310 es
layout(binding = 2, offset = 4) uniform atomic_uint a;
layout(binding = 2) uniform atomic_uint b;
layout(binding = 2, offset = 12) uniform atomic_uint c;
layout(binding = 1, offset = 4) uniform atomic_uint d;
void main() {
}

// AtomicCounter AtomicCounterTest, BasicAtomicCounterDeclaration