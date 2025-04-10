#pragma once

#include <stdint.h>
#include <string>
#include <unordered_map>

#include "DLLExport.h"

namespace ns {
	/**
	 * @brief should be used when a string is created in the function scope and not passed to the function as a parameter
	 * @param size not counting null-terminator as it's added automatically
	 */
	char* allocStringToCharString(const std::string& src);

	/**
	 * @tparam K map key
	 * @tparam V map value
	 * @return allocated array of K pointers(K*)
	 */
	template<typename K, typename V>
	K* allocUnorderedMapKeys(const std::unordered_map<K,V> &map) {
		K *keys = new K[map.size()];
		if (keys == nullptr)
			return nullptr;

		size_t i = 0;
		for (const auto &pair : map)
			keys[i++] = pair.first;

		keys[i] = nullptr; // null-terminator
		return keys;
	}

	/**
	 * @tparam V map value
	 * @return const char**
	 */
	template<typename V>
	const char** allocUnorderedMapStringKeysToCharArray(const std::unordered_map<std::string,V> &map) {
		const char **keys = new const char*[map.size()];
		if (keys == nullptr)
			return nullptr;

		uint64_t i = 0;
		for (auto it = map.begin(); it != map.end(); ++it)
			keys[i++] = it->first.c_str();

		return keys;
	}
}

extern "C" {
	NIKIAPI void ns_freeCharArray(char* string);

	/**
	 * @note does not free the strings inside of it, only the pointers to them
	 * @param strings
	 */
	NIKIAPI void ns_free2DCharArray(const char** strings);
}