Factors:
- Implemented guardbands
  Can be found at Engine\Source\Memory\Private\FixedSizeAllocator.cpp, line 264

- Each FixedSizeAllocator uses only ONE allocation to create itself, its blocks and its BitArray.
  Can be found at Engine\Source\Memory\Private\FixedSizeAllocator.cpp, line 68

- Elegantly handled 32-bit, 64-bit compatability with the BitArray class.
  Only ONE platform specific #ifdef exists, to decide between _BitScanForward & _BitScanForward64.
  Can be found at Engine\Source\Data\Private\BitArray.cpp, lines 126 & 161.
  
- Enforced detailed & informative debug logging throughout engine, tests & game code
  
- Header inclusions are separated into categories (library, engine, game) and arranged alphabetically

- Commented liberally