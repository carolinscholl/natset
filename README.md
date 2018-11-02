# natset

Basic functions for a set of natural numbers in C. 
Natural numbers are simply represented as bits in a very memory-efficient way. The n^th bit represents natural number n (starting with 0): if the bit is set to 1, the number is part of the set, if it is 0, it is not. The main data structure for the set is an array of bytes, so each byte can represent 8 natural numbers. 

E.g. the first byte can take numbers ranging from 0 to 7:

| Decimal value  | Bit representation in set |
| -------------- | ------------------------- |
| 0              | 0000 0001                 |
| 1              | 0000 0010                 |
| 2              | 0000 0100                 |
| 3              | 0000 1000                 |
| 4              | 0001 0000                 |
| 5              | 0010 0000                 |
| 6              | 0100 0000                 |
| 7              | 1000 0000                 |

A set containing all these numbers {0, 1, 2, 3, 4, 5, 6, 7} would then be represented by 1111 1111. The next byte in the byte array represents numbers ranging from 8 to 15 and so on and so forth. 

A little demo is included the main function. 

# Usage

Since this is a C project you need to compile main.c to generate an executable: 

`gcc -o main main.c`
