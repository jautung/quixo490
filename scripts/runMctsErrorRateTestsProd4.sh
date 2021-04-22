#!/bin/bash
DATE=$(date +'%Y-%m-%d-%H%M%S')
LOGFILE="${DATE}-error-rate-tests.log"

N_GAMES_EACH=100
TURN_LIMIT=200
FLAGS_BASE='-p test -N '$N_GAMES_EACH' -n '$TURN_LIMIT' -v error-rate-tests-mcts'

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
echo 'len	xType	oType	xWins	oWins	draws	xTurnTime	oTurnTime' >> logs/$LOGFILE

################################################################################
################################################################################
# X opt O mcts
echo '-l 5 -X opt0.0 -O mcts0,20000' ; ./bin/quixo $FLAGS_BASE -l 5 -X opt0.0 -O mcts0,20000 >> logs/$LOGFILE
echo '-l 5 -X opt0.0 -O mcts0,50000' ; ./bin/quixo $FLAGS_BASE -l 5 -X opt0.0 -O mcts0,35000 >> logs/$LOGFILE
echo '-l 5 -X opt0.0 -O mcts0,100000' ; ./bin/quixo $FLAGS_BASE -l 5 -X opt0.0 -O mcts0,100000 >> logs/$LOGFILE
echo

echo '-l 5 -X opt0.01 -O mcts0,20000' ; ./bin/quixo $FLAGS_BASE -l 5 -X opt0.01 -O mcts0,20000 >> logs/$LOGFILE
echo '-l 5 -X opt0.01 -O mcts0,50000' ; ./bin/quixo $FLAGS_BASE -l 5 -X opt0.01 -O mcts0,35000 >> logs/$LOGFILE
echo '-l 5 -X opt0.01 -O mcts0,100000' ; ./bin/quixo $FLAGS_BASE -l 5 -X opt0.01 -O mcts0,100000 >> logs/$LOGFILE
echo

# O opt X mcts
echo '-l 5 -O opt0.0 -X mcts0,20000' ; ./bin/quixo $FLAGS_BASE -l 5 -O opt0.0 -X mcts0,20000 >> logs/$LOGFILE
echo '-l 5 -O opt0.0 -X mcts0,50000' ; ./bin/quixo $FLAGS_BASE -l 5 -O opt0.0 -X mcts0,35000 >> logs/$LOGFILE
echo '-l 5 -O opt0.0 -X mcts0,100000' ; ./bin/quixo $FLAGS_BASE -l 5 -O opt0.0 -X mcts0,100000 >> logs/$LOGFILE
echo

echo '-l 5 -O opt0.01 -X mcts0,20000' ; ./bin/quixo $FLAGS_BASE -l 5 -O opt0.01 -X mcts0,20000 >> logs/$LOGFILE
echo '-l 5 -O opt0.01 -X mcts0,50000' ; ./bin/quixo $FLAGS_BASE -l 5 -O opt0.01 -X mcts0,35000 >> logs/$LOGFILE
echo '-l 5 -O opt0.01 -X mcts0,100000' ; ./bin/quixo $FLAGS_BASE -l 5 -O opt0.01 -X mcts0,100000 >> logs/$LOGFILE
echo

echo '=== DONE ==='
