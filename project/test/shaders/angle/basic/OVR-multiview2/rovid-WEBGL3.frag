#version 310 es
#extension GL_OVR_multiview2 : require
precision highp float;
out vec4 outColor;
void main() {
  outColor = vec4(gl_ViewID_OVR, 0, 0, 1);
}

// OVR_multiview2 OVRMultiview2FragmentShaderTest, ReadOfViewID