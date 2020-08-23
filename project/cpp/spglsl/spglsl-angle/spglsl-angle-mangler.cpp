#include "spglsl-angle-mangler.h"

#include <sstream>

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
  if (!((str[0] >= 'a' && str[0] <= 'z') || (str[0] >= 'A' && str[1] <= 'Z') || str[0] == '_'))
    return false;
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

/////////////// SpglslAngleReservedWords ///////////////

SpglslAngleReservedWords::SpglslAngleReservedWords() {
}

bool SpglslAngleReservedWords::isReserved(const std::string & name) {
  return this->definitions.count(name) != 0 || spglslIsWordReserved(name);
}

/////////////// SpglslAngleReservedWordsTraverser ///////////////

SpglslAngleReservedWordsTraverser::SpglslAngleReservedWordsTraverser(SpglslAngleReservedWords & target,
    sh::TSymbolTable * symbolTable) :
    sh::TIntermTraverser(true, false, false, symbolTable), target(target) {
}

std::string SpglslAngleReservedWordsTraverser::getTypeName(const sh::TType & type) {
  if (type.getBasicType() == sh::EbtStruct && type.getStruct()) {
    return this->getSymbolName(type.getStruct());
  }
  if (type.getBasicType() == sh::EbtInterfaceBlock && type.getInterfaceBlock()) {
    return this->getSymbolName(type.getInterfaceBlock());
  }
  return type.getBuiltInTypeNameString();
}

std::string SpglslAngleReservedWordsTraverser::getSymbolName(const sh::TSymbol & symbol) {
  if (!symbol.uniqueId().get() || symbol.symbolType() == sh::SymbolType::Empty) {
    return Strings::empty;
  }
  if (symbol.isFunction() && static_cast<const sh::TFunction &>(symbol).isMain()) {
    return "main";
  }
  const sh::ImmutableString & n = symbol.name();
  return std::string(n.data(), n.length());
}

bool SpglslAngleReservedWordsTraverser::add(const sh::ImmutableString & name) {
  return this->add(name.data());
}

bool SpglslAngleReservedWordsTraverser::add(const std::string & name) {
  return spglslReservedWordSet.count(name) == 0 && this->target.definitions.emplace(name).second;
}

void SpglslAngleReservedWordsTraverser::add(const sh::TType & type) {
  this->add(this->getTypeName(type));
  if (type.getBasicType() == sh::EbtStruct && type.getStruct() &&
      this->_declaredStructs.emplace(type.getStruct()).second) {
    for (const auto & field : type.getStruct()->fields()) {
      if (field) {
        auto fieldType = field->type();
        if (fieldType) {
          this->add(*fieldType);
        }
        this->add(field->name());
      }
    }
  } else if (type.getBasicType() == sh::EbtInterfaceBlock && type.getInterfaceBlock() &&
      this->_declaredInterfaces.emplace(type.getInterfaceBlock()).second) {
    for (const auto & field : type.getInterfaceBlock()->fields()) {
      if (field) {
        this->add(field->name());
        auto fieldType = field->type();
        if (fieldType) {
          this->add(*fieldType);
        }
      }
    }
  }
}

void SpglslAngleReservedWordsTraverser::add(const sh::TVariable * variable) {
  if (variable) {
    this->add(this->getSymbolName(variable));
    this->add(variable->getType());
  }
}

bool SpglslAngleReservedWordsTraverser::visitDeclaration(sh::Visit visit, sh::TIntermDeclaration * node) {
  size_t childCount = node->getChildCount();
  for (size_t i = 0; i < childCount; ++i) {
    auto child = node->getChildNode(i);
    if (child) {
      sh::TIntermSymbol * symbolNode = child->getAsSymbolNode();
      if (symbolNode) {
        this->add(&symbolNode->variable());
        continue;
      }
      sh::TIntermBinary * binaryNode = child->getAsBinaryNode();
      if (binaryNode) {
        sh::TIntermTyped * left = binaryNode->getLeft();
        if (left) {
          sh::TIntermSymbol * leftAsSym = left->getAsSymbolNode();
          if (leftAsSym) {
            this->add(&leftAsSym->variable());
          } else {
            this->add(left->getType());
          }
        }
        continue;
      }
    }
  }
  return true;
}

bool SpglslAngleReservedWordsTraverser::visitGlobalQualifierDeclaration(sh::Visit visit,
    sh::TIntermGlobalQualifierDeclaration * node) {
  sh::TIntermSymbol * symbolNode = node->getSymbol();
  if (symbolNode) {
    this->add(&symbolNode->variable());
  }
  return true;
}

void SpglslAngleReservedWordsTraverser::visitFunctionPrototype(sh::TIntermFunctionPrototype * node) {
  const sh::TFunction * func = node->getFunction();
  if (func) {
    this->add(this->getSymbolName(func));
    this->add(func->getReturnType());
    for (size_t i = 0, len = func->getParamCount(); i < len; ++i) {
      this->add(func->getParam(i));
    }
  }
}

bool SpglslAngleReservedWordsTraverser::visitAggregate(sh::Visit visit, sh::TIntermAggregate * node) {
  if (node->getOp() == EOpConstruct) {
    this->add(node->getType());
  }
  return true;
}

/////////////// SpglslAngleManglerNameGenerator ///////////////

static const char * _leadingChars = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
static const char * _allChars = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890";

static const int _leadingCharsLen = (int)strlen(_leadingChars);
static const int _allCharsLen = (int)strlen(_allChars);

SpglslAngleManglerNameGenerator::SpglslAngleManglerNameGenerator(SpglslAngleReservedWords & reserved,
    SpglslAngleManglerNameGenerator * parent) :
    nextNameIndex(parent ? parent->nextNameIndex : 0), reserved(reserved), parent(nullptr) {
}

std::string SpglslAngleManglerNameGenerator::generateShortName(int index) {
  std::ostringstream ss;
  ss.put(_leadingChars[index % _leadingCharsLen]);
  index = floor((double)index / (double)_leadingCharsLen);
  while (index > 0) {
    index -= 1;
    ss.put(_allChars[index % _allCharsLen]);
    index = floor((double)index / (double)_allCharsLen);
  }
  return ss.str();
}

bool SpglslAngleManglerNameGenerator::getShortName(const std::string & input, std::string & out) {
  if (input.length() <= 1) {
    return false;
  }
  const auto found = this->renameMap.find(input);
  if (found != this->renameMap.end()) {
    out = found->second;
    return true;
  }
  for (;;) {
    std::string n = this->generateShortName(this->nextNameIndex);
    if (this->isReserved(n)) {
      ++this->nextNameIndex;
      continue;
    }
    if (n.length() >= input.length()) {
      return false;  // Too long, no point using this.
    }
    ++this->nextNameIndex;
    if (this->usedKeys.emplace(n).second) {
      renameMap.emplace(input, n);
      out = n;
      return true;
    }
  }
}

bool SpglslAngleManglerNameGenerator::isReserved(const std::string & input) {
  if (this->renameMap.find(input) != this->renameMap.end()) {
    return true;
  }
  if (this->usedKeys.find(input) != this->usedKeys.end()) {
    return true;
  }
  if (this->parent) {
    return this->parent->isReserved(input);
  }
  return this->reserved.isReserved(input);
}

/////////////// SpglslAngleManglerTraverser ///////////////

SpglslAngleManglerTraverser::SpglslAngleManglerTraverser(SpglslAngleReservedWords & reserved,
    sh::TSymbolTable * symbolTable) :
    sh::TIntermTraverser(true, false, true, symbolTable),
    reserved(reserved),
    namesRoot(new SpglslAngleManglerNameGenerator(reserved, nullptr)) {
  this->_namesStack.push(this->namesRoot);
}

SpglslAngleManglerTraverser::~SpglslAngleManglerTraverser() {
  while (!this->_namesStack.empty()) {
    this->scopePop();
  }
}

SpglslAngleManglerNameGenerator * SpglslAngleManglerTraverser::scopePush() {
  SpglslAngleManglerNameGenerator * result =
      new SpglslAngleManglerNameGenerator(this->reserved, this->_namesStack.top());
  this->_namesStack.push(result);
  return result;
}

bool SpglslAngleManglerTraverser::scopePop() {
  if (this->_namesStack.empty()) {
    return false;
  }
  delete this->_namesStack.top();
  this->_namesStack.pop();
  return true;
}

bool SpglslAngleManglerTraverser::visitLoop(sh::Visit visit, sh::TIntermLoop * node) {
  if (node->getType() == sh::ELoopFor) {
    if (visit == sh::PreVisit) {
      this->scopePush();
    } else if (visit == sh::PostVisit) {
      this->scopePop();
    }
  }
  return true;
}

bool SpglslAngleManglerTraverser::visitFunctionDefinition(sh::Visit visit, sh::TIntermFunctionDefinition * node) {
  if (visit == sh::PreVisit) {
    const sh::TFunction * func = node->getFunction();

    this->rename(func, false);

    this->scopePush();

    if (func) {
      for (size_t i = 0, len = func->getParamCount(); i < len; ++i) {
        this->rename(func->getParam(i), true);
      }
    }

  } else if (visit == sh::PostVisit) {
    this->scopePop();
  }
  return true;
}

bool SpglslAngleManglerTraverser::visitBlock(sh::Visit visit, sh::TIntermBlock * node) {
  if (visit == sh::PreVisit) {
    this->scopePush();
  } else if (visit == sh::PostVisit) {
    this->scopePop();
  }
  return true;
}

bool SpglslAngleManglerTraverser::visitDeclaration(sh::Visit visit, sh::TIntermDeclaration * node) {
  if (visit == sh::PreVisit) {
    size_t childCount = node->getChildCount();
    for (size_t i = 0; i < childCount; ++i) {
      auto child = node->getChildNode(i);
      if (child) {
        sh::TIntermSymbol * symbolNode = child->getAsSymbolNode();
        if (symbolNode) {
          this->rename(&symbolNode->variable(), false);
          continue;
        }
        sh::TIntermBinary * binaryNode = child->getAsBinaryNode();
        if (binaryNode) {
          sh::TIntermTyped * left = binaryNode->getLeft();
          if (left) {
            sh::TIntermSymbol * leftAsSym = left->getAsSymbolNode();
            if (leftAsSym) {
              this->rename(&leftAsSym->variable(), false);
            }
          }
          continue;
        }
      }
    }
  }
  return true;
}

void SpglslAngleManglerTraverser::rename(const sh::TSymbol * symbol, bool isParameter) {
  if (!symbol) {
    return;
  }

  if (symbol->symbolType() != sh::SymbolType::UserDefined) {
    return;
  }

  if (symbol->isFunction()) {
    const sh::TFunction * func = static_cast<const sh::TFunction *>(symbol);
    if (func->isMain() || func->name().beginsWith("main")) {
      return;
    }
    if (spglslIsWordReserved(func->name().data())) {
      return;
    }
  } else if (symbol->isVariable()) {
    const sh::TVariable * var = static_cast<const sh::TVariable *>(symbol);
    if (var->isInterfaceBlock()) {
      return;
    }

    const sh::TType & type = var->getType();
    if (!isParameter && type.getQualifier() != sh::EvqTemporary && type.getQualifier() != sh::EvqGlobal) {
      return;
    }

    if (spglslIsWordReserved(var->name().data())) {
      return;
    }
  } else {
    return;  // struct and interface blocks still not supported.
  }

  if (this->reserved.symRemap.count(symbol) != 0) {
    return;
  }

  std::string renamed;
  if (this->_namesStack.top()->getShortName(symbol->name().data(), renamed)) {
    this->reserved.symRemap.emplace(symbol, renamed);
  }
}
