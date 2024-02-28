#include <ctime>
#include <iostream>

#define Width 100
#define Height 100

// to visualize - density of pixels!
char level[] = " .-=coaA@#";
#define level_count (sizeof(level) / sizeof(char)) - 1

float grid[Height][Width] = {0.f};
float ra = 21;
float ri = ra / 3;

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

inline int emod(int a, int b) { return (a % b + b) % b; }

int main() {
  srand(time(0));
  random_grid();

  // points, and m & n
  int cx = 0, cy = 0;
  float m = 0, M = 0;
  float n = 0, N = 0;

  for (int dy = -(ra - 1); dy < (ra - 1); dy++) {
    for (int dx = -(ra - 1); dx < (ra - 1); dx++) {
      int x = emod(cx + dx, Width);
      int y = emod(cy + dy, Height);

      if (dx * dx + dy * dy <= ri * ri) {
        m += grid[y][x];
        M++;
      } else if (dx * dx + dy * dy <= ra * ra) {
        n += grid[y][x];
        N++;
      }
    }
  }
  m /= M;
  n /= N;

  std::cout << "m = " << m << " n = " << n << std::endl;

  // display_grid();

  return 0;
}
