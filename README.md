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
    - `./bin/quixo -X opt -i -g 800 -l 4`: play an optimal player X against a random player O on a 4X4 board, with an initial board setting.
    - `./bin/quixo -X opt -O opt -i -g 800 -l 4`: play optimal players against each other on a 4X4 board, with an initial board setting.
  - Benchmarks:
    - `./bin/quixo -p test -X opt -N 1000 -l 3`: test optimal player X against random player O for 1000 games on a 3X3 board.
    - `./bin/quixo -p test -O opt -N 1000 -l 3`: test optimal player O against random player X for 1000 games on a 3X3 board.
    - `./bin/quixo -p test -X opt -N 1000 -n 1000 -l 4`: test optimal player X against random player O for 1000 games on a 4X4 board, for up to 1000 steps per game.
    - `./bin/quixo -p test -X heuris-simple -O opt -N 100 -n 1000 -l 4`: test simple heuristic player X against optimal player O for 1000 games on a 4X4 board, for up to 1000 steps per game.
    - `./bin/quixo -p test -X mcts0,200 -O opt -N 100 -n 1000 -l 4`: test MCTS player X with 0 initial training iterations and 200 training iterations per move against optimal player O for 100 games on a 4X4 board, for up to 1000 steps per game.
    - `./bin/quixo -p test -X q-learn200,0 -O opt -N 100 -n 1000 -l 4`: test Q-learning player X with 200 initial training iterations and 0 training iterations per move against optimal player O for 100 games on a 4X4 board, for up to 1000 steps per game.

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
- Mar 7, 2021: Full computation for 5X5 Quixo has been running on the Zoo (via a `screen`) for \~24 hours.
  - Because of the saving of intermediate results, we can always stop the computation, optimize, further, and start it again.
  - Basic heuristic agent has been built.
  - Basic MCTS has been built using a UCB tree policy and a random default policy. Some preliminary results:
    - 4X4 Quixo:
    ```
    Player X (random) compute time (s): 0.002134 (2.134e-05 average per game)
    Player O (opt) compute time (s): 0.97568 (0.0097568 average per game)
    Results (X-O-D): 3-97-0
    -----
    Player X (heuris-simple) compute time (s): 0.005328 (5.328e-05 average per game)
    Player O (opt) compute time (s): 0.617372 (0.00617372 average per game)
    Results (X-O-D): 15-85-0
    -----
    Player X (mcts100) compute time (s): 11.6579 (0.116579 average per game)
    Player O (opt) compute time (s): 1.76444 (0.0176444 average per game)
    Results (X-O-D): 10-88-2
    -----
    Player X (mcts200) compute time (s): 29.6239 (0.296239 average per game)
    Player O (opt) compute time (s): 1.39183 (0.0139183 average per game)
    Results (X-O-D): 17-82-1
    -----
    Player X (mcts300) compute time (s): 207.602 (2.07602 average per game)
    Player O (opt) compute time (s): 8.94452 (0.0894452 average per game)
    Results (X-O-D): 14-76-10
    -----
    Player X (opt) compute time (s): 1.28966 (0.0128966 average per game)
    Player O (mcts300) compute time (s): 13.2078 (0.132078 average per game)
    Results (X-O-D): 100-0-0
    ```
    - 3X3 Quixo
    ```
    Player X (random) compute time (s): 0.008348 (8.348e-06 average per game)
    Player O (opt) compute time (s): 0.883411 (0.000883411 average per game)
    Results (X-O-D): 164-836-0
    -----
    Player X (heuris-simple) compute time (s): 0.019172 (1.9172e-05 average per game)
    Player O (opt) compute time (s): 0.346575 (0.000346575 average per game)
    Results (X-O-D): 662-338-0
    -----
    Player X (mcts100) compute time (s): 13.093 (0.013093 average per game)
    Player O (opt) compute time (s): 0.85122 (0.00085122 average per game)
    Results (X-O-D): 672-328-0
    -----
    Player X (mcts200) compute time (s): 27.3816 (0.0273816 average per game)
    Player O (opt) compute time (s): 0.695197 (0.000695197 average per game)
    Results (X-O-D): 719-281-0
    -----
    Player X (mcts300) compute time (s): 38.247 (0.038247 average per game)
    Player O (opt) compute time (s): 0.539847 (0.000539847 average per game)
    Results (X-O-D): 798-202-0
    -----
    Player X (opt) compute time (s): 2.40705 (0.00240705 average per game)
    Player O (mcts300) compute time (s): 35.4347 (0.0354347 average per game)
    Results (X-O-D): 1000-0-0
    -----
    Player X (mcts300) compute time (s): 36.0902 (0.0360902 average per game)
    Player O (heuris-simple) compute time (s): 0.010989 (1.0989e-05 average per game)
    Results (X-O-D): 991-9-0
    -----
    Player X (heuris-simple) compute time (s): 0.018869 (1.8869e-05 average per game)
    Player O (mcts300) compute time (s): 29.8628 (0.0298628 average per game)
    Results (X-O-D): 456-544-0
    ```

## To Do / Next Steps
0. **Improve speed of optimal computation.**
   - `gprof`
   - `valgrind callgrind`
   - `-l 4 -T 1 -s`:
     ```
      Total initClass() time (s)            : 0.022844
       ↳ Thread 0 total task time (s)       : 0.022618 (153 tasks)
      Total checkTerminalsClass() time (s)  : 20.7791
       ↳ Thread 0 total task time (s)       : 10.02 (43046721 tasks)
         ↳ Total time in locks (s)          : 0.006086
      Total parentLinkCacheClass() time (s) : 46.0704
       ↳ Thread 0 total task time (s)       : 36.0588 (16612232 tasks)
         ↳ Total time in locks (s)          : 0.012314
      Total valueIterateClass() time (s)    : 58.2625
       ↳ Thread 0 total task time (s)       : 51.6136 (10705288 tasks)
         ↳ Total time in locks (s)          : 0.013543
      Total elimWinOrDrawClass() time (s)   : 0.067547
       ↳ Thread 0 total task time (s)       : 0.067483 (153 tasks)
     ```
   - `-l 4 -T 2 -s`:
     ```
      Total initClass() time (s)            : 0.029857
       ↳ Thread 0 total task time (s)       : 0.02927 (75 tasks)
       ↳ Thread 1 total task time (s)       : 0.026729 (78 tasks)
      Total checkTerminalsClass() time (s)  : 20.4918
       ↳ Thread 0 total task time (s)       : 6.98051 (29372052 tasks)
         ↳ Total time in locks (s)          : 0.003359
       ↳ Thread 1 total task time (s)       : 6.55821 (13674669 tasks)
         ↳ Total time in locks (s)          : 0.003031
      Total parentLinkCacheClass() time (s) : 34.4449
       ↳ Thread 0 total task time (s)       : 26.0258 (8789836 tasks)
         ↳ Total time in locks (s)          : 0.011005
       ↳ Thread 1 total task time (s)       : 24.2516 (7822396 tasks)
         ↳ Total time in locks (s)          : 0.009767
      Total valueIterateClass() time (s)    : 39.1571
       ↳ Thread 0 total task time (s)       : 33.7249 (5400747 tasks)
         ↳ Total time in locks (s)          : 0.011826
       ↳ Thread 1 total task time (s)       : 32.1874 (5298051 tasks)
         ↳ Total time in locks (s)          : 0.011251
      Total elimWinOrDrawClass() time (s)   : 0.044086
       ↳ Thread 0 total task time (s)       : 0.02785 (76 tasks)
       ↳ Thread 1 total task time (s)       : 0.039172 (77 tasks)
     ```
   - `-l 4 -T 4 -s`:
     ```
      Total initClass() time (s)            : 0.029555
       ↳ Thread 0 total task time (s)       : 0.013131 (42 tasks)
       ↳ Thread 1 total task time (s)       : 0.018038 (43 tasks)
       ↳ Thread 2 total task time (s)       : 0.007434 (26 tasks)
       ↳ Thread 3 total task time (s)       : 0.016587 (42 tasks)
      Total checkTerminalsClass() time (s)  : 23.9194
       ↳ Thread 0 total task time (s)       : 5.35768 (4476050 tasks)
         ↳ Total time in locks (s)          : 0.208325
       ↳ Thread 1 total task time (s)       : 6.90439 (12869903 tasks)
         ↳ Total time in locks (s)          : 0.264648
       ↳ Thread 2 total task time (s)       : 6.91162 (12779006 tasks)
         ↳ Total time in locks (s)          : 0.259897
       ↳ Thread 3 total task time (s)       : 6.89694 (12921762 tasks)
         ↳ Total time in locks (s)          : 0.217248
      Total parentLinkCacheClass() time (s) : 38.2504
       ↳ Thread 0 total task time (s)       : 31.7834 (4148042 tasks)
         ↳ Total time in locks (s)          : 1.46793
       ↳ Thread 1 total task time (s)       : 32.24 (4147873 tasks)
         ↳ Total time in locks (s)          : 1.72476
       ↳ Thread 2 total task time (s)       : 32.2761 (4106635 tasks)
         ↳ Total time in locks (s)          : 1.70398
       ↳ Thread 3 total task time (s)       : 32.2349 (4209682 tasks)
         ↳ Total time in locks (s)          : 1.41105
      Total valueIterateClass() time (s)    : 38.7578
       ↳ Thread 0 total task time (s)       : 34.5358 (2624896 tasks)
         ↳ Total time in locks (s)          : 1.93657
       ↳ Thread 1 total task time (s)       : 34.8102 (2685404 tasks)
         ↳ Total time in locks (s)          : 2.25809
       ↳ Thread 2 total task time (s)       : 34.8242 (2666566 tasks)
         ↳ Total time in locks (s)          : 2.25668
       ↳ Thread 3 total task time (s)       : 34.8045 (2728693 tasks)
         ↳ Total time in locks (s)          : 1.91034
      Total elimWinOrDrawClass() time (s)   : 0.044266
       ↳ Thread 0 total task time (s)       : 0.022901 (45 tasks)
       ↳ Thread 1 total task time (s)       : 0.019671 (45 tasks)
       ↳ Thread 2 total task time (s)       : 0.015404 (35 tasks)
       ↳ Thread 3 total task time (s)       : 0.009102 (28 tasks)
     ```
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
