#version 310 es
#extension GL_OVR_multiview : require
precision highp float;
out vec4 outColor;
void main() {
  outColor = vec4(gl_FragCoord.xy, 0, 1);
}

// OVR_multiview OVRMultiviewFragmentShaderTest, ReadOfFragCoord