#!/bin/bash
DATE=$(date +'%Y-%m-%d-%H%M%S')
LOGFILE="${DATE}-game-length-tests.log"

N_GAMES_EACH=200
TURN_LIMIT=200
FLAGS_BASE='-p test -N '$N_GAMES_EACH' -n '$TURN_LIMIT' -v game-length-tests'

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

echo 'N_GAMES_EACH:' $N_GAMES_EACH >> logs/$LOGFILE
echo 'TURN_LIMIT:' $TURN_LIMIT >> logs/$LOGFILE
echo '' >> logs/$LOGFILE
echo 'len	xType	oType	aveNumTurns	stdevNumTurns	numDraws	xTurnTime	oTurnTime' >> logs/$LOGFILE

# -l 3
echo '-l 3 -X opt -O mcts0,0' ; ./bin/quixo $FLAGS_BASE -l 3 -X opt -O mcts0,0 >> logs/$LOGFILE
echo '-l 3 -X opt -O mcts0,100' ; ./bin/quixo $FLAGS_BASE -l 3 -X opt -O mcts0,100 >> logs/$LOGFILE
echo '-l 3 -X opt -O mcts0,200' ; ./bin/quixo $FLAGS_BASE -l 3 -X opt -O mcts0,200 >> logs/$LOGFILE
echo '-l 3 -X opt -O mcts0,300' ; ./bin/quixo $FLAGS_BASE -l 3 -X opt -O mcts0,300 >> logs/$LOGFILE
echo '-l 3 -X opt -O mcts0,400' ; ./bin/quixo $FLAGS_BASE -l 3 -X opt -O mcts0,400 >> logs/$LOGFILE
echo '-l 3 -X opt -O mcts0,500' ; ./bin/quixo $FLAGS_BASE -l 3 -X opt -O mcts0,500 >> logs/$LOGFILE
echo '-l 3 -X opt -O mcts0,600' ; ./bin/quixo $FLAGS_BASE -l 3 -X opt -O mcts0,600 >> logs/$LOGFILE
echo '-l 3 -X opt -O mcts0,700' ; ./bin/quixo $FLAGS_BASE -l 3 -X opt -O mcts0,700 >> logs/$LOGFILE
echo '-l 3 -X opt -O mcts0,800' ; ./bin/quixo $FLAGS_BASE -l 3 -X opt -O mcts0,800 >> logs/$LOGFILE
echo '-l 3 -X opt -O mcts0,900' ; ./bin/quixo $FLAGS_BASE -l 3 -X opt -O mcts0,900 >> logs/$LOGFILE
echo '-l 3 -X opt -O mcts0,1000' ; ./bin/quixo $FLAGS_BASE -l 3 -X opt -O mcts0,1000 >> logs/$LOGFILE
echo '-l 3 -X opt -O mcts0,1200' ; ./bin/quixo $FLAGS_BASE -l 3 -X opt -O mcts0,1200 >> logs/$LOGFILE
echo '-l 3 -X opt -O mcts0,1400' ; ./bin/quixo $FLAGS_BASE -l 3 -X opt -O mcts0,1400 >> logs/$LOGFILE
echo '-l 3 -X opt -O mcts0,1600' ; ./bin/quixo $FLAGS_BASE -l 3 -X opt -O mcts0,1600 >> logs/$LOGFILE
echo '-l 3 -X opt -O mcts0,1800' ; ./bin/quixo $FLAGS_BASE -l 3 -X opt -O mcts0,1800 >> logs/$LOGFILE
echo '-l 3 -X opt -O mcts0,2000' ; ./bin/quixo $FLAGS_BASE -l 3 -X opt -O mcts0,2000 >> logs/$LOGFILE
echo '-l 3 -X opt -O opt' ; ./bin/quixo $FLAGS_BASE -l 3 -X opt -O opt >> logs/$LOGFILE
echo
echo

# -l 4
echo '-l 4 -X opt -O mcts0,0' ; ./bin/quixo $FLAGS_BASE -l 4 -X opt -O mcts0,0 >> logs/$LOGFILE
echo '-l 4 -X opt -O mcts0,100' ; ./bin/quixo $FLAGS_BASE -l 4 -X opt -O mcts0,100 >> logs/$LOGFILE
echo '-l 4 -X opt -O mcts0,200' ; ./bin/quixo $FLAGS_BASE -l 4 -X opt -O mcts0,200 >> logs/$LOGFILE
echo '-l 4 -X opt -O mcts0,300' ; ./bin/quixo $FLAGS_BASE -l 4 -X opt -O mcts0,300 >> logs/$LOGFILE
echo '-l 4 -X opt -O mcts0,400' ; ./bin/quixo $FLAGS_BASE -l 4 -X opt -O mcts0,400 >> logs/$LOGFILE
echo '-l 4 -X opt -O mcts0,500' ; ./bin/quixo $FLAGS_BASE -l 4 -X opt -O mcts0,500 >> logs/$LOGFILE
echo '-l 4 -X opt -O mcts0,600' ; ./bin/quixo $FLAGS_BASE -l 4 -X opt -O mcts0,600 >> logs/$LOGFILE
echo '-l 4 -X opt -O mcts0,700' ; ./bin/quixo $FLAGS_BASE -l 4 -X opt -O mcts0,700 >> logs/$LOGFILE
echo '-l 4 -X opt -O mcts0,800' ; ./bin/quixo $FLAGS_BASE -l 4 -X opt -O mcts0,800 >> logs/$LOGFILE
echo '-l 4 -X opt -O mcts0,900' ; ./bin/quixo $FLAGS_BASE -l 4 -X opt -O mcts0,900 >> logs/$LOGFILE
echo '-l 4 -X opt -O mcts0,1000' ; ./bin/quixo $FLAGS_BASE -l 4 -X opt -O mcts0,1000 >> logs/$LOGFILE
echo '-l 4 -X opt -O mcts0,1200' ; ./bin/quixo $FLAGS_BASE -l 4 -X opt -O mcts0,1200 >> logs/$LOGFILE
echo '-l 4 -X opt -O mcts0,1400' ; ./bin/quixo $FLAGS_BASE -l 4 -X opt -O mcts0,1400 >> logs/$LOGFILE
echo '-l 4 -X opt -O mcts0,1600' ; ./bin/quixo $FLAGS_BASE -l 4 -X opt -O mcts0,1600 >> logs/$LOGFILE
echo '-l 4 -X opt -O mcts0,1800' ; ./bin/quixo $FLAGS_BASE -l 4 -X opt -O mcts0,1800 >> logs/$LOGFILE
echo '-l 4 -X opt -O mcts0,2000' ; ./bin/quixo $FLAGS_BASE -l 4 -X opt -O mcts0,2000 >> logs/$LOGFILE
echo '-l 4 -X opt -O opt' ; ./bin/quixo $FLAGS_BASE -l 4 -X opt -O opt >> logs/$LOGFILE
echo
echo

# -l 5
echo '-l 5 -X opt -O mcts0,0' ; ./bin/quixo $FLAGS_BASE -l 5 -X opt -O mcts0,0 >> logs/$LOGFILE
echo '-l 5 -X opt -O mcts0,100' ; ./bin/quixo $FLAGS_BASE -l 5 -X opt -O mcts0,100 >> logs/$LOGFILE
echo '-l 5 -X opt -O mcts0,200' ; ./bin/quixo $FLAGS_BASE -l 5 -X opt -O mcts0,200 >> logs/$LOGFILE
echo '-l 5 -X opt -O mcts0,300' ; ./bin/quixo $FLAGS_BASE -l 5 -X opt -O mcts0,300 >> logs/$LOGFILE
echo '-l 5 -X opt -O mcts0,400' ; ./bin/quixo $FLAGS_BASE -l 5 -X opt -O mcts0,400 >> logs/$LOGFILE
echo '-l 5 -X opt -O mcts0,500' ; ./bin/quixo $FLAGS_BASE -l 5 -X opt -O mcts0,500 >> logs/$LOGFILE
echo '-l 5 -X opt -O mcts0,600' ; ./bin/quixo $FLAGS_BASE -l 5 -X opt -O mcts0,600 >> logs/$LOGFILE
echo '-l 5 -X opt -O mcts0,700' ; ./bin/quixo $FLAGS_BASE -l 5 -X opt -O mcts0,700 >> logs/$LOGFILE
echo '-l 5 -X opt -O mcts0,800' ; ./bin/quixo $FLAGS_BASE -l 5 -X opt -O mcts0,800 >> logs/$LOGFILE
echo '-l 5 -X opt -O mcts0,900' ; ./bin/quixo $FLAGS_BASE -l 5 -X opt -O mcts0,900 >> logs/$LOGFILE
echo '-l 5 -X opt -O mcts0,1000' ; ./bin/quixo $FLAGS_BASE -l 5 -X opt -O mcts0,1000 >> logs/$LOGFILE
echo '-l 5 -X opt -O mcts0,1200' ; ./bin/quixo $FLAGS_BASE -l 5 -X opt -O mcts0,1200 >> logs/$LOGFILE
echo '-l 5 -X opt -O mcts0,1400' ; ./bin/quixo $FLAGS_BASE -l 5 -X opt -O mcts0,1400 >> logs/$LOGFILE
echo '-l 5 -X opt -O mcts0,1600' ; ./bin/quixo $FLAGS_BASE -l 5 -X opt -O mcts0,1600 >> logs/$LOGFILE
echo '-l 5 -X opt -O mcts0,1800' ; ./bin/quixo $FLAGS_BASE -l 5 -X opt -O mcts0,1800 >> logs/$LOGFILE
echo '-l 5 -X opt -O mcts0,2000' ; ./bin/quixo $FLAGS_BASE -l 5 -X opt -O mcts0,2000 >> logs/$LOGFILE
echo '-l 5 -X opt -O opt' ; ./bin/quixo $FLAGS_BASE -l 5 -X opt -O opt >> logs/$LOGFILE
echo
echo

echo '=== DONE ==='
