#ifndef MIPS_INSTRUCTIONS_H
#define MIPS_INSTRUCTIONS_H

#include <vector>
#include <string>
#include <memory>
#include <cstdint>
#include "scanner.h"

// Forward declarations
class SymbolTableManager;

// Base instruction class
class MIPSInstruction {
public:
    virtual ~MIPSInstruction() = default;
    virtual uint32_t encode() const = 0;
    virtual std::string getMnemonic() const = 0;
    virtual std::string getDescription() const = 0;
    
    // Method to get the instruction as uint32_t for array storage
    uint32_t getInstructionValue() const { return encode(); }
};

// I-Type Instructions (Data Movement & Memory Access)
class ITypeInstruction : public MIPSInstruction {
protected:
    uint32_t opcode_;
    uint32_t rs_;
    uint32_t rt_;
    uint32_t immediate_;
    
public:
    ITypeInstruction(uint32_t opcode, uint32_t rs, uint32_t rt, uint32_t immediate)
        : opcode_(opcode), rs_(rs), rt_(rt), immediate_(immediate) {}
    
    uint32_t encode() const override {
        return (opcode_ << 26) | (rs_ << 21) | (rt_ << 16) | (immediate_ & 0xFFFF);
    }
};

// Load Word instruction
class LoadWordInstruction : public ITypeInstruction {
public:
    LoadWordInstruction(uint32_t rt, uint32_t immediate, uint32_t rs)
        : ITypeInstruction(0x23, rs, rt, immediate) {} // 0x23 = 35 decimal
    
    std::string getMnemonic() const override { return "lw"; }
    std::string getDescription() const override { return "Load Word"; }
};

// Store Word instruction
class StoreWordInstruction : public ITypeInstruction {
public:
    StoreWordInstruction(uint32_t rt, uint32_t immediate, uint32_t rs)
        : ITypeInstruction(0x2B, rs, rt, immediate) {} // 0x2B = 43 decimal
    
    std::string getMnemonic() const override { return "sw"; }
    std::string getDescription() const override { return "Store Word"; }
};

// Branch instructions
class BranchEqualInstruction : public ITypeInstruction {
public:
    BranchEqualInstruction(uint32_t rs, uint32_t rt, uint32_t immediate)
        : ITypeInstruction(0x04, rs, rt, immediate) {} // 0x04 = 4 decimal
    
    std::string getMnemonic() const override { return "beq"; }
    std::string getDescription() const override { return "Branch On Equal"; }
};

class BranchNotEqualInstruction : public ITypeInstruction {
public:
    BranchNotEqualInstruction(uint32_t rs, uint32_t rt, uint32_t immediate)
        : ITypeInstruction(0x05, rs, rt, immediate) {} // 0x05 = 5 decimal
    
    std::string getMnemonic() const override { return "bne"; }
    std::string getDescription() const override { return "Branch On Not Equal"; }
};

// Add Immediate instruction
class AddImmediateInstruction : public ITypeInstruction {
public:
    AddImmediateInstruction(uint32_t rt, uint32_t rs, uint32_t immediate)
        : ITypeInstruction(0x08, rs, rt, immediate) {} // 0x08 = 8 decimal
    
    std::string getMnemonic() const override { return "addi"; }
    std::string getDescription() const override { return "Add Immediate"; }
};

// Set Less Than Immediate instruction
class SetLessThanImmediateInstruction : public ITypeInstruction {
public:
    SetLessThanImmediateInstruction(uint32_t rt, uint32_t rs, uint32_t immediate)
        : ITypeInstruction(0x0A, rs, rt, immediate) {} // 0x0A = 10 decimal
    
    std::string getMnemonic() const override { return "slti"; }
    std::string getDescription() const override { return "Set Less Than Immediate"; }
};

// Set Less Than Immediate Unsigned instruction
class SetLessThanImmediateUnsignedInstruction : public ITypeInstruction {
public:
    SetLessThanImmediateUnsignedInstruction(uint32_t rt, uint32_t rs, uint32_t immediate)
        : ITypeInstruction(0x0B, rs, rt, immediate) {} // 0x0B = 11 decimal
    
    std::string getMnemonic() const override { return "sltiu"; }
    std::string getDescription() const override { return "Set Less Than Immediate Unsigned"; }
};

// R-Type Instructions (Arithmetic & Logic)
class RTypeInstruction : public MIPSInstruction {
protected:
    uint32_t rs_;
    uint32_t rt_;
    uint32_t rd_;
    uint32_t shamt_;
    uint32_t funct_;
    
public:
    RTypeInstruction(uint32_t rs, uint32_t rt, uint32_t rd, uint32_t shamt, uint32_t funct)
        : rs_(rs), rt_(rt), rd_(rd), shamt_(shamt), funct_(funct) {}
    
    uint32_t encode() const override {
        return (rs_ << 21) | (rt_ << 16) | (rd_ << 11) | (shamt_ << 6) | funct_;
    }
};

// Add instruction
class AddInstruction : public RTypeInstruction {
public:
    AddInstruction(uint32_t rd, uint32_t rs, uint32_t rt)
        : RTypeInstruction(rs, rt, rd, 0, 0x20) {} // funct = 0x20 = 32 decimal
    
    std::string getMnemonic() const override { return "add"; }
    std::string getDescription() const override { return "Add"; }
};

// Subtract instruction
class SubtractInstruction : public RTypeInstruction {
public:
    SubtractInstruction(uint32_t rd, uint32_t rs, uint32_t rt)
        : RTypeInstruction(rs, rt, rd, 0, 0x22) {} // funct = 0x22 = 34 decimal
    
    std::string getMnemonic() const override { return "sub"; }
    std::string getDescription() const override { return "Subtract"; }
};

// Set Less Than instruction
class SetLessThanInstruction : public RTypeInstruction {
public:
    SetLessThanInstruction(uint32_t rd, uint32_t rs, uint32_t rt)
        : RTypeInstruction(rs, rt, rd, 0, 0x2A) {} // funct = 0x2A = 42 decimal
    
    std::string getMnemonic() const override { return "slt"; }
    std::string getDescription() const override { return "Set Less Than"; }
};

// Set Less Than Unsigned instruction
class SetLessThanUnsignedInstruction : public RTypeInstruction {
public:
    SetLessThanUnsignedInstruction(uint32_t rd, uint32_t rs, uint32_t rt)
        : RTypeInstruction(rs, rt, rd, 0, 0x2B) {} // funct = 0x2B = 43 decimal
    
    std::string getMnemonic() const override { return "sltu"; }
    std::string getDescription() const override { return "Set Less Than Unsigned"; }
};

// Multiply instruction
class MultiplyInstruction : public RTypeInstruction {
public:
    MultiplyInstruction(uint32_t rs, uint32_t rt)
        : RTypeInstruction(rs, rt, 0, 0, 0x18) {} // funct = 0x18 = 24 decimal
    
    std::string getMnemonic() const override { return "mult"; }
    std::string getDescription() const override { return "Multiply"; }
};

// Multiply Unsigned instruction
class MultiplyUnsignedInstruction : public RTypeInstruction {
public:
    MultiplyUnsignedInstruction(uint32_t rs, uint32_t rt)
        : RTypeInstruction(rs, rt, 0, 0, 0x19) {} // funct = 0x19 = 25 decimal
    
    std::string getMnemonic() const override { return "multu"; }
    std::string getDescription() const override { return "Multiply Unsigned"; }
};

// Divide instruction
class DivideInstruction : public RTypeInstruction {
public:
    DivideInstruction(uint32_t rs, uint32_t rt)
        : RTypeInstruction(rs, rt, 0, 0, 0x1A) {} // funct = 0x1A = 26 decimal
    
    std::string getMnemonic() const override { return "div"; }
    std::string getDescription() const override { return "Divide"; }
};

// Divide Unsigned instruction
class DivideUnsignedInstruction : public RTypeInstruction {
public:
    DivideUnsignedInstruction(uint32_t rs, uint32_t rt)
        : RTypeInstruction(rs, rt, 0, 0, 0x1B) {} // funct = 0x1B = 27 decimal
    
    std::string getMnemonic() const override { return "divu"; }
    std::string getDescription() const override { return "Divide Unsigned"; }
};

// Move From High instruction
class MoveFromHighInstruction : public RTypeInstruction {
public:
    MoveFromHighInstruction(uint32_t rd)
        : RTypeInstruction(0, 0, rd, 0, 0x10) {} // funct = 0x10 = 16 decimal
    
    std::string getMnemonic() const override { return "mfhi"; }
    std::string getDescription() const override { return "Move From High"; }
};

// Move From Low instruction
class MoveFromLowInstruction : public RTypeInstruction {
public:
    MoveFromLowInstruction(uint32_t rd)
        : RTypeInstruction(0, 0, rd, 0, 0x12) {} // funct = 0x12 = 18 decimal
    
    std::string getMnemonic() const override { return "mflo"; }
    std::string getDescription() const override { return "Move From Low"; }
};

// Load Immediate And Skip instruction
class LoadImmediateAndSkipInstruction : public RTypeInstruction {
public:
    LoadImmediateAndSkipInstruction(uint32_t rd)
        : RTypeInstruction(0, 0, rd, 0, 0x14) {} // funct = 0x14 = 20 decimal
    
    std::string getMnemonic() const override { return "lis"; }
    std::string getDescription() const override { return "Load Immediate And Skip"; }
};

// Jump Register instruction
class JumpRegisterInstruction : public RTypeInstruction {
public:
    JumpRegisterInstruction(uint32_t rs)
        : RTypeInstruction(rs, 0, 0, 0, 0x08) {} // funct = 0x08 = 8 decimal
    
    std::string getMnemonic() const override { return "jr"; }
    std::string getDescription() const override { return "Jump Register"; }
};

// Jump And Link Register instruction
class JumpAndLinkRegisterInstruction : public RTypeInstruction {
public:
    JumpAndLinkRegisterInstruction(uint32_t rs)
        : RTypeInstruction(rs, 0, 0, 0, 0x09) {} // funct = 0x09 = 9 decimal
    
    std::string getMnemonic() const override { return "jalr"; }
    std::string getDescription() const override { return "Jump And Link Register"; }
};

// Word directive (not a real instruction, but handled similarly)
class WordDirective : public MIPSInstruction {
private:
    uint32_t value_;
    
public:
    WordDirective(uint32_t value) : value_(value) {}
    
    uint32_t encode() const override { return value_; }
    std::string getMnemonic() const override { return ".word"; }
    std::string getDescription() const override { return "Word Directive"; }
};

// Instruction factory
class InstructionFactory {
public:
    static std::unique_ptr<MIPSInstruction> createInstruction(
        const std::vector<Token>& line, 
        const SymbolTableManager& symbolManager);
};

#endif // MIPS_INSTRUCTIONS_H
