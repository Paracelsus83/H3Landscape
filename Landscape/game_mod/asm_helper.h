#pragma once


#define ASM_CODE_PATCH static __declspec(naked) void __fastcall

#define CODE_PATCH static __declspec(noreturn) void __fastcall

#define GET_FROM_REG(outVar, reg) __asm { mov outVar, reg }

#define SET_EBX(value) __asm { mov ebx, value }
#define SET_ECX(value) __asm { mov ecx, value }

#ifdef _MSC_VER
#pragma warning(disable:4731)
#endif

#define PATCH_RETURN(retValue, retAddr) __asm { \
    __asm mov eax, retValue \
    __asm pop esi \
    __asm mov esp, ebp \
    __asm pop ebp \
    __asm jmp retAddr \
}
