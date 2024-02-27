#include <ctime>
#include <iostream>

#define Width 100
#define Height 100

// to visualize - density of pixels!
char level[] = " .-=coaA@#";
#define level_count (sizeof(level) / sizeof(char)) - 1

float grid[Height][Width] = {0.f};
float ra = 21;

inline float rand_float() { return (float)rand() / (float)RAND_MAX; }

void random_grid() {
  for (size_t i = 0; i < Height; i++) {
    for (size_t j = 0; j < Width; j++) {
      grid[i][j] = rand_float();
    }
  }
}

void display_grid() {
  for (auto &val : grid) {
    for (float &i : val) {
      char ch = level[(int)(i * ((int)level_count - 1))];
      std::cout << ch;
    }
    std::cout << std::endl;
  }
}

int main() {
  srand(time(0));
  random_grid();
  display_grid();

  return 0;
}
