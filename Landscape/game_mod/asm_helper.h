#pragma once


#define CODE_PATCH static __declspec(naked) __fastcall

#define PROLOG(arg1, arg2) __asm { \
    __asm push ebp \
    __asm mov ebp, esp \
    __asm sub esp, __LOCAL_SIZE \
    __asm mov arg1, ecx \
    __asm mov arg2, esi \
}

#define SET_EBX(retAddr) __asm { mov ebx, retAddr }

#define EPILOG(retValue, retAddr) __asm { \
    __asm mov eax, retValue \
    __asm mov esp, ebp \
    __asm pop ebp \
    __asm jmp retAddr \
}
