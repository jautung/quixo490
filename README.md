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
  - Fully moved to block tasks (of factor 10)! `-l 4 -T 16 -L 1000 -s`: (total 25.677s) ! Without `speedcheck` this drops to about 16.485s !
    ```
    Total initClass() time (s)            : 0.011
     ↳ Thread 0 total task time (s)       : 0.004 (30 tasks)
     ↳ Thread 1 total task time (s)       : 0 (7 tasks)
     ↳ Thread 2 total task time (s)       : 0.001 (7 tasks)
     ↳ Thread 3 total task time (s)       : 0.002 (10 tasks)
     ↳ Thread 4 total task time (s)       : 0 (9 tasks)
     ↳ Thread 5 total task time (s)       : 0 (6 tasks)
     ↳ Thread 6 total task time (s)       : 0.001 (10 tasks)
     ↳ Thread 7 total task time (s)       : 0 (4 tasks)
     ↳ Thread 8 total task time (s)       : 0.001 (9 tasks)
     ↳ Thread 9 total task time (s)       : 0 (13 tasks)
     ↳ Thread 10 total task time (s)       : 0 (6 tasks)
     ↳ Thread 11 total task time (s)       : 0 (3 tasks)
     ↳ Thread 12 total task time (s)       : 0.003 (16 tasks)
     ↳ Thread 13 total task time (s)       : 0.001 (8 tasks)
     ↳ Thread 14 total task time (s)       : 0 (7 tasks)
     ↳ Thread 15 total task time (s)       : 0 (8 tasks)
    Total checkTerminalsClass() time (s)  : 2.685
     ↳ Thread 0 total task time (s)       : 2.55 (3331 tasks)
       ↳ Total time waiting for locks (s) : 0.291
       ↳ Total time in CSes (s)           : 0.297
     ↳ Thread 1 total task time (s)       : 2.638 (1306 tasks)
       ↳ Total time waiting for locks (s) : 0.491
       ↳ Total time in CSes (s)           : 0.325
     ↳ Thread 2 total task time (s)       : 2.633 (1308 tasks)
       ↳ Total time waiting for locks (s) : 0.483
       ↳ Total time in CSes (s)           : 0.328
     ↳ Thread 3 total task time (s)       : 2.627 (1593 tasks)
       ↳ Total time waiting for locks (s) : 0.495
       ↳ Total time in CSes (s)           : 0.323
     ↳ Thread 4 total task time (s)       : 2.619 (1331 tasks)
       ↳ Total time waiting for locks (s) : 0.455
       ↳ Total time in CSes (s)           : 0.36
     ↳ Thread 5 total task time (s)       : 2.58 (1057 tasks)
       ↳ Total time waiting for locks (s) : 0.445
       ↳ Total time in CSes (s)           : 0.361
     ↳ Thread 6 total task time (s)       : 2.636 (1235 tasks)
       ↳ Total time waiting for locks (s) : 0.449
       ↳ Total time in CSes (s)           : 0.373
     ↳ Thread 7 total task time (s)       : 2.616 (1241 tasks)
       ↳ Total time waiting for locks (s) : 0.459
       ↳ Total time in CSes (s)           : 0.364
     ↳ Thread 8 total task time (s)       : 2.64 (1220 tasks)
       ↳ Total time waiting for locks (s) : 0.447
       ↳ Total time in CSes (s)           : 0.365
     ↳ Thread 9 total task time (s)       : 2.632 (1260 tasks)
       ↳ Total time waiting for locks (s) : 0.463
       ↳ Total time in CSes (s)           : 0.359
     ↳ Thread 10 total task time (s)       : 2.622 (1173 tasks)
       ↳ Total time waiting for locks (s) : 0.453
       ↳ Total time in CSes (s)           : 0.367
     ↳ Thread 11 total task time (s)       : 2.624 (1251 tasks)
       ↳ Total time waiting for locks (s) : 0.457
       ↳ Total time in CSes (s)           : 0.374
     ↳ Thread 12 total task time (s)       : 2.64 (1548 tasks)
       ↳ Total time waiting for locks (s) : 0.477
       ↳ Total time in CSes (s)           : 0.358
     ↳ Thread 13 total task time (s)       : 2.629 (1225 tasks)
       ↳ Total time waiting for locks (s) : 0.461
       ↳ Total time in CSes (s)           : 0.356
     ↳ Thread 14 total task time (s)       : 2.627 (1299 tasks)
       ↳ Total time waiting for locks (s) : 0.466
       ↳ Total time in CSes (s)           : 0.356
     ↳ Thread 15 total task time (s)       : 2.623 (1521 tasks)
       ↳ Total time waiting for locks (s) : 0.472
       ↳ Total time in CSes (s)           : 0.355
    Total parentLinkCacheClass() time (s) : 8.246
     ↳ Thread 0 total task time (s)       : 8.011 (682 tasks)
       ↳ Total time waiting for locks (s) : 1.875
       ↳ Total time in CSes (s)           : 0.972
     ↳ Thread 1 total task time (s)       : 8.024 (1438 tasks)
       ↳ Total time waiting for locks (s) : 2.003
       ↳ Total time in CSes (s)           : 0.922
     ↳ Thread 2 total task time (s)       : 7.903 (1411 tasks)
       ↳ Total time waiting for locks (s) : 1.99
       ↳ Total time in CSes (s)           : 0.899
     ↳ Thread 3 total task time (s)       : 7.909 (1328 tasks)
       ↳ Total time waiting for locks (s) : 2.016
       ↳ Total time in CSes (s)           : 0.912
     ↳ Thread 4 total task time (s)       : 7.967 (1291 tasks)
       ↳ Total time waiting for locks (s) : 1.885
       ↳ Total time in CSes (s)           : 1.087
     ↳ Thread 5 total task time (s)       : 7.919 (1260 tasks)
       ↳ Total time waiting for locks (s) : 1.879
       ↳ Total time in CSes (s)           : 1.078
     ↳ Thread 6 total task time (s)       : 7.916 (1378 tasks)
       ↳ Total time waiting for locks (s) : 1.858
       ↳ Total time in CSes (s)           : 1.08
     ↳ Thread 7 total task time (s)       : 8.117 (1356 tasks)
       ↳ Total time waiting for locks (s) : 1.931
       ↳ Total time in CSes (s)           : 1.08
     ↳ Thread 8 total task time (s)       : 8.034 (1334 tasks)
       ↳ Total time waiting for locks (s) : 1.917
       ↳ Total time in CSes (s)           : 1.07
     ↳ Thread 9 total task time (s)       : 7.999 (1337 tasks)
       ↳ Total time waiting for locks (s) : 1.888
       ↳ Total time in CSes (s)           : 1.08
     ↳ Thread 10 total task time (s)       : 8.029 (1323 tasks)
       ↳ Total time waiting for locks (s) : 1.893
       ↳ Total time in CSes (s)           : 1.082
     ↳ Thread 11 total task time (s)       : 7.91 (1326 tasks)
       ↳ Total time waiting for locks (s) : 1.836
       ↳ Total time in CSes (s)           : 1.1
     ↳ Thread 12 total task time (s)       : 8.016 (1382 tasks)
       ↳ Total time waiting for locks (s) : 1.908
       ↳ Total time in CSes (s)           : 1.075
     ↳ Thread 13 total task time (s)       : 7.977 (1327 tasks)
       ↳ Total time waiting for locks (s) : 1.92
       ↳ Total time in CSes (s)           : 1.034
     ↳ Thread 14 total task time (s)       : 7.97 (1363 tasks)
       ↳ Total time waiting for locks (s) : 1.927
       ↳ Total time in CSes (s)           : 1.047
     ↳ Thread 15 total task time (s)       : 7.978 (1329 tasks)
       ↳ Total time waiting for locks (s) : 1.938
       ↳ Total time in CSes (s)           : 1.068
    Total valueIterateClass() time (s)    : 13.584
     ↳ Thread 0 total task time (s)       : 13.072 (8295 tasks)
       ↳ Total time waiting for locks (s) : 3.385
       ↳ Total time in CSes (s)           : 2.741
     ↳ Thread 1 total task time (s)       : 12.997 (3327 tasks)
       ↳ Total time waiting for locks (s) : 3.528
       ↳ Total time in CSes (s)           : 1.731
     ↳ Thread 2 total task time (s)       : 13.02 (3300 tasks)
       ↳ Total time waiting for locks (s) : 3.485
       ↳ Total time in CSes (s)           : 1.718
     ↳ Thread 3 total task time (s)       : 13.084 (3280 tasks)
       ↳ Total time waiting for locks (s) : 3.536
       ↳ Total time in CSes (s)           : 1.747
     ↳ Thread 4 total task time (s)       : 13.164 (2604 tasks)
       ↳ Total time waiting for locks (s) : 3.091
       ↳ Total time in CSes (s)           : 2.086
     ↳ Thread 5 total task time (s)       : 13.164 (2418 tasks)
       ↳ Total time waiting for locks (s) : 3.094
       ↳ Total time in CSes (s)           : 2.089
     ↳ Thread 6 total task time (s)       : 13.135 (2686 tasks)
       ↳ Total time waiting for locks (s) : 3.089
       ↳ Total time in CSes (s)           : 2.089
     ↳ Thread 7 total task time (s)       : 13.189 (2612 tasks)
       ↳ Total time waiting for locks (s) : 3.084
       ↳ Total time in CSes (s)           : 2.134
     ↳ Thread 8 total task time (s)       : 13.125 (2623 tasks)
       ↳ Total time waiting for locks (s) : 3.068
       ↳ Total time in CSes (s)           : 2.092
     ↳ Thread 9 total task time (s)       : 13.14 (2670 tasks)
       ↳ Total time waiting for locks (s) : 3.074
       ↳ Total time in CSes (s)           : 2.089
     ↳ Thread 10 total task time (s)       : 13.134 (2822 tasks)
       ↳ Total time waiting for locks (s) : 3.088
       ↳ Total time in CSes (s)           : 2.187
     ↳ Thread 11 total task time (s)       : 13.219 (2631 tasks)
       ↳ Total time waiting for locks (s) : 3.118
       ↳ Total time in CSes (s)           : 2.112
     ↳ Thread 12 total task time (s)       : 13.104 (2858 tasks)
       ↳ Total time waiting for locks (s) : 3.237
       ↳ Total time in CSes (s)           : 1.961
     ↳ Thread 13 total task time (s)       : 12.988 (2795 tasks)
       ↳ Total time waiting for locks (s) : 3.185
       ↳ Total time in CSes (s)           : 1.994
     ↳ Thread 14 total task time (s)       : 13.185 (2994 tasks)
       ↳ Total time waiting for locks (s) : 3.256
       ↳ Total time in CSes (s)           : 2.085
     ↳ Thread 15 total task time (s)       : 13.243 (2936 tasks)
       ↳ Total time waiting for locks (s) : 3.321
       ↳ Total time in CSes (s)           : 1.935
    Total elimWinOrDrawClass() time (s)   : 0.078
     ↳ Thread 0 total task time (s)       : 0.014 (28 tasks)
     ↳ Thread 1 total task time (s)       : 0.012 (9 tasks)
     ↳ Thread 2 total task time (s)       : 0.008 (11 tasks)
     ↳ Thread 3 total task time (s)       : 0.013 (12 tasks)
     ↳ Thread 4 total task time (s)       : 0.001 (7 tasks)
     ↳ Thread 5 total task time (s)       : 0.004 (8 tasks)
     ↳ Thread 6 total task time (s)       : 0.011 (9 tasks)
     ↳ Thread 7 total task time (s)       : 0.009 (6 tasks)
     ↳ Thread 8 total task time (s)       : 0.011 (8 tasks)
     ↳ Thread 9 total task time (s)       : 0 (2 tasks)
     ↳ Thread 10 total task time (s)       : 0.004 (5 tasks)
     ↳ Thread 11 total task time (s)       : 0.004 (13 tasks)
     ↳ Thread 12 total task time (s)       : 0.017 (12 tasks)
     ↳ Thread 13 total task time (s)       : 0.006 (8 tasks)
     ↳ Thread 14 total task time (s)       : 0.004 (7 tasks)
     ↳ Thread 15 total task time (s)       : 0.005 (8 tasks)
    ```
- Some 5X5 tests. Compute but only for 25 and 24.
  - `-p opt-compute24 -l 5 -T 16 -L 1000` (519.111s with speed checking, 196.32s without)
    ```
    Total initClass() time (s)            : 0.412
     ↳ Thread 0 total task time (s)       : 0.196 (16 tasks)
     ↳ Thread 1 total task time (s)       : 0.002 (1 tasks)
     ↳ Thread 2 total task time (s)       : 0.002 (1 tasks)
     ↳ Thread 3 total task time (s)       : 0.112 (3 tasks)
     ↳ Thread 4 total task time (s)       : 0.136 (4 tasks)
     ↳ Thread 5 total task time (s)       : 0.007 (2 tasks)
     ↳ Thread 6 total task time (s)       : 0.063 (4 tasks)
     ↳ Thread 7 total task time (s)       : 0.031 (2 tasks)
     ↳ Thread 8 total task time (s)       : 0 (1 tasks)
     ↳ Thread 9 total task time (s)       : 0 (2 tasks)
     ↳ Thread 10 total task time (s)       : 0.005 (4 tasks)
     ↳ Thread 11 total task time (s)       : 0 (1 tasks)
     ↳ Thread 12 total task time (s)       : 0.147 (2 tasks)
     ↳ Thread 13 total task time (s)       : 0.012 (3 tasks)
     ↳ Thread 14 total task time (s)       : 0.007 (1 tasks)
     ↳ Thread 15 total task time (s)       : 0.045 (4 tasks)
    Total checkTerminalsClass() time (s)  : 136.804
     ↳ Thread 0 total task time (s)       : 133.691 (2414 tasks)
       ↳ Total time waiting for locks (s) : 13.441
       ↳ Total time in CSes (s)           : 10.177
     ↳ Thread 1 total task time (s)       : 133.965 (313 tasks)
       ↳ Total time waiting for locks (s) : 18.111
       ↳ Total time in CSes (s)           : 9.575
     ↳ Thread 2 total task time (s)       : 135.608 (313 tasks)
       ↳ Total time waiting for locks (s) : 18.366
       ↳ Total time in CSes (s)           : 9.685
     ↳ Thread 3 total task time (s)       : 134.469 (311 tasks)
       ↳ Total time waiting for locks (s) : 18.334
       ↳ Total time in CSes (s)           : 9.488
     ↳ Thread 4 total task time (s)       : 133.784 (329 tasks)
       ↳ Total time waiting for locks (s) : 18.35
       ↳ Total time in CSes (s)           : 9.719
     ↳ Thread 5 total task time (s)       : 134.775 (322 tasks)
       ↳ Total time waiting for locks (s) : 18.5
       ↳ Total time in CSes (s)           : 9.739
     ↳ Thread 6 total task time (s)       : 135.161 (301 tasks)
       ↳ Total time waiting for locks (s) : 18.335
       ↳ Total time in CSes (s)           : 9.805
     ↳ Thread 7 total task time (s)       : 134.79 (320 tasks)
       ↳ Total time waiting for locks (s) : 18.819
       ↳ Total time in CSes (s)           : 9.617
     ↳ Thread 8 total task time (s)       : 134.224 (464 tasks)
       ↳ Total time waiting for locks (s) : 18.367
       ↳ Total time in CSes (s)           : 9.708
     ↳ Thread 9 total task time (s)       : 134.119 (303 tasks)
       ↳ Total time waiting for locks (s) : 18.708
       ↳ Total time in CSes (s)           : 9.611
     ↳ Thread 10 total task time (s)       : 133.531 (318 tasks)
       ↳ Total time waiting for locks (s) : 18.395
       ↳ Total time in CSes (s)           : 9.599
     ↳ Thread 11 total task time (s)       : 135.17 (331 tasks)
       ↳ Total time waiting for locks (s) : 18.215
       ↳ Total time in CSes (s)           : 9.738
     ↳ Thread 12 total task time (s)       : 134.408 (314 tasks)
       ↳ Total time waiting for locks (s) : 18.619
       ↳ Total time in CSes (s)           : 9.746
     ↳ Thread 13 total task time (s)       : 135.302 (304 tasks)
       ↳ Total time waiting for locks (s) : 18.896
       ↳ Total time in CSes (s)           : 9.633
     ↳ Thread 14 total task time (s)       : 134.934 (317 tasks)
       ↳ Total time waiting for locks (s) : 18.702
       ↳ Total time in CSes (s)           : 9.7
     ↳ Thread 15 total task time (s)       : 135.29 (328 tasks)
       ↳ Total time waiting for locks (s) : 17.245
       ↳ Total time in CSes (s)           : 9.656
    Total parentLinkCacheClass() time (s) : 25.175
     ↳ Thread 0 total task time (s)       : 24.98 (133 tasks)
       ↳ Total time waiting for locks (s) : 3.55
       ↳ Total time in CSes (s)           : 1.898
     ↳ Thread 1 total task time (s)       : 24.239 (223 tasks)
       ↳ Total time waiting for locks (s) : 3.402
       ↳ Total time in CSes (s)           : 1.717
     ↳ Thread 2 total task time (s)       : 24.204 (228 tasks)
       ↳ Total time waiting for locks (s) : 3.406
       ↳ Total time in CSes (s)           : 1.716
     ↳ Thread 3 total task time (s)       : 24.261 (238 tasks)
       ↳ Total time waiting for locks (s) : 3.405
       ↳ Total time in CSes (s)           : 1.65
     ↳ Thread 4 total task time (s)       : 24.817 (229 tasks)
       ↳ Total time waiting for locks (s) : 3.665
       ↳ Total time in CSes (s)           : 1.741
     ↳ Thread 5 total task time (s)       : 24.694 (221 tasks)
       ↳ Total time waiting for locks (s) : 3.675
       ↳ Total time in CSes (s)           : 1.737
     ↳ Thread 6 total task time (s)       : 24.616 (229 tasks)
       ↳ Total time waiting for locks (s) : 3.616
       ↳ Total time in CSes (s)           : 1.736
     ↳ Thread 7 total task time (s)       : 24.849 (230 tasks)
       ↳ Total time waiting for locks (s) : 3.717
       ↳ Total time in CSes (s)           : 1.702
     ↳ Thread 8 total task time (s)       : 24.976 (227 tasks)
       ↳ Total time waiting for locks (s) : 3.623
       ↳ Total time in CSes (s)           : 1.782
     ↳ Thread 9 total task time (s)       : 24.796 (234 tasks)
       ↳ Total time waiting for locks (s) : 3.722
       ↳ Total time in CSes (s)           : 1.709
     ↳ Thread 10 total task time (s)       : 24.083 (236 tasks)
       ↳ Total time waiting for locks (s) : 3.541
       ↳ Total time in CSes (s)           : 1.676
     ↳ Thread 11 total task time (s)       : 24.321 (227 tasks)
       ↳ Total time waiting for locks (s) : 3.53
       ↳ Total time in CSes (s)           : 1.715
     ↳ Thread 12 total task time (s)       : 24.477 (228 tasks)
       ↳ Total time waiting for locks (s) : 3.668
       ↳ Total time in CSes (s)           : 1.748
     ↳ Thread 13 total task time (s)       : 24.442 (224 tasks)
       ↳ Total time waiting for locks (s) : 3.64
       ↳ Total time in CSes (s)           : 1.737
     ↳ Thread 14 total task time (s)       : 24.349 (240 tasks)
       ↳ Total time waiting for locks (s) : 3.664
       ↳ Total time in CSes (s)           : 1.764
     ↳ Thread 15 total task time (s)       : 24.381 (224 tasks)
       ↳ Total time waiting for locks (s) : 3.533
       ↳ Total time in CSes (s)           : 1.826
    Total valueIterateClass() time (s)    : 342.57
     ↳ Thread 0 total task time (s)       : 328.227 (3945 tasks)
       ↳ Total time waiting for locks (s) : 76.881
       ↳ Total time in CSes (s)           : 53.44
     ↳ Thread 1 total task time (s)       : 337.087 (1781 tasks)
       ↳ Total time waiting for locks (s) : 75.171
       ↳ Total time in CSes (s)           : 39.663
     ↳ Thread 2 total task time (s)       : 337.323 (1809 tasks)
       ↳ Total time waiting for locks (s) : 76.486
       ↳ Total time in CSes (s)           : 39.651
     ↳ Thread 3 total task time (s)       : 337.892 (1829 tasks)
       ↳ Total time waiting for locks (s) : 75.532
       ↳ Total time in CSes (s)           : 39.665
     ↳ Thread 4 total task time (s)       : 331.569 (1434 tasks)
       ↳ Total time waiting for locks (s) : 70.345
       ↳ Total time in CSes (s)           : 40.062
     ↳ Thread 5 total task time (s)       : 333.058 (1482 tasks)
       ↳ Total time waiting for locks (s) : 71.907
       ↳ Total time in CSes (s)           : 39.587
     ↳ Thread 6 total task time (s)       : 329.847 (1450 tasks)
       ↳ Total time waiting for locks (s) : 70.91
       ↳ Total time in CSes (s)           : 39.155
     ↳ Thread 7 total task time (s)       : 330.991 (1467 tasks)
       ↳ Total time waiting for locks (s) : 71.428
       ↳ Total time in CSes (s)           : 39.391
     ↳ Thread 8 total task time (s)       : 331.564 (1434 tasks)
       ↳ Total time waiting for locks (s) : 70.419
       ↳ Total time in CSes (s)           : 39.34
     ↳ Thread 9 total task time (s)       : 333.268 (1445 tasks)
       ↳ Total time waiting for locks (s) : 71.824
       ↳ Total time in CSes (s)           : 38.839
     ↳ Thread 10 total task time (s)       : 330.33 (1465 tasks)
       ↳ Total time waiting for locks (s) : 71.229
       ↳ Total time in CSes (s)           : 38.888
     ↳ Thread 11 total task time (s)       : 329.822 (1460 tasks)
       ↳ Total time waiting for locks (s) : 70.851
       ↳ Total time in CSes (s)           : 38.954
     ↳ Thread 12 total task time (s)       : 334.537 (1592 tasks)
       ↳ Total time waiting for locks (s) : 75.009
       ↳ Total time in CSes (s)           : 38.808
     ↳ Thread 13 total task time (s)       : 327.753 (1529 tasks)
       ↳ Total time waiting for locks (s) : 73.123
       ↳ Total time in CSes (s)           : 38.186
     ↳ Thread 14 total task time (s)       : 331.518 (1559 tasks)
       ↳ Total time waiting for locks (s) : 74.667
       ↳ Total time in CSes (s)           : 38.471
     ↳ Thread 15 total task time (s)       : 334.699 (1941 tasks)
       ↳ Total time waiting for locks (s) : 74.303
       ↳ Total time in CSes (s)           : 46.511
    Total elimWinOrDrawClass() time (s)   : 0.7
     ↳ Thread 0 total task time (s)       : 0.05 (3 tasks)
     ↳ Thread 1 total task time (s)       : 0.119 (5 tasks)
     ↳ Thread 2 total task time (s)       : 0.011 (2 tasks)
     ↳ Thread 3 total task time (s)       : 0.143 (5 tasks)
     ↳ Thread 4 total task time (s)       : 0.31 (5 tasks)
     ↳ Thread 5 total task time (s)       : 0.013 (3 tasks)
     ↳ Thread 6 total task time (s)       : 0.023 (1 tasks)
     ↳ Thread 7 total task time (s)       : 0.06 (3 tasks)
     ↳ Thread 8 total task time (s)       : 0.168 (3 tasks)
     ↳ Thread 9 total task time (s)       : 0.086 (2 tasks)
     ↳ Thread 10 total task time (s)       : 0.007 (3 tasks)
     ↳ Thread 11 total task time (s)       : 0 (2 tasks)
     ↳ Thread 12 total task time (s)       : 0.013 (2 tasks)
     ↳ Thread 13 total task time (s)       : 0 (3 tasks)
     ↳ Thread 14 total task time (s)       : 0.01 (4 tasks)
     ↳ Thread 15 total task time (s)       : 0.195 (5 tasks)

    Auxiliary functions:
     ↳ numStatesClass():
       ↳ Thread 0 total time (s)          : 0
       ↳ Thread 1 total time (s)          : 0
       ↳ Thread 2 total time (s)          : 0
       ↳ Thread 3 total time (s)          : 0
       ↳ Thread 4 total time (s)          : 0
       ↳ Thread 5 total time (s)          : 0
       ↳ Thread 6 total time (s)          : 0
       ↳ Thread 7 total time (s)          : 0
       ↳ Thread 8 total time (s)          : 0
       ↳ Thread 9 total time (s)          : 0
       ↳ Thread 10 total time (s)          : 0
       ↳ Thread 11 total time (s)          : 0
       ↳ Thread 12 total time (s)          : 0
       ↳ Thread 13 total time (s)          : 0
       ↳ Thread 14 total time (s)          : 0
       ↳ Thread 15 total time (s)          : 0
     ↳ indexToState():
       ↳ Thread 0 total time (s)          : 62.423
       ↳ Thread 1 total time (s)          : 24.861
       ↳ Thread 2 total time (s)          : 25.736
       ↳ Thread 3 total time (s)          : 24.794
       ↳ Thread 4 total time (s)          : 25.001
       ↳ Thread 5 total time (s)          : 23.354
       ↳ Thread 6 total time (s)          : 24.83
       ↳ Thread 7 total time (s)          : 23.451
       ↳ Thread 8 total time (s)          : 24.132
       ↳ Thread 9 total time (s)          : 23.209
       ↳ Thread 10 total time (s)          : 22.663
       ↳ Thread 11 total time (s)          : 24.75
       ↳ Thread 12 total time (s)          : 24.696
       ↳ Thread 13 total time (s)          : 24.006
       ↳ Thread 14 total time (s)          : 24.461
       ↳ Thread 15 total time (s)          : 31.78
     ↳ stateToIndex0():
       ↳ Thread 0 total time (s)          : 17.956
       ↳ Thread 1 total time (s)          : 29.518
       ↳ Thread 2 total time (s)          : 29.545
       ↳ Thread 3 total time (s)          : 28.683
       ↳ Thread 4 total time (s)          : 27.184
       ↳ Thread 5 total time (s)          : 27.542
       ↳ Thread 6 total time (s)          : 27.294
       ↳ Thread 7 total time (s)          : 27.584
       ↳ Thread 8 total time (s)          : 27.394
       ↳ Thread 9 total time (s)          : 27.3
       ↳ Thread 10 total time (s)          : 26.977
       ↳ Thread 11 total time (s)          : 26.914
       ↳ Thread 12 total time (s)          : 28.05
       ↳ Thread 13 total time (s)          : 27.278
       ↳ Thread 14 total time (s)          : 28.268
       ↳ Thread 15 total time (s)          : 25.444
     ↳ stateToIndex1():
       ↳ Thread 0 total time (s)          : 17.956
       ↳ Thread 1 total time (s)          : 29.518
       ↳ Thread 2 total time (s)          : 29.545
       ↳ Thread 3 total time (s)          : 28.683
       ↳ Thread 4 total time (s)          : 27.184
       ↳ Thread 5 total time (s)          : 27.542
       ↳ Thread 6 total time (s)          : 27.294
       ↳ Thread 7 total time (s)          : 27.584
       ↳ Thread 8 total time (s)          : 27.394
       ↳ Thread 9 total time (s)          : 27.3
       ↳ Thread 10 total time (s)          : 26.977
       ↳ Thread 11 total time (s)          : 26.914
       ↳ Thread 12 total time (s)          : 28.05
       ↳ Thread 13 total time (s)          : 27.278
       ↳ Thread 14 total time (s)          : 28.268
       ↳ Thread 15 total time (s)          : 25.444
     ↳ stateToIndex2():
       ↳ Thread 0 total time (s)          : 14.761
       ↳ Thread 1 total time (s)          : 23.014
       ↳ Thread 2 total time (s)          : 22.907
       ↳ Thread 3 total time (s)          : 23.616
       ↳ Thread 4 total time (s)          : 22.956
       ↳ Thread 5 total time (s)          : 23.23
       ↳ Thread 6 total time (s)          : 22.914
       ↳ Thread 7 total time (s)          : 23.303
       ↳ Thread 8 total time (s)          : 22.695
       ↳ Thread 9 total time (s)          : 23.265
       ↳ Thread 10 total time (s)          : 22.989
       ↳ Thread 11 total time (s)          : 22.727
       ↳ Thread 12 total time (s)          : 23.564
       ↳ Thread 13 total time (s)          : 23.261
       ↳ Thread 14 total time (s)          : 23.778
       ↳ Thread 15 total time (s)          : 20.771
     ↳ unfilterOState():
       ↳ Thread 0 total time (s)          : 9.247
       ↳ Thread 1 total time (s)          : 3.642
       ↳ Thread 2 total time (s)          : 3.615
       ↳ Thread 3 total time (s)          : 3.603
       ↳ Thread 4 total time (s)          : 3.679
       ↳ Thread 5 total time (s)          : 3.448
       ↳ Thread 6 total time (s)          : 3.635
       ↳ Thread 7 total time (s)          : 3.51
       ↳ Thread 8 total time (s)          : 3.579
       ↳ Thread 9 total time (s)          : 3.455
       ↳ Thread 10 total time (s)          : 3.448
       ↳ Thread 11 total time (s)          : 3.544
       ↳ Thread 12 total time (s)          : 3.605
       ↳ Thread 13 total time (s)          : 3.423
       ↳ Thread 14 total time (s)          : 3.533
       ↳ Thread 15 total time (s)          : 4.365
     ↳ filterOState():
       ↳ Thread 0 total time (s)          : 17.041
       ↳ Thread 1 total time (s)          : 25.734
       ↳ Thread 2 total time (s)          : 25.736
       ↳ Thread 3 total time (s)          : 26.178
       ↳ Thread 4 total time (s)          : 25.002
       ↳ Thread 5 total time (s)          : 25.548
       ↳ Thread 6 total time (s)          : 24.776
       ↳ Thread 7 total time (s)          : 25.632
       ↳ Thread 8 total time (s)          : 25.077
       ↳ Thread 9 total time (s)          : 25.775
       ↳ Thread 10 total time (s)          : 25.047
       ↳ Thread 11 total time (s)          : 24.98
       ↳ Thread 12 total time (s)          : 25.537
       ↳ Thread 13 total time (s)          : 25.2
       ↳ Thread 14 total time (s)          : 25.544
       ↳ Thread 15 total time (s)          : 22.726
    ```
  - `-p opt-compute24 -l 5 -T 16 -L 10000` (522.378s with speed checking, 197.098s without)
    ```
    Total initClass() time (s)            : 0.472
     ↳ Thread 0 total task time (s)       : 0.175 (8 tasks)
     ↳ Thread 1 total task time (s)       : 0.01 (2 tasks)
     ↳ Thread 2 total task time (s)       : 0.019 (2 tasks)
     ↳ Thread 3 total task time (s)       : 0.112 (8 tasks)
     ↳ Thread 4 total task time (s)       : 0.113 (4 tasks)
     ↳ Thread 5 total task time (s)       : 0 (2 tasks)
     ↳ Thread 6 total task time (s)       : 0 (1 tasks)
     ↳ Thread 7 total task time (s)       : 0.179 (3 tasks)
     ↳ Thread 8 total task time (s)       : 0 (1 tasks)
     ↳ Thread 9 total task time (s)       : 0.007 (1 tasks)
     ↳ Thread 10 total task time (s)       : 0.01 (6 tasks)
     ↳ Thread 11 total task time (s)       : 0 (0 tasks)
     ↳ Thread 12 total task time (s)       : 0.003 (6 tasks)
     ↳ Thread 13 total task time (s)       : 0.004 (2 tasks)
     ↳ Thread 14 total task time (s)       : 0.01 (2 tasks)
     ↳ Thread 15 total task time (s)       : 0.262 (3 tasks)
    Total checkTerminalsClass() time (s)  : 138.856
     ↳ Thread 0 total task time (s)       : 135.72 (2145 tasks)
       ↳ Total time waiting for locks (s) : 14.572
       ↳ Total time in CSes (s)           : 10.19
     ↳ Thread 1 total task time (s)       : 136.797 (314 tasks)
       ↳ Total time waiting for locks (s) : 17.808
       ↳ Total time in CSes (s)           : 9.444
     ↳ Thread 2 total task time (s)       : 137.118 (298 tasks)
       ↳ Total time waiting for locks (s) : 17.904
       ↳ Total time in CSes (s)           : 9.528
     ↳ Thread 3 total task time (s)       : 137.595 (455 tasks)
       ↳ Total time waiting for locks (s) : 17.772
       ↳ Total time in CSes (s)           : 9.537
     ↳ Thread 4 total task time (s)       : 137.314 (485 tasks)
       ↳ Total time waiting for locks (s) : 17.924
       ↳ Total time in CSes (s)           : 9.807
     ↳ Thread 5 total task time (s)       : 136.474 (320 tasks)
       ↳ Total time waiting for locks (s) : 18.028
       ↳ Total time in CSes (s)           : 9.706
     ↳ Thread 6 total task time (s)       : 136.184 (324 tasks)
       ↳ Total time waiting for locks (s) : 18.038
       ↳ Total time in CSes (s)           : 9.72
     ↳ Thread 7 total task time (s)       : 136.26 (328 tasks)
       ↳ Total time waiting for locks (s) : 18.023
       ↳ Total time in CSes (s)           : 9.712
     ↳ Thread 8 total task time (s)       : 136.089 (368 tasks)
       ↳ Total time waiting for locks (s) : 16.709
       ↳ Total time in CSes (s)           : 9.555
     ↳ Thread 9 total task time (s)       : 137.33 (313 tasks)
       ↳ Total time waiting for locks (s) : 18.049
       ↳ Total time in CSes (s)           : 9.609
     ↳ Thread 10 total task time (s)       : 136.303 (321 tasks)
       ↳ Total time waiting for locks (s) : 17.981
       ↳ Total time in CSes (s)           : 9.678
     ↳ Thread 11 total task time (s)       : 136.215 (304 tasks)
       ↳ Total time waiting for locks (s) : 17.835
       ↳ Total time in CSes (s)           : 9.596
     ↳ Thread 12 total task time (s)       : 137.97 (377 tasks)
       ↳ Total time waiting for locks (s) : 18.047
       ↳ Total time in CSes (s)           : 9.827
     ↳ Thread 13 total task time (s)       : 137.542 (318 tasks)
       ↳ Total time waiting for locks (s) : 18.253
       ↳ Total time in CSes (s)           : 9.983
     ↳ Thread 14 total task time (s)       : 135.923 (319 tasks)
       ↳ Total time waiting for locks (s) : 18.041
       ↳ Total time in CSes (s)           : 9.724
     ↳ Thread 15 total task time (s)       : 137.403 (313 tasks)
       ↳ Total time waiting for locks (s) : 18.312
       ↳ Total time in CSes (s)           : 9.888
    Total parentLinkCacheClass() time (s) : 25.551
     ↳ Thread 0 total task time (s)       : 25.203 (139 tasks)
       ↳ Total time waiting for locks (s) : 3.258
       ↳ Total time in CSes (s)           : 1.946
     ↳ Thread 1 total task time (s)       : 24.527 (250 tasks)
       ↳ Total time waiting for locks (s) : 3.167
       ↳ Total time in CSes (s)           : 1.689
     ↳ Thread 2 total task time (s)       : 24.586 (231 tasks)
       ↳ Total time waiting for locks (s) : 3.186
       ↳ Total time in CSes (s)           : 1.751
     ↳ Thread 3 total task time (s)       : 24.496 (226 tasks)
       ↳ Total time waiting for locks (s) : 3.145
       ↳ Total time in CSes (s)           : 1.7
     ↳ Thread 4 total task time (s)       : 24.834 (244 tasks)
       ↳ Total time waiting for locks (s) : 3.474
       ↳ Total time in CSes (s)           : 1.681
     ↳ Thread 5 total task time (s)       : 24.53 (233 tasks)
       ↳ Total time waiting for locks (s) : 3.334
       ↳ Total time in CSes (s)           : 1.7
     ↳ Thread 6 total task time (s)       : 25.027 (223 tasks)
       ↳ Total time waiting for locks (s) : 3.441
       ↳ Total time in CSes (s)           : 1.754
     ↳ Thread 7 total task time (s)       : 25.1 (237 tasks)
       ↳ Total time waiting for locks (s) : 3.423
       ↳ Total time in CSes (s)           : 1.738
     ↳ Thread 8 total task time (s)       : 24.828 (203 tasks)
       ↳ Total time waiting for locks (s) : 3.285
       ↳ Total time in CSes (s)           : 1.793
     ↳ Thread 9 total task time (s)       : 25.082 (238 tasks)
       ↳ Total time waiting for locks (s) : 3.424
       ↳ Total time in CSes (s)           : 1.764
     ↳ Thread 10 total task time (s)       : 24.865 (227 tasks)
       ↳ Total time waiting for locks (s) : 3.409
       ↳ Total time in CSes (s)           : 1.697
     ↳ Thread 11 total task time (s)       : 24.974 (221 tasks)
       ↳ Total time waiting for locks (s) : 3.4
       ↳ Total time in CSes (s)           : 1.707
     ↳ Thread 12 total task time (s)       : 24.896 (207 tasks)
       ↳ Total time waiting for locks (s) : 3.391
       ↳ Total time in CSes (s)           : 1.727
     ↳ Thread 13 total task time (s)       : 24.642 (238 tasks)
       ↳ Total time waiting for locks (s) : 3.435
       ↳ Total time in CSes (s)           : 1.711
     ↳ Thread 14 total task time (s)       : 24.679 (225 tasks)
       ↳ Total time waiting for locks (s) : 3.43
       ↳ Total time in CSes (s)           : 1.735
     ↳ Thread 15 total task time (s)       : 24.671 (229 tasks)
       ↳ Total time waiting for locks (s) : 3.43
       ↳ Total time in CSes (s)           : 1.719
    Total valueIterateClass() time (s)    : 343.251
     ↳ Thread 0 total task time (s)       : 332.113 (3942 tasks)
       ↳ Total time waiting for locks (s) : 72.477
       ↳ Total time in CSes (s)           : 58.115
     ↳ Thread 1 total task time (s)       : 335.881 (1869 tasks)
       ↳ Total time waiting for locks (s) : 67.87
       ↳ Total time in CSes (s)           : 39.276
     ↳ Thread 2 total task time (s)       : 334.558 (1818 tasks)
       ↳ Total time waiting for locks (s) : 67.446
       ↳ Total time in CSes (s)           : 39.896
     ↳ Thread 3 total task time (s)       : 335.368 (1875 tasks)
       ↳ Total time waiting for locks (s) : 65.012
       ↳ Total time in CSes (s)           : 39.682
     ↳ Thread 4 total task time (s)       : 329.368 (1470 tasks)
       ↳ Total time waiting for locks (s) : 67.178
       ↳ Total time in CSes (s)           : 38.388
     ↳ Thread 5 total task time (s)       : 330.698 (1475 tasks)
       ↳ Total time waiting for locks (s) : 66.777
       ↳ Total time in CSes (s)           : 38.002
     ↳ Thread 6 total task time (s)       : 329.646 (1396 tasks)
       ↳ Total time waiting for locks (s) : 66.096
       ↳ Total time in CSes (s)           : 38.187
     ↳ Thread 7 total task time (s)       : 331.616 (1439 tasks)
       ↳ Total time waiting for locks (s) : 65.972
       ↳ Total time in CSes (s)           : 38.76
     ↳ Thread 8 total task time (s)       : 329.476 (1647 tasks)
       ↳ Total time waiting for locks (s) : 69.141
       ↳ Total time in CSes (s)           : 41.909
     ↳ Thread 9 total task time (s)       : 331.16 (1436 tasks)
       ↳ Total time waiting for locks (s) : 67.602
       ↳ Total time in CSes (s)           : 37.717
     ↳ Thread 10 total task time (s)       : 333.25 (1453 tasks)
       ↳ Total time waiting for locks (s) : 66.853
       ↳ Total time in CSes (s)           : 39.399
     ↳ Thread 11 total task time (s)       : 329.823 (1420 tasks)
       ↳ Total time waiting for locks (s) : 66.55
       ↳ Total time in CSes (s)           : 38.534
     ↳ Thread 12 total task time (s)       : 331.771 (1569 tasks)
       ↳ Total time waiting for locks (s) : 69.801
       ↳ Total time in CSes (s)           : 36.505
     ↳ Thread 13 total task time (s)       : 334.252 (1590 tasks)
       ↳ Total time waiting for locks (s) : 71.417
       ↳ Total time in CSes (s)           : 36.141
     ↳ Thread 14 total task time (s)       : 333.378 (1586 tasks)
       ↳ Total time waiting for locks (s) : 70.127
       ↳ Total time in CSes (s)           : 36.94
     ↳ Thread 15 total task time (s)       : 334.791 (1637 tasks)
       ↳ Total time waiting for locks (s) : 70.673
       ↳ Total time in CSes (s)           : 36.695
    Total elimWinOrDrawClass() time (s)   : 0.705
     ↳ Thread 0 total task time (s)       : 0.016 (7 tasks)
     ↳ Thread 1 total task time (s)       : 0 (0 tasks)
     ↳ Thread 2 total task time (s)       : 0.026 (3 tasks)
     ↳ Thread 3 total task time (s)       : 0 (2 tasks)
     ↳ Thread 4 total task time (s)       : 0.302 (4 tasks)
     ↳ Thread 5 total task time (s)       : 0.199 (3 tasks)
     ↳ Thread 6 total task time (s)       : 0.003 (3 tasks)
     ↳ Thread 7 total task time (s)       : 0.144 (3 tasks)
     ↳ Thread 8 total task time (s)       : 0.166 (2 tasks)
     ↳ Thread 9 total task time (s)       : 0 (1 tasks)
     ↳ Thread 10 total task time (s)       : 0.001 (2 tasks)
     ↳ Thread 11 total task time (s)       : 0.106 (3 tasks)
     ↳ Thread 12 total task time (s)       : 0.028 (4 tasks)
     ↳ Thread 13 total task time (s)       : 0.055 (3 tasks)
     ↳ Thread 14 total task time (s)       : 0.158 (7 tasks)
     ↳ Thread 15 total task time (s)       : 0.009 (4 tasks)

    Auxiliary functions:
     ↳ numStatesClass():
       ↳ Thread 0 total time (s)          : 0
       ↳ Thread 1 total time (s)          : 0
       ↳ Thread 2 total time (s)          : 0
       ↳ Thread 3 total time (s)          : 0
       ↳ Thread 4 total time (s)          : 0
       ↳ Thread 5 total time (s)          : 0
       ↳ Thread 6 total time (s)          : 0
       ↳ Thread 7 total time (s)          : 0
       ↳ Thread 8 total time (s)          : 0
       ↳ Thread 9 total time (s)          : 0
       ↳ Thread 10 total time (s)          : 0
       ↳ Thread 11 total time (s)          : 0
       ↳ Thread 12 total time (s)          : 0
       ↳ Thread 13 total time (s)          : 0
       ↳ Thread 14 total time (s)          : 0
       ↳ Thread 15 total time (s)          : 0
     ↳ indexToState():
       ↳ Thread 0 total time (s)          : 59.355
       ↳ Thread 1 total time (s)          : 25.23
       ↳ Thread 2 total time (s)          : 25.546
       ↳ Thread 3 total time (s)          : 26.872
       ↳ Thread 4 total time (s)          : 25.824
       ↳ Thread 5 total time (s)          : 23.461
       ↳ Thread 6 total time (s)          : 25.476
       ↳ Thread 7 total time (s)          : 24.508
       ↳ Thread 8 total time (s)          : 33.431
       ↳ Thread 9 total time (s)          : 24.544
       ↳ Thread 10 total time (s)          : 24.225
       ↳ Thread 11 total time (s)          : 24.554
       ↳ Thread 12 total time (s)          : 24.627
       ↳ Thread 13 total time (s)          : 24.571
       ↳ Thread 14 total time (s)          : 24.874
       ↳ Thread 15 total time (s)          : 24.506
     ↳ stateToIndex0():
       ↳ Thread 0 total time (s)          : 18.059
       ↳ Thread 1 total time (s)          : 29.209
       ↳ Thread 2 total time (s)          : 29.603
       ↳ Thread 3 total time (s)          : 29.911
       ↳ Thread 4 total time (s)          : 27.996
       ↳ Thread 5 total time (s)          : 27.942
       ↳ Thread 6 total time (s)          : 28.311
       ↳ Thread 7 total time (s)          : 28.106
       ↳ Thread 8 total time (s)          : 25.864
       ↳ Thread 9 total time (s)          : 28.211
       ↳ Thread 10 total time (s)          : 28.186
       ↳ Thread 11 total time (s)          : 27.673
       ↳ Thread 12 total time (s)          : 28.446
       ↳ Thread 13 total time (s)          : 28.761
       ↳ Thread 14 total time (s)          : 28.939
       ↳ Thread 15 total time (s)          : 29.232
     ↳ stateToIndex1():
       ↳ Thread 0 total time (s)          : 18.059
       ↳ Thread 1 total time (s)          : 29.209
       ↳ Thread 2 total time (s)          : 29.603
       ↳ Thread 3 total time (s)          : 29.911
       ↳ Thread 4 total time (s)          : 27.996
       ↳ Thread 5 total time (s)          : 27.942
       ↳ Thread 6 total time (s)          : 28.311
       ↳ Thread 7 total time (s)          : 28.106
       ↳ Thread 8 total time (s)          : 25.864
       ↳ Thread 9 total time (s)          : 28.211
       ↳ Thread 10 total time (s)          : 28.186
       ↳ Thread 11 total time (s)          : 27.673
       ↳ Thread 12 total time (s)          : 28.446
       ↳ Thread 13 total time (s)          : 28.761
       ↳ Thread 14 total time (s)          : 28.939
       ↳ Thread 15 total time (s)          : 29.232
     ↳ stateToIndex2():
       ↳ Thread 0 total time (s)          : 14.288
       ↳ Thread 1 total time (s)          : 23.47
       ↳ Thread 2 total time (s)          : 23.555
       ↳ Thread 3 total time (s)          : 24.32
       ↳ Thread 4 total time (s)          : 23.679
       ↳ Thread 5 total time (s)          : 23.816
       ↳ Thread 6 total time (s)          : 23.681
       ↳ Thread 7 total time (s)          : 23.159
       ↳ Thread 8 total time (s)          : 21.054
       ↳ Thread 9 total time (s)          : 24.355
       ↳ Thread 10 total time (s)          : 23.921
       ↳ Thread 11 total time (s)          : 23.306
       ↳ Thread 12 total time (s)          : 24.246
       ↳ Thread 13 total time (s)          : 25.203
       ↳ Thread 14 total time (s)          : 24.712
       ↳ Thread 15 total time (s)          : 25.063
     ↳ unfilterOState():
       ↳ Thread 0 total time (s)          : 8.771
       ↳ Thread 1 total time (s)          : 3.64
       ↳ Thread 2 total time (s)          : 3.666
       ↳ Thread 3 total time (s)          : 3.684
       ↳ Thread 4 total time (s)          : 3.758
       ↳ Thread 5 total time (s)          : 3.532
       ↳ Thread 6 total time (s)          : 3.636
       ↳ Thread 7 total time (s)          : 3.711
       ↳ Thread 8 total time (s)          : 4.898
       ↳ Thread 9 total time (s)          : 3.603
       ↳ Thread 10 total time (s)          : 3.593
       ↳ Thread 11 total time (s)          : 3.588
       ↳ Thread 12 total time (s)          : 3.606
       ↳ Thread 13 total time (s)          : 3.601
       ↳ Thread 14 total time (s)          : 3.693
       ↳ Thread 15 total time (s)          : 3.558
     ↳ filterOState():
       ↳ Thread 0 total time (s)          : 16.176
       ↳ Thread 1 total time (s)          : 25.933
       ↳ Thread 2 total time (s)          : 26.597
       ↳ Thread 3 total time (s)          : 26.61
       ↳ Thread 4 total time (s)          : 25.674
       ↳ Thread 5 total time (s)          : 25.825
       ↳ Thread 6 total time (s)          : 24.803
       ↳ Thread 7 total time (s)          : 25.678
       ↳ Thread 8 total time (s)          : 23.147
       ↳ Thread 9 total time (s)          : 25.78
       ↳ Thread 10 total time (s)          : 25.864
       ↳ Thread 11 total time (s)          : 25.507
       ↳ Thread 12 total time (s)          : 25.776
       ↳ Thread 13 total time (s)          : 26.284
       ↳ Thread 14 total time (s)          : 26.249
       ↳ Thread 15 total time (s)          : 26.452
    ```
    - Locks still seem slow, tried implementing reader-writer lock using [Raynal's method](https://en.wikipedia.org/wiki/Readers%E2%80%93writer_lock); speed decreased significantly (from 3:24.49 to 4:46.10), so code was reverted to the simple lock. I suppose the overhead of using two locks per lock is too high...

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
