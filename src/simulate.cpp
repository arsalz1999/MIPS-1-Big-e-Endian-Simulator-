#include "simulate.hpp"
#include <iostream>

simulate::simulate(std::string binaryfile) : mem(binaryfile){

}

//all the functions go here

void simulate::execute(){
    if((register_map.program_counter >= 0x10000000) && (register_map.program_counter < 0x11000000)){
    uint32_t instruction_current = mem.read_instruction(register_map.program_counter);
    opcode = (instruction_current & 0xFC000000) >> 26;
    if((opcode & 0b111111) == 0){
      execute_R(instruction_current);
    }
    else if( (opcode & 0b111111) == 3 || (opcode & 0b111111) == 2 ){
      execute_J(instruction_current);
    }
    else{
      execute_I(instruction_current);
    }
    register_map.program_counter += 4; //increment PC
  }
  else{
    std::exit(-11);
  }
}

void simulate::run(){
  while((register_map.program_counter != 0) && ((register_map.program_counter >= 0x10000000) && (register_map.program_counter <= 0x11000000)))
  {
    simulate::execute();
  }
  if(register_map.program_counter==0)
  {
    uint8_t exitCode = (register_map.read_register(2)&0x000000FF);
    std::cerr<<"exitcode is = "<< static_cast<int16_t>(exitCode) <<std::endl;
    std::exit(exitCode);
  }
  else std::exit(-11);
}

void simulate::execute_R(uint32_t instruction){

    funct = instruction & 0b111111;
    shamt = (instruction>>6) & 0b11111;
    rd = (instruction>>11) & 0b11111;
    rt = (instruction>>16) & 0b11111;
    rs = (instruction>>21) & 0b11111;

    op1 = register_map.read_register(rs);
    op2 = register_map.read_register(rt);
    op1_s = register_map.read_register(rs);
    op2_s = register_map.read_register(rt);

    switch(funct){

    case 32: ADD();  break;
    case 33: ADDU(); break;
    case 36: AND();  break;
    case 26: DIV();  break;
    case 27: DIVU(); break;
    case 9:  JALR(); break;
    case 8:  JR();   break;
    case 16: MFHI(); break;
    case 18: MFLO(); break;
    case 17: MTHI(); break;
    case 19: MTLO(); break;
    case 24: MULT(); break;
    case 25: MULTU();break;
    case 37: OR();   break;
    case 0:  SLL();  break;
    case 4:  SLLV(); break;
    case 42: SLT();  break;
    case 43: SLTU(); break;
    case 3:  SRA();  break;
    case 7:  SRAV(); break;
    case 2:  SRL();  break;
    case 6:  SRLV(); break;
    case 34: SUB();  break;
    case 35: SUBU(); break;
    case 38: XOR();  break;
  }
}

void simulate::execute_J(uint32_t instruction){

}

void simulate::execute_I(uint32_t instruction){

}


void simulate::ADD(){
    if((op1_s>0 && op2_s>0 && op1_s+op2_s<=0) || (op1_s<0 && op2_s<0 && op1_s+op2_s>=0)){
        exit(-10);
    }
    else{
        register_map.write_register(rd, (op1_s + op2_s));
    }
}


void simulate::ADDU(){
  register_map.write_register(rd, (op1 + op2));
}

void simulate::SUB(){
  if((op1_s>=0 && op2_s<0 && op1_s-op2_s<=0) || (op1_s<0 && op2_s>0 && op1_s-op2_s>=0)){
    exit(-10);
  }
  else{
    register_map.write_register(rd, (op1_s - op2_s));
  }
}

void simulate::SUBU(){
  register_map.write_register(rd, (op1 - op2));
}

void simulate::AND(){
  register_map.write_register(rd,(op1&op2));
}

void simulate::OR(){
  register_map.write_register(rd,(op1|op2));
}

void simulate::XOR(){
    register_map.write_register(rd,(op1^op2));
}

void simulate::SLL(){
    register_map.write_register(rd,(op2<<shamt));
}

void simulate::SRL(){
    register_map.write_register(rd,(op2>>shamt));
}

void simulate::SRA(){
    register_map.write_register(rd,(op2_s>>shamt));
}

void simulate::SLLV(){
    register_map.write_register(rd,(op2<<op1));
}

void simulate::SRLV(){
    register_map.write_register(rd,(op2>>op1));
}

void simulate::SRAV(){
    register_map.write_register(rd,(op2_s>>op1));
}


void simulate::SLT(){
    if(op1_s<op2_s) register_map.write_register(rd,1);
    else register_map.write_register(rd,0);
}


void simulate::SLTU(){
     if(op1<op2) register_map.write_register(rd,1);
    else register_map.write_register(rd,0);
}

void simulate::MFHI(){
    register_map.write_register(rd,register_map.hi);
}

void simulate::MTHI(){
    register_map.hi = op1;
}


void simulate::MFLO(){
    register_map.write_register(rd,register_map.lo);
}

void simulate::MTLO(){
    register_map.lo = op1;
}

void simulate::JR(){
    uint32_t op1copy = op1;
    register_map.program_counter += 4;
    execute();
    register_map.program_counter = op1copy;
}

void simulate::JALR(){
    uint32_t op1copy = op1;
    register_map.program_counter += 4;
    execute();
    register_map.write_register(rd,(register_map.program_counter+4));
    register_map.program_counter = op1copy;
}

void simulate::DIV(){

}

void simulate::DIVU(){

}

void simulate::MULT(){

}

void simulate::MULTU(){

}