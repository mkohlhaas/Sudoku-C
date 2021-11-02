#include <stdio.h>
#include <stdbool.h>

#define N_ROWS        (9)
#define N_COLS        (N_ROWS)
#define N_DIGITS      (N_ROWS)
#define QUADRANT_SIZE (N_ROWS / 3)
#define GRID_SIZE     (N_ROWS * N_ROWS)
#define PRINT_SEP      puts(" -------------------------")

typedef int(Grid)[GRID_SIZE];

void
show(Grid g)
{
  for   (int row = 0; row < N_ROWS; row++) {
    if  (!(row % QUADRANT_SIZE)) PRINT_SEP;
    for (int col = 0; col < N_COLS; col++) {
      *g ? printf(col % QUADRANT_SIZE ? "%2d" : " |%2d", *g)
         : printf(col % QUADRANT_SIZE ? "%2c" : " |%2c", ' ');
      g++;
    }
    printf(" |\n"); 
  }
  PRINT_SEP;
}

bool
all_cells_are_filled(Grid g)
{
  for (int i = 0; i < GRID_SIZE; i++) if (!g[i]) return false;
  return true;
}

int
number_of_filled_cells(Grid g, int pos)
{
  int n_filled_cells = 0;
  int row            = pos / N_ROWS;
  int col            = pos % N_COLS;

  for (int i = 0; i < N_ROWS; i++)                                      // vertical
    if (g[i * N_ROWS + col]) n_filled_cells++;

  for (int j = 0; j < N_COLS; j++)                                      // horizontal
    if (g[row * N_ROWS + j]) n_filled_cells++;

  row = row / QUADRANT_SIZE * QUADRANT_SIZE;                            // calculate beginning of quadrant
  col = col / QUADRANT_SIZE * QUADRANT_SIZE;
  for   (int i = row; i < row + QUADRANT_SIZE; i++)                     // quadrant
    for (int j = col; j < col + QUADRANT_SIZE; j++)
      if (g[i * N_ROWS + j]) n_filled_cells++;

  return n_filled_cells;
}

int
most_promissing_cell(Grid g)
{
  int pos;
  int max_n_cells = -1;
  for (int i = 0; i < GRID_SIZE; i++) {
    if (!g[i]) {
      int n_filled_cells = number_of_filled_cells(g, i);
      if (n_filled_cells > max_n_cells) {
        pos         = i;
        max_n_cells = n_filled_cells;
      }
    }
  }
  return pos;
}

int
collect_used_digits(Grid g, int pos)
{                                                                       //                                                                       987654321
  int used = 0;                                                         // used flags every used digit in horizontal, vertical row, quadrant, eg 001101001 -> 1, 4, 6 and 7
  int row  = pos / N_ROWS;
  int col  = pos % N_COLS;

  for (int i = 0; i < N_ROWS; i++)                                      // vertical
    used |= 1 << (g[i * N_ROWS + col]);

  for (int j = 0; j < N_COLS; j++)                                      // horizontal
    used |= 1 << (g[row * N_ROWS + j]);

  row = row / QUADRANT_SIZE * QUADRANT_SIZE;                            // calculate beginning of quadrant
  col = col / QUADRANT_SIZE * QUADRANT_SIZE;
  for   (int i = row; i < row + QUADRANT_SIZE; i++)                     // quadrant
    for (int j = col; j < col + QUADRANT_SIZE; j++)
      used |= 1 << (g[i * N_ROWS + j]);

  return used >> 1;
}

bool
is_grid_solvable(Grid g)
{
  if (all_cells_are_filled(g)) return true;                             // puzzle is (already) solved; nothing to do
  int pos  = most_promissing_cell(g);
  int used = collect_used_digits(g, pos);
  for (g[pos] = 1; g[pos] <= N_DIGITS; g[pos]++, used >>= 1)            // set all digits from 1 to 9 in cell at pos
    if (!(used & 1) && is_grid_solvable(g)) return true;                // try only when digit not already used 
  g[pos] = 0;                                                           // undo tries and ...
  return false;                                                         // ... return false
}

void
solve(char const* const s)
{
  Grid g;
  for (int i = 0; i < GRID_SIZE; i++) g[i] = s[i] - '0';                // convert input to digits
  show(g);
  if   (is_grid_solvable(g)) show(g);
  else                       puts("no solution");
}

int
main()
{
  solve("000000010"
        "400000000"
        "020000000"
        "000050407"
        "008000300"
        "001090000"
        "300400200"
        "050100000"
        "000806000");
}
