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
- Mar 30, 2021: Speed profiling for 4X4 optimal computation:
  - `-l 4 -T 1 -s`: (total 129.04s)
    ```
    Total initClass() time (s)            : 0.023
     ↳ Thread 0 total task time (s)       : 0.023 (153 tasks)
    Total checkTerminalsClass() time (s)  : 21.066
     ↳ Thread 0 total task time (s)       : 17.269 (43046721 tasks)
       ↳ Total time in locks (s)          : 3.062
    Total parentLinkCacheClass() time (s) : 47.03
     ↳ Thread 0 total task time (s)       : 45.389 (16612232 tasks)
       ↳ Total time in locks (s)          : 9.776
    Total valueIterateClass() time (s)    : 59.838
     ↳ Thread 0 total task time (s)       : 58.567 (10705288 tasks)
       ↳ Total time in locks (s)          : 10.446
    Total elimWinOrDrawClass() time (s)   : 0.062
     ↳ Thread 0 total task time (s)       : 0.062 (153 tasks)
    ```
  - `-l 4 -T 2 -s`: (total 97.634s)
    ```
    Total initClass() time (s)            : 0.033
     ↳ Thread 0 total task time (s)       : 0.032 (74 tasks)
     ↳ Thread 1 total task time (s)       : 0.029 (79 tasks)
    Total checkTerminalsClass() time (s)  : 20.852
     ↳ Thread 0 total task time (s)       : 11.824 (29410069 tasks)
       ↳ Total time in locks (s)          : 2.941
     ↳ Thread 1 total task time (s)       : 9.527 (13636652 tasks)
       ↳ Total time in locks (s)          : 2.685
    Total parentLinkCacheClass() time (s) : 35.372
     ↳ Thread 0 total task time (s)       : 31.384 (8814518 tasks)
       ↳ Total time in locks (s)          : 10.248
     ↳ Thread 1 total task time (s)       : 28.998 (7797714 tasks)
       ↳ Total time in locks (s)          : 9.643
    Total valueIterateClass() time (s)    : 40.442
     ↳ Thread 0 total task time (s)       : 37.755 (5425024 tasks)
       ↳ Total time in locks (s)          : 11.024
     ↳ Thread 1 total task time (s)       : 36.085 (5280348 tasks)
       ↳ Total time in locks (s)          : 10.477
    Total elimWinOrDrawClass() time (s)   : 0.041
     ↳ Thread 0 total task time (s)       : 0.028 (75 tasks)
     ↳ Thread 1 total task time (s)       : 0.034 (78 tasks)
    ```
  - `-l 4 -T 4 -s`: (total 103.186s)
    ```
    Total initClass() time (s)            : 0.029
     ↳ Thread 0 total task time (s)       : 0.007 (38 tasks)
     ↳ Thread 1 total task time (s)       : 0.014 (42 tasks)
     ↳ Thread 2 total task time (s)       : 0.016 (33 tasks)
     ↳ Thread 3 total task time (s)       : 0.016 (40 tasks)
    Total checkTerminalsClass() time (s)  : 23.656
     ↳ Thread 0 total task time (s)       : 6.509 (4695482 tasks)
       ↳ Total time in locks (s)          : 3.673
     ↳ Thread 1 total task time (s)       : 9.498 (12621992 tasks)
       ↳ Total time in locks (s)          : 4.817
     ↳ Thread 2 total task time (s)       : 9.541 (12888262 tasks)
       ↳ Total time in locks (s)          : 4.825
     ↳ Thread 3 total task time (s)       : 9.521 (12840985 tasks)
       ↳ Total time in locks (s)          : 4.83
    Total parentLinkCacheClass() time (s) : 38.774
     ↳ Thread 0 total task time (s)       : 34.426 (4117612 tasks)
       ↳ Total time in locks (s)          : 23.494
     ↳ Thread 1 total task time (s)       : 34.986 (4129839 tasks)
       ↳ Total time in locks (s)          : 24.08
     ↳ Thread 2 total task time (s)       : 35.066 (4175860 tasks)
       ↳ Total time in locks (s)          : 24.27
     ↳ Thread 3 total task time (s)       : 35.067 (4188921 tasks)
       ↳ Total time in locks (s)          : 24.264
    Total valueIterateClass() time (s)    : 39.768
     ↳ Thread 0 total task time (s)       : 36.866 (2604869 tasks)
       ↳ Total time in locks (s)          : 22.795
     ↳ Thread 1 total task time (s)       : 37.245 (2670452 tasks)
       ↳ Total time in locks (s)          : 23.305
     ↳ Thread 2 total task time (s)       : 37.305 (2712424 tasks)
       ↳ Total time in locks (s)          : 23.587
     ↳ Thread 3 total task time (s)       : 37.298 (2717775 tasks)
       ↳ Total time in locks (s)          : 23.595
    Total elimWinOrDrawClass() time (s)   : 0.041
     ↳ Thread 0 total task time (s)       : 0.013 (33 tasks)
     ↳ Thread 1 total task time (s)       : 0.018 (37 tasks)
     ↳ Thread 2 total task time (s)       : 0.021 (46 tasks)
     ↳ Thread 3 total task time (s)       : 0.009 (37 tasks)
    ```
  - `-l 4 -T 8 -s`: (total 135.372s)
    ```
    Total initClass() time (s)            : 0.029
     ↳ Thread 0 total task time (s)       : 0.008 (39 tasks)
     ↳ Thread 1 total task time (s)       : 0.009 (21 tasks)
     ↳ Thread 2 total task time (s)       : 0.016 (20 tasks)
     ↳ Thread 3 total task time (s)       : 0.007 (19 tasks)
     ↳ Thread 4 total task time (s)       : 0.002 (11 tasks)
     ↳ Thread 5 total task time (s)       : 0.004 (13 tasks)
     ↳ Thread 6 total task time (s)       : 0.001 (13 tasks)
     ↳ Thread 7 total task time (s)       : 0.005 (17 tasks)
    Total checkTerminalsClass() time (s)  : 40.416
     ↳ Thread 0 total task time (s)       : 6.88 (1709301 tasks)
       ↳ Total time in locks (s)          : 5.682
     ↳ Thread 1 total task time (s)       : 10.327 (5798453 tasks)
       ↳ Total time in locks (s)          : 8.077
     ↳ Thread 2 total task time (s)       : 10.332 (5821739 tasks)
       ↳ Total time in locks (s)          : 8.068
     ↳ Thread 3 total task time (s)       : 10.327 (5892344 tasks)
       ↳ Total time in locks (s)          : 8.068
     ↳ Thread 4 total task time (s)       : 10.391 (6005142 tasks)
       ↳ Total time in locks (s)          : 8.09
     ↳ Thread 5 total task time (s)       : 10.372 (5938756 tasks)
       ↳ Total time in locks (s)          : 8.048
     ↳ Thread 6 total task time (s)       : 10.384 (5976985 tasks)
       ↳ Total time in locks (s)          : 8.026
     ↳ Thread 7 total task time (s)       : 10.397 (5904001 tasks)
       ↳ Total time in locks (s)          : 8.051
    Total parentLinkCacheClass() time (s) : 45.789
     ↳ Thread 0 total task time (s)       : 41.727 (2043129 tasks)
       ↳ Total time in locks (s)          : 36.123
     ↳ Thread 1 total task time (s)       : 43.845 (2082672 tasks)
       ↳ Total time in locks (s)          : 38.414
     ↳ Thread 2 total task time (s)       : 43.826 (2080037 tasks)
       ↳ Total time in locks (s)          : 38.372
     ↳ Thread 3 total task time (s)       : 43.872 (2081285 tasks)
       ↳ Total time in locks (s)          : 38.472
     ↳ Thread 4 total task time (s)       : 43.907 (2087434 tasks)
       ↳ Total time in locks (s)          : 38.467
     ↳ Thread 5 total task time (s)       : 43.898 (2082923 tasks)
       ↳ Total time in locks (s)          : 38.215
     ↳ Thread 6 total task time (s)       : 43.882 (2079813 tasks)
       ↳ Total time in locks (s)          : 38.187
     ↳ Thread 7 total task time (s)       : 43.893 (2074939 tasks)
       ↳ Total time in locks (s)          : 38.125
    Total valueIterateClass() time (s)    : 48.234
     ↳ Thread 0 total task time (s)       : 45.528 (1303856 tasks)
       ↳ Total time in locks (s)          : 38.139
     ↳ Thread 1 total task time (s)       : 46.913 (1345367 tasks)
       ↳ Total time in locks (s)          : 39.803
     ↳ Thread 2 total task time (s)       : 46.901 (1342790 tasks)
       ↳ Total time in locks (s)          : 39.777
     ↳ Thread 3 total task time (s)       : 46.926 (1344105 tasks)
       ↳ Total time in locks (s)          : 39.895
     ↳ Thread 4 total task time (s)       : 46.949 (1347847 tasks)
       ↳ Total time in locks (s)          : 39.913
     ↳ Thread 5 total task time (s)       : 46.948 (1344009 tasks)
       ↳ Total time in locks (s)          : 39.606
     ↳ Thread 6 total task time (s)       : 46.942 (1342622 tasks)
       ↳ Total time in locks (s)          : 39.62
     ↳ Thread 7 total task time (s)       : 46.946 (1337001 tasks)
       ↳ Total time in locks (s)          : 39.561
    Total elimWinOrDrawClass() time (s)   : 0.041
     ↳ Thread 0 total task time (s)       : 0.008 (20 tasks)
     ↳ Thread 1 total task time (s)       : 0.01 (15 tasks)
     ↳ Thread 2 total task time (s)       : 0.003 (14 tasks)
     ↳ Thread 3 total task time (s)       : 0.009 (20 tasks)
     ↳ Thread 4 total task time (s)       : 0.009 (23 tasks)
     ↳ Thread 5 total task time (s)       : 0.003 (13 tasks)
     ↳ Thread 6 total task time (s)       : 0.015 (24 tasks)
     ↳ Thread 7 total task time (s)       : 0.004 (24 tasks)
    ```

## To Do / Next Steps
0. **Improve speed of optimal computation.**
   - `gprof`
   - `valgrind callgrind`
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
