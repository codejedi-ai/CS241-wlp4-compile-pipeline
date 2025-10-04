import struct
import os
from typing import BinaryIO

def print_merl_hex_dump(file_name: str):
    """
    Reads a binary MERL file word-by-word, interprets them as 
    Big-Endian 32-bit integers, and prints a formatted hexadecimal dump.
    """
    # 32-bit word size in bytes
    WORD_SIZE = 4
    # '>I' unpacks 4 bytes as a Big-Endian (>) Unsigned Integer (I)
    FORMAT = '>I'
    
    try:
        with open(file_name, 'rb') as f:
            print(f"\n--- HEX DUMP for {file_name} ---")
            
            offset = 0
            while True:
                # Read 4 bytes (one word)
                word_bytes = f.read(WORD_SIZE)
                
                # Check if we reached the end of the file
                if not word_bytes:
                    break
                
                # Ensure we read exactly 4 bytes
                if len(word_bytes) < WORD_SIZE:
                    # Partial word at the end (should not happen in a valid MERL file)
                    print(f"[{offset:08x}] (Partial word: {word_bytes.hex()})")
                    break

                # Unpack the bytes into a Big-Endian 32-bit integer
                # The result is a tuple, so we take the first element [0]
                word_int = struct.unpack(FORMAT, word_bytes)[0]
                
                # Format the output: [Offset] WordValue
                # :08x ensures the offset is 8 hex digits, :08x ensures the word is 8 hex digits
                print(f"[{offset:08x}] {word_int:08x}")
                
                offset += WORD_SIZE

        print(f"--- End of File: {offset} bytes read ---\n")
        
    except FileNotFoundError:
        print(f"Error: File '{file_name}' not found.")
    except Exception as e:
        print(f"An unexpected error occurred: {e}")


# --- Example Usage ---

# NOTE: This assumes you have run the previous code block to create these files.
if __name__ == "__main__":
            
    # Check if the files exist before attempting to read
    if not os.path.exists("t1.merl") or not os.path.exists("t2.merl"):
        print("Please run the previous code block to generate 't1.merl' and 't2.merl' first.")
    else:
        print_merl_hex_dump("t1.merl")
        print_merl_hex_dump("t2.merl")
        print_merl_hex_dump("output.merl")