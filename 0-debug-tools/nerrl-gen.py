import struct
import os
from typing import List

# --- MERL Data from Previous Step (Big-Endian Hex Strings) ---
# NOTE: This list contains the correct, Big-Endian machine code for t1.merl
T1_MERL_HEX_WORDS: List[str] = [
    # HEADER (3 words: 0x00 - 0x08)
    "10000002", "00000088", "00000034",
    # CODE SECTION (10 words: 0x0C - 0x30)
    "00040014", "0000002A", "AFDFFFFC", "00050014", "00000000", "00A00009", 
    "14800001", "00000064", "8FDFFFFC", "03E00008",
    # LINKER RECORDS (Starting 0x34)
    # REL RECORD (2 words)
    "00000001", "00000010",
    # ESR RECORD (External: funcA) (6 words)
    "00000011", "0000001C", "00000005", "00000066", "00000075", "0000006E", 
    "00000063", "00000041",
    # ESD RECORD (Export: data1) (7 words)
    "00000005", "00000028", "00000005", "00000064", "00000061", "00000074", 
    "00000061", "00000031",
    # PADDING (6 words)
    "00000000", "00000000", "00000000", "00000000", "00000000", "00000000",
]

# NOTE: This list contains the correct, Big-Endian machine code for t2.merl
T2_MERL_HEX_WORDS: List[str] = [
    # HEADER (3 words: 0x00 - 0x08)
    "10000002", "00000074", "00000020",
    # CODE SECTION (8 words: 0x0C - 0x24)
    "00030014", "00000000", "8C630000", "00020014", "00000001", "00621820", 
    "03E00008",
    # LINKER RECORDS (Starting 0x28)
    # REL RECORD (2 words)
    "00000001", "0000001C",
    # ESR RECORD (External: data1) (7 words)
    "00000011", "00000010", "00000005", "00000064", "00000061", "00000074", 
    "00000061", "00000031",
    # ESD RECORD (Export: funcA) (7 words)
    "00000005", "0000000C", "00000005", "00000066", "00000075", "0000006E", 
    "00000063", "00000041",
    # PADDING (2 words)
    "00000000", "00000000", 
]


def save_merl_file(file_name: str, hex_words: List[str]):
    """
    Converts a list of 32-bit hexadecimal strings to Big-Endian binary 
    and saves them to a file.
    """
    binary_data = b''
    
    for hex_str in hex_words:
        # 1. Convert hex string to an integer (base 16)
        try:
            integer_value = int(hex_str, 16)
        except ValueError:
            print(f"Skipping invalid hex string: {hex_str}")
            continue

        # 2. Pack the integer into 4 bytes in Big-Endian format
        # '>' indicates Big-Endian, 'I' indicates unsigned 32-bit integer
        packed_bytes = struct.pack('>I', integer_value)
        binary_data += packed_bytes

    # 3. Write the raw binary data to the file
    try:
        with open(file_name, 'wb') as f:
            f.write(binary_data)
        print(f"Successfully saved {len(hex_words)} words ({len(binary_data)} bytes) to {file_name}")
    except IOError as e:
        print(f"Error writing file {file_name}: {e}")
        
# --- Execution ---
if __name__ == "__main__":
    # Ensure all files are saved in the required binary format
    save_merl_file("t1.merl", T1_MERL_HEX_WORDS)
    save_merl_file("t2.merl", T2_MERL_HEX_WORDS)