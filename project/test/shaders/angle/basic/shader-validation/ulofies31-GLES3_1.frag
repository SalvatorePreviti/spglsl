#version 310 es
precision mediump float;
layout(location = 0) in mat4 v_mat;
layout(location = 4) in vec4 v_color1;
in vec2 v_color2;
layout(location = 0) out vec4 o_color;
void main() {
}

// ShaderValidation FragmentShaderValidationTest, UseLocationOnFragmentInES31