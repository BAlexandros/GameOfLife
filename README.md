## Game of Life

Citing wikipedia, _Game of Life_ is a cellular automation devised by John Conway
(1970).

In the simulation each point of a 2D grid represents a cell.  Each cell can have
two states: dead or alive.  The state of a cell depends on its neighbours, by
the following rules:
1. If a cell has less than two live neighbours, it dies due to underpopulation
2. If a cell has two or three live neighbours, it remains alive
3. If a cell has three live neighbours, it comes to life
4. If a cell has more than three live neighbours, it dies due to overpopulation

The algorithm is implemented in C++, and the simulation is presented in
the terminal.
