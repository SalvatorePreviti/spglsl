#version 310 es
#extension GL_EXT_texture_cube_map_array : enable
precision mediump float;
uniform highp isamplerCubeArray u_sampler;
void main() {
  vec4 color = vec4(texture(u_sampler, vec4(0, 0, 0, 0)));
}

// OES_texture_cube_map_array EXTTextureCubeMapArrayTest, ExtensionEnabledEXT