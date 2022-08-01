#include "spglsl-compile-options.h"

////////////// SpglslCompileOptions //////////////

SpglslCompileOptions::SpglslCompileOptions() :
    compileMode(SpglslCompileMode::Optimize),
    language(EShLangFragment),
    parseShaderVersion(460),
    outputShaderVersion(300),
    recordConstantPrecision(false),
    minify(false),
    mangle(false),
    beautify(false) {
  sh::InitBuiltInResources(&this->angle);
}

SpglslCompileOptions::~SpglslCompileOptions() {
  delete &this->angle;
}

void SpglslCompileOptions::loadFromVal(emscripten::val input, emscripten::val res) {
  this->compileMode = parseSpglslCompileMode(input["compileMode"]);
  this->language = parseEShLanguage(input["language"]);

  if (input["parseVersion"].isNumber()) {
    this->parseShaderVersion = input["parseVersion"].as<int>();
  }

  this->outputShaderVersion = input["outputVersion"].as<int>();

  this->minify = this->compileMode >= SpglslCompileMode::Optimize && input["minify"].as<bool>();
  this->mangle = this->compileMode >= SpglslCompileMode::Optimize && input["mangle"].as<bool>();

  this->mangle_global_map = input["mangle_global_map"];
  this->beautify = this->compileMode >= SpglslCompileMode::Optimize && input["beautify"].as<bool>();
  this->recordConstantPrecision = input["recordConstantPrecision"].as<bool>();

  ShBuiltInResources & a = this->angle;

  a.MaxVertexAttribs = res["maxVertexAttribs"].as<int>();
  a.MaxVertexUniformVectors = res["maxVertexUniformVectors"].as<int>();
  a.MaxVaryingVectors = res["maxVaryingVectors"].as<int>();
  a.MaxVertexTextureImageUnits = res["maxVertexTextureImageUnits"].as<int>();
  a.MaxCombinedTextureImageUnits = res["maxCombinedTextureImageUnits"].as<int>();
  a.MaxTextureImageUnits = res["maxTextureImageUnits"].as<int>();
  a.MaxFragmentUniformVectors = res["maxFragmentUniformVectors"].as<int>();
  a.MaxDrawBuffers = res["maxDrawBuffers"].as<int>();

  // Extensions.
  // Set to 1 to enable the extension, else 0.
  a.OES_standard_derivatives = res["extension_OES_standard_derivatives"].as<bool>();
  a.OES_EGL_image_external = res["extension_OES_EGL_image_external"].as<bool>();
  a.OES_EGL_image_external_essl3 = res["extension_OES_EGL_image_external_essl3"].as<bool>();
  a.NV_EGL_stream_consumer_external = res["extension_NV_EGL_stream_consumer_external"].as<bool>();
  a.ARB_texture_rectangle = res["extension_ARB_texture_rectangle"].as<bool>();
  a.EXT_blend_func_extended = res["extension_EXT_blend_func_extended"].as<bool>();
  a.EXT_draw_buffers = res["extension_EXT_draw_buffers"].as<bool>();
  a.EXT_frag_depth = res["extension_EXT_frag_depth"].as<bool>();
  a.EXT_shader_texture_lod = res["extension_EXT_shader_texture_lod"].as<bool>();
  a.EXT_shader_framebuffer_fetch = res["extension_EXT_shader_framebuffer_fetch"].as<bool>();
  a.NV_shader_framebuffer_fetch = res["extension_NV_shader_framebuffer_fetch"].as<bool>();
  a.NV_shader_noperspective_interpolation = res["extension_NV_shader_noperspective_interpolation"].as<bool>();
  a.ARM_shader_framebuffer_fetch = res["extension_ARM_shader_framebuffer_fetch"].as<bool>();
  a.OVR_multiview = res["extension_OVR_multiview"].as<bool>();
  a.OVR_multiview2 = res["extension_OVR_multiview2"].as<bool>();
  a.EXT_multisampled_render_to_texture = res["extension_EXT_multisampled_render_to_texture"].as<bool>();
  a.EXT_YUV_target = res["extension_EXT_YUV_target"].as<bool>();
  a.EXT_geometry_shader = res["extension_EXT_geometry_shader"].as<bool>();
  a.EXT_gpu_shader5 = res["extension_EXT_gpu_shader5"].as<bool>();
  a.EXT_shader_non_constant_global_initializers =
      res["extension_EXT_shader_non_constant_global_initializers"].as<bool>();
  a.OES_texture_storage_multisample_2d_array = res["extension_OES_texture_storage_multisample_2d_array"].as<bool>();
  a.OES_texture_3D = res["extension_OES_texture_3D"].as<bool>();
  a.ANGLE_texture_multisample = res["extension_ANGLE_texture_multisample"].as<bool>();
  a.ANGLE_multi_draw = res["extension_ANGLE_multi_draw"].as<bool>();
  a.ANGLE_base_vertex_base_instance = res["extension_ANGLE_base_vertex_base_instance"].as<bool>();
  a.WEBGL_video_texture = res["extension_WEBGL_video_texture"].as<bool>();
  a.APPLE_clip_distance = res["extension_APPLE_clip_distance"].as<bool>();
  a.OES_texture_cube_map_array = res["extension_OES_texture_cube_map_array"].as<bool>();
  a.EXT_texture_cube_map_array = res["extension_EXT_texture_cube_map_array"].as<bool>();

  a.NV_draw_buffers = 0;
  a.FragmentPrecisionHigh = 1;

  // GLSL ES 3.0 constants.
  a.MaxVertexOutputVectors = res["maxVertexOutputVectors"].as<int>();
  a.MaxFragmentInputVectors = res["maxFragmentInputVectors"].as<int>();
  a.MinProgramTexelOffset = res["minProgramTexelOffset"].as<int>();
  a.MaxProgramTexelOffset = res["maxProgramTexelOffset"].as<int>();

  // Extension constants.

  // Value of GL_MAX_DUAL_SOURCE_DRAW_BUFFERS_EXT for OpenGL ES output context.
  // Value of GL_MAX_DUAL_SOURCE_DRAW_BUFFERS for OpenGL output context.
  // GLES SL version 100 gl_MaxDualSourceDrawBuffersEXT value for EXT_blend_func_extended.
  a.MaxDualSourceDrawBuffers = res["maxDualSourceDrawBuffers"].as<int>();

  // Value of GL_MAX_VIEWS_OVR.
  a.MaxViewsOVR = res["maxViewsOVR"].as<int>();

  // Name Hashing.
  // Set a 64 bit hash function to enable user-defined name hashing.
  // Default is NULL.
  // ShHashFunction64 HashFunction;

  // The maximum complexity an expression can be when SH_LIMIT_EXPRESSION_COMPLEXITY is turned on.
  a.MaxExpressionComplexity = 65535;

  // The maximum depth a call stack can be.
  a.MaxCallStackDepth = 65535;

  // The maximum number of parameters a function can have when SH_LIMIT_EXPRESSION_COMPLEXITY is
  // turned on.
  a.MaxFunctionParameters = 65535;

  // GLES 3.1 constants

  // texture gather offset constraints.
  a.MinProgramTextureGatherOffset = res["minProgramTextureGatherOffset"].as<int>();
  a.MaxProgramTextureGatherOffset = res["maxProgramTextureGatherOffset"].as<int>();

  // maximum number of available image units
  a.MaxImageUnits = res["maxImageUnits"].as<int>();

  // maximum number of image uniforms in a vertex shader
  a.MaxVertexImageUniforms = res["maxVertexImageUniforms"].as<int>();

  // maximum number of image uniforms in a fragment shader
  a.MaxFragmentImageUniforms = res["maxFragmentImageUniforms"].as<int>();

  // maximum number of image uniforms in a compute shader
  a.MaxComputeImageUniforms = res["maxComputeImageUniforms"].as<int>();

  // maximum total number of image uniforms in a program
  a.MaxCombinedImageUniforms = res["maxCombinedImageUniforms"].as<int>();

  // maximum number of uniform locations
  a.MaxUniformLocations = res["maxUniformLocations"].as<int>();

  // maximum number of ssbos and images in a shader
  a.MaxCombinedShaderOutputResources = res["maxCombinedShaderOutputResources"].as<int>();

  // maximum number of groups in each dimension
  a.MaxComputeWorkGroupCount[0] = res["maxComputeWorkGroupCountX"].as<int>();
  a.MaxComputeWorkGroupCount[1] = res["maxComputeWorkGroupCountY"].as<int>();
  a.MaxComputeWorkGroupCount[2] = res["maxComputeWorkGroupCountZ"].as<int>();

  // maximum number of threads per work group in each dimension
  a.MaxComputeWorkGroupSize[0] = res["maxComputeWorkGroupSizeX"].as<int>();
  a.MaxComputeWorkGroupSize[1] = res["maxComputeWorkGroupSizeY"].as<int>();
  a.MaxComputeWorkGroupSize[2] = res["maxComputeWorkGroupSizeZ"].as<int>();

  // maximum number of total uniform components
  a.MaxComputeUniformComponents = res["maxComputeUniformComponents"].as<int>();

  // maximum number of texture image units in a compute shader
  a.MaxComputeTextureImageUnits = res["maxComputeTextureImageUnits"].as<int>();

  // maximum number of atomic counters in a compute shader
  a.MaxComputeAtomicCounters = res["maxComputeAtomicCounters"].as<int>();

  // maximum number of atomic counter buffers in a compute shader
  a.MaxComputeAtomicCounterBuffers = res["maxComputeAtomicCounterBuffers"].as<int>();

  // maximum number of atomic counters in a vertex shader
  a.MaxVertexAtomicCounters = res["maxVertexAtomicCounters"].as<int>();

  // maximum number of atomic counters in a fragment shader
  a.MaxFragmentAtomicCounters = res["maxFragmentAtomicCounters"].as<int>();

  // maximum number of atomic counters in a program
  a.MaxCombinedAtomicCounters = res["maxCombinedAtomicCounters"].as<int>();

  // maximum binding for an atomic counter
  a.MaxAtomicCounterBindings = res["maxAtomicCounterBindings"].as<int>();

  // maximum number of atomic counter buffers in a vertex shader
  a.MaxVertexAtomicCounterBuffers = res["maxVertexAtomicCounterBuffers"].as<int>();

  // maximum number of atomic counter buffers in a fragment shader
  a.MaxFragmentAtomicCounterBuffers = res["maxFragmentAtomicCounterBuffers"].as<int>();

  // maximum number of atomic counter buffers in a program
  a.MaxCombinedAtomicCounterBuffers = res["maxCombinedAtomicCounterBuffers"].as<int>();

  // maximum number of buffer object storage in machine units
  a.MaxAtomicCounterBufferSize = res["maxAtomicCounterBufferSize"].as<int>();

  // maximum number of uniform block bindings
  a.MaxUniformBufferBindings = res["maxUniformBufferBindings"].as<int>();

  // maximum number of shader storage buffer bindings
  a.MaxShaderStorageBufferBindings = res["maxShaderStorageBufferBindings"].as<int>();

  // maximum point size (higher limit from ALIASED_POINT_SIZE_RANGE)
  a.MaxPointSize = res["maxPointSize"].as<float>();

  // EXT_geometry_shader constants
  a.MaxGeometryUniformComponents = res["maxGeometryUniformComponents"].as<int>();
  a.MaxGeometryUniformBlocks = res["maxGeometryUniformBlocks"].as<int>();
  a.MaxGeometryInputComponents = res["maxGeometryInputComponents"].as<int>();
  a.MaxGeometryOutputComponents = res["maxGeometryOutputComponents"].as<int>();
  a.MaxGeometryOutputVertices = res["maxGeometryOutputVertices"].as<int>();
  a.MaxGeometryTotalOutputComponents = res["maxGeometryTotalOutputComponents"].as<int>();
  a.MaxGeometryTextureImageUnits = res["maxGeometryTextureImageUnits"].as<int>();
  a.MaxGeometryAtomicCounterBuffers = res["maxGeometryAtomicCounterBuffers"].as<int>();
  a.MaxGeometryAtomicCounters = res["maxGeometryAtomicCounters"].as<int>();
  a.MaxGeometryShaderStorageBlocks = res["maxGeometryShaderStorageBlocks"].as<int>();
  a.MaxGeometryShaderInvocations = res["maxGeometryShaderInvocations"].as<int>();
  a.MaxGeometryImageUniforms = res["maxGeometryImageUniforms"].as<int>();

  // Subpixel bits used in rasterization.
  a.SubPixelBits = res["subPixelBits"].as<int>();

  // APPLE_clip_distance/EXT_clip_cull_distance constant
  a.MaxClipDistances = res["maxClipDistances"].as<int>();
}

SpglslCompileMode parseSpglslCompileMode(emscripten::val input) {
  if (input.isString()) {
    return parseSpglslCompileMode(input.as<std::string>());
  }
  return SpglslCompileMode::Optimize;
}

SpglslCompileMode parseSpglslCompileMode(const std::string & input) {
  if (input == "Validate") {
    return SpglslCompileMode::Validate;
  }
  if (input == "Compile") {
    return SpglslCompileMode::Compile;
  }
  if (input == "Optimize") {
    return SpglslCompileMode::Optimize;
  }
  return SpglslCompileMode::Optimize;
}

EShLanguage parseEShLanguage(emscripten::val input) {
  if (input.isString()) {
    return parseEShLanguage(input.as<std::string>());
  }
  return EShLangFragment;
}

EShLanguage parseEShLanguage(const std::string & input) {
  if (input == "Vertex") {
    return EShLangVertex;
  }
  return EShLangFragment;
}
