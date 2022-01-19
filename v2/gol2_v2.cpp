#include <iostream>
#include <random>
#include <time.h>
typedef unsigned char ubyte;

//unsigned char* map_1 = new ubyte[100];
//unsigned char* map_2;
bool map_1[10000];
bool map_2[sizeof(map_1)];

bool *map_1_p = map_1;
bool *map_2_p = map_2;

size_t map_width = 100;
size_t map_height = 100;


inline int countAliveCells(size_t x0, size_t x1, size_t x2,
    size_t y0, size_t y1, size_t y2) {
  return map_1_p[x0 + y0] + map_1_p[x1 + y0] + map_1_p[x2 + y0]
    + map_1_p[x0 + y1] + map_1_p[x2 + y1]
    + map_1_p[x0 + y2] + map_1_p[x1 + y2] + map_1_p[x2 + y2];
}
void CalcNextGen() {
  for (size_t y = 0; y < map_height; ++y) {
    size_t y0 = ((y + map_height - 1) % map_height) * map_width;
    size_t y1 = y * map_width;
    size_t y2 = ((y + 1) % map_height) * map_width;
    for (size_t x = 0; x < map_width; ++x) {
      size_t x0 = (x + map_width - 1) % map_width;
      size_t x2 = (x + 1) % map_width;
 
      int aliveCells = countAliveCells(x0, x, x2, y0, y1, y2);
      map_2_p[y1 + x] =
        aliveCells == 3 || (aliveCells == 2 && map_1_p[x + y1]) ? 1 : 0;
    }
  }
  std::swap(map_1_p, map_2_p);

}
int main(){
    for (int x = 0; x < 10000; x++){
      map_1[x] = std::rand() % 2;
    }
    /*for (int x = 0; x < sizeof(map_1); x++){
      std::cout << map_1[x];
      if ((x+1)%map_width == 0){
        std::cout << std::endl;
      }
    }*/
    clock_t tstart = clock();
    for (int x = 0; x < 10000; x++){
      CalcNextGen();
    }
    std::cout << ((double)(clock() - tstart)/CLOCKS_PER_SEC);
      std::cout << "  " << std::endl;
    /*for (int x = 0; x < sizeof(map_1); x++){
      std::cout << map_1[x];
      if ((x+1)%map_width == 0){
        std::cout << std::endl;
      }
    }*/
    return 0;
}