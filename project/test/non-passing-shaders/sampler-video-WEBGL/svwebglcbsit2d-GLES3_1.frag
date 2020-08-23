#version 300 es
#extension GL_WEBGL_video_texture : require
precision mediump float;
uniform mediump samplerVideoWEBGL s;
out vec4 my_FragColor;
void main() {
  my_FragColor = textureVideoWEBGL(s, vec2(0.0, 0.0));
}

// SamplerVideoWEBGL SamplerVideoWEBGLTest, SamplerVideoWEBGLCanBeSupportedInTexture2D