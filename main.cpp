#include <cmath>
#include <ctime>
#include <iomanip>
#include <ios>
#include <iostream>

#define Width 20
#define Height 20

// to visualize - density of pixels!
char level[] = " .-=coaA@#";
#define level_count (sizeof(level) / sizeof(char)) - 1

// initialization of parameters mentioned in the paper
float grid[Height][Width] = {0.f};
float grid_diff[Height][Width] = {0.f};
float ra = 21;
float ri = ra / 3;
float alpha = 0.028;
// float alpha = 0.0147;
float b1 = 0.278, b2 = 0.365;
float d1 = 0.267, d2 = 0.445;
float dt = 0.01f;

inline float rand_float() { return (float)rand() / (float)RAND_MAX; }

void random_grid() {
  for (size_t i = 0; i < Height; i++) {
    for (size_t j = 0; j < Width; j++) {
      grid[i][j] = rand_float();
    }
  }
}

void display_grid(float grid[Width][Height]) {
  for (size_t i = 0; i < Height; i++) {
    for (size_t j = 0; j < Width; j++) {
      char ch = level[(int)(grid[i][j] * ((int)level_count - 1))];
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

// computing the diff of grid for each point
void compute_grid_diff() {
  for (int cx = 0; cx < Height; cx++) {
    for (int cy = 0; cy < Width; cy++) {
      // points, and m & n
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
      // normalize value of s(n, m) in range -1 to 1 instead of 0-1
      float q = s(n, m);
      grid_diff[cx][cy] = 2 * q - 1;
    }
  }
}

void clamp(float *x, float l, float h) {
  if (*x < l)
    *x = l;
  if (*x > h)
    *x = h;
}

int main() {
  srand(time(0));
  random_grid();

  display_grid(grid);

  for (size_t it = 0; it < 10; it++) {
    compute_grid_diff();
    // apply grid diff
    for (size_t i = 0; i < Height; i++) {
      for (size_t j = 0; j < Width; j++) {
        grid[i][j] += dt * grid_diff[i][j];
        clamp(&grid[i][j], 0, 1);
      }
    }
    display_grid(grid);
  }

  return 0;
}
