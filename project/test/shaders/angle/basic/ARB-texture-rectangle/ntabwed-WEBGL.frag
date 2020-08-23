#version 300 es
#extension GL_ARB_texture_rectangle : require
precision mediump float;
uniform mediump sampler2DRect tex;
void main() {
  vec4 color = texture(tex, vec2(1.0));
  color = textureProj(tex, vec3(1.0));
  color = textureProj(tex, vec4(1.0));
}

// ARB_texture_rectangle ARBTextureRectangleTest, NewTypeAndBuiltingsWithExtensionDirective