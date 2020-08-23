#version 300 es
#extension GL_ARB_texture_rectangle : require
precision mediump float;
mediump uniform sampler2DRect tex;
void main() {
  vec4 color = texture2DRect(tex, vec2(1.0));
  color = texture2DRectProj(tex, vec3(1.0));
  color = texture2DRectProj(tex, vec4(1.0));
}

// ARB_texture_rectangle ARBTextureRectangleTest, NewTypeAndBuiltingsWithExtensionDirective