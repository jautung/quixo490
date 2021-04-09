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
- [Eigen 3.4+](http://eigen.tuxfamily.org/) for matrix operations.
  - For MacOS, this can be downloaded from the above link, and the `*/Eigen` folder can be copied to `./include` (which is `.gitignore`-ed).

### Usage
- Initializing: `export OPT_COMPUTE_ERROR_CHECKING=0 ; export OPT_COMPUTE_SPEED_CHECKING=0 ; export OPT_COMPUTE_MEMORY_CHECKING=0 ; make`.
- `./bin/quixo -h` brings up the possible usages.
- Some common usages are included in `./scripts/`.
- Some other usages:
  - Pre-computing optimal moves:
    - `make purge`: clears any cached data.
    - `./bin/quixo -p opt-compute -l 2`: computes optimal moves for a 2X2 board.
    - `./bin/quixo -p opt-compute -l 2 -m`: does the same as the above, except also checking run-time memory usage.
    - `./bin/quixo -p opt-compute -l 3`: computes optimal moves for a 3X3 board.
    - `./bin/quixo -p opt-compute5 -l 4`: computes optimal moves for a 4X4 board, for number of used tiles decreasing from the maximum (i.e. 16), and _stopping_ at 5.
    - `./bin/quixo -p opt-compute -l 5 -T 16 -L 1000`: computes optimal moves for a 5X5 board, using 16 threads, and 1000 thread-locks per result array.
  - Single evaluations for optimal:
    - `./bin/quixo -p opt-check -l 3 -g 800`: checks a one state on a 3X3 board (i.e. whether it is a win, lose, or draw state); state is interactively provided.
    - `./bin/quixo -p opt-check -l 4 -g 800`: checks a one state on a 4X4 board.
    - `./bin/quixo -p opt-check -l 4 -g 800`: checks a one state on a 5X5 board.
  - Single games (default):
    - `./bin/quixo -g 800 -X interact`: play interactively as X against a random player O (default) on a 5X5 board (default).
    - `./bin/quixo -g 800 -l 3 -X interact`: play interactively as X against a random player O (default) on a 3X3 board.
    - `./bin/quixo -g 800 -l 3 -X opt -O interact`: play interactively as O against an optimal player X on a 3X3 board.
    - `./bin/quixo -g 800 -l 4 -X opt -i`: play an optimal player X against a random player O (default) on a 4X4 board, with an interactively provided initial board setting.
    - `./bin/quixo -g 800 -l 4 -X opt -O opt -i`: play optimal players against each other on a 4X4 board, with an interactively provided initial board setting.
    - `./bin/quixo -g 800 -l 5 -X opt -O opt -n 1000`: play optimal players against each other on a 5X5 board, with a turn limit of 1000 (one turn is defined as one move by each player).
  - Benchmarks:
    - `./bin/quixo -p test -l 3 -N 1000 -X opt`: test optimal player X against random player O (default) for 1000 games on a 3X3 board.
    - `./bin/quixo -p test -l 3 -N 1000 -O opt`: test optimal player O against random player X (default) for 1000 games on a 3X3 board.
    - `./bin/quixo -p test -l 4 -N 1000 -X opt -n 1000`: test optimal player X against random player O (default) for 1000 games on a 4X4 board, with a turn limit of 1000 per game.
    - `./bin/quixo -p test -l 4 -N 100 -X heuris-simple -O opt -n 1000`: test simple heuristic player X against optimal player O for 1000 games on a 4X4 board, with a turn limit of 1000 per game.
    - `./bin/quixo -p test -l 4 -N 100 -X mcts0,200 -O opt -n 1000`: test MCTS player X with 0 initial training iterations and 200 training iterations per move against optimal player O for 100 games on a 4X4 board, with a turn limit of 1000 per game.
    - `./bin/quixo -p test -l 4 -N 100 -X q-learn200,0 -O opt -n 1000`: test Q-learning player X with 200 initial training iterations and 0 training iterations per move against optimal player O for 100 games on a 4X4 board, with a turn limit of 1000 per game.
    - `./bin/quixo -p test -l 5 -N 100 -X opt -O opt -n 1000`: test optimal players against each other for 1000 games on a 5X5 board, with a turn limit of 1000 per game.
    - `./bin/quixo -p test -l 5 -N 100 -X random -O opt -n 1000`: test optimal player O against random player X for 1000 games on a 5X5 board, with a turn limit of 1000 per game.
    - `./bin/quixo -p test -l 5 -N 100 -X opt -O random -n 1000`: test optimal player X against random player O for 1000 games on a 5X5 board, with a turn limit of 1000 per game.

## References
- [Quixo Is Solved](https://arxiv.org/abs/2007.15895).
- [Depth in Strategic Games](https://www.semanticscholar.org/paper/Depth-in-Strategic-Games-Lantz-Isaksen/4dedc67aa2191731bf8cf1822d42cea290e73073).
- [Q-learning and MCTS](https://towardsdatascience.com/deep-reinforcement-learning-and-monte-carlo-tree-search-with-connect-4-ba22a4713e7a).

## Notes Logs
- Feb 25, 2021: tested by writing to dummy files in a for-loop, that the Zoo has >200G of storage (100 files of 2G each).
  - Running on the Zoo using screen. Use `screen -r` to resume, and `Ctrl-A` then `Ctrl-D` to detach.
- Mar 7, 2021: Full computation for 5X5 Quixo has been running on the Zoo (via a `screen`) for \~24 hours.
  - Because of the saving of intermediate results, we can always stop the computation, optimize, further, and start it again.
  - Basic heuristic agent has been built.
  - Basic MCTS has been built using a UCB tree policy and a random default policy.
- Mar 30, 2021: Speed profiling for 4X4 optimal computation.
  - I think the takeaway here is that the locks are far too coarse-grained; waiting for locks takes far too long. We need finer locks.
- Apr 1, 2021: More speed profiling for 4X4 optimal computation.
  - Locks still seem slow, tried implementing reader-writer lock using [Raynal's method](https://en.wikipedia.org/wiki/Readers%E2%80%93writer_lock); speed decreased significantly (from 3:24.49 to 4:46.10), so code was reverted to the simple lock. I suppose the overhead of using two locks per lock is too high...

## To Do / Next Steps
1. **Analysis/improvement of MCTS/Q-learning.**
   - Sanity check that MCTS/Q-learning is doing the right thing. Use them on another game and seeing whether it works (e.g. tic tac toe).
   - Get other features/heuristics.
   - Look at distribution of game lengths to determine '-n'.
   - Getting Q-learning to learn by playing against MCTS/optimal.
   - Test Q-learning and MCTS on positions that are a couple moves in, so that Q-learning needs to only train once at `initState=0b0` and then evaluated from different positions.
   - Check how often states are reused for MCTS.
2. **Evaluate the playing agents.** Against optimal player primarily.
3. **Evaluate depth in Quixo.** Referencing '[Depth in Strategic Games](https://www.semanticscholar.org/paper/Depth-in-Strategic-Games-Lantz-Isaksen/4dedc67aa2191731bf8cf1822d42cea290e73073)', compare the learning rates of non-optimal playing agents between 3X3, 4X4, and 5X5 Quixo. Graph where x-axis is training time or number of training iterations, and y-axis is number of steps to lose OR how much randomization needs to be injected to optimal to make it lose OR percentage of correct moves.
4. _(Stretch)_ **Extensions of Quixo.** As mentioned in '[Quixo Is Solved](https://arxiv.org/abs/2007.15895)', there are extensions of Quixo (such as winning length being different from board length). These can be investigated if time permits.
