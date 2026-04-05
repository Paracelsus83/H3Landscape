#pragma once

#include <nh3api/core/nh3api_std/patcher_x86.hpp>


namespace Asm {

#pragma pack(push, 1)

    template <class... Instructions>
    class Sequence;

    template <class ...Instructions>
    Sequence(Instructions...) -> Sequence<Instructions...>;


    template <class Instr>
    class Sequence<Instr> {
        Instr _instr;
    public:
        constexpr explicit Sequence(Instr instr) : _instr(instr) {}

        constexpr static size_t Size() { return sizeof(Instr); }

        void UpdateAddr(uintptr_t addr) {
            _instr.UpdateAddr(addr);
        }

        class Patch* Apply(class PatcherInstance& p, uintptr_t address) {
            UpdateAddr(address);
            return p.Write(address, this, Size(), EPatcherInstanceWriteMode::CODE_);
        }
    };


    template <class FirstInstr, class... RestInstr>
    class Sequence<FirstInstr, RestInstr...> {
        FirstInstr _instr;
        Sequence<RestInstr...> _rest;

    public:
        constexpr Sequence(FirstInstr first, RestInstr... rest) : _instr(first), _rest(rest...) {}

        constexpr size_t Size() const { return sizeof(*this); }

        void UpdateAddr(uintptr_t addr) {
            _instr.UpdateAddr(addr);
            _rest.UpdateAddr(addr + sizeof(_instr));
        }

        class Patch* Apply(class PatcherInstance& p, uintptr_t address) {
            UpdateAddr(address);
            return p.Write(address, this, Size(), EPatcherInstanceWriteMode::CODE_);
        }
    };


    enum Reg : uint8_t {
        EAX = 0,
        ECX = 1,
        EDX = 2,
        EBX = 3,
        ESP = 4,
        EBP = 5,
        ESI = 6,
        EDI = 7
    };


    struct AbsoluteInstr {
        static constexpr void UpdateAddr(uintptr_t) {};
    };


    class RegRegInstr : public AbsoluteInstr {
        [[maybe_unused]] const uint8_t opcode;
        [[maybe_unused]] const uint8_t regByte;
    protected:
        constexpr RegRegInstr(uint8_t opcode, Reg dstReg, Reg srcReg) :
            opcode(opcode), regByte(dstReg | 0xC0u | (srcReg << 3u)) {
        }
    };


    class RegPtr32Instr : public AbsoluteInstr {
        [[maybe_unused]] const uint8_t opcode;
        [[maybe_unused]] const uint8_t regByte;
        [[maybe_unused]] const int32_t offset;
    public:
        constexpr explicit RegPtr32Instr(uint8_t op, Reg dstReg, Reg baseReg, int32_t ofs) :
            opcode(op), regByte(0x80 | (dstReg << 3u) | baseReg), offset(ofs) {
        }
    };

	/// nop
    class NoOperation : public AbsoluteInstr {
        [[maybe_unused]] const uint8_t opcode = 0x90u;
    };

    /// mov dstReg, srcReg
    struct SetReg : public RegRegInstr {
        constexpr explicit SetReg(Reg dstReg, Reg srcReg) : RegRegInstr(0x89u, dstReg, srcReg) {}
    };

    /// mov dstReg, value (32bit)
    class SetRegConst : public AbsoluteInstr {
        [[maybe_unused]] const uint8_t opcode;
        [[maybe_unused]] const uint32_t value;
    public:
        constexpr explicit SetRegConst(Reg dstReg, uint32_t v) : opcode(0xB8u | dstReg), value(v) {}
    };

    /// mov dstReg, dword ptr [baseReg + offset]
    struct SetRegPtr : public RegPtr32Instr {
        constexpr explicit SetRegPtr(Reg dstReg, Reg baseReg, int32_t ofs) : RegPtr32Instr(0x8Bu, dstReg, baseReg, ofs) {}
    };

    /// xor dstReg, srcReg
    struct XorReg : public RegRegInstr {
        constexpr explicit XorReg(Reg dstReg, Reg srcReg) : RegRegInstr(0x31u, dstReg, srcReg) {}
    };

    /// xor dstReg, dstReg
    struct ClearReg : public XorReg {
        constexpr explicit ClearReg(Reg dstReg) : XorReg(dstReg, dstReg) {}
    };

    /// test reg, dword ptr [baseReg + offset]
    struct TestRegPtr : public RegPtr32Instr {
        constexpr explicit TestRegPtr(Reg dstReg, Reg baseReg, int32_t ofs) : RegPtr32Instr(0x85u, dstReg, baseReg, ofs) {}
    };

    /// push srcReg
    class PushReg : public AbsoluteInstr {
        [[maybe_unused]] const uint8_t opcode;
    public:
        constexpr explicit PushReg(Reg srcReg) : opcode(0x50u | srcReg) {}
    };

    /// pop dstReg
    class PopReg : public AbsoluteInstr {
        [[maybe_unused]] const uint8_t opcode;
    public:
        constexpr explicit PopReg(Reg dstReg) : opcode(0x58u | dstReg) {}
    };

    /// push value (32bit)
    class PushConst32 : public AbsoluteInstr {
        [[maybe_unused]] const uint8_t opcode = 0x68u;
        [[maybe_unused]] const uint32_t value;
    public:
        constexpr explicit PushConst32(uint32_t v) : value(v) {}

        template <typename T>
        constexpr explicit PushConst32(T* addr) : value(addr) {}
    };


    class OperLocalVar : public AbsoluteInstr {
        [[maybe_unused]] const uint8_t opcode;
        [[maybe_unused]] const uint8_t regByte;
        [[maybe_unused]] const int8_t offset;
    protected:
        constexpr explicit OperLocalVar(uint8_t op, int8_t ofs, Reg srcReg) : opcode(op), regByte(0x45u | (srcReg << 3u)), offset(ofs) {}
    };


    /// mov dword ptr [ebp + offset8], srcReg
    struct SetLocalVar : public OperLocalVar {
        constexpr explicit SetLocalVar(int8_t ofs, Reg srcReg) : OperLocalVar(0x89u, ofs, srcReg) {}
    };


    /// cmp dword ptr [ebp + offset8], srcReg
    struct CmpLocalVar : public OperLocalVar {
        constexpr explicit CmpLocalVar(int8_t ofs, Reg reg) : OperLocalVar(0x3Bu, ofs, reg) {}
    };


    template <typename T>
    class RelativeJump {
        [[maybe_unused]] const uint8_t opcode;
        [[maybe_unused]] T relAddr;
    protected:
        constexpr explicit RelativeJump(uint8_t op, uintptr_t targetAddr) : opcode(op), relAddr(T(targetAddr - sizeof(*this))) {}
    public:
        constexpr void UpdateAddr(uintptr_t addr) {
            relAddr -= T(addr);
        }
    };


    class Jump : public RelativeJump<int32_t> {
    public:
        constexpr explicit Jump(uintptr_t targetAddr) : RelativeJump<int32_t>(0xE9u, targetAddr) {}

        template <typename T>
        constexpr explicit Jump(T* targetAddr) : Jump(uintptr_t(targetAddr)) {}
    };


    class Call : public RelativeJump<int32_t> {
    public:
        constexpr explicit Call(uintptr_t targetAddr) : RelativeJump<int32_t>(0xE8u, targetAddr) {}

        template <typename T>
        constexpr explicit Call(T* targetAddr) : Call(uintptr_t(targetAddr)) {}
    };


    class JumpIfZero : public RelativeJump<int8_t> {
    public:
        constexpr explicit JumpIfZero(uintptr_t targetAddr) : RelativeJump<int8_t>(0x74u, targetAddr) {}
    };


    class JumpIfNotZero : public RelativeJump<int8_t> {
    public:
        constexpr explicit JumpIfNotZero(uintptr_t targetAddr) : RelativeJump<int8_t>(0x75u, targetAddr) {}
    };


    class JumpIfEcxZero : public RelativeJump<int8_t> {
    public:
        constexpr explicit JumpIfEcxZero(uintptr_t targetAddr) : RelativeJump<int8_t>(0xE3u, targetAddr) {}
    };

#pragma pack(pop)


inline void WritePseudoFastCall(PatcherInstance& p, uintptr_t insAddr, uintptr_t funcAddr, Reg regArg) {
    Sequence{
        SetReg(ECX, regArg),
        Call(funcAddr)
    }
    .Apply(p, insAddr);
}


inline void WritePseudoFastCall(PatcherInstance& p, uintptr_t insAddr, uintptr_t funcAddr, uintptr_t retAdress) {
    Sequence{
        PushConst32(retAdress),
        Jump(funcAddr)
    }
    .Apply(p, insAddr);
}


inline void WritePseudoFastCall(PatcherInstance& p, uintptr_t insAddr, uintptr_t funcAddr, Reg regArg, uintptr_t retAdress) {
    Sequence{
        SetReg(ECX, regArg),
        PushConst32(retAdress),
        Jump(funcAddr)
    }
    .Apply(p, insAddr);
}


template<typename F, typename ...Args>
inline void WritePseudoFastCall(PatcherInstance& p, uintptr_t callAddr, F* funcAddr, Args... args) {
    WritePseudoFastCall(p, callAddr, uintptr_t(funcAddr), args...);
}


} // namespace Asm
