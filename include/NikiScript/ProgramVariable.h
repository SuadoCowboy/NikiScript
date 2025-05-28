#pragma once

#include <string>
#include <sstream>
#include <unordered_map>

#include "DLL.h"

namespace ns {
	struct ProgramVariable;
	struct Context;

	typedef std::string(*GetProgramVariableValue)(Context* pCtx, ProgramVariable* pVar);
	typedef void(*SetProgramVariableValue)(Context* pCtx, ProgramVariable* pVar, const std::string& str);
	
	struct Context;

	struct NS_API ProgramVariable {
		void* pValue = nullptr;
		std::string description;

		GetProgramVariableValue get = nullptr;
		SetProgramVariableValue set = nullptr;

		ProgramVariable();
		ProgramVariable(void* pValue, const std::string& description, const GetProgramVariableValue& get, const SetProgramVariableValue& set);
	};

	typedef std::unordered_map<std::string, ProgramVariable> ProgramVariables;

	NS_API std::string getString(Context*, ProgramVariable* pVar);
	NS_API void setString(Context*, ProgramVariable* pVar, const std::string& str);

	template<typename T>
	std::string getNumber(Context*, ProgramVariable* pVar) {
		return std::to_string(*static_cast<T*>(pVar->pValue));
	}

	/**
	 * @brief uses std::stoul so any number below that can be used
	 * @tparam T number type
	 * @param pCtx
	 * @param pVar 
	 * @param str
	 */
	template<typename T>
	void setUnsigned(Context*, ProgramVariable* pVar, const std::string& str) {
		try {
			*static_cast<T*>(pVar->pValue) = (T)std::stoul(str);
		} catch (...) {}
	}

	NS_API void setUnsignedLongLong(Context*, ProgramVariable* pVar, const std::string& str);

	NS_API void setFloat(Context*, ProgramVariable* pVar, const std::string& str);
	NS_API void setDouble(Context*, ProgramVariable* pVar, const std::string& str);
	NS_API void setLongDouble(Context*, ProgramVariable* pVar, const std::string& str);

	NS_API void setChar(Context*, ProgramVariable* pVar, const std::string& str);
	NS_API void setShort(Context*, ProgramVariable* pVar, const std::string& str);
	NS_API void setInteger(Context*, ProgramVariable* pVar, const std::string& str);
	NS_API void setLong(Context*, ProgramVariable* pVar, const std::string& str);
	NS_API void setLongLong(Context*, ProgramVariable* pVar, const std::string& str);
}