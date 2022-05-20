#include "spglsl-get-precisions-traverser.h"

#include <angle/src/compiler/translator/Symbol.h>

SpglslGetPrecisionsTraverser::SpglslGetPrecisionsTraverser() : sh::TIntermTraverser(true, false, false, nullptr) {
  this->reset();
}

void SpglslGetPrecisionsTraverser::reset() {
  this->floatPrecision = sh::EbpUndefined;
  this->intPrecision = sh::EbpUndefined;
  this->_floatPrecisionLowp = 0;
  this->_floatPrecisionMediump = 0;
  this->_floatPrecisionHighp = 0;
  this->_intPrecisionLowp = 0;
  this->_intPrecisionMediump = 0;
  this->_intPrecisionHighp = 0;
  this->_declaredStructs.clear();
  this->_declaredInterfaces.clear();
}

void SpglslGetPrecisionsTraverser::traverseNode(sh::TIntermNode * node) {
  if (node) {
    node->traverse(this);
  }
}

void SpglslGetPrecisionsTraverser::add(sh::TBasicType basicType, sh::TPrecision precision) {
  switch (basicType) {
    case sh::EbtInt:
    case sh::EbtUInt:
      switch (precision) {
        case sh::EbpUndefined: return;
        case sh::EbpLow: ++this->_intPrecisionLowp; break;
        case sh::EbpMedium: ++this->_intPrecisionMediump; break;
        case sh::EbpHigh: ++this->_intPrecisionHighp; break;
        default: break;
      }
      break;
    case sh::EbtFloat:
      switch (precision) {
        case sh::EbpUndefined: return;
        case sh::EbpLow: ++this->_floatPrecisionLowp; break;
        case sh::EbpMedium: ++this->_floatPrecisionMediump; break;
        case sh::EbpHigh: ++this->_floatPrecisionHighp; break;
        default: break;
      }
      break;
    default: break;
  }
}

void SpglslGetPrecisionsTraverser::add(const sh::TType & type) {
  if (type.getBasicType() == sh::EbtStruct && type.getStruct() &&
      this->_declaredStructs.emplace(type.getStruct()).second) {
    for (const auto & field : type.getStruct()->fields()) {
      if (field) {
        auto * fieldType = field->type();
        if (fieldType) {
          this->add(*fieldType);
        }
      }
    }
  } else if (type.getBasicType() == sh::EbtInterfaceBlock && type.getInterfaceBlock() &&
      this->_declaredInterfaces.emplace(type.getInterfaceBlock()).second) {
    for (const auto & field : type.getInterfaceBlock()->fields()) {
      if (field) {
        auto * fieldType = field->type();
        if (fieldType) {
          this->add(*fieldType);
        }
      }
    }
  } else {
    this->add(type.getBasicType(), type.getPrecision());
  }
}

void SpglslGetPrecisionsTraverser::add(const sh::TVariable * variable) {
  if (variable) {
    this->add(variable->getType());
  }
}

bool SpglslGetPrecisionsTraverser::visitDeclaration(sh::Visit visit, sh::TIntermDeclaration * node) {
  size_t childCount = node->getChildCount();
  for (size_t i = 0; i < childCount; ++i) {
    auto * child = node->getChildNode(i);
    if (child) {
      sh::TIntermSymbol * symbolNode = child->getAsSymbolNode();
      if (symbolNode) {
        this->add(symbolNode->getType());
        continue;
      }
      sh::TIntermBinary * binaryNode = child->getAsBinaryNode();
      if (binaryNode) {
        sh::TIntermTyped * left = binaryNode->getLeft();
        if (left) {
          this->add(left->getType());
        }
        continue;
      }
    }
  }
  return true;
}

bool SpglslGetPrecisionsTraverser::visitGlobalQualifierDeclaration(sh::Visit visit,
    sh::TIntermGlobalQualifierDeclaration * node) {
  sh::TIntermSymbol * symbolNode = node->getSymbol();
  if (symbolNode) {
    this->add(symbolNode->getType());
  }
  return true;
}

bool SpglslGetPrecisionsTraverser::visitFunctionDefinition(sh::Visit visit, sh::TIntermFunctionDefinition * node) {
  sh::TIntermFunctionPrototype * proto = node->getFunctionPrototype();
  if (proto) {
    const sh::TFunction * func = proto->getFunction();
    if (func) {
      this->add(func->getReturnType());
      for (size_t i = 0, len = func->getParamCount(); i < len; ++i) {
        this->add(func->getParam(i));
      }
    }
  }
  return true;
}

bool SpglslGetPrecisionsTraverser::visitAggregate(sh::Visit visit, sh::TIntermAggregate * node) {
  if (node->getOp() == sh::EOpConstruct) {
    this->add(node->getType());
  }
  return true;
}

void SpglslGetPrecisionsTraverser::count() {
  int fc = this->_floatPrecisionMediump;
  this->floatPrecision = sh::EbpMedium;
  if (this->_floatPrecisionHighp > fc) {
    fc = this->_floatPrecisionHighp;
    this->floatPrecision = sh::EbpHigh;
  }
  if (this->_floatPrecisionLowp > fc) {
    this->floatPrecision = sh::EbpLow;
  }

  fc = this->_intPrecisionMediump;
  this->intPrecision = sh::EbpMedium;
  if (this->_intPrecisionHighp > fc) {
    fc = this->_intPrecisionHighp;
    this->intPrecision = sh::EbpHigh;
  }
  if (this->_intPrecisionLowp > fc) {
    this->intPrecision = sh::EbpLow;
  }
}
