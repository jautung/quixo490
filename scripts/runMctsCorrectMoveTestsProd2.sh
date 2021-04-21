#!/bin/bash
DATE=$(date +'%Y-%m-%d-%H%M%S')
LOGFILE="${DATE}-correct-move-tests.log"

N_STATES_EACH=10000
FLAGS_BASE='-p test-move-correctness -N '$N_STATES_EACH' -v correct-move-tests'

export OPT_COMPUTE_ERROR_CHECKING=0
export OPT_COMPUTE_SPEED_CHECKING=0
export OPT_COMPUTE_MEMORY_CHECKING=0
export MCTS_CACHE_HIT_CHECK=0

clear
make clean
make
clear

touch logs/$LOGFILE

echo '=== START ==='

echo 'N_STATES_EACH:' $N_STATES_EACH >> logs/$LOGFILE
echo '' >> logs/$LOGFILE
echo 'len	xType	fracCorrect	fracCorrectFromLoss	fracCorrectFromDraw	fracCorrectFromWin' >> logs/$LOGFILE

# -l 3
echo '-l 3 -X mcts0,2500' ; ./bin/quixo $FLAGS_BASE -l 3 -X mcts0,2500 >> logs/$LOGFILE
echo '-l 3 -X mcts0,3000' ; ./bin/quixo $FLAGS_BASE -l 3 -X mcts0,3000 >> logs/$LOGFILE
echo '-l 3 -X mcts0,3500' ; ./bin/quixo $FLAGS_BASE -l 3 -X mcts0,3500 >> logs/$LOGFILE
echo '-l 3 -X mcts0,4000' ; ./bin/quixo $FLAGS_BASE -l 3 -X mcts0,4000 >> logs/$LOGFILE
echo '-l 3 -X mcts0,4500' ; ./bin/quixo $FLAGS_BASE -l 3 -X mcts0,4500 >> logs/$LOGFILE
echo '-l 3 -X mcts0,5000' ; ./bin/quixo $FLAGS_BASE -l 3 -X mcts0,5000 >> logs/$LOGFILE
echo '-l 3 -X mcts0,6000' ; ./bin/quixo $FLAGS_BASE -l 3 -X mcts0,6000 >> logs/$LOGFILE
echo '-l 3 -X mcts0,8000' ; ./bin/quixo $FLAGS_BASE -l 3 -X mcts0,8000 >> logs/$LOGFILE
echo '-l 3 -X mcts0,10000' ; ./bin/quixo $FLAGS_BASE -l 3 -X mcts0,10000 >> logs/$LOGFILE
echo
echo

# -l 4
echo '-l 4 -X mcts0,2500' ; ./bin/quixo $FLAGS_BASE -l 4 -X mcts0,2500 >> logs/$LOGFILE
echo '-l 4 -X mcts0,3000' ; ./bin/quixo $FLAGS_BASE -l 4 -X mcts0,3000 >> logs/$LOGFILE
echo '-l 4 -X mcts0,3500' ; ./bin/quixo $FLAGS_BASE -l 4 -X mcts0,3500 >> logs/$LOGFILE
echo '-l 4 -X mcts0,4000' ; ./bin/quixo $FLAGS_BASE -l 4 -X mcts0,4000 >> logs/$LOGFILE
echo '-l 4 -X mcts0,4500' ; ./bin/quixo $FLAGS_BASE -l 4 -X mcts0,4500 >> logs/$LOGFILE
echo '-l 4 -X mcts0,5000' ; ./bin/quixo $FLAGS_BASE -l 4 -X mcts0,5000 >> logs/$LOGFILE
echo '-l 4 -X mcts0,6000' ; ./bin/quixo $FLAGS_BASE -l 4 -X mcts0,6000 >> logs/$LOGFILE
echo '-l 4 -X mcts0,8000' ; ./bin/quixo $FLAGS_BASE -l 4 -X mcts0,8000 >> logs/$LOGFILE
echo '-l 4 -X mcts0,10000' ; ./bin/quixo $FLAGS_BASE -l 4 -X mcts0,10000 >> logs/$LOGFILE
echo
echo

# -l 5
echo '-l 5 -X mcts0,2500' ; ./bin/quixo $FLAGS_BASE -l 5 -X mcts0,2500 >> logs/$LOGFILE
echo '-l 5 -X mcts0,3000' ; ./bin/quixo $FLAGS_BASE -l 5 -X mcts0,3000 >> logs/$LOGFILE
echo '-l 5 -X mcts0,3500' ; ./bin/quixo $FLAGS_BASE -l 5 -X mcts0,3500 >> logs/$LOGFILE
echo '-l 5 -X mcts0,4000' ; ./bin/quixo $FLAGS_BASE -l 5 -X mcts0,4000 >> logs/$LOGFILE
echo '-l 5 -X mcts0,4500' ; ./bin/quixo $FLAGS_BASE -l 5 -X mcts0,4500 >> logs/$LOGFILE
echo '-l 5 -X mcts0,5000' ; ./bin/quixo $FLAGS_BASE -l 5 -X mcts0,5000 >> logs/$LOGFILE
echo '-l 5 -X mcts0,6000' ; ./bin/quixo $FLAGS_BASE -l 5 -X mcts0,6000 >> logs/$LOGFILE
echo '-l 5 -X mcts0,8000' ; ./bin/quixo $FLAGS_BASE -l 5 -X mcts0,8000 >> logs/$LOGFILE
echo '-l 5 -X mcts0,10000' ; ./bin/quixo $FLAGS_BASE -l 5 -X mcts0,10000 >> logs/$LOGFILE
echo
echo

echo '=== DONE ==='
