
================================================================================
MERL File Visualization: m1.merl
================================================================================
Format: Simplified
Code Size: 108 bytes
Total Size: 120 bytes
Has Relocation Records: False
================================================================================
Offset   Hex Value    Type     Description          Assembly                      
----------------------------------------------------------------------------------
00000000 0x10000002   Header   MERL Magic Number   
00000004 0x00000068   Header   Code Size           
00000008 0x00000034   Header   End of Code         
0000000C 0xAFDFFFFC   Code     MIPS Instruction     sw $31, -4($30)               
00000010 0x0000E814   Code     MIPS Instruction     lis $29                       
00000014 0x00000000   Data     Data Word            .word 0x00000000              
00000018 0x03A00009   Code     MIPS Instruction     jalr $29                      
0000001C 0x00001814   Code     MIPS Instruction     lis $3                        
00000020 0x00000030   Data     Data Word            .word 0x00000030              
00000024 0x8C630000   Code     MIPS Instruction     lw $3, 0($3)                  
00000028 0x8FDFFFFC   Code     MIPS Instruction     lw $31, -4($30)               
0000002C 0x03E00008   Code     MIPS Instruction     jr $31                        
00000030 0x00000000   Data     Data Word            .word 0x00000000              
00000034 0x00000001   REL      REL Entry            Relocation Entry              
00000038 0x00000008   Data     Data Word            .word 0x00000008              
0000003C 0x00000001   REL      REL Entry            Relocation Entry              
00000040 0x00000014   Data     Data Word            .word 0x00000014              
00000044 0x00000011   ESR      ESR Entry            Export Symbol Record          
00000048 0x00000014   Data     Data Word            .word 0x00000014              
0000004C 0x00000003   Data     Data Word            .word 0x00000003              
00000050 0x00000066   Data     Data Word            .word 0x00000066              
00000054 0x0000006F   Data     Data Word            .word 0x0000006F              
00000058 0x0000006F   Data     Data Word            .word 0x0000006F              
0000005C 0x00000005   ESD      ESD Entry            Export Symbol Definition      
00000060 0x00000030   Data     Data Word            .word 0x00000030              
00000064 0x00000003   Data     Data Word            .word 0x00000003              
00000068 0x00000062   Data     Data Word            .word 0x00000062              
0000006C 0x00000061   Data     Data Word            .word 0x00000061              
00000070 0x00000072   Data     Data Word            .word 0x00000072              
00000074 0x10000001   Footer   MERL End Marker     
