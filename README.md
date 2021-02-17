# Quixo Project for CPSC 490

[Project Proposal](https://docs.google.com/document/d/1R93C6XVNl6pDwXdOTlAuGolf03njDcPjZosSgq-IqNI/edit#)

## Building and Running

### Requirements
- [TCLAP](http://tclap.sourceforge.net/manual.html) for command line argument processing.
  - For MacOS, this can be downloaded from [SourceForge](https://sourceforge.net/projects/tclap/files/), and the `*/include/tclap` folder can be copied to `./include` (which is `.gitignore`-ed).
- [GLFW 3](https://www.glfw.org/) for graphical interface.
  - For MacOS, this can be done simply with `brew install glfw3`.

### Usage
- `make && ./bin/quixo -h` brings up the possible usages.
- Some common usages:
  - Pre-computing:
    - `make purge`: clears any cached data.
    - `./bin/quixo -p opt-compute -l 2`: computes optimal moves for the optimal player for a 2X2 board.
    - `./bin/quixo -p opt-compute -l 3`: computes optimal moves for the optimal player for a 3X3 board.
    - `./bin/quixo -p opt-compute -l 4`: computes optimal moves for the optimal player for a 4X4 board.
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

## To Do / Next Steps
1. **Verify and/or visualize the optimal solution as currently computed.** Optimal computation now works for up to 3X3 Quixo. It _seems_ to be correct, but some visualization of the results (i.e. another module) might help to verify this to an extent. For example, the frequency of each type of state (i.e. winning, losing, or draw) in each class (i.e. number of Xs and Os). Or, reading random states and evaluating their types (whilst displaying these to the user).
2. **Check RAM memory usage.** In particular, ensure that the _loading_ and _unloading_ of results for each class works as intended. Possibly optimize further by chunking `results` into bytes during runtime (in addition to for storage). Verify whether this time-memory trade-off is even useful (using a smaller cache size).
3. **Improve the speed of optimal computation.** We need 4X4 (and eventually 5X5) Quixo to be solved in a reasonable time. Use tips from the referenced '[Quixo Is Solved](https://arxiv.org/abs/2007.15895)', including the win-or-draw technique, and parallel computation. Possibly find other optimizations methods.
4. **Run the optimal solution finder for 5X5 Quixo.** Run the full computation for 5X5 Quixo. Find a reasonable compromise between time and space, noting that storing the results of all 3<sup>25</sup> states requires at least 3<sup>25</sup>/4 bytes, which is about 200GB. Possibly, only persistently store results for states at periodic intervals (e.g. `numTiles` of `25`, `20`, `15`, `10`, `5`, `0`), and do some amount of value iteration at runtime instead. Possibly request the use of Yale's HPC (both for runtime and for storage). This might take a good amount of compute time.
5. **Develop other playing agents.** Some ideas include MCTS, Q-learning, some simple human-based heuristic, or a neural network trained on either optimal and/or MCTS.
6. **Evaluate the playing agents.** Against optimal player primarily, but against each other might be interesting as well.
7. **Evaluate depth in Quixo.** Referencing '[Depth in Strategic Games](https://www.semanticscholar.org/paper/Depth-in-Strategic-Games-Lantz-Isaksen/4dedc67aa2191731bf8cf1822d42cea290e73073)', compare the learning rates of non-optimal playing agents between 3X3, 4X4, and 5X5 Quixo. Graph where x-axis is training time or number of training iterations, and y-axis is number of steps to lose OR how much randomization needs to be injected to optimal to make it lose OR percentage of correct moves.
8. _(Stretch)_ **Extensions of Quixo.** As mentioned in '[Quixo Is Solved](https://arxiv.org/abs/2007.15895)', there are extensions of Quixo (such as winning length being different from board length). These can be investigated if time permits.
