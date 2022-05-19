#ifndef _SPGLSL_GLSL_PRECISIONS_
#define _SPGLSL_GLSL_PRECISIONS_

#include <angle/src/compiler/translator/BaseTypes.h>
#include <angle/src/compiler/translator/ImmutableString.h>
#include <angle/src/compiler/translator/IntermNode.h>
#include <angle/src/compiler/translator/Operator_autogen.h>
#include <angle/src/compiler/translator/Types.h>

struct SpglslGlslPrecisions {
  sh::TPrecision floatPrecision = sh::EbpUndefined;
  sh::TPrecision intPrecision = sh::EbpUndefined;

  sh::TPrecision defaultFloatPrecision = sh::EbpUndefined;
  sh::TPrecision defaultIntPrecision = sh::EbpUndefined;
};

#endif
