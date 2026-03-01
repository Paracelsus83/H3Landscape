#pragma once


#define ASM_CODE_PATCH static __declspec(naked) void __fastcall

#ifdef _MSC_VER
    extern "C" void* _AddressOfReturnAddress(void);
    #define RETURN_ADDRESS (*static_cast<uintptr_t*>(_AddressOfReturnAddress()))
#elif defined(__GNUG__)
    #define RETURN_ADDRESS (static_cast<uintptr_t*>(__builtin_frame_address(0))[1])
#endif
