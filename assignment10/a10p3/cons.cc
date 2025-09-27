#include <iostream>
#include <cstdint>
#include "a10p1.h"
using namespace std;
// === Insert any helper functions here
// one word is 8 bytes
int64_t *TheArena = arena();
int setmemsize = 128; // set the memory size of 24 bytes
int64_t freeHistHead = (int64_t)TheArena;
// returns the address of a pair of words from the arena, initialized to a and b respectively, or 0 if no memory is available.
int64_t *cons(int64_t a, int64_t *b)
{
  int64_t *ret = (int64_t *)freeHistHead;
  int64_t &nextNode = *(ret + 1); // this is the pointer to the next index of the linked list
  if (nextNode == 0)
  {
    int64_t memorySize = *ret;
    // cout << "ret = " << ret << " " << memorySize << endl;
    if (memorySize < 16)
    {
      return 0;
    }
    freeHistHead = (int64_t)((int64_t *)freeHistHead + 2);
    *(int64_t *)freeHistHead = memorySize - 16;
    *((int64_t *)freeHistHead + 1) = 0;
  }
  else
  {
    freeHistHead = nextNode;
  }
  *ret = a;
  nextNode = (int64_t)b;
  return ret;
}

// returns the first element of the pair whose address is p.
int64_t car(int64_t *p)
{
  return *p;
}

// returns the second element of the pair whose address is p.
int64_t *cdr(int64_t *p)
{
  return (int64_t *)*(p + 1);
}

// sets the first element of p to the value v and returns p.
int64_t *setcar(int64_t *p, int64_t v)
{
  *p = v;
  return p;
}

// sets the second element of p to the value v and returns p.
int64_t *setcdr(int64_t *p, int64_t *v)
{
  *(p + 1) = (int64_t)v;
  return p;
}

// deletes the pair whose address is p, or does nothing if p is null.
void snoc(int64_t *p)
{
  // cout << "Freeing: " << p << endl;
  //  need to use a reference freelisthead is the reference to the head
  //  need to store the value and address of the free list head
  int64_t prev = freeHistHead;                       // *listHead is the head of the list
  int64_t FLH = freeHistHead;                        // *listHead is the head of the list
  int64_t next = ((int64_t) * ((int64_t *)FLH + 1)); // next is a lvalue

  if ((int64_t *)freeHistHead > p)
  {
    // add front
    *p = 16;
    *(p + 1) = FLH;
    freeHistHead = (int64_t)p;
    return;
  }
  // add to the middle

  while (next != 0 && next < (int64_t)p)
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
  // cout << "FLH: " << (int64_t *)FLH << " p: " << p << " next: " << (int64_t *)next << endl;
  *(p + 1) = next;
  *((int64_t *)FLH + 1) = (int64_t)p; // set the next value of FLH to p
}
void printMemory()
{
  cout << "===== Printing Memory =====" << endl;
  for (int i = 0; i <= setmemsize / 8 + 1; i++)
  {
    int64_t *ptr = (TheArena + i);
    if (((int64_t)ptr % 16) == 0)
    {
      cout << ptr << " " << *ptr << "\n";
    }
    else
    {
      cout << ptr << " " << (int64_t *)*ptr << "\n";
    }
  }
}

int main()
{
  int64_t *a = cons(1, NULL);
  int64_t *b = cons(2, NULL);
  int64_t *c = cons(3, NULL);
  int64_t *d = cons(4, NULL);
  int64_t *e = cons(5, NULL);
  int64_t *f = cons(6, NULL);
  printMemory();
  cout << "Freeeing a" << endl;
  snoc(a);
  printMemory();
  cout << "Freeeing a" << endl;
  snoc(b);
  printMemory();
  cout << "Freeeing c" << endl;
  snoc(c);
  printMemory();
  cout << "Freeeing d" << endl;
  snoc(d);
  printMemory();
    cout << "Freeeing e" << endl;
  snoc(e);
  printMemory();
  cout << "Freeeing f" << endl;
  snoc(f);
  printMemory();

  int64_t *curLinkedList = (int64_t *)freeHistHead;
  /*
  while (curLinkedList != 0)
  {

    cout << curLinkedList << endl;

    curLinkedList = (int64_t *)*(curLinkedList + 1);
  }
*/
  cout << endl;
  return 0;
}