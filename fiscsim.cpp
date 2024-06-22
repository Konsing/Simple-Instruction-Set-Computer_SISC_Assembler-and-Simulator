//ficsim.cpp
#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <stdint.h>
using namespace std;
//function to check the string is a digit or not
bool checkDigit(string str) {
 for (int i = 0; i < str.length(); i++)
   if (isdigit(str[i]) == false){
      return false;
    }
    return true;
}

int main(int argc, char* argv[]){
  string line; //string holding each line
  int inst, rn, rm, rd; //registers/instruction
  int index = 0;
  int cycle = 0; //cycles
  int userCycle = 0; //holder for custom cycles from user
  int pc = 0; //program counter
  int bta; //branch target address
  uint8_t instructionMemory[64];
  uint8_t registers[4] = {0, 0, 0, 0};
  bool zFlag = true; //true = 1, false = 0
  string argv3, argv4;
  bool printDisassembly = false;

  if(argc < 2 || argc > 4){ //if not 3 or 4 command line args
    cerr << "Usage: ./fiscsim <object file> [cycles] [-d]\n";
    return -1;
  }

  string hexFile = argv[1];
  
  if(argc == 3){ //if 3 arguments check if -d or int
    argv3 = argv[2];
    if(argv3 == "-d"){
      printDisassembly = true;
    } else {
      if(checkDigit(argv3)){
        userCycle = (stoi(argv3) + 1);
      } else {
        cerr << "Usage: ./fiscsim <object file> [cycles] [-d]\n";
        return -1;
      }
    }
  } else if(argc == 4){ //if argc 4, check if -d or ints in either
    argv3 = argv[2];
    argv4 = argv[3];
    
    //if both arguments 3 and 4 are -d, return error
    if(argv3 == "-d" && argv4 == "-d"){
      cerr << "Both command line operators cannot be '-d'\n";
      return -1;
    }
    //if both arguments 3 and 4 are ints, return error
    if(checkDigit(argv3) && checkDigit(argv4)){
      cerr << "Both command line operators cannot be ints\n";
      return -1;
    }
    //if argv3 is -d. print d. Else, check if argv3 is an int.
    //if argv3 is int, this is cycles, else, this is not accepted
    if(argv3 == "-d"){
      printDisassembly = true;
    } else {
      if(checkDigit(argv3)){
        userCycle = (stoi(argv3) + 1);
      } else {
        cerr << "Usage: ./fiscsim <object file> [cycles] [-d]\n";
        return -1;
      }
    }
    //if argv4 is -d. print d. Else, check if argv4 is an int.
    //if argv4 is int, this is cycles, else, this is not accepted
    if(argv4 == "-d"){
      printDisassembly = true;
    } else {
      if(checkDigit(argv4)){
        userCycle = (stoi(argv4) + 1);
      } else {
        cerr << "Usage: ./fiscsim <object file> [cycles] [-d]\n";
        return -1;
      }
    }
  }
  
  ifstream inputFile(hexFile);
  
  if(inputFile.is_open()){
    //Call getline once to get rid of v2.0 raw
    getline(inputFile, line);
    if(line.compare("v2.0 raw") == 0){
      //run
    } else{
      cout << "Hex file does not contain v2.0 raw.\n";
      return -1;
    }
    
    while(getline(inputFile, line)){ //fill the instruct mem
      stringstream ss;
      ss << line;
      int val;
      ss >> hex >> val;  
      instructionMemory[index] = val;
      index++;
    } //end of while getline loop
    
    if(userCycle == 0){//if no user input cycles, default 20
      userCycle = 21;
    }

    while(cycle < userCycle){
      cycle++;
      
      if (cycle == userCycle){
        break;
      }      
      //get instruction of from bits
      inst = instructionMemory[pc] >> 6;
      
      if(inst == 3){ //if BNZ, get the address only
        bta = instructionMemory[pc] & 63; //bta holds address
        if(zFlag == true){ //jump (pc change) if z != 0
          //nothing
        } else {
          pc = bta-1;
        }
      } else { //if not BNZ, set rd, rn, and rm
        rd = instructionMemory[pc] & 3;
        rn = (instructionMemory[pc] >> 4) & 3;
        if(inst != 2){ //only set rm if inst is "NOT"
        rm = (instructionMemory[pc] >> 2) & 3;
        }
      }

      if(inst == 0){ //add
        registers[rd] = registers[rn] + registers[rm];
      } else if(inst == 1){//and
        registers[rd] = registers[rn] & registers[rm];
      } else if(inst == 2){//not
        registers[rd] = ~registers[rn];
      }
      
      if(registers[rd] == 0){ //set the zFlag depending on rd
        zFlag = true;
      } else {
        zFlag = false;
      }

      cout << "Cycle:" << dec << cycle;
      if(inst == 3){
        cout << " State:PC:" << setfill('0') << setw(2) << pc+1;
      } else {
        cout << " State:PC:" << setfill('0') << setw(2) << pc+1;
      }
      cout << " Z:" << zFlag;
      cout << " R0: " << setw(2) << uppercase << hex 
        << (unsigned)registers[0];
      cout << " R1: " << setw(2) << uppercase << hex 
        << (unsigned)registers[1];
      cout << " R2: " << setw(2) << uppercase << hex 
        << (unsigned)registers[2];
      cout << " R3: " << setw(2) << uppercase << hex
        << (unsigned)registers[3];

      if(printDisassembly == true){ //print disassembly if -d
        cout << "\nDisassembly: ";
  
        if(inst == 0){
          cout << "add ";
        } else if(inst == 1){
          cout << "and ";
        } else if(inst == 2){
          cout << "not ";
        } else if(inst == 3){
          cout << "bnz ";
          cout << setfill('0') << setw(2) << dec << bta;
        }   
  
        if(inst != 3){
          if(rd == 0){
            cout << "r0 ";
          } else if(rd == 1){
            cout << "r1 ";
          } else if(rd == 2){
            cout << "r2 ";
          } else if(rd == 3){
            cout << "r3 ";
          }
        }
  
        if(inst != 3){
          if(rn == 0){
            cout << "r0 ";
          } else if(rn == 1){
            cout << "r1 ";
          } else if(rn == 2){
            cout << "r2 ";
          } else if(rn == 3){
            cout << "r3 ";
          }
        }
  
        if(inst != 2 && inst != 3){
          if(rm == 0){
            cout << "r0 ";
          } else if(rm == 1){
            cout << "r1 ";
          } else if(rm == 2){
            cout << "r2 ";
          } else if(rm == 3){
            cout << "r3 ";
          }
        }
      }
   
      cout << endl << endl;
      pc++;
    }// end of for loop
  } else {
    cerr << "Could not open '.h' file." << endl;
    return -1;
  } //end of "could not open file" else statement

  return 0;
}