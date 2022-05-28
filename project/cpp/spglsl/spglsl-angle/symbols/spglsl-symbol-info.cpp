#include "./spglsl-symbol-info.h"
#include <string>
#include "../spglsl-scoped-traverser.h"

void _loadSymbolName(SpglslSymbolInfo & entry) {
  const sh::TSymbol * symbol = entry.symbol;
  if (!symbol || !symbol->uniqueId().get() || symbol->symbolType() == sh::SymbolType::Empty) {
    return;
  }
  if (symbol->isFunction() && static_cast<const sh::TFunction *>(symbol)->isMain()) {
    entry.symbolName = "main";
    return;
  }

  const sh::ImmutableString & n = symbol->name();
  size_t len = n.length();

  if (symbol->isVariable()) {
    const auto * variable = static_cast<const sh::TVariable *>(symbol);
    if (variable->getType().isInterfaceBlock()) {
      // Interface blocks declared as "_NUM" should be unnamed, they wrongly come like this from glslang.
      bool isHiddenName = len > 1 && n[0] == '_';
      for (size_t i = 1; i < len; ++i) {
        if (!std::isdigit(n[i])) {
          isHiddenName = false;
          break;
        }
      }
      if (isHiddenName) {
        return;
      }
    }
  }

  entry.symbolName = std::string(n.data(), len);
}

bool SpglslSymbols::has(const sh::TSymbol * symbol) const {
  auto found = this->_map.find(symbol);
  return found != this->_map.end() && found->second.symbol != nullptr;
}

SpglslSymbolInfo & SpglslSymbols::get(const sh::TSymbol * symbol) {
  SpglslSymbolInfo & result = this->_map[symbol];
  if (!result.symbol && symbol) {
    result.symbol = symbol;
    _loadSymbolName(result);
  }
  return result;
}

SpglslSymbols::SpglslSymbols(sh::TSymbolTable * symbolTable) : symbolTable(symbolTable) {
  auto & n = this->_map[nullptr];
}

static std::unordered_set<std::string> spglslReservedWordSet({"main", "and", "or", "xor", "not", "EmitVertex",
    "EndPrimitive", "abs", "acos", "acosh", "active", "all", "angle_BaseInstance", "angle_BaseVertex", "any", "asin",
    "asinh", "asm", "atan", "atanh", "atomicAdd", "atomicAnd", "atomicCompSwap", "atomicCounter",
    "atomicCounterDecrement", "atomicCounterIncrement", "atomicExchange", "atomicMax", "atomicMin", "atomicOr",
    "atomicXor", "attribute", "barrier", "bitCount", "bitfieldExtract", "bitfieldInsert", "bitfieldReverse", "bool",
    "break", "bvec2", "bvec3", "bvec4", "case", "cast", "ceil", "centroid", "clamp", "class", "common", "const",
    "continue", "cos", "cosh", "cross", "dFdx", "dFdy", "default", "degrees", "determinant", "diff", "discard",
    "distance", "dmat2", "dmat2x2", "dmat2x3", "dmat2x4", "dmat3", "dmat3x2", "dmat3x3", "dmat3x4", "dmat4", "dmat4x2",
    "dmat4x3", "dmat4x4", "do", "dot", "double", "dvec2", "dvec3", "dvec4", "else", "enum", "equal", "exp", "exp2",
    "extern", "external", "faceforward", "false", "far", "filter", "findLSB", "findMSB", "fixed", "flat", "float",
    "floatBitsToInt", "floatBitsToUint", "floor", "fma", "for", "fract", "frexp", "fvec2", "fvec3", "fvec4", "fwidth",
    "gl_BaseInstance", "gl_BaseVertex", "gl_ClipDistance", "gl_DepthRange", "gl_DepthRangeParameters", "gl_DrawID",
    "gl_FragColor", "gl_FragCoord", "gl_FragData", "gl_FragDepth", "gl_FragDepthEXT", "gl_FrontFacing",
    "gl_GlobalInvocationID", "gl_HelperInvocation", "gl_InstanceID", "gl_InstanceIndex", "gl_InvocationID",
    "gl_LastFragColor", "gl_LastFragColorARM", "gl_LastFragData", "gl_Layer", "gl_LocalInvocationID",
    "gl_LocalInvocationIndex", "gl_MaxAtomicCounterBindings", "gl_MaxAtomicCounterBufferSize", "gl_MaxClipDistances",
    "gl_MaxCombinedAtomicCounterBuffers", "gl_MaxCombinedAtomicCounterBuffers(", "gl_MaxCombinedAtomicCounters",
    "gl_MaxCombinedImageUniforms", "gl_MaxCombinedShaderOutputResources", "gl_MaxCombinedShaderOutputResources(",
    "gl_MaxCombinedTextureImageUnits", "gl_MaxComputeAtomicCounterBuffers", "gl_MaxComputeAtomicCounterBuffers(",
    "gl_MaxComputeAtomicCounters", "gl_MaxComputeImageUniforms", "gl_MaxComputeTextureImageUnits",
    "gl_MaxComputeUniformComponents", "gl_MaxComputeWorkGroupCount", "gl_MaxComputeWorkGroupSize", "gl_MaxDrawBuffers",
    "gl_MaxDualSourceDrawBuffersEXT", "gl_MaxFragmentAtomicCounterBuffers", "gl_MaxFragmentAtomicCounterBuffers(",
    "gl_MaxFragmentAtomicCounters", "gl_MaxFragmentImageUniforms", "gl_MaxFragmentInputVectors",
    "gl_MaxFragmentUniformVectors", "gl_MaxGeometryAtomicCounterBuffers", "gl_MaxGeometryAtomicCounterBuffers(",
    "gl_MaxGeometryAtomicCounters", "gl_MaxGeometryImageUniforms", "gl_MaxGeometryInputComponents",
    "gl_MaxGeometryOutputComponents", "gl_MaxGeometryOutputVertices", "gl_MaxGeometryTextureImageUnits",
    "gl_MaxGeometryTotalOutputComponents", "gl_MaxGeometryTotalOutputComponents(", "gl_MaxGeometryUniformComponents",
    "gl_MaxImageUnits", "gl_MaxProgramTexelOffset", "gl_MaxTextureImageUnits", "gl_MaxVaryingVectors",
    "gl_MaxVertexAtomicCounterBuffers", "gl_MaxVertexAtomicCounterBuffers(", "gl_MaxVertexAtomicCounters",
    "gl_MaxVertexAttribs", "gl_MaxVertexImageUniforms", "gl_MaxVertexOutputVectors", "gl_MaxVertexTextureImageUnits",
    "gl_MaxVertexUniformVectors", "gl_MinProgramTexelOffset", "gl_NumWorkGroups", "gl_PerVertex", "gl_PointCoord",
    "gl_PointSize", "gl_Position", "gl_PrimitiveID", "gl_PrimitiveIDIn", "gl_SecondaryFragColorEXT",
    "gl_SecondaryFragDataEXT", "gl_VertexID", "gl_VertexIndex", "gl_ViewID_OVR", "gl_ViewportIndex", "gl_WorkGroupID",
    "gl_WorkGroupSize", "gl_in", "goto", "greaterThan", "greaterThanEqual", "groupMemoryBarrier", "half", "highp",
    "hvec2", "hvec3", "hvec4", "if", "iimage1D", "iimage1DArray", "iimage2D", "iimage2DArray", "iimage3D",
    "iimageBuffer", "iimageCube", "image1D", "image1DArray", "image1DArrayShadow", "image1DShadow", "image2D",
    "image2DArray", "image2DArrayShadow", "image2DShadow", "image3D", "imageBuffer", "imageCube", "imageLoad",
    "imageSize", "imageStore", "imulExtended", "in", "inline", "inout", "input", "int", "intBitsToFloat", "interface",
    "invariant", "inverse", "inversesqrt", "isampler1D", "isampler1DArray", "isampler2D", "isampler2DArray",
    "isampler2DMS", "isampler2DMSArray", "isampler2DRect", "isampler3D", "isamplerBuffer", "isamplerCube",
    "isamplerCubeArray", "isinf", "isnan", "ivec2", "ivec3", "ivec4", "layout", "ldexp", "length", "lessThan",
    "lessThanEqual", "log", "log2", "long", "lowp", "mat2", "mat2x2", "mat2x3", "mat2x4", "mat3", "mat3x2", "mat3x3",
    "mat3x4", "mat4", "mat4x2", "mat4x3", "mat4x4", "matrixCompMult", "max", "mediump", "memoryBarrier",
    "memoryBarrierAtomicCounter", "memoryBarrierBuffer", "memoryBarrierImage", "memoryBarrierShared", "min", "mix",
    "mod", "modf", "namespace", "near", "noinline", "noperspective", "normalize", "not", "notEqual", "out",
    "outerProduct", "output", "packHalf2x16", "packSnorm2x16", "packSnorm4x8", "packUnorm2x16", "packUnorm4x8",
    "packed", "partition", "patch", "pow", "precision", "public", "radians", "reflect", "refract", "return",
    "rgb_2_yuv", "round", "roundEven", "row_major", "sample", "sampler1D", "sampler1DArray", "sampler1DArrayShadow",
    "sampler1DShadow", "sampler2D", "sampler2DArray", "sampler2DArrayShadow", "sampler2DMS", "sampler2DMSArray",
    "sampler2DRect", "sampler2DRectShadow", "sampler2DShadow", "sampler3D", "sampler3DRect", "samplerBuffer",
    "samplerCube", "samplerCubeArray", "samplerCubeArrayShadow", "samplerCubeShadow", "short", "sign", "sin", "sinh",
    "sizeof", "smooth", "smoothstep", "sqrt", "static", "step", "struct", "subroutine", "superp", "switch", "tan",
    "tanh", "template", "texelFetch", "texelFetchOffset", "texture", "texture2D", "texture2DGradEXT", "texture2DLod",
    "texture2DLodEXT", "texture2DProj", "texture2DProjGradEXT", "texture2DProjLod", "texture2DProjLodEXT",
    "texture2DRect", "texture2DRectProj", "texture3D", "texture3DLod", "texture3DProj", "texture3DProjLod",
    "textureCube", "textureCubeGradEXT", "textureCubeLod", "textureCubeLodEXT", "textureGather", "textureGatherOffset",
    "textureGatherOffsets", "textureGrad", "textureGradOffset", "textureLod", "textureLodOffset", "textureOffset",
    "textureProj", "textureProjGrad", "textureProjGradOffset", "textureProjLod", "textureProjLodOffset",
    "textureProjOffset", "textureSize", "textureVideoWEBGL", "this", "transpose", "true", "trunc", "typedef",
    "uaddCarry", "uimage1D", "uimage1DArray", "uimage2D", "uimage2DArray", "uimage3D", "uimageBuffer", "uimageCube",
    "uint", "uintBitsToFloat", "umulExtended", "uniform", "union", "unpackHalf2x16", "unpackSnorm2x16",
    "unpackSnorm4x8", "unpackUnorm2x16", "unpackUnorm4x8", "unsigned", "usampler1D", "usampler1DArray", "usampler2D",
    "usampler2DArray", "usampler2DMS", "usampler2DMSArray", "usampler2DRect", "usampler3D", "usamplerBuffer",
    "usamplerCube", "usamplerCubeArray", "using", "usubBorrow", "uvec2", "uvec3", "uvec4", "varying", "vec2", "vec3",
    "vec4", "void", "volatile", "while", "yuv_2_rgb"});

bool spglslIsValidIdentifier(const std::string & str) {
  // If first character is invalid
  if (!((str[0] >= 'a' && str[0] <= 'z') || (str[0] >= 'A' && str[1] <= 'Z') || str[0] == '_')) {
    return false;
  }
  for (int i = 1; i < str.length(); i++) {
    if (!((str[i] >= 'a' && str[i] <= 'z') || (str[i] >= 'A' && str[i] <= 'Z') || (str[i] >= '0' && str[i] <= '9') ||
            str[i] == '_'))
      return false;
  }
  return true;
}

bool spglslIsWordReserved(const std::string & word) {
  if (word.length() == 0) {
    return true;
  }
  if (word.length() >= 3 && word[0] == 'g' && word[1] == 'l' && word[2] == '_') {
    return true;
  }
  if (!spglslIsValidIdentifier(word)) {
    return true;
  }
  return spglslReservedWordSet.count(word) != 0;
}

bool SpglslSymbolInfo::isReserved() const {
  const auto * symbol = this->symbol;

  if (!symbol || this->symbolName.empty()) {
    return true;
  }

  auto st = symbol->symbolType();
  if (st != sh::SymbolType::UserDefined && st != sh::SymbolType::AngleInternal) {
    return true;
  }

  if (symbol->isFunction()) {
    const auto * func = static_cast<const sh::TFunction *>(symbol);
    return (func->isMain() || func->name().beginsWith("main"));
  }

  if (symbol->isVariable()) {
    const auto * var = static_cast<const sh::TVariable *>(symbol);
    if (var->isInterfaceBlock()) {
      return true;
    }

    const sh::TType & type = var->getType();

    switch (type.getQualifier()) {
      case sh::EvqParamIn:
      case sh::EvqParamOut:
      case sh::EvqParamInOut:
      case sh::EvqParamConst:
      case sh::EvqTemporary:
      case sh::EvqGlobal:
      case sh::EvqConst: return false;

      default: return true;
    }
  }

  // Note: interfaces are considered always reserved.

  return !symbol->isStruct();
}
