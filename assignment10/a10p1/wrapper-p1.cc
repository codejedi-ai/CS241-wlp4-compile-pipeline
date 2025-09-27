#include <iostream>
#include <cstdint>
#include "a10p1.h"
using namespace std;
// === Insert any helper functions here

int64_t wain(int64_t* a, int64_t n) {
  // Insert mainline code here
  int64_t * largeArray = arena();
  int curMaxVal = 0;
  int array_Size = largeArray[0]/8;
  for (int i = 0; i < n; i ++){
     int f = *(a + i);
     largeArray[f] ++;
     if(largeArray[f] > curMaxVal){
       curMaxVal = largeArray[f];
     }
  }
  return curMaxVal; // replace as appropriate
}

// Do not modify the code below.
int main(int argc, char *argv[]) {
  int64_t l, c;
  int64_t* a;
  std::cout << "Enter length of array: ";
  std::cin >> l; 
  a = new int64_t[l];
  for(int64_t i = 0; i < l; ++i) {
    std::cout << "Enter value of array element " << i << " ";
    std::cin >> a[i];
  }
  c = wain(a,l);
  delete [] a;
  std::cerr << c << std::endl;
  return c;
}
