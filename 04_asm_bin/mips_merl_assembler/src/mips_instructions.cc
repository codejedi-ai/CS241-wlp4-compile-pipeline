#include "mips_instructions.h"
#include "symbol_table.h"
#include <stdexcept>

std::unique_ptr<MIPSInstruction> InstructionFactory::createInstruction(
    const std::vector<Token>& line, 
    const SymbolTableManager& symbolManager) {
    
    if (line.empty()) {
        throw std::runtime_error("Empty instruction line");
    }
    
    int ind = 0;
    
    // Skip labels
    while (ind < static_cast<int>(line.size()) && line[ind].getKind() == Token::LABEL) {
        ind++;
    }
    if (ind >= static_cast<int>(line.size())) {
        throw std::runtime_error("No instruction found after labels");
    }
    
    // Handle .word directive
    if (line[ind].getKind() == Token::WORD) {
        ind++;
        if (ind < static_cast<int>(line.size())) {
            if (line[ind].getKind() == Token::INT || line[ind].getKind() == Token::HEXINT) {
                uint32_t value = line[ind].toNumber();
                return std::make_unique<WordDirective>(value);
            } else if (line[ind].getKind() == Token::ID) {
                // Symbol reference - use placeholder value 0
                return std::make_unique<WordDirective>(0);
            }
        }
        throw std::runtime_error("Invalid .word directive");
    }
    
    // Get instruction mnemonic
    std::string mnemonic = line[ind].getLexeme();
    
    // Helper function to get register number
    auto getReg = [&](int index) -> uint32_t {
        if (index >= static_cast<int>(line.size()) || line[index].getKind() != Token::REG) {
            throw std::runtime_error("Expected register token");
        }
        return line[index].toNumber();
    };
    
    // Helper function to get immediate value
    auto getImm = [&](int index) -> uint32_t {
        if (index >= static_cast<int>(line.size()) || 
            (line[index].getKind() != Token::INT && line[index].getKind() != Token::HEXINT)) {
            throw std::runtime_error("Expected immediate value token");
        }
        return line[index].toNumber();
    };
    
    // Create instruction based on mnemonic
    if (mnemonic == "lw") {
        if (line.size() < 6) throw std::runtime_error("Invalid lw instruction");
        uint32_t rt = getReg(ind + 1);
        uint32_t immediate = getImm(ind + 3);
        uint32_t rs = getReg(ind + 5);
        return std::make_unique<LoadWordInstruction>(rt, immediate, rs);
    }
    else if (mnemonic == "sw") {
        if (line.size() < 6) throw std::runtime_error("Invalid sw instruction");
        uint32_t rt = getReg(ind + 1);
        uint32_t immediate = getImm(ind + 3);
        uint32_t rs = getReg(ind + 5);
        return std::make_unique<StoreWordInstruction>(rt, immediate, rs);
    }
    else if (mnemonic == "beq") {
        if (line.size() < 6) throw std::runtime_error("Invalid beq instruction");
        uint32_t rs = getReg(ind + 1);
        uint32_t rt = getReg(ind + 3);
        uint32_t immediate = getImm(ind + 5);
        return std::make_unique<BranchEqualInstruction>(rs, rt, immediate);
    }
    else if (mnemonic == "bne") {
        if (line.size() < 6) throw std::runtime_error("Invalid bne instruction");
        uint32_t rs = getReg(ind + 1);
        uint32_t rt = getReg(ind + 3);
        uint32_t immediate = getImm(ind + 5);
        return std::make_unique<BranchNotEqualInstruction>(rs, rt, immediate);
    }
    else if (mnemonic == "addi") {
        if (line.size() < 6) throw std::runtime_error("Invalid addi instruction");
        uint32_t rt = getReg(ind + 1);
        uint32_t rs = getReg(ind + 3);
        uint32_t immediate = getImm(ind + 5);
        return std::make_unique<AddImmediateInstruction>(rt, rs, immediate);
    }
    else if (mnemonic == "slti") {
        if (line.size() < 6) throw std::runtime_error("Invalid slti instruction");
        uint32_t rt = getReg(ind + 1);
        uint32_t rs = getReg(ind + 3);
        uint32_t immediate = getImm(ind + 5);
        return std::make_unique<SetLessThanImmediateInstruction>(rt, rs, immediate);
    }
    else if (mnemonic == "sltiu") {
        if (line.size() < 6) throw std::runtime_error("Invalid sltiu instruction");
        uint32_t rt = getReg(ind + 1);
        uint32_t rs = getReg(ind + 3);
        uint32_t immediate = getImm(ind + 5);
        return std::make_unique<SetLessThanImmediateUnsignedInstruction>(rt, rs, immediate);
    }
    else if (mnemonic == "add") {
        if (line.size() < 6) throw std::runtime_error("Invalid add instruction");
        uint32_t rd = getReg(ind + 1);
        uint32_t rs = getReg(ind + 3);
        uint32_t rt = getReg(ind + 5);
        return std::make_unique<AddInstruction>(rd, rs, rt);
    }
    else if (mnemonic == "sub") {
        if (line.size() < 6) throw std::runtime_error("Invalid sub instruction");
        uint32_t rd = getReg(ind + 1);
        uint32_t rs = getReg(ind + 3);
        uint32_t rt = getReg(ind + 5);
        return std::make_unique<SubtractInstruction>(rd, rs, rt);
    }
    else if (mnemonic == "slt") {
        if (line.size() < 6) throw std::runtime_error("Invalid slt instruction");
        uint32_t rd = getReg(ind + 1);
        uint32_t rs = getReg(ind + 3);
        uint32_t rt = getReg(ind + 5);
        return std::make_unique<SetLessThanInstruction>(rd, rs, rt);
    }
    else if (mnemonic == "sltu") {
        if (line.size() < 6) throw std::runtime_error("Invalid sltu instruction");
        uint32_t rd = getReg(ind + 1);
        uint32_t rs = getReg(ind + 3);
        uint32_t rt = getReg(ind + 5);
        return std::make_unique<SetLessThanUnsignedInstruction>(rd, rs, rt);
    }
    else if (mnemonic == "mult") {
        if (line.size() < 4) throw std::runtime_error("Invalid mult instruction");
        uint32_t rs = getReg(ind + 1);
        uint32_t rt = getReg(ind + 3);
        return std::make_unique<MultiplyInstruction>(rs, rt);
    }
    else if (mnemonic == "multu") {
        if (line.size() < 4) throw std::runtime_error("Invalid multu instruction");
        uint32_t rs = getReg(ind + 1);
        uint32_t rt = getReg(ind + 3);
        return std::make_unique<MultiplyUnsignedInstruction>(rs, rt);
    }
    else if (mnemonic == "div") {
        if (line.size() < 4) throw std::runtime_error("Invalid div instruction");
        uint32_t rs = getReg(ind + 1);
        uint32_t rt = getReg(ind + 3);
        return std::make_unique<DivideInstruction>(rs, rt);
    }
    else if (mnemonic == "divu") {
        if (line.size() < 4) throw std::runtime_error("Invalid divu instruction");
        uint32_t rs = getReg(ind + 1);
        uint32_t rt = getReg(ind + 3);
        return std::make_unique<DivideUnsignedInstruction>(rs, rt);
    }
    else if (mnemonic == "mfhi") {
        if (line.size() < 2) throw std::runtime_error("Invalid mfhi instruction");
        uint32_t rd = getReg(ind + 1);
        return std::make_unique<MoveFromHighInstruction>(rd);
    }
    else if (mnemonic == "mflo") {
        if (line.size() < 2) throw std::runtime_error("Invalid mflo instruction");
        uint32_t rd = getReg(ind + 1);
        return std::make_unique<MoveFromLowInstruction>(rd);
    }
    else if (mnemonic == "lis") {
        if (line.size() < 2) throw std::runtime_error("Invalid lis instruction");
        uint32_t rd = getReg(ind + 1);
        return std::make_unique<LoadImmediateAndSkipInstruction>(rd);
    }
    else if (mnemonic == "jr") {
        if (line.size() < 2) throw std::runtime_error("Invalid jr instruction");
        uint32_t rs = getReg(ind + 1);
        return std::make_unique<JumpRegisterInstruction>(rs);
    }
    else if (mnemonic == "jalr") {
        if (line.size() < 2) throw std::runtime_error("Invalid jalr instruction");
        uint32_t rs = getReg(ind + 1);
        return std::make_unique<JumpAndLinkRegisterInstruction>(rs);
    }
    else {
        throw std::runtime_error("Unknown instruction: " + mnemonic);
    }
}
