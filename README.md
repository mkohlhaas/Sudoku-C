Batch solve the most difficult Sudoku puzzles in the world:
```shell
$ parallel --eta ./sudoku :::: sudoku17.txt > res.txt
$ tail -f res.txt  # in another shell
```
