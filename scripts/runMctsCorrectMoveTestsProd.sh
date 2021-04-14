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
echo '-l 3 -X mcts0,0' ; ./bin/quixo $FLAGS_BASE -l 3 -X mcts0,0 >> logs/$LOGFILE
echo '-l 3 -X mcts0,100' ; ./bin/quixo $FLAGS_BASE -l 3 -X mcts0,100 >> logs/$LOGFILE
echo '-l 3 -X mcts0,200' ; ./bin/quixo $FLAGS_BASE -l 3 -X mcts0,200 >> logs/$LOGFILE
echo '-l 3 -X mcts0,300' ; ./bin/quixo $FLAGS_BASE -l 3 -X mcts0,300 >> logs/$LOGFILE
echo '-l 3 -X mcts0,400' ; ./bin/quixo $FLAGS_BASE -l 3 -X mcts0,400 >> logs/$LOGFILE
echo '-l 3 -X mcts0,500' ; ./bin/quixo $FLAGS_BASE -l 3 -X mcts0,500 >> logs/$LOGFILE
echo '-l 3 -X mcts0,600' ; ./bin/quixo $FLAGS_BASE -l 3 -X mcts0,600 >> logs/$LOGFILE
echo '-l 3 -X mcts0,700' ; ./bin/quixo $FLAGS_BASE -l 3 -X mcts0,700 >> logs/$LOGFILE
echo '-l 3 -X mcts0,800' ; ./bin/quixo $FLAGS_BASE -l 3 -X mcts0,800 >> logs/$LOGFILE
echo '-l 3 -X mcts0,900' ; ./bin/quixo $FLAGS_BASE -l 3 -X mcts0,900 >> logs/$LOGFILE
echo '-l 3 -X mcts0,1000' ; ./bin/quixo $FLAGS_BASE -l 3 -X mcts0,1000 >> logs/$LOGFILE
echo '-l 3 -X mcts0,1200' ; ./bin/quixo $FLAGS_BASE -l 3 -X mcts0,1200 >> logs/$LOGFILE
echo '-l 3 -X mcts0,1400' ; ./bin/quixo $FLAGS_BASE -l 3 -X mcts0,1400 >> logs/$LOGFILE
echo '-l 3 -X mcts0,1600' ; ./bin/quixo $FLAGS_BASE -l 3 -X mcts0,1600 >> logs/$LOGFILE
echo '-l 3 -X mcts0,1800' ; ./bin/quixo $FLAGS_BASE -l 3 -X mcts0,1800 >> logs/$LOGFILE
echo '-l 3 -X mcts0,2000' ; ./bin/quixo $FLAGS_BASE -l 3 -X mcts0,2000 >> logs/$LOGFILE
echo '-l 3 -X opt' ; ./bin/quixo $FLAGS_BASE -l 3 -X opt >> logs/$LOGFILE
echo
echo

# -l 4
echo '-l 4 -X mcts0,0' ; ./bin/quixo $FLAGS_BASE -l 4 -X mcts0,0 >> logs/$LOGFILE
echo '-l 4 -X mcts0,100' ; ./bin/quixo $FLAGS_BASE -l 4 -X mcts0,100 >> logs/$LOGFILE
echo '-l 4 -X mcts0,200' ; ./bin/quixo $FLAGS_BASE -l 4 -X mcts0,200 >> logs/$LOGFILE
echo '-l 4 -X mcts0,300' ; ./bin/quixo $FLAGS_BASE -l 4 -X mcts0,300 >> logs/$LOGFILE
echo '-l 4 -X mcts0,400' ; ./bin/quixo $FLAGS_BASE -l 4 -X mcts0,400 >> logs/$LOGFILE
echo '-l 4 -X mcts0,500' ; ./bin/quixo $FLAGS_BASE -l 4 -X mcts0,500 >> logs/$LOGFILE
echo '-l 4 -X mcts0,600' ; ./bin/quixo $FLAGS_BASE -l 4 -X mcts0,600 >> logs/$LOGFILE
echo '-l 4 -X mcts0,700' ; ./bin/quixo $FLAGS_BASE -l 4 -X mcts0,700 >> logs/$LOGFILE
echo '-l 4 -X mcts0,800' ; ./bin/quixo $FLAGS_BASE -l 4 -X mcts0,800 >> logs/$LOGFILE
echo '-l 4 -X mcts0,900' ; ./bin/quixo $FLAGS_BASE -l 4 -X mcts0,900 >> logs/$LOGFILE
echo '-l 4 -X mcts0,1000' ; ./bin/quixo $FLAGS_BASE -l 4 -X mcts0,1000 >> logs/$LOGFILE
echo '-l 4 -X mcts0,1200' ; ./bin/quixo $FLAGS_BASE -l 4 -X mcts0,1200 >> logs/$LOGFILE
echo '-l 4 -X mcts0,1400' ; ./bin/quixo $FLAGS_BASE -l 4 -X mcts0,1400 >> logs/$LOGFILE
echo '-l 4 -X mcts0,1600' ; ./bin/quixo $FLAGS_BASE -l 4 -X mcts0,1600 >> logs/$LOGFILE
echo '-l 4 -X mcts0,1800' ; ./bin/quixo $FLAGS_BASE -l 4 -X mcts0,1800 >> logs/$LOGFILE
echo '-l 4 -X mcts0,2000' ; ./bin/quixo $FLAGS_BASE -l 4 -X mcts0,2000 >> logs/$LOGFILE
echo '-l 4 -X opt' ; ./bin/quixo $FLAGS_BASE -l 4 -X opt >> logs/$LOGFILE
echo
echo

# -l 5
echo '-l 5 -X mcts0,0' ; ./bin/quixo $FLAGS_BASE -l 5 -X mcts0,0 >> logs/$LOGFILE
echo '-l 5 -X mcts0,100' ; ./bin/quixo $FLAGS_BASE -l 5 -X mcts0,100 >> logs/$LOGFILE
echo '-l 5 -X mcts0,200' ; ./bin/quixo $FLAGS_BASE -l 5 -X mcts0,200 >> logs/$LOGFILE
echo '-l 5 -X mcts0,300' ; ./bin/quixo $FLAGS_BASE -l 5 -X mcts0,300 >> logs/$LOGFILE
echo '-l 5 -X mcts0,400' ; ./bin/quixo $FLAGS_BASE -l 5 -X mcts0,400 >> logs/$LOGFILE
echo '-l 5 -X mcts0,500' ; ./bin/quixo $FLAGS_BASE -l 5 -X mcts0,500 >> logs/$LOGFILE
echo '-l 5 -X mcts0,600' ; ./bin/quixo $FLAGS_BASE -l 5 -X mcts0,600 >> logs/$LOGFILE
echo '-l 5 -X mcts0,700' ; ./bin/quixo $FLAGS_BASE -l 5 -X mcts0,700 >> logs/$LOGFILE
echo '-l 5 -X mcts0,800' ; ./bin/quixo $FLAGS_BASE -l 5 -X mcts0,800 >> logs/$LOGFILE
echo '-l 5 -X mcts0,900' ; ./bin/quixo $FLAGS_BASE -l 5 -X mcts0,900 >> logs/$LOGFILE
echo '-l 5 -X mcts0,1000' ; ./bin/quixo $FLAGS_BASE -l 5 -X mcts0,1000 >> logs/$LOGFILE
echo '-l 5 -X mcts0,1200' ; ./bin/quixo $FLAGS_BASE -l 5 -X mcts0,1200 >> logs/$LOGFILE
echo '-l 5 -X mcts0,1400' ; ./bin/quixo $FLAGS_BASE -l 5 -X mcts0,1400 >> logs/$LOGFILE
echo '-l 5 -X mcts0,1600' ; ./bin/quixo $FLAGS_BASE -l 5 -X mcts0,1600 >> logs/$LOGFILE
echo '-l 5 -X mcts0,1800' ; ./bin/quixo $FLAGS_BASE -l 5 -X mcts0,1800 >> logs/$LOGFILE
echo '-l 5 -X mcts0,2000' ; ./bin/quixo $FLAGS_BASE -l 5 -X mcts0,2000 >> logs/$LOGFILE
echo '-l 5 -X opt' ; ./bin/quixo $FLAGS_BASE -l 5 -X opt >> logs/$LOGFILE
echo
echo

echo '=== DONE ==='
