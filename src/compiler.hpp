#ifndef COMPILER_HPP
#define COMPILER_HPP

#include "memory.hpp"
#include <stdlib.h>
#include <string>
#include "register.hpp"

class compiler{
    private:
        
        //constructing objects
        memory mem;
        registers register_map;

        //creating needed variables for R type Instructions
        uint16_t funct;
        uint16_t shamt;
        uint16_t rd;
        uint16_t rt;
        uint16_t rs;
        
        uint16_t opcode;

        int32_t  op1_s;
        int32_t  op2_s;
        uint32_t op1;
        uint32_t op2;

    public:
        compiler(std::string binaryfile);
        void run();
        void execute();

        void execute_R(uint32_t instruction);
        void execute_J(uint32_t instruction);
        void execute_I(uint32_t instruction);

        //R type instrucrions
        void ADD();
        void ADDU();
        void AND();
        void DIV();
        void DIVU();
        void JALR();
        void JR();
        void MFHI();
        void MFLO();
        void MTHI();
        void MTLO();
        void MULT();
        void MULTU();
        void OR();
        void SLL();
        void SLLV();
        void SLT();
        void SLTU();
        void SRA();
        void SRAV();
        void SRL();
        void SRLV();
        void SUB();
        void SUBU();
        void XOR();

        void J(uint32_t instruction);
        void JAL(uint32_t instruction);

        void ADDI();
        void ADDIU();
        void ANDI();
        void BEQ();
        void BRANCHES();
        //void BLTZ();
        //void BLTZAL();
        //void BGEZ();
        //void BGEZAL();
        void BGTZ();
        void BLEZ();
        void BNE();
        void LB();
        void LBU();
        void LH();
        void LHU();
        void LUI();
        void LW();
        void LWL();
        void LWR();
        void ORI();
        void SLTI();
        void SLTIU();
        void SB();
        void SH();
        void SW();
        void XORI();


};

#endif