#pragma once

#include <stdint.h>

#include "DLLExport.h"
#include "Token.h"

typedef ns::TokenType ns_TokenType;
typedef ns::Token ns_Token;

#ifdef __cplusplus
extern "C" {
#endif

// Create/Destroy
NIKIAPI ns_Token* ns_TokenNew(ns_TokenType type, const char* value);
NIKIAPI void ns_TokenDelete(ns_Token* token);

NIKIAPI ns_TokenType ns_TokenGetType(ns_Token* pToken);
NIKIAPI void ns_TokenSetType(ns_Token* pToken, ns_TokenType type);

// Value Get/Set
NIKIAPI const char* ns_TokenGetValue(ns_Token* token);
NIKIAPI void ns_TokenSetValue(ns_Token* token, const char* value);

// Reference count
NIKIAPI size_t ns_TokenGetReferenceCount(ns_Token* token);

// Get reference at index
NIKIAPI uint64_t ns_TokenGetReferenceIndex(ns_Token* token, size_t i);
NIKIAPI const char* ns_TokenGetReferenceString(ns_Token* token, size_t i);

// Add a reference
NIKIAPI void ns_TokenAddReference(ns_Token* token, uint64_t index, const char* str);

// Clear references
NIKIAPI void ns_TokenClearReferences(ns_Token* token);

#ifdef __cplusplus
}
#endif