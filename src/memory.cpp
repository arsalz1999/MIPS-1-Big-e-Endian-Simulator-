#include "memory.hpp"
#include <iostream>
#include <fstream>

memory::memory(std::string name_bin){

    ADDR_DATA.resize(0x4000000);
    std::fill(ADDR_DATA.begin(), ADDR_DATA.end(), 0);

    //reading the binary file
    std::ifstream input_file(name_bin.c_str(), std::ifstream::binary);

    if(!input_file.is_open()){
        std::exit(-21);
    }

    //calculating the size of binary file
    input_file.seekg(0, std::ios::end);
    int bin_size = input_file.tellg();
    input_file.seekg(0, std::ios::beg);

    if(bin_size > 0x1000000){
        std::exit(-11);
    }


    //loads the input binaries into an array
    char bin_input[bin_size];
    input_file.read(bin_input, bin_size);
    input_file.close();

    //computes the number of instructions
    total_instructions = bin_size/4;
    
    //loading the binary instructions 
    int add_index=0;
    for (int i = 0; i < total_instructions; i++){
        add_index=i*4;
        ADDR_INSTR.push_back(((bin_input[add_index]<<24)&0xFF000000)
        |((bin_input[add_index+1]<<16)&0x00FF0000)|
        ((bin_input[add_index+2]<<8)&0x0000FF00)|
        ((bin_input[add_index+3])&0x000000FF));
    }
}

uint32_t memory::read_instruction(uint32_t pc){
    if((pc >= 0x10000000) && (pc < 0x11000000) && (pc % 4 == 0)){
        uint32_t pc_position = 0.25*(pc-0x10000000);
        if(pc_position < ADDR_INSTR.size()) return ADDR_INSTR[pc_position];
        else return 0;
    }
    else std::exit(-11);
}

int32_t memory::load_from_memory(int pc_position){
  if(pc_position==0x30000000){
    int data_in = std::getchar(); 
    if(std::cin.eof()) return 0xFFFFFFFF; 
    if(!std::cin.good()) std::exit(-21);
    return data_in; 
  }

  if((pc_position % 4 == 0) && (pc_position >= 0x20000000) && (pc_position < 0x24000000)){
    uint32_t real_pc_position = (pc_position-0x20000000);
    return (((int32_t(ADDR_DATA[real_pc_position]<<24))&0xFF000000)|
    ((int32_t(ADDR_DATA[real_pc_position+1]<<16))&0x00FF0000)|
    ((int32_t(ADDR_DATA[real_pc_position+2])<<8)&0x0000FF00)|
    (int32_t(ADDR_DATA[pc_position+3])&0x000000FF));
  }
  else std::exit(-11); 
}

int32_t memory::load_byte_from_memory(int pc_position){
  if((pc_position >= 0x30000000)&&(pc_position < 0x30000004)){
    char data_in = std::getchar();
    if(std::cin.eof() || feof(stdin)) return -1;
    if(!std::cin.good()) std::exit(-21);
    if(pc_position==0x30000003) return int32_t(data_in);
    return 0x00000000;
  }
  if((pc_position >= 0x20000000) && (pc_position < 0x24000000)){
    uint32_t real_pc_position = (pc_position-0x20000000);
    int byte = (0x000000FF & ADDR_DATA[real_pc_position]);
    int bitmask = 0x00000080;
    if(bitmask & ADDR_DATA[real_pc_position]){
      byte += 0xFFFFFF00;
    }
    return byte;
  }
  else std::exit(-11);
}


uint32_t memory::load_unsigned_byte_from_memory(int pc_position){
  if((pc_position >= 0x30000000) && (pc_position < 0x30000004)){
    char data_in = std::getchar();
    if(std::cin.eof() || feof(stdin)) return 0x000000FF;
    if(!std::cin.good()) std::exit(-21);
    if(pc_position==0x30000003) return (int32_t(data_in) & 0x000000FF);
    return 0;
  }
  if((pc_position >= 0x20000000) && (pc_position < 0x24000000)){
    uint32_t real_pc_position = (pc_position - 0x20000000);
    return uint32_t(int32_t(ADDR_DATA[real_pc_position])&0x000000FF);
  }
  else std::exit(-11);
}

int32_t memory::load_half_word_from_memory(int pc_position){
  
  if((pc_position == 0x30000000) || (pc_position == 0x30000002)){
    char data_in = std::getchar();
    if(std::cin.eof() || feof(stdin))   return 0xFFFFFFFF;
    if(!std::cin.good()) std::exit(-21);
    if(pc_position == 0x30000002) return (int32_t)(data_in) & 0xFF;
    return 0;
  }
  if((pc_position >= 0x20000000) && (pc_position < 0x24000000) && (pc_position % 2 == 0))
  {
    uint32_t real_pc_position = (pc_position - 0x20000000);
    int16_t sextend_halfword = (((ADDR_DATA[real_pc_position]<<8)&0xFF00)|
    (ADDR_DATA[real_pc_position+1]&0xFF));
    return (int32_t)sextend_halfword;
  }
  else std::exit(-11);
  
}

uint32_t memory::load_unsigned_half_word_from_memory(int pc_position){
  if((pc_position == 0x30000000) || (pc_position == 0x30000002)){
    char data_in = std::getchar();
    if(std::cin.eof() || feof(stdin)) return 0x0000FFFF;
    if(!std::cin.good()) std::exit(-21);
    if(pc_position == 0x30000002) return (int32_t(data_in) & 0x000000FF);
    return 0;
  }
  if((pc_position >= 0x20000000) && (pc_position < 0x24000000) && (pc_position % 2 == 0)){
    uint32_t real_pc_position = (pc_position - 0x20000000);
    return (int32_t((int16_t(ADDR_DATA[real_pc_position]<<8)&0xFF00)|
    (int16_t(ADDR_DATA[real_pc_position+1])&0xFF))&0xFFFF);
  }
  else std::exit(-11);
}

int32_t memory::load_word_right_from_memory(int pc_position){
  if(pc_position >= 0x30000000 && pc_position < 0x30000004){
    int data_in = std::getchar();
    if(std::cin.eof() || feof(stdin)) return 0xFFFFFFFF;
    if(!std::cin.good()) std::exit(-21);
    if(pc_position == 0x30000003) return (data_in);
    else return 0;
  }
  if((pc_position >= 0x20000000) && (pc_position < 0x24000000)){
    uint32_t real_pc_position = (pc_position - 0x20000000);
    switch (real_pc_position % 4) {
      case 0: return(int32_t(ADDR_DATA[real_pc_position])&0x000000FF); break;
      case 1: return(int32_t(((ADDR_DATA[real_pc_position-1]<<8)&0x0000FF00)
      |(ADDR_DATA[real_pc_position]&0x000000FF))); break;
      case 2: return(int32_t(((ADDR_DATA[real_pc_position-2]<<16)&0x00FF0000)|
      ((ADDR_DATA[real_pc_position-1]<<8)&0x0000FF00)|(ADDR_DATA[real_pc_position]&0x000000FF))); break;
      case 3: return(int32_t(((ADDR_DATA[real_pc_position-3]<<24)&0xFF000000)|
      ((ADDR_DATA[real_pc_position-2]<<16)&0x00FF0000)|((ADDR_DATA[real_pc_position-1]<<8)&0x0000FF00)|
      (ADDR_DATA[real_pc_position]&0x000000FF))); break;
    }
  }
  else std::exit(-11);
}

int32_t memory::load_word_left_from_memory(int pc_position){
  if(pc_position >= 0x30000000 && pc_position < 0x30000004){
    int data_in = std::getchar();
    if(std::cin.eof() || feof(stdin)) return 0xFFFFFFFF;
    if(!std::cin.good()) std::exit(-21);
    if(pc_position == 0x30000000) return (data_in);
    else return 0;
  }

  if((pc_position >= 0x20000000) && (pc_position < 0x24000000)){
    uint32_t real_pc_position = (pc_position-0x20000000);
    switch (real_pc_position % 4) {
      case 0: return (((ADDR_DATA[real_pc_position]<<24)&0xFF000000)|
      ((ADDR_DATA[real_pc_position+1]<<16)&0x00FF0000)|
      ((ADDR_DATA[real_pc_position+2]<<8)&0x0000FF00)|
      (ADDR_DATA[real_pc_position+3]&0x000000FF)); break;
      case 1: return (int32_t(((ADDR_DATA[real_pc_position]<<24)&0xFF000000)|
      ((ADDR_DATA[real_pc_position+1]<<16)&0x00FF0000)|
      ((ADDR_DATA[real_pc_position+2]<<8)&0x0000FF00))&0xFFFFFF00); break;
      case 2: return (int32_t(((ADDR_DATA[real_pc_position]<<24)&0xFF000000)|
      ((ADDR_DATA[real_pc_position+1]<<16)&0x00FF0000))&0xFFFF0000); break;
      case 3: return(int32_t(ADDR_DATA[real_pc_position]<<24)&0xFF000000); break;
      }
  }
  else std::exit(-11);
}
