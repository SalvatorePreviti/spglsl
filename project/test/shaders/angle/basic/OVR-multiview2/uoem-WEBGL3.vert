#version 310 es
#ifdef GL_OVR_multiview2
#  if (GL_OVR_multiview2 == 1)
void main() {
  gl_Position = vec4(0.0, 0.0, 0.0, 1.0);
}
#  endif
#endif

// OVR_multiview2 OVRMultiview2VertexShaderTest, UseOfExtensionMacro