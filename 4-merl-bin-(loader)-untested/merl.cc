#include "merl.h"
#include <cstdint>
#include <cstring>
#include <iostream>
#include <vector>
using namespace std;

// Function to load MERL data and return uint32 array
vector<uint32_t> relocate_merl(const vector<uint32_t> &data,
                               uint32_t loadAddress) {
  int pos = 0;
  vector<uint32_t> memory;
  uint32_t magicNumber = data[pos++];
  uint32_t endModAddress = data[pos++];
  uint32_t endModCodeAddress = data[pos++];
  for (int i = 3; i < endModCodeAddress / 4; i++) {
    uint32_t value = data[i];
    memory.push_back(value);
  }
  pos = endModCodeAddress;
  while (pos < data.size()) {
    uint32_t recordType = data[pos++];
    if (recordType == REL) {
      uint32_t relAddress = data[pos++];
      uint32_t index = relAddress - 12;
      index = index / 4;
      memory[index] += (loadAddress - 12);
    } else if (recordType == ESR) {
      uint32_t esrAddress = data[pos++];
      uint32_t esrSize = data[pos++];
      for (int i = 0; i < esrSize; i++) {
        uint32_t esrValue = data[pos++];
      }
    } else if (recordType == ESD) {
      uint32_t esdAddress = data[pos++];
      uint32_t esdSize = data[pos++];
      for (int i = 0; i < esdSize; i++) {
        uint32_t esdValue = data[pos++];
      }
    } else {
      cerr << "Error: Invalid record type" << endl;

      exit(1);
    }
  }
  return memory;
}
