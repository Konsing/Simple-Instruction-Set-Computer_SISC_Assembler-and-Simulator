//fiscas.cpp
#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <string>
#include <cstring>
#include <algorithm>
using namespace std;

int main(int argc, char* argv[]) {
  string line;
  string parsed;
  int operation = 0;
  int operationFirst = 0;
  // symbolTable contains Label and Address
  map<string, int> symbolTable;
  // order contains address and instructions
  map<int, string> order;
  // will contain addresses and hex
  map<int, string> orderFinal;

  if(argc < 3 || argc > 4){
    cerr << "Usage: fiscas <asm file> <hex file> [-l]\n";
    return -1;
  }

  ifstream file(argv[1]);

  if (file.is_open()) { //if file open run 2 while loops
    while (getline(file, line)) { //first while loop, fill symbolTable
      //convert line to lower case
      for_each(line.begin(), line.end(), [](char & c) {
        c = ::tolower(c);
      });
      //clear the comments
      if (line.find(';') != string::npos) {
        int semiCPos = line.find(';');
        line.erase(semiCPos, string::npos);
        parsed = line.substr(0, semiCPos);
      } else {
        parsed = line.substr(0, string::npos);
      }

      //if parsed strring empty, continue, else extend it
      //and checkfor instructions
      if (parsed.empty()) {
        continue;
      } else {
        string parseInstructions, parseInstruction;

        //if string is less tha 9 wide, extend
        if(parsed.size() < 9){
          parsed.append("            ");
        }

        //get the instruction
        parseInstructions = parsed.substr(8, string::npos);
        parseInstruction = parseInstructions.substr(0, 3);

        //check if its a proper instruction
        if (parseInstruction == "add") {
          operationFirst++;
        } else if (parseInstruction == "and") {
          operationFirst++;
        } else if (parseInstruction == "not") {
          operationFirst++;
        } else if (parseInstruction == "bnz") {
          operationFirst++;
        } else if (parseInstruction == "   ") {
          //get the labels and check for duplicates
          if (parsed.find(':') != string::npos) {
            int colonPos = parsed.find(':');
            parsed.erase(colonPos, string::npos);
            parsed = parsed.substr(0, colonPos);
          } else {
            //if not an instruction, go to next
            continue;
          }
          
          // store and prevent duplicate labels (labels stored only)
          for (auto i = symbolTable.begin(); i != symbolTable.end(); ++i){
            if (parsed == i->first) {
              cerr << "You cannot have duplicate labels\n";
              return -1;
            }
          }  

          symbolTable.emplace(parsed, operationFirst);
          
          continue;
        } else {
          cerr << "Instruction formating error." << endl;
          return -1;
        }

        // get the labels
        if (parsed.find(':') != string::npos) {
          int colonPos = parsed.find(':');
          parsed.erase(colonPos, string::npos);
          parsed = parsed.substr(0, colonPos);
        } else {
          continue;
        }

        // store and prevent duplicate labels (labels stored only)
        for (auto i = symbolTable.begin(); i != symbolTable.end(); ++i){
          if (parsed == i->first) {
            cerr << "You cannot have duplicate labels\n";
            return -1;
          }
        }  

        symbolTable.emplace(parsed, operationFirst - 1);
      } // end of else statement*/
    } // end of ifirst while loop

    // clear the getline for 2nd pass through the file
    file.clear();
    file.seekg(0, file.beg);

    // second pass through file, remove comments and 
    //get the hex code
    while (getline(file, line)) {
      //convert line to lower case
      for_each(line.begin(), line.end(), [](char & c) {
        c = ::tolower(c);
      });
      int addVar = 0;
      int andVar = 1;
      int notVar = 2;
      int bnzVar = 3;

      int r0Var = 0;
      int r1Var = 1;
      int r2Var = 2;
      int r3Var = 3;

      int integerValue = 0;
      string rest = "", rest2 = "", rest3 = "", rest4 = "", rest5 = "";
      int iTempVar1 = 0, iTempVar2 = 0, iTempVar3 = 0, iTempVar4 = 0;

      //delete comments
      if (line.find(';') != string::npos) {
        int semiCPos = line.find(';');
        line.erase(semiCPos, string::npos);
        parsed = line.substr(0, semiCPos);
      } else {
        parsed = line.substr(0, string::npos);
      }
      
      if (parsed.empty()) {
        continue;
      } else if (parsed.size() < 9) {
          // instructions empty
        continue;
      } else {
        // check if instructions valid,
        //if so, increase operation and shift bits
        rest = parsed.substr(8, string::npos);
        rest2 = rest.substr(0, 3);

        if (rest2 == "add") {
          operation++;
          iTempVar1 = addVar;
          iTempVar1 = iTempVar1 << 6;
        } else if (rest2 == "and") {
          operation++;
          iTempVar1 = andVar;
          iTempVar1 = iTempVar1 << 6;
        } else if (rest2 == "not") {
          operation++;
          iTempVar1 = notVar;
          iTempVar1 = iTempVar1 << 6;
        } else if (rest2 == "bnz") {
          operation++;
          iTempVar1 = bnzVar;
          iTempVar1 = iTempVar1 << 6;
        } else if (rest2 == "   ") {
          continue;
        } else {
          cerr << "Instruction formating error." << endl;
          return -1;
        }

        order[operation - 1] = rest;

        // if register 1 exists, create a substr of it
        if (rest.size() >= 6) {
          rest3 = rest.substr(4, 2);
        } else {
          cerr << "Wrong formatting after instruction.\n";
          return -1;
        }

        // first register
        if (rest3 == "r0" || rest3 == "  ") {
          iTempVar2 = r0Var;
          iTempVar2 = iTempVar2;
        } else if (rest3 == "r1") {
          iTempVar2 = r1Var;
          iTempVar2 = iTempVar2;
        } else if (rest3 == "r2") {
          iTempVar2 = r2Var;
          iTempVar2 = iTempVar2;
        } else if (rest3 == "r3") {
          iTempVar2 = r3Var;
          iTempVar2 = iTempVar2;
        } else {
          // extend the line with white space
          if (rest.size() < 14) {
            rest.append("             ");
          }
          // grab the label in Rd register position (first register)
          string removeFirstInst = rest.substr(4, 10);
          int finalChar = removeFirstInst.find(' ');
          removeFirstInst = removeFirstInst.substr(0, finalChar);

          // check if the label is in the map, if it is, get the address
          //"OR" it with the instruction, and convert it to he
          bool checkIfExists = false;
          for (auto i = symbolTable.begin(); i != symbolTable.end(); ++i) {
            if (removeFirstInst == i->first) {
              checkIfExists = true;
              integerValue = (iTempVar1 | i->second);
              
              stringstream ss;
              ss << setfill('0') << setw(2) << uppercase 
              << hex << integerValue;
              orderFinal[operation - 1] = ss.str();
            }
          }

          if(checkIfExists == false){
            cerr << "That label in the Rd register does NOT exist.\n";
            exit(0);
          }

          continue;
        }

        // add white space if register 2 doesn't exist
        if (rest.size() < 8) {
          rest.append("   ");
        }
        rest4 = rest.substr(7, 2);

        // second register
        if (rest4 == "r0" || rest4 == "  ") {
          iTempVar3 = r0Var;
          iTempVar3 = iTempVar3 << 4;
        } else if (rest4 == "r1") {
          iTempVar3 = r1Var;
          iTempVar3 = iTempVar3 << 4;
        } else if (rest4 == "r2") {
          iTempVar3 = r2Var;
          iTempVar3 = iTempVar3 << 4;
        } else if (rest4 == "r3") {
          iTempVar3 = r3Var;
          iTempVar3 = iTempVar3 << 4;
        } else {
          cerr << "Wrong register name for Rn\n";
          return -1;
        }

        // add white space if third register doesn't exist
        if (rest.size() < 10) {
          rest.append("   ");
        }
        rest5 = rest.substr(10, 2);

        // third register
        if (rest5 == "r0" || rest5 == "  ") {
          iTempVar4 = r0Var;
          iTempVar4 = iTempVar4 << 2;
        } else if (rest5 == "r1") {
          iTempVar4 = r1Var;
          iTempVar4 = iTempVar4 << 2;
        } else if (rest5 == "r2") {
          iTempVar4 = r2Var;
          iTempVar4 = iTempVar4 << 2;
        } else if (rest5 == "r3") {
          iTempVar4 = r3Var;
          iTempVar4 = iTempVar4 << 2;
        } else {
          cerr << "Wrong register name for Rm\n";
          return -1;
        }
        //"OR" all the instructions and store them as hex alongside address
        integerValue = (iTempVar1 | iTempVar2 | iTempVar3 | iTempVar4);
        stringstream ss;
        ss << setfill('0') << setw(2) << uppercase << hex << integerValue;
        orderFinal[operation - 1] = ss.str();
      } // end of the else statement
    }   // end of second while loop

    //output the hex code to a file
    ofstream outFile;
    outFile.open(argv[2]);

    if(!outFile.is_open()){
        cerr << "Hex file could not be opened.\n";
        return -1;
    } else {  
      outFile << "v2.0 raw";
      for(auto i = orderFinal.begin(); i != orderFinal.end(); ++i){
          outFile << "\n" << i->second;
      }
    }

    //if user provides 4th command line operator [-l], print all info
    if(argc == 4){
      if(strcmp(argv[3], "-l") == 0){
        // Label map output
        cout << "\n*** LABEL LIST ***\n";
        for (auto i = symbolTable.begin(); i != symbolTable.end(); ++i) {
          cout << left << setfill(' ') << setw(8) << i->first;
          cout << right << setfill('0') << setw(2) << i->second << '\n';
        }
        //Prints the key and val of orderFinal map, 
        //then prints the val of order map (instructions)
        cout << "*** MACHINE PROGRAM ***\n";
        for(auto i = order.begin(), j = orderFinal.begin(); 
          i != order.end(); ++i, ++j){
          cout << setfill('0') << setw(2) << j->first << ':' << j->second;
          cout << '\t' << i->second << '\n';
        }
      } else {
        cerr << "Not the correct output command. Use -l" << endl;
        return -1;
      }
    }
  } // end of if file.is_open()
  return 0;
}