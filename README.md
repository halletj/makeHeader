# makeHeader

Simple program to read any and write a header file containing the contents as a byte array.
The timestamp of the input file and output file are compared, if not changed the output file is left alone.

# Args
```
makeHeader <name> <input-file> <output-file>
```
# Example usage
```
makeHeader mylogo mylogo.png mylogo.h
```
# Example output 
```
#pragma once
#include <cstddef>
const unsigned char mylogo_data[] = {
0x61,0x64,0x64,0x5f,0x65,0x78,0x65,0x63,0x75,0x74,0x61,0x62,0x6c,0x65,0x28,0x6d,
0x61,0x6b,0x65,0x48,0x65,0x61,0x64,0x65,0x72,0x20,0x6d,0x61,0x6b,0x65,0x48,0x65,
0x61,0x64,0x65,0x72,0x2e,0x63,0x70,0x70,0x29,0xa,0x74,0x61,0x72,0x67,0x65,0x74,
0x5f,0x63,0x6f,0x6d,0x70,0x69,0x6c,0x65,0x5f,0x66,0x65,0x61,0x74,0x75,0x72,0x65,
0x73,0x28,0x6d,0x61,0x6b,0x65,0x48,0x65,0x61,0x64,0x65,0x72,0x20,0x50,0x52,0x49,
0x56,0x41,0x54,0x45,0x20,0x63,0x78,0x78,0x5f,0x73,0x74,0x64,0x5f,0x31,0x37,0x29

};
const size_t mylogo_size = 96;
```
