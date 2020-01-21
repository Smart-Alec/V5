#include "vex.h"

using namespace vex;

//Binary Conversion Functions
const char* hex_char_to_bin(char c){
  switch(toupper(c)){
        case '0': return "0000";
        case '1': return "0001";
        case '2': return "0010";
        case '3': return "0011";
        case '4': return "0100";
        case '5': return "0101";
        case '6': return "0110";
        case '7': return "0111";
        case '8': return "1000";
        case '9': return "1001";
        case 'A': return "1010";
        case 'B': return "1011";
        case 'C': return "1100";
        case 'D': return "1101";
        case 'E': return "1110";
        case 'F': return "1111";
        default: return "0000";
    }
}

std::string hex_to_bin(const std::string& hex){
    std::string bin;
    for(unsigned i = 0; i < hex.length(); ++i)
       bin += hex_char_to_bin(hex[i]);
    return bin;
}

//Send 3 bytes to ws2812b
void send_bits(char bits[24], vex::digital_out port){
  for(int i = 0; i < strlen(bits); i++){
    if(strncmp(&bits[i], "1", 1) == 0){
    }
  }
}

struct Ws2812b{
  int length;
};

int main(){

  vexcodeInit();
}
