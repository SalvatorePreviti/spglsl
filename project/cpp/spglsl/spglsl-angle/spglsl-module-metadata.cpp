#include "spglsl-module-metadata.h"

#include <angle/include/angle_gl.h>

SpglslModuleMetadata::SpglslModuleMetadata() :
    shaderVersion(100),
    earlyFragmentTestsSpecified(false),
    numViews(0),
    geometryShaderMaxVertices(0),
    geometryShaderInvocations(0),
    geometryShaderInputPrimitiveType(sh::EptUndefined),
    geometryShaderOutputPrimitiveType(sh::EptUndefined),
    computeShaderLocalSizeDeclared(false),
    early_fragment_tests(false),
    highPrecisionSupported(true) {
  this->computeShaderLocalSize.fill(0);
}

void SpglslModuleMetadata::setFromParseContext(const sh::TParseContext & parseContext) {
  this->shaderVersion = parseContext.getShaderVersion();
  this->shaderType = parseContext.getShaderType();
  this->shaderSpec = parseContext.getShaderSpec();

  this->pragma = parseContext.pragma();

  this->earlyFragmentTestsSpecified = parseContext.isEarlyFragmentTestsSpecified();

  this->computeShaderLocalSizeDeclared = parseContext.isComputeShaderLocalSizeDeclared();
  this->computeShaderLocalSize = parseContext.getComputeShaderLocalSize();

  this->numViews = parseContext.getNumViews();

  this->geometryShaderInputPrimitiveType = parseContext.getGeometryShaderInputPrimitiveType();
  this->geometryShaderOutputPrimitiveType = parseContext.getGeometryShaderOutputPrimitiveType();
  this->geometryShaderMaxVertices = parseContext.getGeometryShaderMaxVertices();
  this->geometryShaderInvocations = parseContext.getGeometryShaderInvocations();

  this->early_fragment_tests = this->shaderType == GL_FRAGMENT_SHADER && parseContext.isEarlyFragmentTestsSpecified();
}
