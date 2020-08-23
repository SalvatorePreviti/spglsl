#version 310 es
#extension GL_OVR_multiview : require
layout(num_views = 2) in;
layout(num_views = 2) in;  // Duplicated on purpose
in vec4 pos;
out float myOutput;
void main() {
  if (gl_ViewID_OVR == 0u) {
    gl_Position = pos;
    myOutput = 1.0;
  } else {
    gl_Position = pos + vec4(1.0, 0.0, 0.0, 0.0);
    myOutput = 2.0;
  }
  gl_Position += (gl_ViewID_OVR == 0u) ? 1.0 : 0.0;
}

// OVR_multiview OVRMultiviewVertexShaderTest, ViewIDUsed