# Quixo Project for CPSC 490

[Project Proposal](https://docs.google.com/document/d/1R93C6XVNl6pDwXdOTlAuGolf03njDcPjZosSgq-IqNI/edit#)

## Building and Running

### Requirements
- [TCLAP](http://tclap.sourceforge.net/manual.html) for command line argument processing.
  - For MacOS, this can be downloaded from [SourceForge](https://sourceforge.net/projects/tclap/files/), and the `*/include/tclap` folder can be copied to `./include` (which is `.gitignore`-ed).
- [GLFW 3](https://www.glfw.org/) for graphical interface.
  - For MacOS, this can be done simply with `brew install glfw3`.
- [OpenMP](https://www.openmp.org/) for parallelization.
  - For MacOS, this can be done simply with `brew install llvm libomp`.

### Usage
- `make && ./bin/quixo -h` brings up the possible usages.
- Some common usages:
  - Pre-computing:
    - `make purge`: clears any cached data.
    - `./bin/quixo -p opt-compute -l 2`: computes optimal moves for the optimal player for a 2X2 board.
    - `./bin/quixo -p opt-compute -l 2 -m`: does the same as the above, except also checking run-time memory usage.
    - `./bin/quixo -p opt-compute -l 3`: computes optimal moves for the optimal player for a 3X3 board.
    - `./bin/quixo -p opt-compute -l 4`: computes optimal moves for the optimal player for a 4X4 board.
  - Single evaluations for optimal:
    - `./bin/quixo -p opt-check -l 3 -g 800`: computes one optimal move for an interactively provided 3X3 board state.
  - Single games:
    - `./bin/quixo -X interact -g 800`: play interactively as X against a random player O.
    - `./bin/quixo -X interact -g 800 -l 3`: play interactively as X against a random player O on a 3X3 board.
    - `./bin/quixo -X opt -O interact -g 800 -l 3`: play interactively as O against an optimal player X on a 3X3 board.
  - Benchmarks:
    - `./bin/quixo -p test -X opt -n 1000 -l 3`: test optimal player X against random player O for 1000 iterations on a 3X3 board.
    - `./bin/quixo -p test -O opt -n 1000 -l 3`: test optimal player O against random player X for 1000 iterations on a 3X3 board.

## References
- [Quixo Is Solved](https://arxiv.org/abs/2007.15895).
- [Depth in Strategic Games](https://www.semanticscholar.org/paper/Depth-in-Strategic-Games-Lantz-Isaksen/4dedc67aa2191731bf8cf1822d42cea290e73073).
- [Q-learning and MCTS](https://towardsdatascience.com/deep-reinforcement-learning-and-monte-carlo-tree-search-with-connect-4-ba22a4713e7a).

## Notes
- Feb 25, 2021: tested by writing to dummy files in a for-loop, that the Zoo has >200G of storage (100 files of 2G each).
- Running on the Zoo using screen. Use `screen -r` to resume, and `Ctrl-A` then `Ctrl-D` to detach.
- Comparing optimized versions to original versions:
  - `make clean && make purge && make`
  - `./bin/quixo -p opt-compute -l 3 -T 16 && mkdir tmp && cp data/optimal/len3* tmp/`
  - `diff -rq ../data_frozen/3/ tmp/ ; rm -rf tmp`

## To Do / Next Steps
0. **Optimize further for a bit.** Investigate whether finer grain locks or pthreads are faster for 4X4 Quixo.
1. **Run the optimal solution finder for 5X5 Quixo.** Run the full computation for 5X5 Quixo.
2. **Develop other playing agents.** Some ideas include MCTS, Q-learning, some simple human-based heuristic, or a neural network trained on either optimal and/or MCTS.
3. **Evaluate the playing agents.** Against optimal player primarily.
4. **Evaluate depth in Quixo.** Referencing '[Depth in Strategic Games](https://www.semanticscholar.org/paper/Depth-in-Strategic-Games-Lantz-Isaksen/4dedc67aa2191731bf8cf1822d42cea290e73073)', compare the learning rates of non-optimal playing agents between 3X3, 4X4, and 5X5 Quixo. Graph where x-axis is training time or number of training iterations, and y-axis is number of steps to lose OR how much randomization needs to be injected to optimal to make it lose OR percentage of correct moves.
5. _(Stretch)_ **Extensions of Quixo.** As mentioned in '[Quixo Is Solved](https://arxiv.org/abs/2007.15895)', there are extensions of Quixo (such as winning length being different from board length). These can be investigated if time permits.
