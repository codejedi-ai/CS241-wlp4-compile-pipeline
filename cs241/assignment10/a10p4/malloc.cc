#include <iostream>
#include <cstdint>
#include "a10p1.h"
using namespace std;
// === Insert any helper functions here
// one word is 8 bytes
int64_t *TheArena = arena();
int setmemsize = 1024;
int64_t freeListRoot = (int64_t)TheArena;
int64_t *mymalloc(int64_t size)
{
  int64_t wordNeedAllocateinBytes = 8 * (size + 1); // bytes
  int64_t *freeListHead = &freeListRoot;
  // dereference to get the actual value which contains the previouse node
  // pointer reference to anouther pointer
  // the current node is the one it points to
  int64_t *curNode = (int64_t *)(*freeListHead);
  while (curNode != 0)
  {
    curNode = (int64_t *)(*freeListHead);
    int64_t avalSize = *curNode;
    if (avalSize >= wordNeedAllocateinBytes)
    {
      if (avalSize - wordNeedAllocateinBytes == 8)
      {
        size++;
        wordNeedAllocateinBytes += 8;
      }
      break;
    }
    freeListHead = ((int64_t *)*freeListHead) + 1;
    curNode = (int64_t *)(*freeListHead);
  }
  if (curNode == 0)
    return 0;
  // curNode is the adress we keep the size value
  // splitting
  int64_t avalSize = *curNode;
  if (*curNode > wordNeedAllocateinBytes)
  {
    *(curNode + size + 1) = *curNode - wordNeedAllocateinBytes; // making the size of the new segment
    *(curNode + size + 2) = *(curNode + 1);                     // making the size of the new segment
    *freeListHead = (int64_t)(curNode + size + 1);
  }
  else
  {
    *freeListHead = *(curNode + 1);
  }
  *curNode = wordNeedAllocateinBytes;
  return curNode + 1;
}

// deallocates the memory stored at address.
// assumes that address contains either an address allocated by mymalloc, in which case it deallocates that memory,
// or the value 0 (NULL), in which case myfree does nothing.
void myfree(int64_t *address)
{
  if (!address)
    return;
  // cout << "Freeing: " << p << endl;
  //  need to use a reference freelisthead is the reference to the head
  //  need to store the value and address of the free list head
  int64_t prev = freeListRoot;                       // *listHead is the head of the list
  int64_t FLH = freeListRoot;                        // *listHead is the head of the list
  int64_t next = ((int64_t) * ((int64_t *)FLH + 1)); // next is a lvalue
  address = address - 1;
  if ((int64_t *)freeListRoot > address)
  {
    // add front
    // we also need to merge
    cout << "add front " << endl;
    *(address + 1) = FLH;
    freeListRoot = (int64_t)address;
    // need to have past, present and future nodes
    if ((int64_t)address + *address == *(address + 1))
    {
      // The address is equal to the one $size bits over implieing the next block of addresses is also empty
      *address = *address + *(int64_t *)*(address + 1);
      *(address + 1) = *(1 + (int64_t *)FLH);
    }
    return;
  }
  // add to the middle
  // this checks wether or not the next value is smaller than the address. if the next value is then the
  // current address FLH is definatelly smaller than the address and the nect value is larger
  while (next != 0 && next < (int64_t)address)
  {
    next = (*((int64_t *)FLH + 1));
    // cout << "FLH: " << (int64_t *)FLH << " next: " << (int64_t *)next << endl;
    if (FLH == next)
    {
      //  cout << "Infinite Cycle detected\n";
      return;
    }
    FLH = next; // *listHead is the head of the list
    next = (*((int64_t *)FLH + 1));
  }
  // next is the next pointer
  // address is the current
  // FLH is the previouse
  cout << "add from linked list " << endl;
  // cout << "FLH: " << (int64_t *)FLH << " p: " << p << " next: " << (int64_t *)next << endl;
  *(address + 1) = next; // set the address pointing from address to next
  cout << (int64_t)address + *address << " == " << *(address + 1) << endl;
  if ((int64_t)address + *address == *(address + 1))
  {
    // merge with the right
    // The address is equal to the one $size bits over implieing the next block of addresses is also empty
    *address = *address + *(int64_t *)next;
    *(address + 1) = *(1 + (int64_t *)next);
  }
  *((int64_t *)FLH + 1) = (int64_t)address; // set the next value of FLH to p
  cout << (int64_t)address + *address << " == " << *(address + 1) << endl;

  if (*(int64_t *)FLH + FLH == (int64_t)address)
  {
    // merge with the left
    // The address is equal to the one $size bits over implieing the next block of addresses is also empty
    *(int64_t *)FLH = *(int64_t *)FLH + *address;
    *((int64_t *)FLH + 1) = *(1 + address); // need to know th evalue of the next
  }
}

void printMemory()
{
  cout << "===== Printing Memory =====" << endl;
  for (int i = 0; i <= setmemsize / 8 + 1; i++)
  {
    int64_t *ptr = (TheArena + i);

    cout << ptr << " in dec: " << *ptr << " in hex: " << (int64_t *)*ptr << "\n";
  }
}
void printFreeList()
{
  while (freeListRoot != 0)
  {
    cout << (int64_t *)freeListRoot << endl;
    freeListRoot = *((int64_t *)freeListRoot + 1);
  }
}

int main() {
    int64_t result = 0;
    int64_t* one = mymalloc(241);
    int64_t* two = mymalloc(241);
    myfree(one);

    int64_t* evil = mymalloc(240);
    int64_t* good = mymalloc(1);
    evil[3] = 240; 
    good[0] = evil[3]+1;
    myfree(evil);

    int64_t* cs[241];
    for(int i=0; i<241; i++) {
        cs[i] = mymalloc(1);
        cs[i][0] = 1;
    }
    int64_t* big = mymalloc(1000);
    for(int i=0; i<1000; i++) {
        big[i] = 1000;
    }
    for(int i=0; i<241; i++) {
        result += cs[i][0];
        myfree(cs[i]);
    }

    result += good[0];
    myfree(good);

    // Returns nonzero exit code if the result is wrong
    return result != 241*2;
}
/*
int main2()
{
  int64_t *addr = mymalloc(4);
  int64_t *addr_1 = mymalloc(4);
  int64_t *addr_2 = mymalloc(4);
  int64_t *addr_3 = mymalloc(4);

  int64_t *addr_4 = mymalloc(4);
  printMemory();
  myfree(addr_1);
  printMemory();
  myfree(addr_2);
  printMemory();

  int64_t *addr_5 = mymalloc(8);
  cout << "Special case " << endl;
  for(int i = 0; i < 8; i ++){
    addr_5[i] = -1;
  }
  printMemory();
  printFreeList();
  // Returns nonzero exit code if the result is wrong
  return addr == 0;

 main2();
}
  */