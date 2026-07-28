#pragma once


#ifdef _MSC_VER
    extern "C" void* _AddressOfReturnAddress(void);
    #define RETURN_ADDRESS *static_cast<uintptr_t*>(_AddressOfReturnAddress())
    #define PARAM32_STACK (static_cast<uint32_t*>(_AddressOfReturnAddress()) + 1)
#elif defined(__GNUG__)
    #define RETURN_ADDRESS static_cast<uintptr_t*>(__builtin_frame_address(0))[1]
    #define PARAM32_STACK (static_cast<uint32_t*>(__builtin_frame_address(0)) + 2)
#endif
