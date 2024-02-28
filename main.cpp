#include <cmath>
#include <ctime>
#include <iostream>

#define Width 100
#define Height 100

// to visualize - density of pixels!
char level[] = " .-=coaA@#";
#define level_count (sizeof(level) / sizeof(char)) - 1

// initialization of parameters mentioned in the paper
float grid[Height][Width] = {0.f};
float ra = 21;
float ri = ra / 3;
float alpha = 0.028;
// float alpha = 0.0147;
float b1 = 0.278, b2 = 0.365;
float d1 = 0.267, d2 = 0.445;

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

// declaration of the functions mentioned in the paper
float sigma1(float x, float a) {
  return 1.f / (1.f + expf(-(x - a) * 4 / alpha));
}

float sigma2(float x, float a, float b) {
  return sigma1(x, a) * (1 - sigma1(x, b));
}

float sigma_m(float x, float y, float m) {
  return x * (1 - sigma1(m, 0.5f)) + y * sigma1(m, 0.5f);
}

// transition function mentioned in the paper
float s(float n, float m) {
  return sigma2(n, sigma_m(b1, d1, m), sigma_m(b2, d2, m));
}

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

  std::cout << "s(n, m) = " << s(n, m) << std::endl;

  // display_grid();

  return 0;
}
