That's a much clearer way to organize the instruction set! Here is the MIPS Instruction Reference from your document, categorized into separate tables based on function, using the requested Markdown format.

---

## I. Data Movement & Memory Access (I-Type)

These instructions handle loading and storing data between registers and memory.

| Name | Assembly Syntax | Machine Syntax (Opcode) | Format | Semantics/Behavior |
| :--- | :--- | :--- | :--- | :--- |
| **Load Word** | `lw $t, i($s)` | `1000 11ss ssst tttt iiii iiii iiii iiii` | I | `$t = \text{MEM}[\$s + i]:4$ |
| **Store Word** | `sw $t, i($s)` | `1010 11ss ssst tttt iiii iiii iiii iiii` | I | $\text{MEM}[\$s + i]:4 = \$t$ |

---

## II. Arithmetic & Logic (R-Type)

These instructions perform core computations and comparisons.

| Name | Assembly Syntax | Machine Syntax (Function Code) | Format | Semantics/Behavior |
| :--- | :--- | :--- | :--- | :--- |
| **Add** | `add $d, $s, $t` | `...0010 0000` (0x20) | R | `$d = $s + $t$ |
| **Subtract** | `sub $d, $s, $t` | `...0010 0010` (0x22) | R | `$d = $s - $t$ |
| **Set Less Than** | `slt $d, $s, $t` | `...0010 1010` (0x2A) | R | `$d = 1` if $\$s < \$t$; $0$ otherwise (signed) |
| **Set Less Than Unsigned** | `sltu $d, $s, $t` | `...0010 1011` (0x2B) | R | `$d = 1$ if $\$s < \$t$; $0$ otherwise (unsigned) |
| **Multiply** | `mult $s, $t` | `...0001 1000` (0x18) | R | $\text{hi}:\text{lo} = \$s \times \$t$ (signed product) |
| **Multiply Unsigned** | `multu $s, $t` | `...0001 1001` (0x19) | R | $\text{hi}:\text{lo} = \$s \times \$t$ (unsigned product) |
| **Divide** | `div $s, $t` | `...0001 1010` (0x1A) | R | $\text{lo} = \$s / \$t$; $\text{hi} = \$s \% \$t$ (signed) |
| **Divide Unsigned** | `divu $s, $t` | `...0001 1011` (0x1B) | R | $\text{lo} = \$s / \$t$; $\text{hi} = \$s \% \$t$ (unsigned) |
| **Move From High/Remainder** | `mfhi $d` | `...0001 0000` (0x10) | R | `$d = \text{hi}$ |
| **Move From Low/Quotient** | `mflo $d` | `...0001 0010` (0x12) | R | `$d = \text{lo}$ |

---

## III. Control Flow (Branch & Jump)

These instructions alter the Program Counter ($\text{pc}$) to manage conditional execution and function calls.

| Name | Assembly Syntax | Machine Syntax (Opcode/Function) | Format | Semantics/Behavior |
| :--- | :--- | :--- | :--- | :--- |
| **Branch On Equal** | `beq $s, $t, i` | `0001 00...iiii iiii iiii iiii` (0x04) | I | `if ($s == $t) \text{pc} += i \times 4$ |
| **Branch On Not Equal** | `bne $s, $t, i` | `0001 01...iiii iiii iiii iiii` (0x05) | I | `if ($s \neq $t) \text{pc} += i \times 4$ |
| **Jump Register** | `jr $s` | `...0000 1000` (0x08) | R | $\text{pc} = \$s$ |
| **Jump And Link Register** | `jalr $s` | `...0000 1001` (0x09) | R | $\text{temp} = \$s; \$31 = \text{pc}; \text{pc} = \text{temp}$ |

---

## IV. Special & Directives

These handle 32-bit immediate loading and memory initialization.

| Name | Assembly Syntax | Machine Syntax (Function Code) | Format | Semantics/Behavior |
| :--- | :--- | :--- | :--- | :--- |
| **Load Immediate And Skip** | `lis $d` | `...0001 0100` (0x14) | R | `$d = \text{MEM}[\text{pc}]; \text{pc} = \text{pc} + 4$ (Loads the next word into $d$) |
| **Word** | `.word i` | `iiii iiii iiii iiii iiii iiii iiii iiii` | Directive | Reserves and initializes 4 bytes of memory with the 32-bit value `i`. |

---

## V. Special Memory Mappings (I/O)

These memory addresses trigger I/O operations when accessed.

| Memory Address | Operation | Behavior |
| :--- | :--- | :--- |
| **`0xFFFF000C`** | **Store Word (`sw`)** | Sends the least-significant byte of the word to **standard output**. |
| **`0xFFFF0004`** | **Load Word (`lw`)** | Places the next byte from **standard input** into the least significant byte of the destination register. |