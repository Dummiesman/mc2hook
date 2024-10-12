#pragma once
#include "mc2hook_defs.h"
#define COERCE_FLOAT(x)  *reinterpret_cast< float* >( &x )
#define STRFY(x) #x
#define ASSERT_SIZEOF(t,s)      static_assert(sizeof(t) <= s, "Size mismatch for type '" STRFY(t) "'; too big (expected " STRFY(s) ")"); \
                                static_assert(sizeof(t) == s, "Size mismatch for type '" STRFY(t) "'; too small (expected " STRFY(s) ")")