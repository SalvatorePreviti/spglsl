#include "spglsl-t-compiler.h"

SpglslTCompiler::SpglslTCompiler(sh::GLenum shaderType, ShShaderSpec shaderSpec) :
    sh::TCompiler(shaderType, shaderSpec, SH_ESSL_OUTPUT) {
}

bool SpglslTCompiler::shouldFlattenPragmaStdglInvariantAll() {
  return true;
}

bool SpglslTCompiler::translate(sh::TIntermBlock * root,
    const ShCompileOptions & compileOptions,
    sh::PerformanceDiagnostics * perfDiagnostics) {
  return false;
}

SpglslTCompilerHolder::SpglslTCompilerHolder(sh::GLenum shaderType, int shaderVersion) :
    tCompiler(shaderType, shaderVersion > 300 ? SH_WEBGL3_SPEC : SH_WEBGL2_SPEC),
    extensionBehavior(const_cast<sh::TExtensionBehavior &>(this->tCompiler.getExtensionBehavior())),
    infoSink(this->tCompiler.getInfoSink()),
    diagnostics(this->infoSink.info) {
}

TScopedSymbolTableLevel::TScopedSymbolTableLevel(sh::TSymbolTable * symbolTable) : symbolTable(symbolTable) {
  symbolTable->push();
}

TScopedSymbolTableLevel::~TScopedSymbolTableLevel() {
  auto * table = this->symbolTable;
  if (table) {
    while (!table->isEmpty())
      table->pop();
  }
}
