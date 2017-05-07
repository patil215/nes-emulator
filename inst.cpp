#include "inst.h"
#include "memory.h"
#include <stdlib.h>
#include <stdio.h>
#include <iostream>

Inst::Inst(Type type_t, Admode admode_t) {
  type = type_t;
  admode = admode_t;
}

Inst parseInstruction(int pos, Memory & memory) {
  u_int8_t code = memory.read(pos);
  switch (code) {
    case 0x69:
      return Inst(Inst::ADC, Inst::IMMEDIATE);
    case 0x65:
      return Inst(Inst::ADC, Inst::ZERO_PAGE);
    case 0x75:
      return Inst(Inst::ADC, Inst::ZERO_PAGE_X);
    case 0x6D:
      return Inst(Inst::ADC, Inst::ABSOLUTE);
    case 0x7D:
      return Inst(Inst::ADC, Inst::ABSOLUTE_X);
    case 0x79:
      return Inst(Inst::ADC, Inst::ABSOLUTE_Y);
    case 0x61:
      return Inst(Inst::ADC, Inst::INDIRECT_X);
    case 0x71:
      return Inst(Inst::ADC, Inst::INDIRECT_Y);

    case 0x29:
      return Inst(Inst::AND, Inst::IMMEDIATE);
    case 0x25:
      return Inst(Inst::AND, Inst::ZERO_PAGE);
    case 0x35:
      return Inst(Inst::AND, Inst::ZERO_PAGE_X);
    case 0x2D:
      return Inst(Inst::AND, Inst::ABSOLUTE);
    case 0x3D:
      return Inst(Inst::AND, Inst::ABSOLUTE_X);
    case 0x39:
      return Inst(Inst::AND, Inst::ABSOLUTE_Y);
    case 0x21:
      return Inst(Inst::AND, Inst::INDIRECT_X);
    case 0x31:
      return Inst(Inst::AND, Inst::INDIRECT_Y);

    case 0x0A:
      return Inst(Inst::ASL, Inst::ACCUMULATOR);
    case 0x06:
      return Inst(Inst::ASL, Inst::ZERO_PAGE);
    case 0x16:
      return Inst(Inst::ASL, Inst::ZERO_PAGE_X);
    case 0x0E:
      return Inst(Inst::ASL, Inst::ABSOLUTE);
    case 0x1E:
      return Inst(Inst::ASL, Inst::ABSOLUTE_X);

    case 0x24:
      return Inst(Inst::BIT, Inst::ZERO_PAGE);
    case 0x2C:
      return Inst(Inst::BIT, Inst::ABSOLUTE);

    case 0x10:
      return Inst(Inst::BPL, Inst::RELATIVE);
    case 0x30:
      return Inst(Inst::BMI, Inst::RELATIVE);
    case 0x50:
      return Inst(Inst::BVC, Inst::RELATIVE);
    case 0x70:
      return Inst(Inst::BVS, Inst::RELATIVE);
    case 0x90:
      return Inst(Inst::BCC, Inst::RELATIVE);
    case 0xB0:
      return Inst(Inst::BCS, Inst::RELATIVE);
    case 0xD0:
      return Inst(Inst::BNE, Inst::RELATIVE);
    case 0xF0:
      return Inst(Inst::BEQ, Inst::RELATIVE);

    case 0x00:
      return Inst(Inst::BRK, Inst::NONE);

    case 0xC9:
      return Inst(Inst::CMP, Inst::IMMEDIATE);
    case 0xC5:
      return Inst(Inst::CMP, Inst::ZERO_PAGE);
    case 0xD5:
      return Inst(Inst::CMP, Inst::ZERO_PAGE_X);
    case 0xCD:
      return Inst(Inst::CMP, Inst::ABSOLUTE);
    case 0xDD:
      return Inst(Inst::CMP, Inst::ABSOLUTE_X);
    case 0xD9:
      return Inst(Inst::CMP, Inst::ABSOLUTE_Y);
    case 0xC1:
      return Inst(Inst::CMP, Inst::INDIRECT_X);
    case 0xD1:
      return Inst(Inst::CMP, Inst::INDIRECT_Y);

    case 0xE0:
      return Inst(Inst::CPX, Inst::IMMEDIATE);
    case 0xE4:
      return Inst(Inst::CPX, Inst::ZERO_PAGE);
    case 0xEC:
      return Inst(Inst::CPX, Inst::ABSOLUTE);

    case 0xC0:
      return Inst(Inst::CPY, Inst::IMMEDIATE);
    case 0xC4:
      return Inst(Inst::CPY, Inst::ZERO_PAGE);
    case 0xCC:
      return Inst(Inst::CPY, Inst::ABSOLUTE);


    case 0xC6:
      return Inst(Inst::DEC, Inst::ZERO_PAGE);
    case 0xD6:
      return Inst(Inst::DEC, Inst::ZERO_PAGE_X);
    case 0xCE:
      return Inst(Inst::DEC, Inst::ABSOLUTE);
    case 0xDE:
      return Inst(Inst::DEC, Inst::ABSOLUTE_X);

    case 0x49:
      return Inst(Inst::EOR, Inst::IMMEDIATE);
    case 0x45:
      return Inst(Inst::EOR, Inst::ZERO_PAGE);
    case 0x55:
      return Inst(Inst::EOR, Inst::ZERO_PAGE_X);
    case 0x4D:
      return Inst(Inst::EOR, Inst::ABSOLUTE);
    case 0x5D:
      return Inst(Inst::EOR, Inst::ABSOLUTE_X);
    case 0x59:
      return Inst(Inst::EOR, Inst::ABSOLUTE_Y);
    case 0x41:
      return Inst(Inst::EOR, Inst::INDIRECT_X);
    case 0x51:
      return Inst(Inst::EOR, Inst::INDIRECT_Y);

    case 0x18:
      return Inst(Inst::CLC, Inst::NONE);
    case 0x38:
      return Inst(Inst::SEC, Inst::NONE);
    case 0x58:
      return Inst(Inst::CLI, Inst::NONE);
    case 0x78:
      return Inst(Inst::SEI, Inst::NONE);
    case 0xB8:
      return Inst(Inst::CLV, Inst::NONE);
    case 0xD8:
      return Inst(Inst::CLD, Inst::NONE);
    case 0xF8:
      return Inst(Inst::SED, Inst::NONE);


    case 0xE6:
      return Inst(Inst::INC, Inst::ZERO_PAGE);
    case 0xF6:
      return Inst(Inst::INC, Inst::ZERO_PAGE_X);
    case 0xEE:
      return Inst(Inst::INC, Inst::ABSOLUTE);
    case 0xFE:
      return Inst(Inst::INC, Inst::ABSOLUTE_X);

    case 0x4C:
      return Inst(Inst::JMP, Inst::ABSOLUTE);
    case 0x6C:
      return Inst(Inst::JMP, Inst::INDIRECT);

    case 0x20:
      return Inst(Inst::JSR, Inst::ABSOLUTE);

    case 0xA9:
      return Inst(Inst::LDA, Inst::IMMEDIATE);
    case 0xA5:
      return Inst(Inst::LDA, Inst::ZERO_PAGE);
    case 0xB5:
      return Inst(Inst::LDA, Inst::ZERO_PAGE_X);
    case 0xAD:
      return Inst(Inst::LDA, Inst::ABSOLUTE);
    case 0xBD:
      return Inst(Inst::LDA, Inst::ABSOLUTE_X);
    case 0xB9:
      return Inst(Inst::LDA, Inst::ABSOLUTE_Y);
    case 0xA1:
      return Inst(Inst::LDA, Inst::INDIRECT_X);
    case 0xB1:
      return Inst(Inst::LDA, Inst::INDIRECT_Y);

    case 0xA2:
      // TODO: does this load the value or memory[value]?
      return Inst(Inst::LDX, Inst::IMMEDIATE);
    case 0xA6:
      return Inst(Inst::LDX, Inst::ZERO_PAGE);
    case 0xB6:
      return Inst(Inst::LDX, Inst::ZERO_PAGE_Y);
    case 0xAE:
      return Inst(Inst::LDX, Inst::ABSOLUTE);
    case 0xBE:
      return Inst(Inst::LDX, Inst::ABSOLUTE_Y);

    case 0xA0:
      return Inst(Inst::LDY, Inst::IMMEDIATE);
    case 0xA4:
      return Inst(Inst::LDY, Inst::ZERO_PAGE);
    case 0xB4:
      return Inst(Inst::LDY, Inst::ZERO_PAGE_X);
    case 0xAC:
      return Inst(Inst::LDY, Inst::ABSOLUTE);
    case 0xBC:
      return Inst(Inst::LDY, Inst::ABSOLUTE_X);

    case 0x4A:
      return Inst(Inst::LSR, Inst::ACCUMULATOR);
    case 0x46:
      return Inst(Inst::LSR, Inst::ZERO_PAGE);
    case 0x56:
      return Inst(Inst::LSR, Inst::ZERO_PAGE_X);
    case 0x4E:
      return Inst(Inst::LSR, Inst::ABSOLUTE);
    case 0x5E:
      return Inst(Inst::LSR, Inst::ABSOLUTE_X);

    case 0xEA:
      return Inst(Inst::NOP, Inst::NONE);

    case 0x09:
      return Inst(Inst::ORA, Inst::IMMEDIATE);
    case 0x05:
      return Inst(Inst::ORA, Inst::ZERO_PAGE);
    case 0x15:
      return Inst(Inst::ORA, Inst::ZERO_PAGE_X);
    case 0x0D:
      return Inst(Inst::ORA, Inst::ABSOLUTE);
    case 0x1D:
      return Inst(Inst::ORA, Inst::ABSOLUTE_X);
    case 0x19:
      return Inst(Inst::ORA, Inst::ABSOLUTE_Y);
    case 0x01:
      return Inst(Inst::ORA, Inst::INDIRECT_X);
    case 0x11:
      return Inst(Inst::ORA, Inst::INDIRECT_Y);

    case 0xAA:
      return Inst(Inst::TAX, Inst::NONE);
    case 0x8A:
      return Inst(Inst::TXA, Inst::NONE);
    case 0xCA:
      return Inst(Inst::DEX, Inst::NONE);
    case 0xE8:
      return Inst(Inst::INX, Inst::NONE);
    case 0xA8:
      return Inst(Inst::TAY, Inst::NONE);
    case 0x98:
      return Inst(Inst::TYA, Inst::NONE);
    case 0x88:
      return Inst(Inst::DEY, Inst::NONE);
    case 0xC8:
      return Inst(Inst::INY, Inst::NONE);

    case 0x2A:
      return Inst(Inst::ROL, Inst::ACCUMULATOR);
    case 0x26:
      return Inst(Inst::ROL, Inst::ZERO_PAGE);
    case 0x36:
      return Inst(Inst::ROL, Inst::ZERO_PAGE_X);
    case 0x2E:
      return Inst(Inst::ROL, Inst::ABSOLUTE);
    case 0x3E:
      return Inst(Inst::ROL, Inst::ABSOLUTE_X);

    case 0x6A:
      return Inst(Inst::ROR, Inst::ACCUMULATOR);
    case 0x66:
      return Inst(Inst::ROR, Inst::ZERO_PAGE);
    case 0x76:
      return Inst(Inst::ROR, Inst::ZERO_PAGE_X);
    case 0x6E:
      return Inst(Inst::ROR, Inst::ABSOLUTE);
    case 0x7E:
      return Inst(Inst::ROR, Inst::ABSOLUTE_X);

    case 0x40:
      return Inst(Inst::RTI, Inst::NONE);

    case 0x60:
      return Inst(Inst::RTS, Inst::NONE);
     
    case 0xE9:
      return Inst(Inst::SBC, Inst::IMMEDIATE);
    case 0xE5:
      return Inst(Inst::SBC, Inst::ZERO_PAGE);
    case 0xF5:
      return Inst(Inst::SBC, Inst::ZERO_PAGE_X);
    case 0xED:
      return Inst(Inst::SBC, Inst::ABSOLUTE);
    case 0xFD:
      return Inst(Inst::SBC, Inst::ABSOLUTE_X);
    case 0xF9:
      return Inst(Inst::SBC, Inst::ABSOLUTE_Y);
    case 0xE1:
      return Inst(Inst::SBC, Inst::INDIRECT_X);
    case 0xF1:
      return Inst(Inst::SBC, Inst::INDIRECT_Y);

    case 0x85:
      return Inst(Inst::STA, Inst::ZERO_PAGE);
    case 0x95:
      return Inst(Inst::STA, Inst::ZERO_PAGE_X);
    case 0x8D:
      return Inst(Inst::STA, Inst::ABSOLUTE);
    case 0x9D:
      return Inst(Inst::STA, Inst::ABSOLUTE_X);
    case 0x99:
      return Inst(Inst::STA, Inst::ABSOLUTE_Y);
    case 0x81:
      return Inst(Inst::STA, Inst::INDIRECT_X);
    case 0x91:
      return Inst(Inst::STA, Inst::INDIRECT_Y);

    case 0x9A:
      return Inst(Inst::TXS, Inst::NONE);
    case 0xBA:
      return Inst(Inst::TSX, Inst::NONE);
    case 0x48:
      return Inst(Inst::PHA, Inst::NONE);
    case 0x68:
      return Inst(Inst::PLA, Inst::NONE);
    case 0x08:
      return Inst(Inst::PHP, Inst::NONE);
    case 0x28:
      return Inst(Inst::PLP, Inst::NONE);

    case 0x86:
      return Inst(Inst::STX, Inst::ZERO_PAGE);
    case 0x96:
      return Inst(Inst::STX, Inst::ZERO_PAGE_Y);
    case 0x8E:
      return Inst(Inst::STX, Inst::ABSOLUTE);

    case 0x84:
      return Inst(Inst::STX, Inst::ZERO_PAGE);
    case 0x94:
      return Inst(Inst::STX, Inst::ZERO_PAGE_X);
    case 0x8C:
      return Inst(Inst::STX, Inst::ABSOLUTE);

    default:
      return Inst(Inst::NOP, Inst::NONE);
  }
}

u_int16_t readDoubleByte(u_int16_t pos, Memory & memory) {
  u_int16_t res = memory.read(pos + 2);
  res = res << 8;
  res += memory.read(pos + 1);
  return res;
}

void loadVal(Inst & inst, Memory & memory, u_int8_t X, u_int8_t Y, u_int8_t A, int pos) {
  // TODO the signed unsigned addition casting is all wrong!!!
  Inst::Admode admode = inst.admode;
  switch (admode) {
    case Inst::ABSOLUTE:
      inst.adr = readDoubleByte(pos, memory);
      inst.val = memory.read(inst.adr);
      break;
    case Inst::ACCUMULATOR:
      inst.val = A;
      break;
    case Inst::IMMEDIATE:
      inst.val = memory.read(pos + 1);
      break;
    case Inst::ABSOLUTE_X:
      inst.adr = readDoubleByte(pos, memory);
      inst.adr += X;
      inst.val = memory.read(inst.adr);
      break;
    case Inst::ABSOLUTE_Y:
      inst.adr = readDoubleByte(pos, memory);
      inst.adr += Y;
      inst.val = memory.read(inst.adr);
      break;
    case Inst::ZERO_PAGE_X:
      inst.adr = memory.read(pos + 1) + X;
      inst.val = memory.read(inst.adr);
      break;
    case Inst::ZERO_PAGE_Y:
      inst.adr = memory.read(pos + 1) + Y;
      inst.val = memory.read(inst.adr);
      break;
    case Inst::INDIRECT_X:
      {
        u_int8_t op = memory.read(pos + 1);
        op += X;
        inst.adr = memory.read(op + 1);
        inst.adr = inst.adr << 8;
        inst.adr += memory.read(op);
        inst.val = memory.read(inst.adr);
        break;
      }
    case Inst::INDIRECT:
      {
        u_int16_t address = memory.read(pos + 2);
        address = address << 8;
        address += memory.read(pos + 1);
        inst.adr = memory.read(address + 1);
        inst.adr = inst.adr << 8;
        inst.adr += memory.read(address);
        inst.val = memory.read(inst.adr);
        break;
      }
    case Inst::INDIRECT_Y:
      {
        u_int8_t op = memory.read(pos + 1);
        inst.adr = memory.read(op + 1);
        inst.adr = inst.adr << 8;
        inst.adr += memory.read(op);
        inst.adr += Y;
        inst.val = memory.read(inst.adr);
        break;
      }
    case Inst::RELATIVE:
      inst.val = memory.read(pos + 1);
      break;
    case Inst::ZERO_PAGE:
      inst.adr = memory.read(pos + 1);
      inst.val = memory.read(inst.adr);
      break;
    default:
      break;
  }
}