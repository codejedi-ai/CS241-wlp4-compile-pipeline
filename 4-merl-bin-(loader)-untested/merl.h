#define REL 0x00000001
#define ESR 0x00000011
#define ESD 0x00000005
#include <vector>
#include <cstdint>
using namespace std;
// Function to load MERL data and return uint32 array
vector<uint32_t> relocate_merl(const vector<uint32_t> &data,
    uint32_t loadAddress);
