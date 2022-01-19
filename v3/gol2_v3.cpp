#include <iostream>
#include <random>
#include <time.h>
#include <bitset>
typedef unsigned char ubyte;



size_t const map_width = 16;
size_t const map_height = 128;
size_t bitWidth = 8;

uint8_t map_1[map_width*map_height];
uint8_t map_2[sizeof(map_1)];

uint8_t *map_1_p = map_1;
uint8_t *map_2_p = map_2;

uint64_t above;
uint64_t same;
uint32_t below;
void init(){
  int y = 0;
  int x = 0;
    size_t y0 = ((y + map_height - 1) % map_height) * map_width;
    size_t y1 = y * map_width;
    size_t y2 = ((y + 1) % map_height) * map_width;
    size_t x0 = (x + map_width - 1) % map_width;
    size_t x2 = (x + 1) % map_width;
    above += map_1_p[y0 + x0] << 16;
    above += map_1_p[y0 + x] << 8;
    //above += map_1_p[y0 + x2];
    same += map_1_p[y1 + x0] << 16;
    same += map_1_p[y1 + x] << 8;
    //same += map_1_p[y1 + x2];
    below += map_1_p[y2 + x0] << 16;
    below += map_1_p[y2 + x] << 8;
    //below += map_1_p[y2 + x2];

    /*std::cout << y0 << std::endl;
    std::cout << y1 << std::endl;
    std::cout << y2 <<std::endl;
    std::cout << x0 <<std::endl;
    std::cout << x2 <<std::endl;

    std::cout << "00000000000000001000000000000000" << std::endl;
    
    std::cout << std::bitset<32>(above) << std::endl;
    std::cout << std::bitset<32>(same) << std::endl;
    std::cout << std::bitset<32>(below) << std::endl;
    std::cout << std::endl;*/
    
}
inline int countAliveCells() {
  /*std::cout << (above & 0x10000) << (above & 0x8000) << (above & 0x4000) <<std::endl;
  std::cout << (same & 0x10000)  << (same & 0x4000) <<std::endl;
  std::cout << (below & 0x10000) << (below & 0x8000) << (below & 0x4000) <<std::endl;
  std::cout << std::endl;*/
  return  ((above & 0x40) + (same & 0x40) + (below & 0x40)>> 6)
       + ((above & 0x80) + (below & 0x80) >> 7)
       + ((above & 0x100) + (same & 0x100) + (below & 0x100) >> 8);
}
void CalcNextGen() {
  for (size_t y = 0; y < map_height; ++y) {
    for (size_t x = 0; x < map_width; ++x) {
      int result = 0;
      size_t x0 = (x + map_width - 1) % map_width;
      size_t x2 = (x + 1) % map_width;
      size_t y0 = ((y + map_height - 1) % map_height) * map_width;
      size_t y1 = y * map_width;
      size_t y2 = ((y + 1) % map_height) * map_width;

      //std::cout << y0 << " " << y1 << " " << y2 << " " << x0 << " " << x2 <<std::endl;

      above += map_1_p[y0 + x2];
      same += map_1_p[y1 + x2];
      below += map_1_p[y2 + x2];

      /*std::cout << std::bitset<8>(above) << std::endl;
      std::cout << std::bitset<8>(same) << std::endl;
      std::cout << std::bitset<8>(below) << std::endl;
      std::cout << std::endl;*/

      for (size_t b = 0; b < bitWidth; b++){
        int aliveCells = countAliveCells();
        //std::cout << std::bitset<8>(map_1[y]);
        /*std::cout << aliveCells;
        if ((b + 1)%8 == 0){
        std::cout << std::endl;
        }*/
      
        above <<= 1;
        same <<= 1;
        below <<= 1;
        result = result << 1
          | (aliveCells == 3 || (aliveCells == 2 && (same & 0x100)) ? 1u : 0u);
          //std::cout << ((same & 0x100)) << " " << y << x << b <<std::endl;
      }
      map_2_p[y1 + x0] = result;

    }
  }
  std::swap(map_1, map_2);
}
int main(){
  
    for (int x = 0; x < map_width* map_height; x++){
      map_1[x] = std::rand() % 0xFF;
    }
    init();
    for (int x = 0; x < sizeof(map_1); x++){
      std::cout << std::bitset<8>(map_1[x]);
      if ((x+1)%map_width == 0){
        std::cout << std::endl;
      }
    }
    std::cout << std::endl;
    clock_t tstart = clock();
    for (int x = 0; x < 10000; x++){
      CalcNextGen();
    }
    std::cout << ((double)(clock() - tstart)/CLOCKS_PER_SEC);
      std::cout << "  " << std::endl;
    for (int x = 0; x < sizeof(map_1); x++){
      std::cout << std::bitset<8>(map_1[x]);
      if ((x+1)%map_width == 0){
        std::cout << std::endl;
      }
    }
    return 0;
}
