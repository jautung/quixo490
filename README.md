# Quixo Project for CPSC 490

## Building and Running

### Requirements
- [TCLAP](http://tclap.sourceforge.net/manual.html) for command line argument processing.
  - For MacOS, this can be downloaded from [SourceForge](https://sourceforge.net/projects/tclap/files/), and the `*/include/tclap` folder can be copied to `./include` (which is `.gitignore`-ed).
- [GLFW 3](https://www.glfw.org/) for graphical interface.
  - For MacOS, this can be done simply with `brew install glfw3`.

### Usage
- `make && ./bin/quixo -h` brings up the possible usages.
- Some common usages:
  - `make purge`: clears any cached data.
  - `./bin/quixo -p opt-compute -l 2`: pre-computes optimal moves for the optimal player for a 2X2 board.
  - `./bin/quixo -p opt-compute -l 3`: pre-computes optimal moves for the optimal player for a 3X3 board.
  - `./bin/quixo -p opt-compute -l 4`: pre-computes optimal moves for the optimal player for a 4X4 board.
  - `./bin/quixo -X interact -g 800`: play interactively as X against a random player O.
  - `./bin/quixo -X interact -g 800 -l 3`: play interactively as X against a random player O on a 3X3 board.
  - `./bin/quixo -X opt -O interact -g 800 -l 3`: play interactively as O against an optimal player X on a 3X3 board.
  - `./bin/quixo -p test -X opt -n 1000 -l 3`: test optimal player X against random player O for 1000 iterations on a 3X3 board.
  - `./bin/quixo -p test -O opt -n 1000 -l 3`: test optimal player O against random player X for 1000 iterations on a 3X3 board.

## References
- [Quixo Is Solved](https://arxiv.org/abs/2007.15895).
- [Depth in Strategic Games](https://www.semanticscholar.org/paper/Depth-in-Strategic-Games-Lantz-Isaksen/4dedc67aa2191731bf8cf1822d42cea290e73073).
