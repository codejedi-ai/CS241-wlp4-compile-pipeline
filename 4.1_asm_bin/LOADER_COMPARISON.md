# Loader Comparison: Theoretical vs Actual Implementation

## Theoretical Loader (Your Code)

### Algorithm
```pseudocode
read_word()
endMod ← read_word()
codeSize ← read_word() - 12
α ← findFreeRAM(codeSize)

// 1. Load the Code into RAM
for (int i = 0; i < codeSize; i += 4)
    MEM[α + i] ← read_word()

// 2. Process Relocation Records
i ← codeSize + 12
while (i < endMod)
    format ← read_word()
    if (format == 1)
        rel ← read_word()
        // Relocation step: Update an address inside the loaded code
        MEM[α + rel - 12] += α - 12
    else
        ERROR // Handle other formats or an invalid record
    i += 8 // Advance past the 'format' word (4 bytes) and the 'rel' word (4 bytes)
```

### Features
- ✅ **MERL Magic Validation**: Checks for valid MERL file
- ✅ **End Module Reading**: Reads endMod to find module boundary
- ✅ **Code Size Calculation**: Calculates codeSize = endMod - 12
- ✅ **RAM Allocation**: Finds free RAM address (α)
- ✅ **Code Loading**: Loads code into allocated RAM
- ✅ **Relocation Processing**: Updates addresses with relocation records
- ✅ **Address Fixing**: Adjusts addresses based on load location

## Actual Loader (merl.cc)

### Algorithm
```cpp
// Read entire file into memory
vector<char> data;
while (cin.get(byte)) {
    data.push_back(byte);
}

// Check MERL magic number
int magic = (data[0] << 24) | (data[1] << 16) | (data[2] << 8) | data[3];
if (magic != 0x10000002) {
    cerr << "Error: Not a valid MERL file" << endl;
    return 1;
}

// Get code size directly
int codeSize = (data[4] << 24) | (data[5] << 16) | (data[6] << 8) | data[7];

// Output the code section
for (int i = 8; i < 8 + codeSize && i < data.size(); i++) {
    cout << data[i];
}
```

### Features
- ✅ **MERL Magic Validation**: Checks for valid MERL file
- ❌ **End Module Reading**: Does NOT read endMod
- ❌ **Code Size Calculation**: Reads codeSize directly from header
- ❌ **RAM Allocation**: Does NOT allocate RAM
- ❌ **Code Loading**: Does NOT load into RAM
- ❌ **Relocation Processing**: Does NOT process relocation records
- ❌ **Address Fixing**: Does NOT fix addresses

## Key Differences

### 1. **MERL File Structure Understanding**

**Theoretical**: Understands full MERL format with relocation records
```
[MERL Magic][Code Size][Code Section][Relocation Records][End Marker]
```

**Actual**: Only understands basic MERL format
```
[MERL Magic][Code Size][Code Section][End Marker]
```

### 2. **Relocation Handling**

**Theoretical**: 
- Processes relocation records
- Updates addresses based on load location
- Handles format 1 relocation records
- Fixes addresses: `MEM[α + rel - 12] += α - 12`

**Actual**: 
- No relocation processing
- No address fixing
- Just extracts raw code

### 3. **Memory Management**

**Theoretical**:
- Allocates RAM (`α ← findFreeRAM(codeSize)`)
- Loads code into memory
- Updates memory locations

**Actual**:
- No memory allocation
- No memory loading
- Just outputs to stdout

### 4. **Load Address Usage**

**Theoretical**:
- Uses load address for relocation
- Adjusts addresses based on load location
- Implements proper memory layout

**Actual**:
- Load address parameter ignored
- No address adjustment
- Simplified for educational purposes

## Similarity Score: **~20%**

### What's Similar
- ✅ MERL magic number validation
- ✅ Basic file reading
- ✅ Code size extraction

### What's Missing
- ❌ Relocation record processing
- ❌ Memory allocation and loading
- ❌ Address fixing and adjustment
- ❌ End module boundary handling
- ❌ Load address utilization

## Conclusion

The actual `merl.cc` implementation is a **highly simplified version** designed for educational purposes. It only implements the basic MERL file reading and code extraction, but lacks the sophisticated relocation processing, memory management, and address fixing that a real loader would need.

**The theoretical loader you provided is much more complete and represents what a real MERL loader should do!**
