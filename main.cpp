#include <cmath>
#include <cstdio>
#include <ctime>
#include <iostream>

#define Width 100
#define Height 100

// to visualize - density of pixels!
char level[] = " .-=coaA@#";
#define level_count (sizeof(level) / sizeof(char)) - 1

// initialization of parameters mentioned in the paper
float grid[Height][Width] = {0.f};
float grid_diff[Height][Width] = {0.f};
float ra = 11;
float ri = ra / 3;
float alpha_n = 0.028;
float alpha_m = 0.147;
float b1 = 0.278, b2 = 0.365;
float d1 = 0.267, d2 = 0.445;
float dt = 0.03f;

inline float rand_float() { return (float)rand() / (float)RAND_MAX; }

void random_grid() {
  size_t h = Height / 2;
  size_t w = Width / 2;
  for (size_t i = 0; i < h; i++) {
    for (size_t j = 0; j < w; j++) {
      size_t x = i + Height / 2 - h / 2;
      size_t y = j + Width / 2 - w / 2;
      grid[x][y] = rand_float();
    }
  }
}

void display_grid(float grid[Width][Height]) {
  for (size_t i = 0; i < Height; i++) {
    for (size_t j = 0; j < Width; j++) {
      char ch = level[(int)(grid[i][j] * ((int)level_count - 1))];
      fputc(ch, stdout);
      fputc(ch, stdout);
    }
    fputc('\n', stdout);
  }
}

inline int emod(int a, int b) { return (a % b + b) % b; }

// declaration of the functions mentioned in the paper
float sigma(float x, float a, float alpha) {
  return 1.f / (1.f + expf(-(x - a) * 4 / alpha));
}

float sigma_n(float x, float a, float b) {
  return sigma(x, a, alpha_n) * (1 - sigma(x, b, alpha_n));
}

float sigma_m(float x, float y, float m) {
  return x * (1 - sigma(m, 0.5f, alpha_m)) + y * sigma(m, 0.5f, alpha_m);
}

// transition function mentioned in the paper
float s(float n, float m) {
  return sigma_n(n, sigma_m(b1, d1, m), sigma_m(b2, d2, m));
}

// computing the diff of grid for each point
void compute_grid_diff() {
  for (int cy = 0; cy < Height; cy++) {
    for (int cx = 0; cx < Width; cx++) {
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
      grid_diff[cy][cx] = 2 * q - 1;
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

  while (true) {
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
