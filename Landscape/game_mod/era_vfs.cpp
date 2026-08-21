#include "era_vfs.hpp"
#include <Windows.h>


template <typename Ret, typename... Args>
inline static auto GetDllFunc(HINSTANCE hDll, const char funcName[]) {
	return reinterpret_cast<Ret(__stdcall*)(Args...)>(GetProcAddress(hDll, funcName));
}

namespace EraVfs {

bool IsModActive(std::string_view modName) {
	HINSTANCE hDll = GetModuleHandleA("vfs.dll");
	if (!hDll) { return false; }

	auto getModList = GetDllFunc<const void*>(hDll, "GetSerializedModListA");
	if (!getModList) { return false; }

	const void* modList = getModList();
	if (modList == nullptr) { return false; }

	bool result = false;
	int numOfMods = *reinterpret_cast<const int*>(modList);
	if (numOfMods > 0) {
		const char* modListPtr = reinterpret_cast<const char*>(modList) + sizeof(int);
		do {
			const size_t modNameLen = *reinterpret_cast<const size_t*>(modListPtr);
			modListPtr += sizeof(size_t);
			if (std::string_view(modListPtr, modNameLen) == modName) {
				result = true;
				break;
			}
			modListPtr += modNameLen;
		} while (--numOfMods > 0);
	}

	if (auto freeModList = GetDllFunc<void, const void*>(hDll, "MemFree")) {
		freeModList(modList);
	}

	return result;
}

} // namespace EraVfs
