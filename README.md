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
  - `make clean && make && ./bin/quixo -p opt-compute -l 5 -T 16 -L 1000`.
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
  - `-l 4 -T 1 -s`: (total 143.72s)
    ```
    Total initClass() time (s)            : 0.022
     ↳ Thread 0 total task time (s)       : 0.021 (153 tasks)
    Total checkTerminalsClass() time (s)  : 22.916
     ↳ Thread 0 total task time (s)       : 19.213 (43046721 tasks)
       ↳ Total time waiting for locks (s) : 1.915
       ↳ Total time in CSes (s)           : 2.022
    Total parentLinkCacheClass() time (s) : 53.169
     ↳ Thread 0 total task time (s)       : 51.598 (16612232 tasks)
       ↳ Total time waiting for locks (s) : 6.298
       ↳ Total time in CSes (s)           : 6.323
    Total valueIterateClass() time (s)    : 66.378
     ↳ Thread 0 total task time (s)       : 65.095 (10705288 tasks)
       ↳ Total time waiting for locks (s) : 6.93
       ↳ Total time in CSes (s)           : 6.775
    Total elimWinOrDrawClass() time (s)   : 0.069
     ↳ Thread 0 total task time (s)       : 0.069 (153 tasks)
    ```
  - `-l 4 -T 2 -s`: (total 127.997s)
    ```
    Total initClass() time (s)            : 0.028
     ↳ Thread 0 total task time (s)       : 0.027 (75 tasks)
     ↳ Thread 1 total task time (s)       : 0.025 (78 tasks)
    Total checkTerminalsClass() time (s)  : 23.543
     ↳ Thread 0 total task time (s)       : 15.019 (29487213 tasks)
       ↳ Total time waiting for locks (s) : 2.85
       ↳ Total time in CSes (s)           : 2.16
     ↳ Thread 1 total task time (s)       : 12.661 (13559508 tasks)
       ↳ Total time waiting for locks (s) : 2.788
       ↳ Total time in CSes (s)           : 2.027
    Total parentLinkCacheClass() time (s) : 49.001
     ↳ Thread 0 total task time (s)       : 44.83 (8793821 tasks)
       ↳ Total time waiting for locks (s) : 11.749
       ↳ Total time in CSes (s)           : 7.825
     ↳ Thread 1 total task time (s)       : 42.507 (7818411 tasks)
       ↳ Total time waiting for locks (s) : 11.7
       ↳ Total time in CSes (s)           : 7.633
    Total valueIterateClass() time (s)    : 54.481
     ↳ Thread 0 total task time (s)       : 51.551 (5412320 tasks)
       ↳ Total time waiting for locks (s) : 12.719
       ↳ Total time in CSes (s)           : 8.18
     ↳ Thread 1 total task time (s)       : 49.991 (5284490 tasks)
       ↳ Total time waiting for locks (s) : 12.557
       ↳ Total time in CSes (s)           : 7.886
    Total elimWinOrDrawClass() time (s)   : 0.044
     ↳ Thread 0 total task time (s)       : 0.033 (77 tasks)
     ↳ Thread 1 total task time (s)       : 0.036 (76 tasks)
    ```
  - `-l 4 -T 4 -s`: (total 145.945s)
    ```
    Total initClass() time (s)            : 0.03
     ↳ Thread 0 total task time (s)       : 0.015 (43 tasks)
     ↳ Thread 1 total task time (s)       : 0.013 (39 tasks)
     ↳ Thread 2 total task time (s)       : 0.012 (37 tasks)
     ↳ Thread 3 total task time (s)       : 0.015 (34 tasks)
    Total checkTerminalsClass() time (s)  : 27.378
     ↳ Thread 0 total task time (s)       : 11.527 (6847161 tasks)
       ↳ Total time waiting for locks (s) : 6.039
       ↳ Total time in CSes (s)           : 1.371
     ↳ Thread 1 total task time (s)       : 14.173 (12205626 tasks)
       ↳ Total time waiting for locks (s) : 7.062
       ↳ Total time in CSes (s)           : 1.78
     ↳ Thread 2 total task time (s)       : 14.229 (12286266 tasks)
       ↳ Total time waiting for locks (s) : 7.098
       ↳ Total time in CSes (s)           : 1.727
     ↳ Thread 3 total task time (s)       : 13.897 (11707668 tasks)
       ↳ Total time waiting for locks (s) : 7.067
       ↳ Total time in CSes (s)           : 1.675
    Total parentLinkCacheClass() time (s) : 57.324
     ↳ Thread 0 total task time (s)       : 53.054 (4212555 tasks)
       ↳ Total time waiting for locks (s) : 33.322
       ↳ Total time in CSes (s)           : 5.686
     ↳ Thread 1 total task time (s)       : 53.571 (4124647 tasks)
       ↳ Total time waiting for locks (s) : 34.752
       ↳ Total time in CSes (s)           : 5.516
     ↳ Thread 2 total task time (s)       : 53.613 (4128971 tasks)
       ↳ Total time waiting for locks (s) : 34.92
       ↳ Total time in CSes (s)           : 5.356
     ↳ Thread 3 total task time (s)       : 53.592 (4146059 tasks)
       ↳ Total time waiting for locks (s) : 35.089
       ↳ Total time in CSes (s)           : 5.315
    Total valueIterateClass() time (s)    : 60.297
     ↳ Thread 0 total task time (s)       : 57.427 (2643306 tasks)
       ↳ Total time waiting for locks (s) : 33.836
       ↳ Total time in CSes (s)           : 6.197
     ↳ Thread 1 total task time (s)       : 57.782 (2682017 tasks)
       ↳ Total time waiting for locks (s) : 35.126
       ↳ Total time in CSes (s)           : 6.14
     ↳ Thread 2 total task time (s)       : 57.813 (2687011 tasks)
       ↳ Total time waiting for locks (s) : 35.493
       ↳ Total time in CSes (s)           : 5.98
     ↳ Thread 3 total task time (s)       : 57.776 (2683952 tasks)
       ↳ Total time waiting for locks (s) : 35.63
       ↳ Total time in CSes (s)           : 5.932
    Total elimWinOrDrawClass() time (s)   : 0.044
     ↳ Thread 0 total task time (s)       : 0.02 (42 tasks)
     ↳ Thread 1 total task time (s)       : 0.021 (42 tasks)
     ↳ Thread 2 total task time (s)       : 0.013 (32 tasks)
     ↳ Thread 3 total task time (s)       : 0.015 (37 tasks)
    ```
  - `-l 4 -T 8 -s`: (total 167.974s)
    ```
    Total initClass() time (s)            : 0.01
     ↳ Thread 0 total task time (s)       : 0.005 (44 tasks)
     ↳ Thread 1 total task time (s)       : 0.001 (19 tasks)
     ↳ Thread 2 total task time (s)       : 0.001 (14 tasks)
     ↳ Thread 3 total task time (s)       : 0 (17 tasks)
     ↳ Thread 4 total task time (s)       : 0.002 (13 tasks)
     ↳ Thread 5 total task time (s)       : 0.003 (19 tasks)
     ↳ Thread 6 total task time (s)       : 0.001 (13 tasks)
     ↳ Thread 7 total task time (s)       : 0.001 (14 tasks)
    Total checkTerminalsClass() time (s)  : 42.61
     ↳ Thread 0 total task time (s)       : 10.413 (2313548 tasks)
       ↳ Total time waiting for locks (s) : 8.178
       ↳ Total time in CSes (s)           : 0.56
     ↳ Thread 1 total task time (s)       : 14.207 (5711261 tasks)
       ↳ Total time waiting for locks (s) : 10.661
       ↳ Total time in CSes (s)           : 0.865
     ↳ Thread 2 total task time (s)       : 14.213 (5738331 tasks)
       ↳ Total time waiting for locks (s) : 10.657
       ↳ Total time in CSes (s)           : 0.878
     ↳ Thread 3 total task time (s)       : 14.253 (5864998 tasks)
       ↳ Total time waiting for locks (s) : 10.684
       ↳ Total time in CSes (s)           : 0.868
     ↳ Thread 4 total task time (s)       : 14.26 (5876938 tasks)
       ↳ Total time waiting for locks (s) : 10.642
       ↳ Total time in CSes (s)           : 0.876
     ↳ Thread 5 total task time (s)       : 14.251 (5836216 tasks)
       ↳ Total time waiting for locks (s) : 10.572
       ↳ Total time in CSes (s)           : 0.893
     ↳ Thread 6 total task time (s)       : 14.268 (5891899 tasks)
       ↳ Total time waiting for locks (s) : 10.593
       ↳ Total time in CSes (s)           : 0.878
     ↳ Thread 7 total task time (s)       : 14.247 (5813530 tasks)
       ↳ Total time waiting for locks (s) : 10.651
       ↳ Total time in CSes (s)           : 0.885
    Total parentLinkCacheClass() time (s) : 60.62
     ↳ Thread 0 total task time (s)       : 56.57 (2077622 tasks)
       ↳ Total time waiting for locks (s) : 46.807
       ↳ Total time in CSes (s)           : 2.733
     ↳ Thread 1 total task time (s)       : 58.714 (2078549 tasks)
       ↳ Total time waiting for locks (s) : 49.321
       ↳ Total time in CSes (s)           : 2.648
     ↳ Thread 2 total task time (s)       : 58.72 (2076110 tasks)
       ↳ Total time waiting for locks (s) : 49.176
       ↳ Total time in CSes (s)           : 2.691
     ↳ Thread 3 total task time (s)       : 58.741 (2077391 tasks)
       ↳ Total time waiting for locks (s) : 49.251
       ↳ Total time in CSes (s)           : 2.653
     ↳ Thread 4 total task time (s)       : 58.763 (2078700 tasks)
       ↳ Total time waiting for locks (s) : 48.977
       ↳ Total time in CSes (s)           : 2.693
     ↳ Thread 5 total task time (s)       : 58.739 (2074493 tasks)
       ↳ Total time waiting for locks (s) : 48.721
       ↳ Total time in CSes (s)           : 2.759
     ↳ Thread 6 total task time (s)       : 58.741 (2074373 tasks)
       ↳ Total time waiting for locks (s) : 48.844
       ↳ Total time in CSes (s)           : 2.702
     ↳ Thread 7 total task time (s)       : 58.747 (2074994 tasks)
       ↳ Total time waiting for locks (s) : 49.06
       ↳ Total time in CSes (s)           : 2.739
    Total valueIterateClass() time (s)    : 63.733
     ↳ Thread 0 total task time (s)       : 61.004 (1317895 tasks)
       ↳ Total time waiting for locks (s) : 49.07
       ↳ Total time in CSes (s)           : 3.025
     ↳ Thread 1 total task time (s)       : 62.433 (1343702 tasks)
       ↳ Total time waiting for locks (s) : 51.142
       ↳ Total time in CSes (s)           : 2.932
     ↳ Thread 2 total task time (s)       : 62.433 (1341013 tasks)
       ↳ Total time waiting for locks (s) : 51.145
       ↳ Total time in CSes (s)           : 2.979
     ↳ Thread 3 total task time (s)       : 62.449 (1342613 tasks)
       ↳ Total time waiting for locks (s) : 51.196
       ↳ Total time in CSes (s)           : 2.943
     ↳ Thread 4 total task time (s)       : 62.46 (1342648 tasks)
       ↳ Total time waiting for locks (s) : 50.969
       ↳ Total time in CSes (s)           : 2.955
     ↳ Thread 5 total task time (s)       : 62.452 (1339811 tasks)
       ↳ Total time waiting for locks (s) : 50.531
       ↳ Total time in CSes (s)           : 3.022
     ↳ Thread 6 total task time (s)       : 62.451 (1339568 tasks)
       ↳ Total time waiting for locks (s) : 50.594
       ↳ Total time in CSes (s)           : 2.96
     ↳ Thread 7 total task time (s)       : 62.457 (1340394 tasks)
       ↳ Total time waiting for locks (s) : 50.94
       ↳ Total time in CSes (s)           : 2.996
    Total elimWinOrDrawClass() time (s)   : 0.045
     ↳ Thread 0 total task time (s)       : 0.009 (32 tasks)
     ↳ Thread 1 total task time (s)       : 0.014 (24 tasks)
     ↳ Thread 2 total task time (s)       : 0.008 (16 tasks)
     ↳ Thread 3 total task time (s)       : 0.007 (18 tasks)
     ↳ Thread 4 total task time (s)       : 0.004 (12 tasks)
     ↳ Thread 5 total task time (s)       : 0.008 (19 tasks)
     ↳ Thread 6 total task time (s)       : 0.006 (20 tasks)
     ↳ Thread 7 total task time (s)       : 0.009 (12 tasks)
    ```
  - I think the takeaway here is that the locks are far too coarse-grained; waiting for locks takes far too long. We need finer locks.
- Apr 1, 2021: More speed profiling for 4X4 optimal computation:
  - `-l 4 -T 8 -L 10 -s`: (total 70.312s)
    ```
    Total initClass() time (s)            : 0.025
     ↳ Thread 0 total task time (s)       : 0.006 (41 tasks)
     ↳ Thread 1 total task time (s)       : 0.005 (10 tasks)
     ↳ Thread 2 total task time (s)       : 0.002 (10 tasks)
     ↳ Thread 3 total task time (s)       : 0.006 (20 tasks)
     ↳ Thread 4 total task time (s)       : 0.007 (15 tasks)
     ↳ Thread 5 total task time (s)       : 0.004 (18 tasks)
     ↳ Thread 6 total task time (s)       : 0.008 (22 tasks)
     ↳ Thread 7 total task time (s)       : 0.005 (17 tasks)
    Total checkTerminalsClass() time (s)  : 34.582
     ↳ Thread 0 total task time (s)       : 1.386 (486291 tasks)
       ↳ Total time waiting for locks (s) : 0.433
       ↳ Total time in CSes (s)           : 0.226
     ↳ Thread 1 total task time (s)       : 4.822 (5923414 tasks)
       ↳ Total time waiting for locks (s) : 1.264
       ↳ Total time in CSes (s)           : 0.774
     ↳ Thread 2 total task time (s)       : 4.83 (5858940 tasks)
       ↳ Total time waiting for locks (s) : 1.288
       ↳ Total time in CSes (s)           : 0.779
     ↳ Thread 3 total task time (s)       : 4.877 (6153719 tasks)
       ↳ Total time waiting for locks (s) : 1.301
       ↳ Total time in CSes (s)           : 0.782
     ↳ Thread 4 total task time (s)       : 4.886 (6167732 tasks)
       ↳ Total time waiting for locks (s) : 1.276
       ↳ Total time in CSes (s)           : 0.783
     ↳ Thread 5 total task time (s)       : 4.877 (6109536 tasks)
       ↳ Total time waiting for locks (s) : 1.28
       ↳ Total time in CSes (s)           : 0.794
     ↳ Thread 6 total task time (s)       : 4.901 (6194776 tasks)
       ↳ Total time waiting for locks (s) : 1.288
       ↳ Total time in CSes (s)           : 0.791
     ↳ Thread 7 total task time (s)       : 4.882 (6152313 tasks)
       ↳ Total time waiting for locks (s) : 1.289
       ↳ Total time in CSes (s)           : 0.795
    Total parentLinkCacheClass() time (s) : 16.564
     ↳ Thread 0 total task time (s)       : 11.558 (1659783 tasks)
       ↳ Total time waiting for locks (s) : 3.877
       ↳ Total time in CSes (s)           : 1.829
     ↳ Thread 1 total task time (s)       : 14.456 (2119395 tasks)
       ↳ Total time waiting for locks (s) : 5.136
       ↳ Total time in CSes (s)           : 2.255
     ↳ Thread 2 total task time (s)       : 14.452 (2112583 tasks)
       ↳ Total time waiting for locks (s) : 5.193
       ↳ Total time in CSes (s)           : 2.266
     ↳ Thread 3 total task time (s)       : 14.464 (2146796 tasks)
       ↳ Total time waiting for locks (s) : 5.19
       ↳ Total time in CSes (s)           : 2.244
     ↳ Thread 4 total task time (s)       : 14.473 (2140465 tasks)
       ↳ Total time waiting for locks (s) : 5.093
       ↳ Total time in CSes (s)           : 2.27
     ↳ Thread 5 total task time (s)       : 14.474 (2132799 tasks)
       ↳ Total time waiting for locks (s) : 5.142
       ↳ Total time in CSes (s)           : 2.29
     ↳ Thread 6 total task time (s)       : 14.473 (2151424 tasks)
       ↳ Total time waiting for locks (s) : 5.16
       ↳ Total time in CSes (s)           : 2.277
     ↳ Thread 7 total task time (s)       : 14.466 (2148987 tasks)
       ↳ Total time waiting for locks (s) : 5.163
       ↳ Total time in CSes (s)           : 2.292
    Total valueIterateClass() time (s)    : 18.212
     ↳ Thread 0 total task time (s)       : 14.915 (1177139 tasks)
       ↳ Total time waiting for locks (s) : 4.571
       ↳ Total time in CSes (s)           : 2.202
     ↳ Thread 1 total task time (s)       : 16.837 (1347365 tasks)
       ↳ Total time waiting for locks (s) : 5.585
       ↳ Total time in CSes (s)           : 2.459
     ↳ Thread 2 total task time (s)       : 16.829 (1347024 tasks)
       ↳ Total time waiting for locks (s) : 5.649
       ↳ Total time in CSes (s)           : 2.475
     ↳ Thread 3 total task time (s)       : 16.842 (1364829 tasks)
       ↳ Total time waiting for locks (s) : 5.648
       ↳ Total time in CSes (s)           : 2.461
     ↳ Thread 4 total task time (s)       : 16.857 (1363791 tasks)
       ↳ Total time waiting for locks (s) : 5.562
       ↳ Total time in CSes (s)           : 2.496
     ↳ Thread 5 total task time (s)       : 16.844 (1367285 tasks)
       ↳ Total time waiting for locks (s) : 5.605
       ↳ Total time in CSes (s)           : 2.475
     ↳ Thread 6 total task time (s)       : 16.854 (1366986 tasks)
       ↳ Total time waiting for locks (s) : 5.586
       ↳ Total time in CSes (s)           : 2.498
     ↳ Thread 7 total task time (s)       : 16.861 (1362598 tasks)
       ↳ Total time waiting for locks (s) : 5.612
       ↳ Total time in CSes (s)           : 2.509
    Total elimWinOrDrawClass() time (s)   : 0.052
     ↳ Thread 0 total task time (s)       : 0.01 (30 tasks)
     ↳ Thread 1 total task time (s)       : 0.016 (20 tasks)
     ↳ Thread 2 total task time (s)       : 0.009 (12 tasks)
     ↳ Thread 3 total task time (s)       : 0.015 (19 tasks)
     ↳ Thread 4 total task time (s)       : 0.009 (15 tasks)
     ↳ Thread 5 total task time (s)       : 0.003 (13 tasks)
     ↳ Thread 6 total task time (s)       : 0.012 (23 tasks)
     ↳ Thread 7 total task time (s)       : 0.006 (21 tasks)
    ```
  - `-l 4 -T 8 -L 100 -s`: (total 63.358s)
    ```
    Total initClass() time (s)            : 0.028
     ↳ Thread 0 total task time (s)       : 0.021 (59 tasks)
     ↳ Thread 1 total task time (s)       : 0.002 (13 tasks)
     ↳ Thread 2 total task time (s)       : 0.003 (11 tasks)
     ↳ Thread 3 total task time (s)       : 0.006 (15 tasks)
     ↳ Thread 4 total task time (s)       : 0.003 (13 tasks)
     ↳ Thread 5 total task time (s)       : 0.001 (10 tasks)
     ↳ Thread 6 total task time (s)       : 0.007 (17 tasks)
     ↳ Thread 7 total task time (s)       : 0.006 (15 tasks)
    Total checkTerminalsClass() time (s)  : 33.007
     ↳ Thread 0 total task time (s)       : 1.074 (415478 tasks)
       ↳ Total time waiting for locks (s) : 0.246
       ↳ Total time in CSes (s)           : 0.16
     ↳ Thread 1 total task time (s)       : 4.398 (6025298 tasks)
       ↳ Total time waiting for locks (s) : 0.95
       ↳ Total time in CSes (s)           : 0.674
     ↳ Thread 2 total task time (s)       : 4.411 (6043822 tasks)
       ↳ Total time waiting for locks (s) : 0.955
       ↳ Total time in CSes (s)           : 0.672
     ↳ Thread 3 total task time (s)       : 4.407 (6194955 tasks)
       ↳ Total time waiting for locks (s) : 0.953
       ↳ Total time in CSes (s)           : 0.67
     ↳ Thread 4 total task time (s)       : 4.459 (6137410 tasks)
       ↳ Total time waiting for locks (s) : 0.949
       ↳ Total time in CSes (s)           : 0.688
     ↳ Thread 5 total task time (s)       : 4.452 (6107394 tasks)
       ↳ Total time waiting for locks (s) : 0.952
       ↳ Total time in CSes (s)           : 0.691
     ↳ Thread 6 total task time (s)       : 4.45 (6074951 tasks)
       ↳ Total time waiting for locks (s) : 0.946
       ↳ Total time in CSes (s)           : 0.693
     ↳ Thread 7 total task time (s)       : 4.443 (6047413 tasks)
       ↳ Total time waiting for locks (s) : 0.945
       ↳ Total time in CSes (s)           : 0.695
    Total parentLinkCacheClass() time (s) : 13.754
     ↳ Thread 0 total task time (s)       : 8.713 (1490257 tasks)
       ↳ Total time waiting for locks (s) : 2.028
       ↳ Total time in CSes (s)           : 1.28
     ↳ Thread 1 total task time (s)       : 11.621 (2144117 tasks)
       ↳ Total time waiting for locks (s) : 2.79
       ↳ Total time in CSes (s)           : 1.737
     ↳ Thread 2 total task time (s)       : 11.641 (2165094 tasks)
       ↳ Total time waiting for locks (s) : 2.803
       ↳ Total time in CSes (s)           : 1.73
     ↳ Thread 3 total task time (s)       : 11.619 (2163854 tasks)
       ↳ Total time waiting for locks (s) : 2.765
       ↳ Total time in CSes (s)           : 1.763
     ↳ Thread 4 total task time (s)       : 11.62 (2165591 tasks)
       ↳ Total time waiting for locks (s) : 2.788
       ↳ Total time in CSes (s)           : 1.732
     ↳ Thread 5 total task time (s)       : 11.653 (2157239 tasks)
       ↳ Total time waiting for locks (s) : 2.794
       ↳ Total time in CSes (s)           : 1.747
     ↳ Thread 6 total task time (s)       : 11.664 (2153614 tasks)
       ↳ Total time waiting for locks (s) : 2.77
       ↳ Total time in CSes (s)           : 1.778
     ↳ Thread 7 total task time (s)       : 11.644 (2172466 tasks)
       ↳ Total time waiting for locks (s) : 2.798
       ↳ Total time in CSes (s)           : 1.755
    Total valueIterateClass() time (s)    : 15.596
     ↳ Thread 0 total task time (s)       : 12.293 (1086382 tasks)
       ↳ Total time waiting for locks (s) : 2.835
       ↳ Total time in CSes (s)           : 1.721
     ↳ Thread 1 total task time (s)       : 14.214 (1364098 tasks)
       ↳ Total time waiting for locks (s) : 3.396
       ↳ Total time in CSes (s)           : 1.972
     ↳ Thread 2 total task time (s)       : 14.248 (1354749 tasks)
       ↳ Total time waiting for locks (s) : 3.353
       ↳ Total time in CSes (s)           : 2.027
     ↳ Thread 3 total task time (s)       : 14.232 (1367702 tasks)
       ↳ Total time waiting for locks (s) : 3.364
       ↳ Total time in CSes (s)           : 2.022
     ↳ Thread 4 total task time (s)       : 14.228 (1377231 tasks)
       ↳ Total time waiting for locks (s) : 3.366
       ↳ Total time in CSes (s)           : 2.031
     ↳ Thread 5 total task time (s)       : 14.223 (1383810 tasks)
       ↳ Total time waiting for locks (s) : 3.376
       ↳ Total time in CSes (s)           : 2
     ↳ Thread 6 total task time (s)       : 14.237 (1381024 tasks)
       ↳ Total time waiting for locks (s) : 3.353
       ↳ Total time in CSes (s)           : 2.024
     ↳ Thread 7 total task time (s)       : 14.241 (1381385 tasks)
       ↳ Total time waiting for locks (s) : 3.365
       ↳ Total time in CSes (s)           : 2.047
    Total elimWinOrDrawClass() time (s)   : 0.053
     ↳ Thread 0 total task time (s)       : 0.014 (37 tasks)
     ↳ Thread 1 total task time (s)       : 0.013 (15 tasks)
     ↳ Thread 2 total task time (s)       : 0.001 (12 tasks)
     ↳ Thread 3 total task time (s)       : 0.012 (20 tasks)
     ↳ Thread 4 total task time (s)       : 0.018 (17 tasks)
     ↳ Thread 5 total task time (s)       : 0.011 (18 tasks)
     ↳ Thread 6 total task time (s)       : 0.004 (14 tasks)
     ↳ Thread 7 total task time (s)       : 0.007 (20 tasks)
    ```
  - `-l 4 -T 8 -L 1000 -s`: (total 60.006s)
    ```
    Total initClass() time (s)            : 0.018
     ↳ Thread 0 total task time (s)       : 0.012 (51 tasks)
     ↳ Thread 1 total task time (s)       : 0.002 (10 tasks)
     ↳ Thread 2 total task time (s)       : 0.001 (15 tasks)
     ↳ Thread 3 total task time (s)       : 0.002 (16 tasks)
     ↳ Thread 4 total task time (s)       : 0.004 (20 tasks)
     ↳ Thread 5 total task time (s)       : 0.001 (11 tasks)
     ↳ Thread 6 total task time (s)       : 0.003 (18 tasks)
     ↳ Thread 7 total task time (s)       : 0.003 (12 tasks)
    Total checkTerminalsClass() time (s)  : 30.784
     ↳ Thread 0 total task time (s)       : 1.025 (410965 tasks)
       ↳ Total time waiting for locks (s) : 0.236
       ↳ Total time in CSes (s)           : 0.149
     ↳ Thread 1 total task time (s)       : 4.318 (5906986 tasks)
       ↳ Total time waiting for locks (s) : 0.947
       ↳ Total time in CSes (s)           : 0.649
     ↳ Thread 2 total task time (s)       : 4.364 (6080348 tasks)
       ↳ Total time waiting for locks (s) : 0.946
       ↳ Total time in CSes (s)           : 0.65
     ↳ Thread 3 total task time (s)       : 4.36 (6176938 tasks)
       ↳ Total time waiting for locks (s) : 0.95
       ↳ Total time in CSes (s)           : 0.653
     ↳ Thread 4 total task time (s)       : 4.373 (6071520 tasks)
       ↳ Total time waiting for locks (s) : 0.935
       ↳ Total time in CSes (s)           : 0.669
     ↳ Thread 5 total task time (s)       : 4.387 (6165810 tasks)
       ↳ Total time waiting for locks (s) : 0.947
       ↳ Total time in CSes (s)           : 0.662
     ↳ Thread 6 total task time (s)       : 4.393 (6128840 tasks)
       ↳ Total time waiting for locks (s) : 0.941
       ↳ Total time in CSes (s)           : 0.672
     ↳ Thread 7 total task time (s)       : 4.38 (6105314 tasks)
       ↳ Total time waiting for locks (s) : 0.948
       ↳ Total time in CSes (s)           : 0.663
    Total parentLinkCacheClass() time (s) : 13.462
     ↳ Thread 0 total task time (s)       : 8.376 (1489171 tasks)
       ↳ Total time waiting for locks (s) : 1.926
       ↳ Total time in CSes (s)           : 1.197
     ↳ Thread 1 total task time (s)       : 11.316 (2154435 tasks)
       ↳ Total time waiting for locks (s) : 2.747
       ↳ Total time in CSes (s)           : 1.601
     ↳ Thread 2 total task time (s)       : 11.345 (2157640 tasks)
       ↳ Total time waiting for locks (s) : 2.715
       ↳ Total time in CSes (s)           : 1.613
     ↳ Thread 3 total task time (s)       : 11.334 (2189213 tasks)
       ↳ Total time waiting for locks (s) : 2.707
       ↳ Total time in CSes (s)           : 1.622
     ↳ Thread 4 total task time (s)       : 11.333 (2147716 tasks)
       ↳ Total time waiting for locks (s) : 2.689
       ↳ Total time in CSes (s)           : 1.675
     ↳ Thread 5 total task time (s)       : 11.354 (2165707 tasks)
       ↳ Total time waiting for locks (s) : 2.725
       ↳ Total time in CSes (s)           : 1.654
     ↳ Thread 6 total task time (s)       : 11.363 (2152818 tasks)
       ↳ Total time waiting for locks (s) : 2.715
       ↳ Total time in CSes (s)           : 1.665
     ↳ Thread 7 total task time (s)       : 11.371 (2155532 tasks)
       ↳ Total time waiting for locks (s) : 2.722
       ↳ Total time in CSes (s)           : 1.664
    Total valueIterateClass() time (s)    : 14.688
     ↳ Thread 0 total task time (s)       : 11.596 (1121142 tasks)
       ↳ Total time waiting for locks (s) : 2.463
       ↳ Total time in CSes (s)           : 1.497
     ↳ Thread 1 total task time (s)       : 13.304 (1369011 tasks)
       ↳ Total time waiting for locks (s) : 2.889
       ↳ Total time in CSes (s)           : 1.779
     ↳ Thread 2 total task time (s)       : 13.322 (1371564 tasks)
       ↳ Total time waiting for locks (s) : 2.873
       ↳ Total time in CSes (s)           : 1.743
     ↳ Thread 3 total task time (s)       : 13.333 (1374628 tasks)
       ↳ Total time waiting for locks (s) : 2.874
       ↳ Total time in CSes (s)           : 1.78
     ↳ Thread 4 total task time (s)       : 13.319 (1365344 tasks)
       ↳ Total time waiting for locks (s) : 2.873
       ↳ Total time in CSes (s)           : 1.799
     ↳ Thread 5 total task time (s)       : 13.336 (1372214 tasks)
       ↳ Total time waiting for locks (s) : 2.852
       ↳ Total time in CSes (s)           : 1.85
     ↳ Thread 6 total task time (s)       : 13.336 (1362349 tasks)
       ↳ Total time waiting for locks (s) : 2.858
       ↳ Total time in CSes (s)           : 1.81
     ↳ Thread 7 total task time (s)       : 13.338 (1372036 tasks)
       ↳ Total time waiting for locks (s) : 2.887
       ↳ Total time in CSes (s)           : 1.834
    Total elimWinOrDrawClass() time (s)   : 0.053
     ↳ Thread 0 total task time (s)       : 0.017 (38 tasks)
     ↳ Thread 1 total task time (s)       : 0.006 (19 tasks)
     ↳ Thread 2 total task time (s)       : 0.021 (16 tasks)
     ↳ Thread 3 total task time (s)       : 0.007 (12 tasks)
     ↳ Thread 4 total task time (s)       : 0.003 (14 tasks)
     ↳ Thread 5 total task time (s)       : 0.005 (15 tasks)
     ↳ Thread 6 total task time (s)       : 0.009 (15 tasks)
     ↳ Thread 7 total task time (s)       : 0.012 (24 tasks)
    ```
  - `-l 4 -T 16 -L 1000 -s`: (total 61.387s)
    ```
    Total initClass() time (s)            : 0.011
     ↳ Thread 0 total task time (s)       : 0.005 (41 tasks)
     ↳ Thread 1 total task time (s)       : 0.001 (12 tasks)
     ↳ Thread 2 total task time (s)       : 0 (7 tasks)
     ↳ Thread 3 total task time (s)       : 0.001 (8 tasks)
     ↳ Thread 4 total task time (s)       : 0 (6 tasks)
     ↳ Thread 5 total task time (s)       : 0.001 (10 tasks)
     ↳ Thread 6 total task time (s)       : 0 (4 tasks)
     ↳ Thread 7 total task time (s)       : 0 (7 tasks)
     ↳ Thread 8 total task time (s)       : 0.001 (9 tasks)
     ↳ Thread 9 total task time (s)       : 0 (6 tasks)
     ↳ Thread 10 total task time (s)       : 0.001 (11 tasks)
     ↳ Thread 11 total task time (s)       : 0.001 (11 tasks)
     ↳ Thread 12 total task time (s)       : 0 (6 tasks)
     ↳ Thread 13 total task time (s)       : 0 (5 tasks)
     ↳ Thread 14 total task time (s)       : 0 (5 tasks)
     ↳ Thread 15 total task time (s)       : 0 (5 tasks)
    Total checkTerminalsClass() time (s)  : 39.598
     ↳ Thread 0 total task time (s)       : 0.21 (93125 tasks)
       ↳ Total time waiting for locks (s) : 0.044
       ↳ Total time in CSes (s)           : 0.028
     ↳ Thread 1 total task time (s)       : 2.624 (2794601 tasks)
       ↳ Total time waiting for locks (s) : 0.54
       ↳ Total time in CSes (s)           : 0.347
     ↳ Thread 2 total task time (s)       : 2.666 (2867956 tasks)
       ↳ Total time waiting for locks (s) : 0.548
       ↳ Total time in CSes (s)           : 0.355
     ↳ Thread 3 total task time (s)       : 2.642 (2880556 tasks)
       ↳ Total time waiting for locks (s) : 0.551
       ↳ Total time in CSes (s)           : 0.352
     ↳ Thread 4 total task time (s)       : 2.709 (2834936 tasks)
       ↳ Total time waiting for locks (s) : 0.53
       ↳ Total time in CSes (s)           : 0.374
     ↳ Thread 5 total task time (s)       : 2.757 (2865744 tasks)
       ↳ Total time waiting for locks (s) : 0.533
       ↳ Total time in CSes (s)           : 0.38
     ↳ Thread 6 total task time (s)       : 2.799 (2903575 tasks)
       ↳ Total time waiting for locks (s) : 0.55
       ↳ Total time in CSes (s)           : 0.406
     ↳ Thread 7 total task time (s)       : 2.708 (2871120 tasks)
       ↳ Total time waiting for locks (s) : 0.533
       ↳ Total time in CSes (s)           : 0.378
     ↳ Thread 8 total task time (s)       : 2.712 (2886330 tasks)
       ↳ Total time waiting for locks (s) : 0.535
       ↳ Total time in CSes (s)           : 0.378
     ↳ Thread 9 total task time (s)       : 2.707 (2900082 tasks)
       ↳ Total time waiting for locks (s) : 0.532
       ↳ Total time in CSes (s)           : 0.381
     ↳ Thread 10 total task time (s)       : 2.738 (2933182 tasks)
       ↳ Total time waiting for locks (s) : 0.539
       ↳ Total time in CSes (s)           : 0.382
     ↳ Thread 11 total task time (s)       : 2.712 (2902096 tasks)
       ↳ Total time waiting for locks (s) : 0.541
       ↳ Total time in CSes (s)           : 0.376
     ↳ Thread 12 total task time (s)       : 2.661 (2869121 tasks)
       ↳ Total time waiting for locks (s) : 0.534
       ↳ Total time in CSes (s)           : 0.367
     ↳ Thread 13 total task time (s)       : 2.671 (2978570 tasks)
       ↳ Total time waiting for locks (s) : 0.53
       ↳ Total time in CSes (s)           : 0.371
     ↳ Thread 14 total task time (s)       : 2.645 (2785204 tasks)
       ↳ Total time waiting for locks (s) : 0.525
       ↳ Total time in CSes (s)           : 0.375
     ↳ Thread 15 total task time (s)       : 2.571 (2680523 tasks)
       ↳ Total time waiting for locks (s) : 0.513
       ↳ Total time in CSes (s)           : 0.36
    Total parentLinkCacheClass() time (s) : 9.906
     ↳ Thread 0 total task time (s)       : 2.786 (322643 tasks)
       ↳ Total time waiting for locks (s) : 0.59
       ↳ Total time in CSes (s)           : 0.358
     ↳ Thread 1 total task time (s)       : 7.604 (1141891 tasks)
       ↳ Total time waiting for locks (s) : 1.69
       ↳ Total time in CSes (s)           : 0.965
     ↳ Thread 2 total task time (s)       : 7.634 (1168651 tasks)
       ↳ Total time waiting for locks (s) : 1.713
       ↳ Total time in CSes (s)           : 0.986
     ↳ Thread 3 total task time (s)       : 7.602 (1174111 tasks)
       ↳ Total time waiting for locks (s) : 1.704
       ↳ Total time in CSes (s)           : 0.958
     ↳ Thread 4 total task time (s)       : 7.754 (1036723 tasks)
       ↳ Total time waiting for locks (s) : 1.517
       ↳ Total time in CSes (s)           : 1.137
     ↳ Thread 5 total task time (s)       : 7.735 (1039668 tasks)
       ↳ Total time waiting for locks (s) : 1.543
       ↳ Total time in CSes (s)           : 1.142
     ↳ Thread 6 total task time (s)       : 7.74 (1038333 tasks)
       ↳ Total time waiting for locks (s) : 1.548
       ↳ Total time in CSes (s)           : 1.156
     ↳ Thread 7 total task time (s)       : 7.708 (1047170 tasks)
       ↳ Total time waiting for locks (s) : 1.583
       ↳ Total time in CSes (s)           : 1.135
     ↳ Thread 8 total task time (s)       : 7.701 (1071197 tasks)
       ↳ Total time waiting for locks (s) : 1.568
       ↳ Total time in CSes (s)           : 1.116
     ↳ Thread 9 total task time (s)       : 7.72 (1061516 tasks)
       ↳ Total time waiting for locks (s) : 1.556
       ↳ Total time in CSes (s)           : 1.128
     ↳ Thread 10 total task time (s)       : 7.742 (1048964 tasks)
       ↳ Total time waiting for locks (s) : 1.548
       ↳ Total time in CSes (s)           : 1.122
     ↳ Thread 11 total task time (s)       : 7.709 (1070093 tasks)
       ↳ Total time waiting for locks (s) : 1.575
       ↳ Total time in CSes (s)           : 1.107
     ↳ Thread 12 total task time (s)       : 7.656 (1098022 tasks)
       ↳ Total time waiting for locks (s) : 1.61
       ↳ Total time in CSes (s)           : 1.082
     ↳ Thread 13 total task time (s)       : 7.763 (1130508 tasks)
       ↳ Total time waiting for locks (s) : 1.643
       ↳ Total time in CSes (s)           : 1.107
     ↳ Thread 14 total task time (s)       : 7.667 (1096192 tasks)
       ↳ Total time waiting for locks (s) : 1.603
       ↳ Total time in CSes (s)           : 1.085
     ↳ Thread 15 total task time (s)       : 7.558 (1066550 tasks)
       ↳ Total time waiting for locks (s) : 1.57
       ↳ Total time in CSes (s)           : 1.093
    Total valueIterateClass() time (s)    : 10.194
     ↳ Thread 0 total task time (s)       : 6.341 (456808 tasks)
       ↳ Total time waiting for locks (s) : 1.226
       ↳ Total time in CSes (s)           : 0.741
     ↳ Thread 1 total task time (s)       : 9.063 (724987 tasks)
       ↳ Total time waiting for locks (s) : 1.793
       ↳ Total time in CSes (s)           : 1.01
     ↳ Thread 2 total task time (s)       : 9.088 (732078 tasks)
       ↳ Total time waiting for locks (s) : 1.809
       ↳ Total time in CSes (s)           : 1.018
     ↳ Thread 3 total task time (s)       : 9.068 (739103 tasks)
       ↳ Total time waiting for locks (s) : 1.804
       ↳ Total time in CSes (s)           : 1.007
     ↳ Thread 4 total task time (s)       : 9.145 (655932 tasks)
       ↳ Total time waiting for locks (s) : 1.622
       ↳ Total time in CSes (s)           : 1.206
     ↳ Thread 5 total task time (s)       : 9.153 (652405 tasks)
       ↳ Total time waiting for locks (s) : 1.625
       ↳ Total time in CSes (s)           : 1.285
     ↳ Thread 6 total task time (s)       : 9.149 (657977 tasks)
       ↳ Total time waiting for locks (s) : 1.649
       ↳ Total time in CSes (s)           : 1.234
     ↳ Thread 7 total task time (s)       : 9.12 (663895 tasks)
       ↳ Total time waiting for locks (s) : 1.676
       ↳ Total time in CSes (s)           : 1.232
     ↳ Thread 8 total task time (s)       : 9.128 (670724 tasks)
       ↳ Total time waiting for locks (s) : 1.656
       ↳ Total time in CSes (s)           : 1.255
     ↳ Thread 9 total task time (s)       : 9.134 (669336 tasks)
       ↳ Total time waiting for locks (s) : 1.649
       ↳ Total time in CSes (s)           : 1.206
     ↳ Thread 10 total task time (s)       : 9.139 (662190 tasks)
       ↳ Total time waiting for locks (s) : 1.64
       ↳ Total time in CSes (s)           : 1.201
     ↳ Thread 11 total task time (s)       : 9.132 (667460 tasks)
       ↳ Total time waiting for locks (s) : 1.653
       ↳ Total time in CSes (s)           : 1.263
     ↳ Thread 12 total task time (s)       : 9.095 (689466 tasks)
       ↳ Total time waiting for locks (s) : 1.701
       ↳ Total time in CSes (s)           : 1.198
     ↳ Thread 13 total task time (s)       : 9.137 (696103 tasks)
       ↳ Total time waiting for locks (s) : 1.714
       ↳ Total time in CSes (s)           : 1.212
     ↳ Thread 14 total task time (s)       : 9.103 (685966 tasks)
       ↳ Total time waiting for locks (s) : 1.687
       ↳ Total time in CSes (s)           : 1.211
     ↳ Thread 15 total task time (s)       : 9.012 (683393 tasks)
       ↳ Total time waiting for locks (s) : 1.687
       ↳ Total time in CSes (s)           : 1.151
    Total elimWinOrDrawClass() time (s)   : 0.077
     ↳ Thread 0 total task time (s)       : 0.017 (33 tasks)
     ↳ Thread 1 total task time (s)       : 0.008 (9 tasks)
     ↳ Thread 2 total task time (s)       : 0.017 (5 tasks)
     ↳ Thread 3 total task time (s)       : 0.006 (13 tasks)
     ↳ Thread 4 total task time (s)       : 0.005 (10 tasks)
     ↳ Thread 5 total task time (s)       : 0.005 (6 tasks)
     ↳ Thread 6 total task time (s)       : 0.01 (5 tasks)
     ↳ Thread 7 total task time (s)       : 0 (7 tasks)
     ↳ Thread 8 total task time (s)       : 0.001 (4 tasks)
     ↳ Thread 9 total task time (s)       : 0.008 (12 tasks)
     ↳ Thread 10 total task time (s)       : 0.016 (12 tasks)
     ↳ Thread 11 total task time (s)       : 0.004 (3 tasks)
     ↳ Thread 12 total task time (s)       : 0.008 (7 tasks)
     ↳ Thread 13 total task time (s)       : 0.002 (7 tasks)
     ↳ Thread 14 total task time (s)       : 0.001 (8 tasks)
     ↳ Thread 15 total task time (s)       : 0.013 (12 tasks)
    ```
  - Fully moved to block tasks (of factor 10)! `-l 4 -T 16 -L 1000 -s`: (total 20.767s) ! Without `speedcheck` this drops to about ~15s !
    ```
    Total initClass() time (s)            : 0.045
     ↳ Thread 0 total task time (s)       : 0.017 (25 tasks)
     ↳ Thread 1 total task time (s)       : 0.003 (7 tasks)
     ↳ Thread 2 total task time (s)       : 0.005 (12 tasks)
     ↳ Thread 3 total task time (s)       : 0.003 (8 tasks)
     ↳ Thread 4 total task time (s)       : 0.003 (6 tasks)
     ↳ Thread 5 total task time (s)       : 0.004 (7 tasks)
     ↳ Thread 6 total task time (s)       : 0.006 (7 tasks)
     ↳ Thread 7 total task time (s)       : 0.005 (9 tasks)
     ↳ Thread 8 total task time (s)       : 0 (7 tasks)
     ↳ Thread 9 total task time (s)       : 0.001 (6 tasks)
     ↳ Thread 10 total task time (s)       : 0.004 (12 tasks)
     ↳ Thread 11 total task time (s)       : 0.003 (7 tasks)
     ↳ Thread 12 total task time (s)       : 0.009 (14 tasks)
     ↳ Thread 13 total task time (s)       : 0.008 (7 tasks)
     ↳ Thread 14 total task time (s)       : 0.007 (11 tasks)
     ↳ Thread 15 total task time (s)       : 0.001 (8 tasks)
    Total checkTerminalsClass() time (s)  : 2.69
     ↳ Thread 0 total task time (s)       : 2.635 (4055 tasks)
       ↳ Total time waiting for locks (s) : 0.327
       ↳ Total time in CSes (s)           : 0.308
     ↳ Thread 1 total task time (s)       : 2.64 (1268 tasks)
       ↳ Total time waiting for locks (s) : 0.493
       ↳ Total time in CSes (s)           : 0.326
     ↳ Thread 2 total task time (s)       : 2.644 (1355 tasks)
       ↳ Total time waiting for locks (s) : 0.505
       ↳ Total time in CSes (s)           : 0.321
     ↳ Thread 3 total task time (s)       : 2.643 (1608 tasks)
       ↳ Total time waiting for locks (s) : 0.514
       ↳ Total time in CSes (s)           : 0.321
     ↳ Thread 4 total task time (s)       : 2.63 (1173 tasks)
       ↳ Total time waiting for locks (s) : 0.467
       ↳ Total time in CSes (s)           : 0.374
     ↳ Thread 5 total task time (s)       : 2.645 (1179 tasks)
       ↳ Total time waiting for locks (s) : 0.465
       ↳ Total time in CSes (s)           : 0.377
     ↳ Thread 6 total task time (s)       : 2.649 (1174 tasks)
       ↳ Total time waiting for locks (s) : 0.464
       ↳ Total time in CSes (s)           : 0.375
     ↳ Thread 7 total task time (s)       : 2.647 (1145 tasks)
       ↳ Total time waiting for locks (s) : 0.471
       ↳ Total time in CSes (s)           : 0.376
     ↳ Thread 8 total task time (s)       : 2.644 (1347 tasks)
       ↳ Total time waiting for locks (s) : 0.468
       ↳ Total time in CSes (s)           : 0.381
     ↳ Thread 9 total task time (s)       : 2.637 (1159 tasks)
       ↳ Total time waiting for locks (s) : 0.46
       ↳ Total time in CSes (s)           : 0.365
     ↳ Thread 10 total task time (s)       : 2.641 (1297 tasks)
       ↳ Total time waiting for locks (s) : 0.463
       ↳ Total time in CSes (s)           : 0.368
     ↳ Thread 11 total task time (s)       : 2.622 (1196 tasks)
       ↳ Total time waiting for locks (s) : 0.463
       ↳ Total time in CSes (s)           : 0.373
     ↳ Thread 12 total task time (s)       : 2.648 (1255 tasks)
       ↳ Total time waiting for locks (s) : 0.479
       ↳ Total time in CSes (s)           : 0.359
     ↳ Thread 13 total task time (s)       : 2.647 (1149 tasks)
       ↳ Total time waiting for locks (s) : 0.471
       ↳ Total time in CSes (s)           : 0.354
     ↳ Thread 14 total task time (s)       : 2.651 (1290 tasks)
       ↳ Total time waiting for locks (s) : 0.475
       ↳ Total time in CSes (s)           : 0.37
     ↳ Thread 15 total task time (s)       : 2.656 (1249 tasks)
       ↳ Total time waiting for locks (s) : 0.472
       ↳ Total time in CSes (s)           : 0.367
    Total parentLinkCacheClass() time (s) : 7.799
     ↳ Thread 0 total task time (s)       : 7.48 (806 tasks)
       ↳ Total time waiting for locks (s) : 1.548
       ↳ Total time in CSes (s)           : 0.938
     ↳ Thread 1 total task time (s)       : 7.517 (1405 tasks)
       ↳ Total time waiting for locks (s) : 1.624
       ↳ Total time in CSes (s)           : 0.889
     ↳ Thread 2 total task time (s)       : 7.53 (1371 tasks)
       ↳ Total time waiting for locks (s) : 1.682
       ↳ Total time in CSes (s)           : 0.889
     ↳ Thread 3 total task time (s)       : 7.557 (1396 tasks)
       ↳ Total time waiting for locks (s) : 1.693
       ↳ Total time in CSes (s)           : 0.896
     ↳ Thread 4 total task time (s)       : 7.588 (1324 tasks)
       ↳ Total time waiting for locks (s) : 1.566
       ↳ Total time in CSes (s)           : 1.066
     ↳ Thread 5 total task time (s)       : 7.606 (1279 tasks)
       ↳ Total time waiting for locks (s) : 1.555
       ↳ Total time in CSes (s)           : 1.061
     ↳ Thread 6 total task time (s)       : 7.623 (1303 tasks)
       ↳ Total time waiting for locks (s) : 1.511
       ↳ Total time in CSes (s)           : 1.075
     ↳ Thread 7 total task time (s)       : 7.454 (1308 tasks)
       ↳ Total time waiting for locks (s) : 1.51
       ↳ Total time in CSes (s)           : 1.052
     ↳ Thread 8 total task time (s)       : 7.548 (1296 tasks)
       ↳ Total time waiting for locks (s) : 1.538
       ↳ Total time in CSes (s)           : 1.07
     ↳ Thread 9 total task time (s)       : 7.585 (1372 tasks)
       ↳ Total time waiting for locks (s) : 1.556
       ↳ Total time in CSes (s)           : 1.063
     ↳ Thread 10 total task time (s)       : 7.671 (1331 tasks)
       ↳ Total time waiting for locks (s) : 1.553
       ↳ Total time in CSes (s)           : 1.055
     ↳ Thread 11 total task time (s)       : 7.368 (1341 tasks)
       ↳ Total time waiting for locks (s) : 1.465
       ↳ Total time in CSes (s)           : 1.032
     ↳ Thread 12 total task time (s)       : 7.461 (1358 tasks)
       ↳ Total time waiting for locks (s) : 1.534
       ↳ Total time in CSes (s)           : 0.982
     ↳ Thread 13 total task time (s)       : 7.501 (1321 tasks)
       ↳ Total time waiting for locks (s) : 1.548
       ↳ Total time in CSes (s)           : 0.976
     ↳ Thread 14 total task time (s)       : 7.624 (1341 tasks)
       ↳ Total time waiting for locks (s) : 1.55
       ↳ Total time in CSes (s)           : 1.045
     ↳ Thread 15 total task time (s)       : 7.617 (1313 tasks)
       ↳ Total time waiting for locks (s) : 1.552
       ↳ Total time in CSes (s)           : 1.023
    Total valueIterateClass() time (s)    : 9.241
     ↳ Thread 0 total task time (s)       : 8.758 (8949 tasks)
       ↳ Total time waiting for locks (s) : 1.599
       ↳ Total time in CSes (s)           : 1.03
     ↳ Thread 1 total task time (s)       : 8.695 (2764 tasks)
       ↳ Total time waiting for locks (s) : 1.634
       ↳ Total time in CSes (s)           : 0.913
     ↳ Thread 2 total task time (s)       : 8.937 (3132 tasks)
       ↳ Total time waiting for locks (s) : 1.724
       ↳ Total time in CSes (s)           : 0.925
     ↳ Thread 3 total task time (s)       : 8.904 (2743 tasks)
       ↳ Total time waiting for locks (s) : 1.707
       ↳ Total time in CSes (s)           : 0.921
     ↳ Thread 4 total task time (s)       : 9.077 (2575 tasks)
       ↳ Total time waiting for locks (s) : 1.634
       ↳ Total time in CSes (s)           : 1.132
     ↳ Thread 5 total task time (s)       : 9.055 (2583 tasks)
       ↳ Total time waiting for locks (s) : 1.606
       ↳ Total time in CSes (s)           : 1.171
     ↳ Thread 6 total task time (s)       : 9.08 (2710 tasks)
       ↳ Total time waiting for locks (s) : 1.599
       ↳ Total time in CSes (s)           : 1.203
     ↳ Thread 7 total task time (s)       : 9.013 (2720 tasks)
       ↳ Total time waiting for locks (s) : 1.622
       ↳ Total time in CSes (s)           : 1.136
     ↳ Thread 8 total task time (s)       : 9.064 (2974 tasks)
       ↳ Total time waiting for locks (s) : 1.634
       ↳ Total time in CSes (s)           : 1.135
     ↳ Thread 9 total task time (s)       : 8.854 (2735 tasks)
       ↳ Total time waiting for locks (s) : 1.551
       ↳ Total time in CSes (s)           : 1.185
     ↳ Thread 10 total task time (s)       : 9.108 (2873 tasks)
       ↳ Total time waiting for locks (s) : 1.61
       ↳ Total time in CSes (s)           : 1.189
     ↳ Thread 11 total task time (s)       : 8.805 (2700 tasks)
       ↳ Total time waiting for locks (s) : 1.558
       ↳ Total time in CSes (s)           : 1.115
     ↳ Thread 12 total task time (s)       : 8.945 (3062 tasks)
       ↳ Total time waiting for locks (s) : 1.619
       ↳ Total time in CSes (s)           : 1.119
     ↳ Thread 13 total task time (s)       : 8.939 (2743 tasks)
       ↳ Total time waiting for locks (s) : 1.625
       ↳ Total time in CSes (s)           : 1.083
     ↳ Thread 14 total task time (s)       : 8.984 (2937 tasks)
       ↳ Total time waiting for locks (s) : 1.623
       ↳ Total time in CSes (s)           : 1.094
     ↳ Thread 15 total task time (s)       : 8.986 (2971 tasks)
       ↳ Total time waiting for locks (s) : 1.607
       ↳ Total time in CSes (s)           : 1.081
    Total elimWinOrDrawClass() time (s)   : 0.079
     ↳ Thread 0 total task time (s)       : 0.022 (24 tasks)
     ↳ Thread 1 total task time (s)       : 0.005 (8 tasks)
     ↳ Thread 2 total task time (s)       : 0.005 (12 tasks)
     ↳ Thread 3 total task time (s)       : 0.002 (8 tasks)
     ↳ Thread 4 total task time (s)       : 0.007 (9 tasks)
     ↳ Thread 5 total task time (s)       : 0.003 (6 tasks)
     ↳ Thread 6 total task time (s)       : 0.013 (11 tasks)
     ↳ Thread 7 total task time (s)       : 0.004 (4 tasks)
     ↳ Thread 8 total task time (s)       : 0.002 (11 tasks)
     ↳ Thread 9 total task time (s)       : 0.007 (14 tasks)
     ↳ Thread 10 total task time (s)       : 0.017 (9 tasks)
     ↳ Thread 11 total task time (s)       : 0.004 (10 tasks)
     ↳ Thread 12 total task time (s)       : 0.015 (9 tasks)
     ↳ Thread 13 total task time (s)       : 0 (2 tasks)
     ↳ Thread 14 total task time (s)       : 0.007 (6 tasks)
     ↳ Thread 15 total task time (s)       : 0.01 (10 tasks)
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
