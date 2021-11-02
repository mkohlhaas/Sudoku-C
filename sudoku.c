#include <stdio.h>
#include <stdbool.h>

#define N_ROWS        (9)
#define N_COLS        (N_ROWS)
#define N_DIGITS      (N_ROWS)
#define QUADRANT_SIZE (N_ROWS / 3)
#define GRID_SIZE     (N_ROWS * N_ROWS)

typedef int(Grid)[GRID_SIZE];

void
show(Grid x)
{
  for   (int row = 0; row < N_ROWS; row++) {
    if  (!(row % QUADRANT_SIZE)) puts(" -------------------------");
    for (int col = 0; col < N_COLS; col++) {
      if   (*x == 0) { printf(col % QUADRANT_SIZE ? "%2c" : " |%2c", ' '); x++; }
      else           { printf(col % QUADRANT_SIZE ? "%2d" : " |%2d",  *x); x++; }
    }
    printf(" |\n"); 
  }
  puts(" -------------------------");
}

// returns true if we can fill cells at pos and all
// following cells or the whole grid is filled
bool
trycell(Grid x, int pos)
{
  int row  = pos / N_ROWS;
  int col  = pos % N_COLS;
  int used = 0;                                                      // used will flag every used digit in horizontal, vertical row or quadrant, e.g. 001101001 -> 1, 4, 6 and 7.

  if (pos == GRID_SIZE) return true;                                 // we filled the whole grid
  if (x[pos])           return trycell(x, pos + 1);                  // cell is already filled; try next cell

  {
    // pos is empty
    // now collect already used digits; result will be in used
    for (int i = 0; i < N_ROWS; i++)
      used |= 1 << (x[i * N_ROWS + col] - 1);                        // check vertical

    for (int j = 0; j < N_COLS; j++)
      used |= 1 << (x[row * N_ROWS + j] - 1);                        // check horizontal

    row = row / QUADRANT_SIZE * QUADRANT_SIZE;                       // calculate beginning of quadrant
    col = col / QUADRANT_SIZE * QUADRANT_SIZE;
    for   (int i = row; i < row + QUADRANT_SIZE; i++)
      for (int j = col; j < col + QUADRANT_SIZE; j++)
        used |= 1 << (x[i * N_ROWS + j] - 1);                        // check quadrant
  }

  for (x[pos] = 1; x[pos] <= N_DIGITS; x[pos]++, used >>= 1)         // set all digits from 1 to 9 in cell at pos
    if (!(used & 1) && trycell(x, pos + 1)) return true;             // only try when digit not already used 

  x[pos] = 0;                                                        // undo try and ...
  return false;                                                      // ... return false
}

void
solve(const char* s)
{
  Grid x;
  for (int i = 0; i < GRID_SIZE; i++) x[i] = s[i] - '0';             // convert input to digits

  show(x);
  if   (trycell(x, 0)) show(x);
  else                 puts("no solution");
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
