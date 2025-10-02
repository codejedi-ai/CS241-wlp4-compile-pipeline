#ifndef MERL_H
#define MERL_H

#include <vector>
#include <cstdint>

// Function to load MERL data and return uint32 array
std::vector<uint32_t> load_merl(const std::vector<uint32_t>& data, uint32_t loadAddress);

#endif
