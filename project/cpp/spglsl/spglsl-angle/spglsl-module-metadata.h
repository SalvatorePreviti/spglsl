#ifndef _SPGLSL_MODULE_METADATA_H_
#define _SPGLSL_MODULE_METADATA_H_

#include <angle/src/compiler/translator/ParseContext.h>
#include <angle/src/compiler/translator/Pragma.h>

class SpglslModuleMetadata {
 public:
  sh::GLenum shaderType;
  ShShaderSpec shaderSpec;
  int shaderVersion;
  TPragma pragma;
  bool earlyFragmentTestsSpecified;
  int numViews;
  int geometryShaderMaxVertices;
  int geometryShaderInvocations;
  sh::TLayoutPrimitiveType geometryShaderInputPrimitiveType;
  sh::TLayoutPrimitiveType geometryShaderOutputPrimitiveType;
  bool computeShaderLocalSizeDeclared;
  sh::WorkGroupSize computeShaderLocalSize;
  bool early_fragment_tests;
  bool highPrecisionSupported;

  SpglslModuleMetadata();

  void setFromParseContext(const sh::TParseContext & parseContext);
};

#endif