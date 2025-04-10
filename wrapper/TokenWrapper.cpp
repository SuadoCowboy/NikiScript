#include "TokenWrapper.h"

ns_Token* ns_TokenNew(ns_TokenType type, const char *value) {
	ns::Token *pToken = new ns::Token(type);
	pToken->value = value;

	return pToken;
}

void ns_TokenDelete(ns_Token *pToken) {
	delete pToken;
}

ns_TokenType ns_TokenGetType(ns_Token* pToken) {
	return pToken->type;
}

void ns_TokenSetType(ns_Token* pToken, ns_TokenType type) {
	pToken->type = type;
}

const char *ns_TokenGetValue(ns_Token *pToken) {
	return pToken->value.c_str();
}

void ns_TokenSetValue(ns_Token *pToken, const char *value) {
	pToken->value = value ? value : "";
}

size_t ns_TokenGetReferenceCount(ns_Token *pToken) {
	return pToken->references.size();
}

uint64_t ns_TokenGetReferenceIndex(ns_Token *pToken, size_t i) {
	auto& refs = pToken->references;
	return refs[i].first;
}

const char *ns_TokenGetReferenceString(ns_Token *pToken, size_t i) {
	auto& refs = pToken->references;
	return refs[i].second.c_str();
}

void ns_TokenAddReference(ns_Token *pToken, uint64_t index, const char *str) {
	pToken->references.emplace_back(index, str ? std::string(str) : "");
}

void ns_TokenClearReferences(ns_Token *pToken) {
	pToken->references.clear();
}