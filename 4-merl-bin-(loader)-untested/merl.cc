#include "merl.h"
#include <cstdint>
#include <cstring>
#include <iostream>
#include <vector>
#define WORD_SIZE 4
using namespace std;
// make the word type
using word = uint32_t;
// Function to load MERL data and return word array
vector<word> relocate_merl(const vector<word> &data,
                               word loadAddress) {
  int pos = 0;
  vector<word> memory;
  word magicNumber = data[pos++];
  word endModAddress = data[pos++];
  word endModCodeAddress = data[pos++];
  for (int i = 3; i < endModCodeAddress / WORD_SIZE; i++) {
    word value = data[i];
    memory.push_back(value);
  }
  pos = endModCodeAddress;
  while (pos < data.size()) {
    word recordType = data[pos++];
    if (recordType == REL) {
      word relAddress = data[pos++];
      word index = relAddress - WORD_SIZE * 3;
      index = index / WORD_SIZE;
      memory[index] += (loadAddress - WORD_SIZE * 3);
    } else if (recordType == ESR) {
      word esrAddress = data[pos++];
      word esrSize = data[pos++];
      for (int i = 0; i < esrSize; i++) {
        word esrValue = data[pos++];
      }
    } else if (recordType == ESD) {
      word esdAddress = data[pos++];
      word esdSize = data[pos++];
      for (int i = 0; i < esdSize; i++) {
        word esdValue = data[pos++];
      }
    } else {
      cerr << "Error: Invalid record type" << endl;

      exit(1);
    }
  }
  return memory;
}
