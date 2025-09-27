#include <iostream>
#include <cstdint>
#include "a10p2.h"

// === Insert any helper functions here
// one word is 8 bytes
int64_t *readLinked_List(int &bytesRead)
{

  int64_t *ret = NULL;
  char c1;
  
  if (std::cin >> std::noskipws >> c1)
  {
    bytesRead += 1;
    ret = readLinked_List(bytesRead);
    ret = cons(c1, ret);
  }
  return ret;
}
void printLinkedList(int64_t *linkedList)
{
  if (linkedList == NULL)
    return;
  std::cout << (char)car(linkedList);
  printLinkedList(cdr(linkedList));
}
void deleteList(int64_t *linkedList)
{
  if (linkedList == NULL)
    return;
  deleteList(cdr(linkedList));
  snoc(linkedList);
}
int64_t wain(int64_t *, int64_t)
{
  // Insert mainline code here
  int bytesRead = 0;
  int64_t *p = readLinked_List(bytesRead);
  printLinkedList(p);
  printLinkedList(p);
  deleteList(p);
  return bytesRead; // replace as appropriate
}

// Do not modify the code below.
int main(int argc, char *argv[])
{
  int ret = wain(0, 0);
  std::cerr << ret << std::endl;
  return ret;
}
