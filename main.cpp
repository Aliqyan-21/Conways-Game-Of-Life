#include <ctime>
#include <iostream>

#define Width 10
#define Height 10

float grid[Height][Width] = {0.f};

inline float rand_float() { return (float)rand() / (float)RAND_MAX; }

int main() {
  srand(time(0));
  for (size_t i = 0; i < Height; i++) {
    for (size_t j = 0; j < Width; j++) {
      grid[i][j] = rand_float();
    }
  }

  for (auto &val : grid) {
    for (float &i : val) {
      std::cout << i << " ";
    }
    std::cout << std::endl;
  }
  return 0;
}
