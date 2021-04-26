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
# X opt-steps O mcts
echo '-l 3 -X opt-steps0.0 -O mcts0,2500' ; ./bin/quixo $FLAGS_BASE -l 3 -X opt-steps0.0 -O mcts0,2500 >> logs/$LOGFILE
echo '-l 3 -X opt-steps0.0 -O mcts0,3000' ; ./bin/quixo $FLAGS_BASE -l 3 -X opt-steps0.0 -O mcts0,3000 >> logs/$LOGFILE
echo '-l 3 -X opt-steps0.0 -O mcts0,3500' ; ./bin/quixo $FLAGS_BASE -l 3 -X opt-steps0.0 -O mcts0,3500 >> logs/$LOGFILE
echo '-l 3 -X opt-steps0.0 -O mcts0,4000' ; ./bin/quixo $FLAGS_BASE -l 3 -X opt-steps0.0 -O mcts0,4000 >> logs/$LOGFILE
echo '-l 3 -X opt-steps0.0 -O mcts0,4500' ; ./bin/quixo $FLAGS_BASE -l 3 -X opt-steps0.0 -O mcts0,4500 >> logs/$LOGFILE
echo '-l 3 -X opt-steps0.0 -O mcts0,5000' ; ./bin/quixo $FLAGS_BASE -l 3 -X opt-steps0.0 -O mcts0,5000 >> logs/$LOGFILE
echo '-l 3 -X opt-steps0.0 -O mcts0,6000' ; ./bin/quixo $FLAGS_BASE -l 3 -X opt-steps0.0 -O mcts0,6000 >> logs/$LOGFILE
echo '-l 3 -X opt-steps0.0 -O mcts0,8000' ; ./bin/quixo $FLAGS_BASE -l 3 -X opt-steps0.0 -O mcts0,8000 >> logs/$LOGFILE
echo '-l 3 -X opt-steps0.0 -O mcts0,10000' ; ./bin/quixo $FLAGS_BASE -l 3 -X opt-steps0.0 -O mcts0,10000 >> logs/$LOGFILE
echo

echo '-l 3 -X opt-steps0.01 -O mcts0,2500' ; ./bin/quixo $FLAGS_BASE -l 3 -X opt-steps0.01 -O mcts0,2500 >> logs/$LOGFILE
echo '-l 3 -X opt-steps0.01 -O mcts0,3000' ; ./bin/quixo $FLAGS_BASE -l 3 -X opt-steps0.01 -O mcts0,3000 >> logs/$LOGFILE
echo '-l 3 -X opt-steps0.01 -O mcts0,3500' ; ./bin/quixo $FLAGS_BASE -l 3 -X opt-steps0.01 -O mcts0,3500 >> logs/$LOGFILE
echo '-l 3 -X opt-steps0.01 -O mcts0,4000' ; ./bin/quixo $FLAGS_BASE -l 3 -X opt-steps0.01 -O mcts0,4000 >> logs/$LOGFILE
echo '-l 3 -X opt-steps0.01 -O mcts0,4500' ; ./bin/quixo $FLAGS_BASE -l 3 -X opt-steps0.01 -O mcts0,4500 >> logs/$LOGFILE
echo '-l 3 -X opt-steps0.01 -O mcts0,5000' ; ./bin/quixo $FLAGS_BASE -l 3 -X opt-steps0.01 -O mcts0,5000 >> logs/$LOGFILE
echo '-l 3 -X opt-steps0.01 -O mcts0,6000' ; ./bin/quixo $FLAGS_BASE -l 3 -X opt-steps0.01 -O mcts0,6000 >> logs/$LOGFILE
echo '-l 3 -X opt-steps0.01 -O mcts0,8000' ; ./bin/quixo $FLAGS_BASE -l 3 -X opt-steps0.01 -O mcts0,8000 >> logs/$LOGFILE
echo '-l 3 -X opt-steps0.01 -O mcts0,10000' ; ./bin/quixo $FLAGS_BASE -l 3 -X opt-steps0.01 -O mcts0,10000 >> logs/$LOGFILE
echo

echo '-l 3 -X opt-steps0.05 -O mcts0,2500' ; ./bin/quixo $FLAGS_BASE -l 3 -X opt-steps0.05 -O mcts0,2500 >> logs/$LOGFILE
echo '-l 3 -X opt-steps0.05 -O mcts0,3000' ; ./bin/quixo $FLAGS_BASE -l 3 -X opt-steps0.05 -O mcts0,3000 >> logs/$LOGFILE
echo '-l 3 -X opt-steps0.05 -O mcts0,3500' ; ./bin/quixo $FLAGS_BASE -l 3 -X opt-steps0.05 -O mcts0,3500 >> logs/$LOGFILE
echo '-l 3 -X opt-steps0.05 -O mcts0,4000' ; ./bin/quixo $FLAGS_BASE -l 3 -X opt-steps0.05 -O mcts0,4000 >> logs/$LOGFILE
echo '-l 3 -X opt-steps0.05 -O mcts0,4500' ; ./bin/quixo $FLAGS_BASE -l 3 -X opt-steps0.05 -O mcts0,4500 >> logs/$LOGFILE
echo '-l 3 -X opt-steps0.05 -O mcts0,5000' ; ./bin/quixo $FLAGS_BASE -l 3 -X opt-steps0.05 -O mcts0,5000 >> logs/$LOGFILE
echo '-l 3 -X opt-steps0.05 -O mcts0,6000' ; ./bin/quixo $FLAGS_BASE -l 3 -X opt-steps0.05 -O mcts0,6000 >> logs/$LOGFILE
echo '-l 3 -X opt-steps0.05 -O mcts0,8000' ; ./bin/quixo $FLAGS_BASE -l 3 -X opt-steps0.05 -O mcts0,8000 >> logs/$LOGFILE
echo '-l 3 -X opt-steps0.05 -O mcts0,10000' ; ./bin/quixo $FLAGS_BASE -l 3 -X opt-steps0.05 -O mcts0,10000 >> logs/$LOGFILE
echo

echo '-l 3 -X opt-steps0.1 -O mcts0,2500' ; ./bin/quixo $FLAGS_BASE -l 3 -X opt-steps0.1 -O mcts0,2500 >> logs/$LOGFILE
echo '-l 3 -X opt-steps0.1 -O mcts0,3000' ; ./bin/quixo $FLAGS_BASE -l 3 -X opt-steps0.1 -O mcts0,3000 >> logs/$LOGFILE
echo '-l 3 -X opt-steps0.1 -O mcts0,3500' ; ./bin/quixo $FLAGS_BASE -l 3 -X opt-steps0.1 -O mcts0,3500 >> logs/$LOGFILE
echo '-l 3 -X opt-steps0.1 -O mcts0,4000' ; ./bin/quixo $FLAGS_BASE -l 3 -X opt-steps0.1 -O mcts0,4000 >> logs/$LOGFILE
echo '-l 3 -X opt-steps0.1 -O mcts0,4500' ; ./bin/quixo $FLAGS_BASE -l 3 -X opt-steps0.1 -O mcts0,4500 >> logs/$LOGFILE
echo '-l 3 -X opt-steps0.1 -O mcts0,5000' ; ./bin/quixo $FLAGS_BASE -l 3 -X opt-steps0.1 -O mcts0,5000 >> logs/$LOGFILE
echo '-l 3 -X opt-steps0.1 -O mcts0,6000' ; ./bin/quixo $FLAGS_BASE -l 3 -X opt-steps0.1 -O mcts0,6000 >> logs/$LOGFILE
echo '-l 3 -X opt-steps0.1 -O mcts0,8000' ; ./bin/quixo $FLAGS_BASE -l 3 -X opt-steps0.1 -O mcts0,8000 >> logs/$LOGFILE
echo '-l 3 -X opt-steps0.1 -O mcts0,10000' ; ./bin/quixo $FLAGS_BASE -l 3 -X opt-steps0.1 -O mcts0,10000 >> logs/$LOGFILE
echo

echo '-l 3 -X opt-steps0.2 -O mcts0,2500' ; ./bin/quixo $FLAGS_BASE -l 3 -X opt-steps0.2 -O mcts0,2500 >> logs/$LOGFILE
echo '-l 3 -X opt-steps0.2 -O mcts0,3000' ; ./bin/quixo $FLAGS_BASE -l 3 -X opt-steps0.2 -O mcts0,3000 >> logs/$LOGFILE
echo '-l 3 -X opt-steps0.2 -O mcts0,3500' ; ./bin/quixo $FLAGS_BASE -l 3 -X opt-steps0.2 -O mcts0,3500 >> logs/$LOGFILE
echo '-l 3 -X opt-steps0.2 -O mcts0,4000' ; ./bin/quixo $FLAGS_BASE -l 3 -X opt-steps0.2 -O mcts0,4000 >> logs/$LOGFILE
echo '-l 3 -X opt-steps0.2 -O mcts0,4500' ; ./bin/quixo $FLAGS_BASE -l 3 -X opt-steps0.2 -O mcts0,4500 >> logs/$LOGFILE
echo '-l 3 -X opt-steps0.2 -O mcts0,5000' ; ./bin/quixo $FLAGS_BASE -l 3 -X opt-steps0.2 -O mcts0,5000 >> logs/$LOGFILE
echo '-l 3 -X opt-steps0.2 -O mcts0,6000' ; ./bin/quixo $FLAGS_BASE -l 3 -X opt-steps0.2 -O mcts0,6000 >> logs/$LOGFILE
echo '-l 3 -X opt-steps0.2 -O mcts0,8000' ; ./bin/quixo $FLAGS_BASE -l 3 -X opt-steps0.2 -O mcts0,8000 >> logs/$LOGFILE
echo '-l 3 -X opt-steps0.2 -O mcts0,10000' ; ./bin/quixo $FLAGS_BASE -l 3 -X opt-steps0.2 -O mcts0,10000 >> logs/$LOGFILE
echo

echo '-l 3 -X opt-steps0.5 -O mcts0,2500' ; ./bin/quixo $FLAGS_BASE -l 3 -X opt-steps0.5 -O mcts0,2500 >> logs/$LOGFILE
echo '-l 3 -X opt-steps0.5 -O mcts0,3000' ; ./bin/quixo $FLAGS_BASE -l 3 -X opt-steps0.5 -O mcts0,3000 >> logs/$LOGFILE
echo '-l 3 -X opt-steps0.5 -O mcts0,3500' ; ./bin/quixo $FLAGS_BASE -l 3 -X opt-steps0.5 -O mcts0,3500 >> logs/$LOGFILE
echo '-l 3 -X opt-steps0.5 -O mcts0,4000' ; ./bin/quixo $FLAGS_BASE -l 3 -X opt-steps0.5 -O mcts0,4000 >> logs/$LOGFILE
echo '-l 3 -X opt-steps0.5 -O mcts0,4500' ; ./bin/quixo $FLAGS_BASE -l 3 -X opt-steps0.5 -O mcts0,4500 >> logs/$LOGFILE
echo '-l 3 -X opt-steps0.5 -O mcts0,5000' ; ./bin/quixo $FLAGS_BASE -l 3 -X opt-steps0.5 -O mcts0,5000 >> logs/$LOGFILE
echo '-l 3 -X opt-steps0.5 -O mcts0,6000' ; ./bin/quixo $FLAGS_BASE -l 3 -X opt-steps0.5 -O mcts0,6000 >> logs/$LOGFILE
echo '-l 3 -X opt-steps0.5 -O mcts0,8000' ; ./bin/quixo $FLAGS_BASE -l 3 -X opt-steps0.5 -O mcts0,8000 >> logs/$LOGFILE
echo '-l 3 -X opt-steps0.5 -O mcts0,10000' ; ./bin/quixo $FLAGS_BASE -l 3 -X opt-steps0.5 -O mcts0,10000 >> logs/$LOGFILE
echo
echo

# O opt-steps X mcts
echo '-l 3 -O opt-steps0.0 -X mcts0,2500' ; ./bin/quixo $FLAGS_BASE -l 3 -O opt-steps0.0 -X mcts0,2500 >> logs/$LOGFILE
echo '-l 3 -O opt-steps0.0 -X mcts0,3000' ; ./bin/quixo $FLAGS_BASE -l 3 -O opt-steps0.0 -X mcts0,3000 >> logs/$LOGFILE
echo '-l 3 -O opt-steps0.0 -X mcts0,3500' ; ./bin/quixo $FLAGS_BASE -l 3 -O opt-steps0.0 -X mcts0,3500 >> logs/$LOGFILE
echo '-l 3 -O opt-steps0.0 -X mcts0,4000' ; ./bin/quixo $FLAGS_BASE -l 3 -O opt-steps0.0 -X mcts0,4000 >> logs/$LOGFILE
echo '-l 3 -O opt-steps0.0 -X mcts0,4500' ; ./bin/quixo $FLAGS_BASE -l 3 -O opt-steps0.0 -X mcts0,4500 >> logs/$LOGFILE
echo '-l 3 -O opt-steps0.0 -X mcts0,5000' ; ./bin/quixo $FLAGS_BASE -l 3 -O opt-steps0.0 -X mcts0,5000 >> logs/$LOGFILE
echo '-l 3 -O opt-steps0.0 -X mcts0,6000' ; ./bin/quixo $FLAGS_BASE -l 3 -O opt-steps0.0 -X mcts0,6000 >> logs/$LOGFILE
echo '-l 3 -O opt-steps0.0 -X mcts0,8000' ; ./bin/quixo $FLAGS_BASE -l 3 -O opt-steps0.0 -X mcts0,8000 >> logs/$LOGFILE
echo '-l 3 -O opt-steps0.0 -X mcts0,10000' ; ./bin/quixo $FLAGS_BASE -l 3 -O opt-steps0.0 -X mcts0,10000 >> logs/$LOGFILE
echo

echo '-l 3 -O opt-steps0.01 -X mcts0,2500' ; ./bin/quixo $FLAGS_BASE -l 3 -O opt-steps0.01 -X mcts0,2500 >> logs/$LOGFILE
echo '-l 3 -O opt-steps0.01 -X mcts0,3000' ; ./bin/quixo $FLAGS_BASE -l 3 -O opt-steps0.01 -X mcts0,3000 >> logs/$LOGFILE
echo '-l 3 -O opt-steps0.01 -X mcts0,3500' ; ./bin/quixo $FLAGS_BASE -l 3 -O opt-steps0.01 -X mcts0,3500 >> logs/$LOGFILE
echo '-l 3 -O opt-steps0.01 -X mcts0,4000' ; ./bin/quixo $FLAGS_BASE -l 3 -O opt-steps0.01 -X mcts0,4000 >> logs/$LOGFILE
echo '-l 3 -O opt-steps0.01 -X mcts0,4500' ; ./bin/quixo $FLAGS_BASE -l 3 -O opt-steps0.01 -X mcts0,4500 >> logs/$LOGFILE
echo '-l 3 -O opt-steps0.01 -X mcts0,5000' ; ./bin/quixo $FLAGS_BASE -l 3 -O opt-steps0.01 -X mcts0,5000 >> logs/$LOGFILE
echo '-l 3 -O opt-steps0.01 -X mcts0,6000' ; ./bin/quixo $FLAGS_BASE -l 3 -O opt-steps0.01 -X mcts0,6000 >> logs/$LOGFILE
echo '-l 3 -O opt-steps0.01 -X mcts0,8000' ; ./bin/quixo $FLAGS_BASE -l 3 -O opt-steps0.01 -X mcts0,8000 >> logs/$LOGFILE
echo '-l 3 -O opt-steps0.01 -X mcts0,10000' ; ./bin/quixo $FLAGS_BASE -l 3 -O opt-steps0.01 -X mcts0,10000 >> logs/$LOGFILE
echo

echo '-l 3 -O opt-steps0.05 -X mcts0,2500' ; ./bin/quixo $FLAGS_BASE -l 3 -O opt-steps0.05 -X mcts0,2500 >> logs/$LOGFILE
echo '-l 3 -O opt-steps0.05 -X mcts0,3000' ; ./bin/quixo $FLAGS_BASE -l 3 -O opt-steps0.05 -X mcts0,3000 >> logs/$LOGFILE
echo '-l 3 -O opt-steps0.05 -X mcts0,3500' ; ./bin/quixo $FLAGS_BASE -l 3 -O opt-steps0.05 -X mcts0,3500 >> logs/$LOGFILE
echo '-l 3 -O opt-steps0.05 -X mcts0,4000' ; ./bin/quixo $FLAGS_BASE -l 3 -O opt-steps0.05 -X mcts0,4000 >> logs/$LOGFILE
echo '-l 3 -O opt-steps0.05 -X mcts0,4500' ; ./bin/quixo $FLAGS_BASE -l 3 -O opt-steps0.05 -X mcts0,4500 >> logs/$LOGFILE
echo '-l 3 -O opt-steps0.05 -X mcts0,5000' ; ./bin/quixo $FLAGS_BASE -l 3 -O opt-steps0.05 -X mcts0,5000 >> logs/$LOGFILE
echo '-l 3 -O opt-steps0.05 -X mcts0,6000' ; ./bin/quixo $FLAGS_BASE -l 3 -O opt-steps0.05 -X mcts0,6000 >> logs/$LOGFILE
echo '-l 3 -O opt-steps0.05 -X mcts0,8000' ; ./bin/quixo $FLAGS_BASE -l 3 -O opt-steps0.05 -X mcts0,8000 >> logs/$LOGFILE
echo '-l 3 -O opt-steps0.05 -X mcts0,10000' ; ./bin/quixo $FLAGS_BASE -l 3 -O opt-steps0.05 -X mcts0,10000 >> logs/$LOGFILE
echo

echo '-l 3 -O opt-steps0.1 -X mcts0,2500' ; ./bin/quixo $FLAGS_BASE -l 3 -O opt-steps0.1 -X mcts0,2500 >> logs/$LOGFILE
echo '-l 3 -O opt-steps0.1 -X mcts0,3000' ; ./bin/quixo $FLAGS_BASE -l 3 -O opt-steps0.1 -X mcts0,3000 >> logs/$LOGFILE
echo '-l 3 -O opt-steps0.1 -X mcts0,3500' ; ./bin/quixo $FLAGS_BASE -l 3 -O opt-steps0.1 -X mcts0,3500 >> logs/$LOGFILE
echo '-l 3 -O opt-steps0.1 -X mcts0,4000' ; ./bin/quixo $FLAGS_BASE -l 3 -O opt-steps0.1 -X mcts0,4000 >> logs/$LOGFILE
echo '-l 3 -O opt-steps0.1 -X mcts0,4500' ; ./bin/quixo $FLAGS_BASE -l 3 -O opt-steps0.1 -X mcts0,4500 >> logs/$LOGFILE
echo '-l 3 -O opt-steps0.1 -X mcts0,5000' ; ./bin/quixo $FLAGS_BASE -l 3 -O opt-steps0.1 -X mcts0,5000 >> logs/$LOGFILE
echo '-l 3 -O opt-steps0.1 -X mcts0,6000' ; ./bin/quixo $FLAGS_BASE -l 3 -O opt-steps0.1 -X mcts0,6000 >> logs/$LOGFILE
echo '-l 3 -O opt-steps0.1 -X mcts0,8000' ; ./bin/quixo $FLAGS_BASE -l 3 -O opt-steps0.1 -X mcts0,8000 >> logs/$LOGFILE
echo '-l 3 -O opt-steps0.1 -X mcts0,10000' ; ./bin/quixo $FLAGS_BASE -l 3 -O opt-steps0.1 -X mcts0,10000 >> logs/$LOGFILE
echo

echo '-l 3 -O opt-steps0.2 -X mcts0,2500' ; ./bin/quixo $FLAGS_BASE -l 3 -O opt-steps0.2 -X mcts0,2500 >> logs/$LOGFILE
echo '-l 3 -O opt-steps0.2 -X mcts0,3000' ; ./bin/quixo $FLAGS_BASE -l 3 -O opt-steps0.2 -X mcts0,3000 >> logs/$LOGFILE
echo '-l 3 -O opt-steps0.2 -X mcts0,3500' ; ./bin/quixo $FLAGS_BASE -l 3 -O opt-steps0.2 -X mcts0,3500 >> logs/$LOGFILE
echo '-l 3 -O opt-steps0.2 -X mcts0,4000' ; ./bin/quixo $FLAGS_BASE -l 3 -O opt-steps0.2 -X mcts0,4000 >> logs/$LOGFILE
echo '-l 3 -O opt-steps0.2 -X mcts0,4500' ; ./bin/quixo $FLAGS_BASE -l 3 -O opt-steps0.2 -X mcts0,4500 >> logs/$LOGFILE
echo '-l 3 -O opt-steps0.2 -X mcts0,5000' ; ./bin/quixo $FLAGS_BASE -l 3 -O opt-steps0.2 -X mcts0,5000 >> logs/$LOGFILE
echo '-l 3 -O opt-steps0.2 -X mcts0,6000' ; ./bin/quixo $FLAGS_BASE -l 3 -O opt-steps0.2 -X mcts0,6000 >> logs/$LOGFILE
echo '-l 3 -O opt-steps0.2 -X mcts0,8000' ; ./bin/quixo $FLAGS_BASE -l 3 -O opt-steps0.2 -X mcts0,8000 >> logs/$LOGFILE
echo '-l 3 -O opt-steps0.2 -X mcts0,10000' ; ./bin/quixo $FLAGS_BASE -l 3 -O opt-steps0.2 -X mcts0,10000 >> logs/$LOGFILE
echo

echo '-l 3 -O opt-steps0.5 -X mcts0,2500' ; ./bin/quixo $FLAGS_BASE -l 3 -O opt-steps0.5 -X mcts0,2500 >> logs/$LOGFILE
echo '-l 3 -O opt-steps0.5 -X mcts0,3000' ; ./bin/quixo $FLAGS_BASE -l 3 -O opt-steps0.5 -X mcts0,3000 >> logs/$LOGFILE
echo '-l 3 -O opt-steps0.5 -X mcts0,3500' ; ./bin/quixo $FLAGS_BASE -l 3 -O opt-steps0.5 -X mcts0,3500 >> logs/$LOGFILE
echo '-l 3 -O opt-steps0.5 -X mcts0,4000' ; ./bin/quixo $FLAGS_BASE -l 3 -O opt-steps0.5 -X mcts0,4000 >> logs/$LOGFILE
echo '-l 3 -O opt-steps0.5 -X mcts0,4500' ; ./bin/quixo $FLAGS_BASE -l 3 -O opt-steps0.5 -X mcts0,4500 >> logs/$LOGFILE
echo '-l 3 -O opt-steps0.5 -X mcts0,5000' ; ./bin/quixo $FLAGS_BASE -l 3 -O opt-steps0.5 -X mcts0,5000 >> logs/$LOGFILE
echo '-l 3 -O opt-steps0.5 -X mcts0,6000' ; ./bin/quixo $FLAGS_BASE -l 3 -O opt-steps0.5 -X mcts0,6000 >> logs/$LOGFILE
echo '-l 3 -O opt-steps0.5 -X mcts0,8000' ; ./bin/quixo $FLAGS_BASE -l 3 -O opt-steps0.5 -X mcts0,8000 >> logs/$LOGFILE
echo '-l 3 -O opt-steps0.5 -X mcts0,10000' ; ./bin/quixo $FLAGS_BASE -l 3 -O opt-steps0.5 -X mcts0,10000 >> logs/$LOGFILE
echo
echo

echo '=== DONE ==='
