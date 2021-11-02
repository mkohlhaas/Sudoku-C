#include <stdio.h>
#include <stdbool.h>

#define N_ROWS        (9)
#define N_COLS        (N_ROWS)
#define N_DIGITS      (N_ROWS)
#define QUADRANT_SIZE (N_ROWS / 3)
#define GRID_SIZE     (N_ROWS * N_ROWS)
#define PRINT_SEP      puts(" -------------------------")

typedef int Position;
typedef int(Grid)[GRID_SIZE];

void
show_grid(Grid g)
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

void
print_grid(Grid g)
{
  for (Position pos = 0; pos < GRID_SIZE; pos++) printf("%d", g[pos]);
  putchar('\n');
}

bool
all_cells_are_filled(Grid g)
{
  for (Position pos = 0; pos < GRID_SIZE; pos++) if (!g[pos]) return false;
  return true;
}

int
number_of_filled_cells(Grid g, Position pos)
{
  int n_filled_cells = 0;
  int row            = pos / N_ROWS;
  int col            = pos % N_COLS;

  for (int i = 0; i < N_ROWS; i++)                                        // vertical
    if (g[i * N_ROWS + col]) n_filled_cells++;

  for (int j = 0; j < N_COLS; j++)                                        // horizontal
    if (g[row * N_ROWS + j]) n_filled_cells++;

  row = row / QUADRANT_SIZE * QUADRANT_SIZE;                              // calculate beginning of quadrant
  col = col / QUADRANT_SIZE * QUADRANT_SIZE;
  for   (int i = row; i < row + QUADRANT_SIZE; i++)                       // quadrant
    for (int j = col; j < col + QUADRANT_SIZE; j++)
      if (g[i * N_ROWS + j]) n_filled_cells++;

  return n_filled_cells;
}

Position
most_promissing_cell(Grid g)
{
  Position res;
  int max_n_cells = -1;
  for (Position pos = 0; pos < GRID_SIZE; pos++) {
    if (!g[pos]) {
      int n_filled_cells = number_of_filled_cells(g, pos);
      if (n_filled_cells > max_n_cells) {
        res         = pos;
        max_n_cells = n_filled_cells;
      }
    }
  }
  return res;
}

int
used_digits(Grid g, int pos)
{                                                                         //                                                                       987654321
  int used = 0;                                                           // used flags every used digit in horizontal, vertical row, quadrant, eg 001101001 -> 1, 4, 6 and 7
  int row  = pos / N_ROWS;
  int col  = pos % N_COLS;

  for (int i = 0; i < N_ROWS; i++)                                        // vertical
    used |= 1 << (g[i * N_ROWS + col]);

  for (int j = 0; j < N_COLS; j++)                                        // horizontal
    used |= 1 << (g[row * N_ROWS + j]);

  row = row / QUADRANT_SIZE * QUADRANT_SIZE;                              // calculate beginning of quadrant
  col = col / QUADRANT_SIZE * QUADRANT_SIZE;
  for   (int i = row; i < row + QUADRANT_SIZE; i++)                       // quadrant
    for (int j = col; j < col + QUADRANT_SIZE; j++)
      used |= 1 << (g[i * N_ROWS + j]);

  return used >> 1;
}

bool
is_grid_solvable(Grid g)
{
  if (all_cells_are_filled(g)) return true;                               // puzzle is (already) solved; nothing to do
  Position pos  = most_promissing_cell(g);
  int      used = used_digits(g, pos);
  for (g[pos] = 1; g[pos] <= N_DIGITS; g[pos]++, used >>= 1)              // set all digits from 1 to 9 in cell at pos
    if (!(used & 1) && is_grid_solvable(g)) return true;                  // try only when digit not already used 
  g[pos] = 0;                                                             // undo tries and ...
  return false;                                                           // ... return false
}

void
solve(char const* const s)
{
  Grid g;
  for (Position pos = 0; pos < GRID_SIZE; pos++) g[pos] = s[pos] - '0';   // convert input to digits
  printf("%s\n", s);
  if   (is_grid_solvable(g)) print_grid(g);
  else                       puts("no solution");
}

int
main(int argc, char** argv)
{
  if   (argc != 2) printf("Please provide a sudoku puzzle, e.g.\n%s 000000010400000000020000000000050407008000300001090000300400200050100000000806000\n", argv[0]);
  else             solve(argv[1]);
}
