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
echo '-l 5 -X opt0.0 -O mcts0,1100' ; ./bin/quixo $FLAGS_BASE -l 5 -X opt0.0 -O mcts0,1100 >> logs/$LOGFILE
echo '-l 5 -X opt0.0 -O mcts0,1200' ; ./bin/quixo $FLAGS_BASE -l 5 -X opt0.0 -O mcts0,1200 >> logs/$LOGFILE
echo '-l 5 -X opt0.0 -O mcts0,1300' ; ./bin/quixo $FLAGS_BASE -l 5 -X opt0.0 -O mcts0,1300 >> logs/$LOGFILE
echo '-l 5 -X opt0.0 -O mcts0,1400' ; ./bin/quixo $FLAGS_BASE -l 5 -X opt0.0 -O mcts0,1400 >> logs/$LOGFILE
echo '-l 5 -X opt0.0 -O mcts0,1500' ; ./bin/quixo $FLAGS_BASE -l 5 -X opt0.0 -O mcts0,1500 >> logs/$LOGFILE
echo '-l 5 -X opt0.0 -O mcts0,1600' ; ./bin/quixo $FLAGS_BASE -l 5 -X opt0.0 -O mcts0,1600 >> logs/$LOGFILE
echo '-l 5 -X opt0.0 -O mcts0,1700' ; ./bin/quixo $FLAGS_BASE -l 5 -X opt0.0 -O mcts0,1700 >> logs/$LOGFILE
echo '-l 5 -X opt0.0 -O mcts0,1800' ; ./bin/quixo $FLAGS_BASE -l 5 -X opt0.0 -O mcts0,1800 >> logs/$LOGFILE
echo '-l 5 -X opt0.0 -O mcts0,1900' ; ./bin/quixo $FLAGS_BASE -l 5 -X opt0.0 -O mcts0,1900 >> logs/$LOGFILE
echo '-l 5 -X opt0.0 -O mcts0,2000' ; ./bin/quixo $FLAGS_BASE -l 5 -X opt0.0 -O mcts0,2000 >> logs/$LOGFILE
echo

echo '-l 5 -X opt0.01 -O mcts0,1100' ; ./bin/quixo $FLAGS_BASE -l 5 -X opt0.01 -O mcts0,1100 >> logs/$LOGFILE
echo '-l 5 -X opt0.01 -O mcts0,1200' ; ./bin/quixo $FLAGS_BASE -l 5 -X opt0.01 -O mcts0,1200 >> logs/$LOGFILE
echo '-l 5 -X opt0.01 -O mcts0,1300' ; ./bin/quixo $FLAGS_BASE -l 5 -X opt0.01 -O mcts0,1300 >> logs/$LOGFILE
echo '-l 5 -X opt0.01 -O mcts0,1400' ; ./bin/quixo $FLAGS_BASE -l 5 -X opt0.01 -O mcts0,1400 >> logs/$LOGFILE
echo '-l 5 -X opt0.01 -O mcts0,1500' ; ./bin/quixo $FLAGS_BASE -l 5 -X opt0.01 -O mcts0,1500 >> logs/$LOGFILE
echo '-l 5 -X opt0.01 -O mcts0,1600' ; ./bin/quixo $FLAGS_BASE -l 5 -X opt0.01 -O mcts0,1600 >> logs/$LOGFILE
echo '-l 5 -X opt0.01 -O mcts0,1700' ; ./bin/quixo $FLAGS_BASE -l 5 -X opt0.01 -O mcts0,1700 >> logs/$LOGFILE
echo '-l 5 -X opt0.01 -O mcts0,1800' ; ./bin/quixo $FLAGS_BASE -l 5 -X opt0.01 -O mcts0,1800 >> logs/$LOGFILE
echo '-l 5 -X opt0.01 -O mcts0,1900' ; ./bin/quixo $FLAGS_BASE -l 5 -X opt0.01 -O mcts0,1900 >> logs/$LOGFILE
echo '-l 5 -X opt0.01 -O mcts0,2000' ; ./bin/quixo $FLAGS_BASE -l 5 -X opt0.01 -O mcts0,2000 >> logs/$LOGFILE
echo

echo '-l 5 -X opt0.05 -O mcts0,1100' ; ./bin/quixo $FLAGS_BASE -l 5 -X opt0.05 -O mcts0,1100 >> logs/$LOGFILE
echo '-l 5 -X opt0.05 -O mcts0,1200' ; ./bin/quixo $FLAGS_BASE -l 5 -X opt0.05 -O mcts0,1200 >> logs/$LOGFILE
echo '-l 5 -X opt0.05 -O mcts0,1300' ; ./bin/quixo $FLAGS_BASE -l 5 -X opt0.05 -O mcts0,1300 >> logs/$LOGFILE
echo '-l 5 -X opt0.05 -O mcts0,1400' ; ./bin/quixo $FLAGS_BASE -l 5 -X opt0.05 -O mcts0,1400 >> logs/$LOGFILE
echo '-l 5 -X opt0.05 -O mcts0,1500' ; ./bin/quixo $FLAGS_BASE -l 5 -X opt0.05 -O mcts0,1500 >> logs/$LOGFILE
echo '-l 5 -X opt0.05 -O mcts0,1600' ; ./bin/quixo $FLAGS_BASE -l 5 -X opt0.05 -O mcts0,1600 >> logs/$LOGFILE
echo '-l 5 -X opt0.05 -O mcts0,1700' ; ./bin/quixo $FLAGS_BASE -l 5 -X opt0.05 -O mcts0,1700 >> logs/$LOGFILE
echo '-l 5 -X opt0.05 -O mcts0,1800' ; ./bin/quixo $FLAGS_BASE -l 5 -X opt0.05 -O mcts0,1800 >> logs/$LOGFILE
echo '-l 5 -X opt0.05 -O mcts0,1900' ; ./bin/quixo $FLAGS_BASE -l 5 -X opt0.05 -O mcts0,1900 >> logs/$LOGFILE
echo '-l 5 -X opt0.05 -O mcts0,2000' ; ./bin/quixo $FLAGS_BASE -l 5 -X opt0.05 -O mcts0,2000 >> logs/$LOGFILE
echo

echo '-l 5 -X opt0.1 -O mcts0,1100' ; ./bin/quixo $FLAGS_BASE -l 5 -X opt0.1 -O mcts0,1100 >> logs/$LOGFILE
echo '-l 5 -X opt0.1 -O mcts0,1200' ; ./bin/quixo $FLAGS_BASE -l 5 -X opt0.1 -O mcts0,1200 >> logs/$LOGFILE
echo '-l 5 -X opt0.1 -O mcts0,1300' ; ./bin/quixo $FLAGS_BASE -l 5 -X opt0.1 -O mcts0,1300 >> logs/$LOGFILE
echo '-l 5 -X opt0.1 -O mcts0,1400' ; ./bin/quixo $FLAGS_BASE -l 5 -X opt0.1 -O mcts0,1400 >> logs/$LOGFILE
echo '-l 5 -X opt0.1 -O mcts0,1500' ; ./bin/quixo $FLAGS_BASE -l 5 -X opt0.1 -O mcts0,1500 >> logs/$LOGFILE
echo '-l 5 -X opt0.1 -O mcts0,1600' ; ./bin/quixo $FLAGS_BASE -l 5 -X opt0.1 -O mcts0,1600 >> logs/$LOGFILE
echo '-l 5 -X opt0.1 -O mcts0,1700' ; ./bin/quixo $FLAGS_BASE -l 5 -X opt0.1 -O mcts0,1700 >> logs/$LOGFILE
echo '-l 5 -X opt0.1 -O mcts0,1800' ; ./bin/quixo $FLAGS_BASE -l 5 -X opt0.1 -O mcts0,1800 >> logs/$LOGFILE
echo '-l 5 -X opt0.1 -O mcts0,1900' ; ./bin/quixo $FLAGS_BASE -l 5 -X opt0.1 -O mcts0,1900 >> logs/$LOGFILE
echo '-l 5 -X opt0.1 -O mcts0,2000' ; ./bin/quixo $FLAGS_BASE -l 5 -X opt0.1 -O mcts0,2000 >> logs/$LOGFILE
echo

echo '-l 5 -X opt0.2 -O mcts0,1100' ; ./bin/quixo $FLAGS_BASE -l 5 -X opt0.2 -O mcts0,1100 >> logs/$LOGFILE
echo '-l 5 -X opt0.2 -O mcts0,1200' ; ./bin/quixo $FLAGS_BASE -l 5 -X opt0.2 -O mcts0,1200 >> logs/$LOGFILE
echo '-l 5 -X opt0.2 -O mcts0,1300' ; ./bin/quixo $FLAGS_BASE -l 5 -X opt0.2 -O mcts0,1300 >> logs/$LOGFILE
echo '-l 5 -X opt0.2 -O mcts0,1400' ; ./bin/quixo $FLAGS_BASE -l 5 -X opt0.2 -O mcts0,1400 >> logs/$LOGFILE
echo '-l 5 -X opt0.2 -O mcts0,1500' ; ./bin/quixo $FLAGS_BASE -l 5 -X opt0.2 -O mcts0,1500 >> logs/$LOGFILE
echo '-l 5 -X opt0.2 -O mcts0,1600' ; ./bin/quixo $FLAGS_BASE -l 5 -X opt0.2 -O mcts0,1600 >> logs/$LOGFILE
echo '-l 5 -X opt0.2 -O mcts0,1700' ; ./bin/quixo $FLAGS_BASE -l 5 -X opt0.2 -O mcts0,1700 >> logs/$LOGFILE
echo '-l 5 -X opt0.2 -O mcts0,1800' ; ./bin/quixo $FLAGS_BASE -l 5 -X opt0.2 -O mcts0,1800 >> logs/$LOGFILE
echo '-l 5 -X opt0.2 -O mcts0,1900' ; ./bin/quixo $FLAGS_BASE -l 5 -X opt0.2 -O mcts0,1900 >> logs/$LOGFILE
echo '-l 5 -X opt0.2 -O mcts0,2000' ; ./bin/quixo $FLAGS_BASE -l 5 -X opt0.2 -O mcts0,2000 >> logs/$LOGFILE
echo

echo '-l 5 -X opt0.5 -O mcts0,1100' ; ./bin/quixo $FLAGS_BASE -l 5 -X opt0.5 -O mcts0,1100 >> logs/$LOGFILE
echo '-l 5 -X opt0.5 -O mcts0,1200' ; ./bin/quixo $FLAGS_BASE -l 5 -X opt0.5 -O mcts0,1200 >> logs/$LOGFILE
echo '-l 5 -X opt0.5 -O mcts0,1300' ; ./bin/quixo $FLAGS_BASE -l 5 -X opt0.5 -O mcts0,1300 >> logs/$LOGFILE
echo '-l 5 -X opt0.5 -O mcts0,1400' ; ./bin/quixo $FLAGS_BASE -l 5 -X opt0.5 -O mcts0,1400 >> logs/$LOGFILE
echo '-l 5 -X opt0.5 -O mcts0,1500' ; ./bin/quixo $FLAGS_BASE -l 5 -X opt0.5 -O mcts0,1500 >> logs/$LOGFILE
echo '-l 5 -X opt0.5 -O mcts0,1600' ; ./bin/quixo $FLAGS_BASE -l 5 -X opt0.5 -O mcts0,1600 >> logs/$LOGFILE
echo '-l 5 -X opt0.5 -O mcts0,1700' ; ./bin/quixo $FLAGS_BASE -l 5 -X opt0.5 -O mcts0,1700 >> logs/$LOGFILE
echo '-l 5 -X opt0.5 -O mcts0,1800' ; ./bin/quixo $FLAGS_BASE -l 5 -X opt0.5 -O mcts0,1800 >> logs/$LOGFILE
echo '-l 5 -X opt0.5 -O mcts0,1900' ; ./bin/quixo $FLAGS_BASE -l 5 -X opt0.5 -O mcts0,1900 >> logs/$LOGFILE
echo '-l 5 -X opt0.5 -O mcts0,2000' ; ./bin/quixo $FLAGS_BASE -l 5 -X opt0.5 -O mcts0,2000 >> logs/$LOGFILE
echo
echo

# O opt X mcts
echo '-l 5 -O opt0.0 -X mcts0,1100' ; ./bin/quixo $FLAGS_BASE -l 5 -O opt0.0 -X mcts0,1100 >> logs/$LOGFILE
echo '-l 5 -O opt0.0 -X mcts0,1200' ; ./bin/quixo $FLAGS_BASE -l 5 -O opt0.0 -X mcts0,1200 >> logs/$LOGFILE
echo '-l 5 -O opt0.0 -X mcts0,1300' ; ./bin/quixo $FLAGS_BASE -l 5 -O opt0.0 -X mcts0,1300 >> logs/$LOGFILE
echo '-l 5 -O opt0.0 -X mcts0,1400' ; ./bin/quixo $FLAGS_BASE -l 5 -O opt0.0 -X mcts0,1400 >> logs/$LOGFILE
echo '-l 5 -O opt0.0 -X mcts0,1500' ; ./bin/quixo $FLAGS_BASE -l 5 -O opt0.0 -X mcts0,1500 >> logs/$LOGFILE
echo '-l 5 -O opt0.0 -X mcts0,1600' ; ./bin/quixo $FLAGS_BASE -l 5 -O opt0.0 -X mcts0,1600 >> logs/$LOGFILE
echo '-l 5 -O opt0.0 -X mcts0,1700' ; ./bin/quixo $FLAGS_BASE -l 5 -O opt0.0 -X mcts0,1700 >> logs/$LOGFILE
echo '-l 5 -O opt0.0 -X mcts0,1800' ; ./bin/quixo $FLAGS_BASE -l 5 -O opt0.0 -X mcts0,1800 >> logs/$LOGFILE
echo '-l 5 -O opt0.0 -X mcts0,1900' ; ./bin/quixo $FLAGS_BASE -l 5 -O opt0.0 -X mcts0,1900 >> logs/$LOGFILE
echo '-l 5 -O opt0.0 -X mcts0,2000' ; ./bin/quixo $FLAGS_BASE -l 5 -O opt0.0 -X mcts0,2000 >> logs/$LOGFILE
echo

echo '-l 5 -O opt0.01 -X mcts0,1100' ; ./bin/quixo $FLAGS_BASE -l 5 -O opt0.01 -X mcts0,1100 >> logs/$LOGFILE
echo '-l 5 -O opt0.01 -X mcts0,1200' ; ./bin/quixo $FLAGS_BASE -l 5 -O opt0.01 -X mcts0,1200 >> logs/$LOGFILE
echo '-l 5 -O opt0.01 -X mcts0,1300' ; ./bin/quixo $FLAGS_BASE -l 5 -O opt0.01 -X mcts0,1300 >> logs/$LOGFILE
echo '-l 5 -O opt0.01 -X mcts0,1400' ; ./bin/quixo $FLAGS_BASE -l 5 -O opt0.01 -X mcts0,1400 >> logs/$LOGFILE
echo '-l 5 -O opt0.01 -X mcts0,1500' ; ./bin/quixo $FLAGS_BASE -l 5 -O opt0.01 -X mcts0,1500 >> logs/$LOGFILE
echo '-l 5 -O opt0.01 -X mcts0,1600' ; ./bin/quixo $FLAGS_BASE -l 5 -O opt0.01 -X mcts0,1600 >> logs/$LOGFILE
echo '-l 5 -O opt0.01 -X mcts0,1700' ; ./bin/quixo $FLAGS_BASE -l 5 -O opt0.01 -X mcts0,1700 >> logs/$LOGFILE
echo '-l 5 -O opt0.01 -X mcts0,1800' ; ./bin/quixo $FLAGS_BASE -l 5 -O opt0.01 -X mcts0,1800 >> logs/$LOGFILE
echo '-l 5 -O opt0.01 -X mcts0,1900' ; ./bin/quixo $FLAGS_BASE -l 5 -O opt0.01 -X mcts0,1900 >> logs/$LOGFILE
echo '-l 5 -O opt0.01 -X mcts0,2000' ; ./bin/quixo $FLAGS_BASE -l 5 -O opt0.01 -X mcts0,2000 >> logs/$LOGFILE
echo

echo '-l 5 -O opt0.05 -X mcts0,1100' ; ./bin/quixo $FLAGS_BASE -l 5 -O opt0.05 -X mcts0,1100 >> logs/$LOGFILE
echo '-l 5 -O opt0.05 -X mcts0,1200' ; ./bin/quixo $FLAGS_BASE -l 5 -O opt0.05 -X mcts0,1200 >> logs/$LOGFILE
echo '-l 5 -O opt0.05 -X mcts0,1300' ; ./bin/quixo $FLAGS_BASE -l 5 -O opt0.05 -X mcts0,1300 >> logs/$LOGFILE
echo '-l 5 -O opt0.05 -X mcts0,1400' ; ./bin/quixo $FLAGS_BASE -l 5 -O opt0.05 -X mcts0,1400 >> logs/$LOGFILE
echo '-l 5 -O opt0.05 -X mcts0,1500' ; ./bin/quixo $FLAGS_BASE -l 5 -O opt0.05 -X mcts0,1500 >> logs/$LOGFILE
echo '-l 5 -O opt0.05 -X mcts0,1600' ; ./bin/quixo $FLAGS_BASE -l 5 -O opt0.05 -X mcts0,1600 >> logs/$LOGFILE
echo '-l 5 -O opt0.05 -X mcts0,1700' ; ./bin/quixo $FLAGS_BASE -l 5 -O opt0.05 -X mcts0,1700 >> logs/$LOGFILE
echo '-l 5 -O opt0.05 -X mcts0,1800' ; ./bin/quixo $FLAGS_BASE -l 5 -O opt0.05 -X mcts0,1800 >> logs/$LOGFILE
echo '-l 5 -O opt0.05 -X mcts0,1900' ; ./bin/quixo $FLAGS_BASE -l 5 -O opt0.05 -X mcts0,1900 >> logs/$LOGFILE
echo '-l 5 -O opt0.05 -X mcts0,2000' ; ./bin/quixo $FLAGS_BASE -l 5 -O opt0.05 -X mcts0,2000 >> logs/$LOGFILE
echo

echo '-l 5 -O opt0.1 -X mcts0,1100' ; ./bin/quixo $FLAGS_BASE -l 5 -O opt0.1 -X mcts0,1100 >> logs/$LOGFILE
echo '-l 5 -O opt0.1 -X mcts0,1200' ; ./bin/quixo $FLAGS_BASE -l 5 -O opt0.1 -X mcts0,1200 >> logs/$LOGFILE
echo '-l 5 -O opt0.1 -X mcts0,1300' ; ./bin/quixo $FLAGS_BASE -l 5 -O opt0.1 -X mcts0,1300 >> logs/$LOGFILE
echo '-l 5 -O opt0.1 -X mcts0,1400' ; ./bin/quixo $FLAGS_BASE -l 5 -O opt0.1 -X mcts0,1400 >> logs/$LOGFILE
echo '-l 5 -O opt0.1 -X mcts0,1500' ; ./bin/quixo $FLAGS_BASE -l 5 -O opt0.1 -X mcts0,1500 >> logs/$LOGFILE
echo '-l 5 -O opt0.1 -X mcts0,1600' ; ./bin/quixo $FLAGS_BASE -l 5 -O opt0.1 -X mcts0,1600 >> logs/$LOGFILE
echo '-l 5 -O opt0.1 -X mcts0,1700' ; ./bin/quixo $FLAGS_BASE -l 5 -O opt0.1 -X mcts0,1700 >> logs/$LOGFILE
echo '-l 5 -O opt0.1 -X mcts0,1800' ; ./bin/quixo $FLAGS_BASE -l 5 -O opt0.1 -X mcts0,1800 >> logs/$LOGFILE
echo '-l 5 -O opt0.1 -X mcts0,1900' ; ./bin/quixo $FLAGS_BASE -l 5 -O opt0.1 -X mcts0,1900 >> logs/$LOGFILE
echo '-l 5 -O opt0.1 -X mcts0,2000' ; ./bin/quixo $FLAGS_BASE -l 5 -O opt0.1 -X mcts0,2000 >> logs/$LOGFILE
echo

echo '-l 5 -O opt0.2 -X mcts0,1100' ; ./bin/quixo $FLAGS_BASE -l 5 -O opt0.2 -X mcts0,1100 >> logs/$LOGFILE
echo '-l 5 -O opt0.2 -X mcts0,1200' ; ./bin/quixo $FLAGS_BASE -l 5 -O opt0.2 -X mcts0,1200 >> logs/$LOGFILE
echo '-l 5 -O opt0.2 -X mcts0,1300' ; ./bin/quixo $FLAGS_BASE -l 5 -O opt0.2 -X mcts0,1300 >> logs/$LOGFILE
echo '-l 5 -O opt0.2 -X mcts0,1400' ; ./bin/quixo $FLAGS_BASE -l 5 -O opt0.2 -X mcts0,1400 >> logs/$LOGFILE
echo '-l 5 -O opt0.2 -X mcts0,1500' ; ./bin/quixo $FLAGS_BASE -l 5 -O opt0.2 -X mcts0,1500 >> logs/$LOGFILE
echo '-l 5 -O opt0.2 -X mcts0,1600' ; ./bin/quixo $FLAGS_BASE -l 5 -O opt0.2 -X mcts0,1600 >> logs/$LOGFILE
echo '-l 5 -O opt0.2 -X mcts0,1700' ; ./bin/quixo $FLAGS_BASE -l 5 -O opt0.2 -X mcts0,1700 >> logs/$LOGFILE
echo '-l 5 -O opt0.2 -X mcts0,1800' ; ./bin/quixo $FLAGS_BASE -l 5 -O opt0.2 -X mcts0,1800 >> logs/$LOGFILE
echo '-l 5 -O opt0.2 -X mcts0,1900' ; ./bin/quixo $FLAGS_BASE -l 5 -O opt0.2 -X mcts0,1900 >> logs/$LOGFILE
echo '-l 5 -O opt0.2 -X mcts0,2000' ; ./bin/quixo $FLAGS_BASE -l 5 -O opt0.2 -X mcts0,2000 >> logs/$LOGFILE
echo

echo '-l 5 -O opt0.5 -X mcts0,1100' ; ./bin/quixo $FLAGS_BASE -l 5 -O opt0.5 -X mcts0,1100 >> logs/$LOGFILE
echo '-l 5 -O opt0.5 -X mcts0,1200' ; ./bin/quixo $FLAGS_BASE -l 5 -O opt0.5 -X mcts0,1200 >> logs/$LOGFILE
echo '-l 5 -O opt0.5 -X mcts0,1300' ; ./bin/quixo $FLAGS_BASE -l 5 -O opt0.5 -X mcts0,1300 >> logs/$LOGFILE
echo '-l 5 -O opt0.5 -X mcts0,1400' ; ./bin/quixo $FLAGS_BASE -l 5 -O opt0.5 -X mcts0,1400 >> logs/$LOGFILE
echo '-l 5 -O opt0.5 -X mcts0,1500' ; ./bin/quixo $FLAGS_BASE -l 5 -O opt0.5 -X mcts0,1500 >> logs/$LOGFILE
echo '-l 5 -O opt0.5 -X mcts0,1600' ; ./bin/quixo $FLAGS_BASE -l 5 -O opt0.5 -X mcts0,1600 >> logs/$LOGFILE
echo '-l 5 -O opt0.5 -X mcts0,1700' ; ./bin/quixo $FLAGS_BASE -l 5 -O opt0.5 -X mcts0,1700 >> logs/$LOGFILE
echo '-l 5 -O opt0.5 -X mcts0,1800' ; ./bin/quixo $FLAGS_BASE -l 5 -O opt0.5 -X mcts0,1800 >> logs/$LOGFILE
echo '-l 5 -O opt0.5 -X mcts0,1900' ; ./bin/quixo $FLAGS_BASE -l 5 -O opt0.5 -X mcts0,1900 >> logs/$LOGFILE
echo '-l 5 -O opt0.5 -X mcts0,2000' ; ./bin/quixo $FLAGS_BASE -l 5 -O opt0.5 -X mcts0,2000 >> logs/$LOGFILE
echo
echo

################################################################################
################################################################################
# X opt O mcts
echo '-l 4 -X opt0.0 -O mcts0,1100' ; ./bin/quixo $FLAGS_BASE -l 4 -X opt0.0 -O mcts0,1100 >> logs/$LOGFILE
echo '-l 4 -X opt0.0 -O mcts0,1200' ; ./bin/quixo $FLAGS_BASE -l 4 -X opt0.0 -O mcts0,1200 >> logs/$LOGFILE
echo '-l 4 -X opt0.0 -O mcts0,1300' ; ./bin/quixo $FLAGS_BASE -l 4 -X opt0.0 -O mcts0,1300 >> logs/$LOGFILE
echo '-l 4 -X opt0.0 -O mcts0,1400' ; ./bin/quixo $FLAGS_BASE -l 4 -X opt0.0 -O mcts0,1400 >> logs/$LOGFILE
echo '-l 4 -X opt0.0 -O mcts0,1500' ; ./bin/quixo $FLAGS_BASE -l 4 -X opt0.0 -O mcts0,1500 >> logs/$LOGFILE
echo '-l 4 -X opt0.0 -O mcts0,1600' ; ./bin/quixo $FLAGS_BASE -l 4 -X opt0.0 -O mcts0,1600 >> logs/$LOGFILE
echo '-l 4 -X opt0.0 -O mcts0,1700' ; ./bin/quixo $FLAGS_BASE -l 4 -X opt0.0 -O mcts0,1700 >> logs/$LOGFILE
echo '-l 4 -X opt0.0 -O mcts0,1800' ; ./bin/quixo $FLAGS_BASE -l 4 -X opt0.0 -O mcts0,1800 >> logs/$LOGFILE
echo '-l 4 -X opt0.0 -O mcts0,1900' ; ./bin/quixo $FLAGS_BASE -l 4 -X opt0.0 -O mcts0,1900 >> logs/$LOGFILE
echo '-l 4 -X opt0.0 -O mcts0,2000' ; ./bin/quixo $FLAGS_BASE -l 4 -X opt0.0 -O mcts0,2000 >> logs/$LOGFILE
echo

echo '-l 4 -X opt0.01 -O mcts0,1100' ; ./bin/quixo $FLAGS_BASE -l 4 -X opt0.01 -O mcts0,1100 >> logs/$LOGFILE
echo '-l 4 -X opt0.01 -O mcts0,1200' ; ./bin/quixo $FLAGS_BASE -l 4 -X opt0.01 -O mcts0,1200 >> logs/$LOGFILE
echo '-l 4 -X opt0.01 -O mcts0,1300' ; ./bin/quixo $FLAGS_BASE -l 4 -X opt0.01 -O mcts0,1300 >> logs/$LOGFILE
echo '-l 4 -X opt0.01 -O mcts0,1400' ; ./bin/quixo $FLAGS_BASE -l 4 -X opt0.01 -O mcts0,1400 >> logs/$LOGFILE
echo '-l 4 -X opt0.01 -O mcts0,1500' ; ./bin/quixo $FLAGS_BASE -l 4 -X opt0.01 -O mcts0,1500 >> logs/$LOGFILE
echo '-l 4 -X opt0.01 -O mcts0,1600' ; ./bin/quixo $FLAGS_BASE -l 4 -X opt0.01 -O mcts0,1600 >> logs/$LOGFILE
echo '-l 4 -X opt0.01 -O mcts0,1700' ; ./bin/quixo $FLAGS_BASE -l 4 -X opt0.01 -O mcts0,1700 >> logs/$LOGFILE
echo '-l 4 -X opt0.01 -O mcts0,1800' ; ./bin/quixo $FLAGS_BASE -l 4 -X opt0.01 -O mcts0,1800 >> logs/$LOGFILE
echo '-l 4 -X opt0.01 -O mcts0,1900' ; ./bin/quixo $FLAGS_BASE -l 4 -X opt0.01 -O mcts0,1900 >> logs/$LOGFILE
echo '-l 4 -X opt0.01 -O mcts0,2000' ; ./bin/quixo $FLAGS_BASE -l 4 -X opt0.01 -O mcts0,2000 >> logs/$LOGFILE
echo

echo '-l 4 -X opt0.05 -O mcts0,1100' ; ./bin/quixo $FLAGS_BASE -l 4 -X opt0.05 -O mcts0,1100 >> logs/$LOGFILE
echo '-l 4 -X opt0.05 -O mcts0,1200' ; ./bin/quixo $FLAGS_BASE -l 4 -X opt0.05 -O mcts0,1200 >> logs/$LOGFILE
echo '-l 4 -X opt0.05 -O mcts0,1300' ; ./bin/quixo $FLAGS_BASE -l 4 -X opt0.05 -O mcts0,1300 >> logs/$LOGFILE
echo '-l 4 -X opt0.05 -O mcts0,1400' ; ./bin/quixo $FLAGS_BASE -l 4 -X opt0.05 -O mcts0,1400 >> logs/$LOGFILE
echo '-l 4 -X opt0.05 -O mcts0,1500' ; ./bin/quixo $FLAGS_BASE -l 4 -X opt0.05 -O mcts0,1500 >> logs/$LOGFILE
echo '-l 4 -X opt0.05 -O mcts0,1600' ; ./bin/quixo $FLAGS_BASE -l 4 -X opt0.05 -O mcts0,1600 >> logs/$LOGFILE
echo '-l 4 -X opt0.05 -O mcts0,1700' ; ./bin/quixo $FLAGS_BASE -l 4 -X opt0.05 -O mcts0,1700 >> logs/$LOGFILE
echo '-l 4 -X opt0.05 -O mcts0,1800' ; ./bin/quixo $FLAGS_BASE -l 4 -X opt0.05 -O mcts0,1800 >> logs/$LOGFILE
echo '-l 4 -X opt0.05 -O mcts0,1900' ; ./bin/quixo $FLAGS_BASE -l 4 -X opt0.05 -O mcts0,1900 >> logs/$LOGFILE
echo '-l 4 -X opt0.05 -O mcts0,2000' ; ./bin/quixo $FLAGS_BASE -l 4 -X opt0.05 -O mcts0,2000 >> logs/$LOGFILE
echo

echo '-l 4 -X opt0.1 -O mcts0,1100' ; ./bin/quixo $FLAGS_BASE -l 4 -X opt0.1 -O mcts0,1100 >> logs/$LOGFILE
echo '-l 4 -X opt0.1 -O mcts0,1200' ; ./bin/quixo $FLAGS_BASE -l 4 -X opt0.1 -O mcts0,1200 >> logs/$LOGFILE
echo '-l 4 -X opt0.1 -O mcts0,1300' ; ./bin/quixo $FLAGS_BASE -l 4 -X opt0.1 -O mcts0,1300 >> logs/$LOGFILE
echo '-l 4 -X opt0.1 -O mcts0,1400' ; ./bin/quixo $FLAGS_BASE -l 4 -X opt0.1 -O mcts0,1400 >> logs/$LOGFILE
echo '-l 4 -X opt0.1 -O mcts0,1500' ; ./bin/quixo $FLAGS_BASE -l 4 -X opt0.1 -O mcts0,1500 >> logs/$LOGFILE
echo '-l 4 -X opt0.1 -O mcts0,1600' ; ./bin/quixo $FLAGS_BASE -l 4 -X opt0.1 -O mcts0,1600 >> logs/$LOGFILE
echo '-l 4 -X opt0.1 -O mcts0,1700' ; ./bin/quixo $FLAGS_BASE -l 4 -X opt0.1 -O mcts0,1700 >> logs/$LOGFILE
echo '-l 4 -X opt0.1 -O mcts0,1800' ; ./bin/quixo $FLAGS_BASE -l 4 -X opt0.1 -O mcts0,1800 >> logs/$LOGFILE
echo '-l 4 -X opt0.1 -O mcts0,1900' ; ./bin/quixo $FLAGS_BASE -l 4 -X opt0.1 -O mcts0,1900 >> logs/$LOGFILE
echo '-l 4 -X opt0.1 -O mcts0,2000' ; ./bin/quixo $FLAGS_BASE -l 4 -X opt0.1 -O mcts0,2000 >> logs/$LOGFILE
echo

echo '-l 4 -X opt0.2 -O mcts0,1100' ; ./bin/quixo $FLAGS_BASE -l 4 -X opt0.2 -O mcts0,1100 >> logs/$LOGFILE
echo '-l 4 -X opt0.2 -O mcts0,1200' ; ./bin/quixo $FLAGS_BASE -l 4 -X opt0.2 -O mcts0,1200 >> logs/$LOGFILE
echo '-l 4 -X opt0.2 -O mcts0,1300' ; ./bin/quixo $FLAGS_BASE -l 4 -X opt0.2 -O mcts0,1300 >> logs/$LOGFILE
echo '-l 4 -X opt0.2 -O mcts0,1400' ; ./bin/quixo $FLAGS_BASE -l 4 -X opt0.2 -O mcts0,1400 >> logs/$LOGFILE
echo '-l 4 -X opt0.2 -O mcts0,1500' ; ./bin/quixo $FLAGS_BASE -l 4 -X opt0.2 -O mcts0,1500 >> logs/$LOGFILE
echo '-l 4 -X opt0.2 -O mcts0,1600' ; ./bin/quixo $FLAGS_BASE -l 4 -X opt0.2 -O mcts0,1600 >> logs/$LOGFILE
echo '-l 4 -X opt0.2 -O mcts0,1700' ; ./bin/quixo $FLAGS_BASE -l 4 -X opt0.2 -O mcts0,1700 >> logs/$LOGFILE
echo '-l 4 -X opt0.2 -O mcts0,1800' ; ./bin/quixo $FLAGS_BASE -l 4 -X opt0.2 -O mcts0,1800 >> logs/$LOGFILE
echo '-l 4 -X opt0.2 -O mcts0,1900' ; ./bin/quixo $FLAGS_BASE -l 4 -X opt0.2 -O mcts0,1900 >> logs/$LOGFILE
echo '-l 4 -X opt0.2 -O mcts0,2000' ; ./bin/quixo $FLAGS_BASE -l 4 -X opt0.2 -O mcts0,2000 >> logs/$LOGFILE
echo

echo '-l 4 -X opt0.5 -O mcts0,1100' ; ./bin/quixo $FLAGS_BASE -l 4 -X opt0.5 -O mcts0,1100 >> logs/$LOGFILE
echo '-l 4 -X opt0.5 -O mcts0,1200' ; ./bin/quixo $FLAGS_BASE -l 4 -X opt0.5 -O mcts0,1200 >> logs/$LOGFILE
echo '-l 4 -X opt0.5 -O mcts0,1300' ; ./bin/quixo $FLAGS_BASE -l 4 -X opt0.5 -O mcts0,1300 >> logs/$LOGFILE
echo '-l 4 -X opt0.5 -O mcts0,1400' ; ./bin/quixo $FLAGS_BASE -l 4 -X opt0.5 -O mcts0,1400 >> logs/$LOGFILE
echo '-l 4 -X opt0.5 -O mcts0,1500' ; ./bin/quixo $FLAGS_BASE -l 4 -X opt0.5 -O mcts0,1500 >> logs/$LOGFILE
echo '-l 4 -X opt0.5 -O mcts0,1600' ; ./bin/quixo $FLAGS_BASE -l 4 -X opt0.5 -O mcts0,1600 >> logs/$LOGFILE
echo '-l 4 -X opt0.5 -O mcts0,1700' ; ./bin/quixo $FLAGS_BASE -l 4 -X opt0.5 -O mcts0,1700 >> logs/$LOGFILE
echo '-l 4 -X opt0.5 -O mcts0,1800' ; ./bin/quixo $FLAGS_BASE -l 4 -X opt0.5 -O mcts0,1800 >> logs/$LOGFILE
echo '-l 4 -X opt0.5 -O mcts0,1900' ; ./bin/quixo $FLAGS_BASE -l 4 -X opt0.5 -O mcts0,1900 >> logs/$LOGFILE
echo '-l 4 -X opt0.5 -O mcts0,2000' ; ./bin/quixo $FLAGS_BASE -l 4 -X opt0.5 -O mcts0,2000 >> logs/$LOGFILE
echo
echo

# O opt X mcts
echo '-l 4 -O opt0.0 -X mcts0,1100' ; ./bin/quixo $FLAGS_BASE -l 4 -O opt0.0 -X mcts0,1100 >> logs/$LOGFILE
echo '-l 4 -O opt0.0 -X mcts0,1200' ; ./bin/quixo $FLAGS_BASE -l 4 -O opt0.0 -X mcts0,1200 >> logs/$LOGFILE
echo '-l 4 -O opt0.0 -X mcts0,1300' ; ./bin/quixo $FLAGS_BASE -l 4 -O opt0.0 -X mcts0,1300 >> logs/$LOGFILE
echo '-l 4 -O opt0.0 -X mcts0,1400' ; ./bin/quixo $FLAGS_BASE -l 4 -O opt0.0 -X mcts0,1400 >> logs/$LOGFILE
echo '-l 4 -O opt0.0 -X mcts0,1500' ; ./bin/quixo $FLAGS_BASE -l 4 -O opt0.0 -X mcts0,1500 >> logs/$LOGFILE
echo '-l 4 -O opt0.0 -X mcts0,1600' ; ./bin/quixo $FLAGS_BASE -l 4 -O opt0.0 -X mcts0,1600 >> logs/$LOGFILE
echo '-l 4 -O opt0.0 -X mcts0,1700' ; ./bin/quixo $FLAGS_BASE -l 4 -O opt0.0 -X mcts0,1700 >> logs/$LOGFILE
echo '-l 4 -O opt0.0 -X mcts0,1800' ; ./bin/quixo $FLAGS_BASE -l 4 -O opt0.0 -X mcts0,1800 >> logs/$LOGFILE
echo '-l 4 -O opt0.0 -X mcts0,1900' ; ./bin/quixo $FLAGS_BASE -l 4 -O opt0.0 -X mcts0,1900 >> logs/$LOGFILE
echo '-l 4 -O opt0.0 -X mcts0,2000' ; ./bin/quixo $FLAGS_BASE -l 4 -O opt0.0 -X mcts0,2000 >> logs/$LOGFILE
echo

echo '-l 4 -O opt0.01 -X mcts0,1100' ; ./bin/quixo $FLAGS_BASE -l 4 -O opt0.01 -X mcts0,1100 >> logs/$LOGFILE
echo '-l 4 -O opt0.01 -X mcts0,1200' ; ./bin/quixo $FLAGS_BASE -l 4 -O opt0.01 -X mcts0,1200 >> logs/$LOGFILE
echo '-l 4 -O opt0.01 -X mcts0,1300' ; ./bin/quixo $FLAGS_BASE -l 4 -O opt0.01 -X mcts0,1300 >> logs/$LOGFILE
echo '-l 4 -O opt0.01 -X mcts0,1400' ; ./bin/quixo $FLAGS_BASE -l 4 -O opt0.01 -X mcts0,1400 >> logs/$LOGFILE
echo '-l 4 -O opt0.01 -X mcts0,1500' ; ./bin/quixo $FLAGS_BASE -l 4 -O opt0.01 -X mcts0,1500 >> logs/$LOGFILE
echo '-l 4 -O opt0.01 -X mcts0,1600' ; ./bin/quixo $FLAGS_BASE -l 4 -O opt0.01 -X mcts0,1600 >> logs/$LOGFILE
echo '-l 4 -O opt0.01 -X mcts0,1700' ; ./bin/quixo $FLAGS_BASE -l 4 -O opt0.01 -X mcts0,1700 >> logs/$LOGFILE
echo '-l 4 -O opt0.01 -X mcts0,1800' ; ./bin/quixo $FLAGS_BASE -l 4 -O opt0.01 -X mcts0,1800 >> logs/$LOGFILE
echo '-l 4 -O opt0.01 -X mcts0,1900' ; ./bin/quixo $FLAGS_BASE -l 4 -O opt0.01 -X mcts0,1900 >> logs/$LOGFILE
echo '-l 4 -O opt0.01 -X mcts0,2000' ; ./bin/quixo $FLAGS_BASE -l 4 -O opt0.01 -X mcts0,2000 >> logs/$LOGFILE
echo

echo '-l 4 -O opt0.05 -X mcts0,1100' ; ./bin/quixo $FLAGS_BASE -l 4 -O opt0.05 -X mcts0,1100 >> logs/$LOGFILE
echo '-l 4 -O opt0.05 -X mcts0,1200' ; ./bin/quixo $FLAGS_BASE -l 4 -O opt0.05 -X mcts0,1200 >> logs/$LOGFILE
echo '-l 4 -O opt0.05 -X mcts0,1300' ; ./bin/quixo $FLAGS_BASE -l 4 -O opt0.05 -X mcts0,1300 >> logs/$LOGFILE
echo '-l 4 -O opt0.05 -X mcts0,1400' ; ./bin/quixo $FLAGS_BASE -l 4 -O opt0.05 -X mcts0,1400 >> logs/$LOGFILE
echo '-l 4 -O opt0.05 -X mcts0,1500' ; ./bin/quixo $FLAGS_BASE -l 4 -O opt0.05 -X mcts0,1500 >> logs/$LOGFILE
echo '-l 4 -O opt0.05 -X mcts0,1600' ; ./bin/quixo $FLAGS_BASE -l 4 -O opt0.05 -X mcts0,1600 >> logs/$LOGFILE
echo '-l 4 -O opt0.05 -X mcts0,1700' ; ./bin/quixo $FLAGS_BASE -l 4 -O opt0.05 -X mcts0,1700 >> logs/$LOGFILE
echo '-l 4 -O opt0.05 -X mcts0,1800' ; ./bin/quixo $FLAGS_BASE -l 4 -O opt0.05 -X mcts0,1800 >> logs/$LOGFILE
echo '-l 4 -O opt0.05 -X mcts0,1900' ; ./bin/quixo $FLAGS_BASE -l 4 -O opt0.05 -X mcts0,1900 >> logs/$LOGFILE
echo '-l 4 -O opt0.05 -X mcts0,2000' ; ./bin/quixo $FLAGS_BASE -l 4 -O opt0.05 -X mcts0,2000 >> logs/$LOGFILE
echo

echo '-l 4 -O opt0.1 -X mcts0,1100' ; ./bin/quixo $FLAGS_BASE -l 4 -O opt0.1 -X mcts0,1100 >> logs/$LOGFILE
echo '-l 4 -O opt0.1 -X mcts0,1200' ; ./bin/quixo $FLAGS_BASE -l 4 -O opt0.1 -X mcts0,1200 >> logs/$LOGFILE
echo '-l 4 -O opt0.1 -X mcts0,1300' ; ./bin/quixo $FLAGS_BASE -l 4 -O opt0.1 -X mcts0,1300 >> logs/$LOGFILE
echo '-l 4 -O opt0.1 -X mcts0,1400' ; ./bin/quixo $FLAGS_BASE -l 4 -O opt0.1 -X mcts0,1400 >> logs/$LOGFILE
echo '-l 4 -O opt0.1 -X mcts0,1500' ; ./bin/quixo $FLAGS_BASE -l 4 -O opt0.1 -X mcts0,1500 >> logs/$LOGFILE
echo '-l 4 -O opt0.1 -X mcts0,1600' ; ./bin/quixo $FLAGS_BASE -l 4 -O opt0.1 -X mcts0,1600 >> logs/$LOGFILE
echo '-l 4 -O opt0.1 -X mcts0,1700' ; ./bin/quixo $FLAGS_BASE -l 4 -O opt0.1 -X mcts0,1700 >> logs/$LOGFILE
echo '-l 4 -O opt0.1 -X mcts0,1800' ; ./bin/quixo $FLAGS_BASE -l 4 -O opt0.1 -X mcts0,1800 >> logs/$LOGFILE
echo '-l 4 -O opt0.1 -X mcts0,1900' ; ./bin/quixo $FLAGS_BASE -l 4 -O opt0.1 -X mcts0,1900 >> logs/$LOGFILE
echo '-l 4 -O opt0.1 -X mcts0,2000' ; ./bin/quixo $FLAGS_BASE -l 4 -O opt0.1 -X mcts0,2000 >> logs/$LOGFILE
echo

echo '-l 4 -O opt0.2 -X mcts0,1100' ; ./bin/quixo $FLAGS_BASE -l 4 -O opt0.2 -X mcts0,1100 >> logs/$LOGFILE
echo '-l 4 -O opt0.2 -X mcts0,1200' ; ./bin/quixo $FLAGS_BASE -l 4 -O opt0.2 -X mcts0,1200 >> logs/$LOGFILE
echo '-l 4 -O opt0.2 -X mcts0,1300' ; ./bin/quixo $FLAGS_BASE -l 4 -O opt0.2 -X mcts0,1300 >> logs/$LOGFILE
echo '-l 4 -O opt0.2 -X mcts0,1400' ; ./bin/quixo $FLAGS_BASE -l 4 -O opt0.2 -X mcts0,1400 >> logs/$LOGFILE
echo '-l 4 -O opt0.2 -X mcts0,1500' ; ./bin/quixo $FLAGS_BASE -l 4 -O opt0.2 -X mcts0,1500 >> logs/$LOGFILE
echo '-l 4 -O opt0.2 -X mcts0,1600' ; ./bin/quixo $FLAGS_BASE -l 4 -O opt0.2 -X mcts0,1600 >> logs/$LOGFILE
echo '-l 4 -O opt0.2 -X mcts0,1700' ; ./bin/quixo $FLAGS_BASE -l 4 -O opt0.2 -X mcts0,1700 >> logs/$LOGFILE
echo '-l 4 -O opt0.2 -X mcts0,1800' ; ./bin/quixo $FLAGS_BASE -l 4 -O opt0.2 -X mcts0,1800 >> logs/$LOGFILE
echo '-l 4 -O opt0.2 -X mcts0,1900' ; ./bin/quixo $FLAGS_BASE -l 4 -O opt0.2 -X mcts0,1900 >> logs/$LOGFILE
echo '-l 4 -O opt0.2 -X mcts0,2000' ; ./bin/quixo $FLAGS_BASE -l 4 -O opt0.2 -X mcts0,2000 >> logs/$LOGFILE
echo

echo '-l 4 -O opt0.5 -X mcts0,1100' ; ./bin/quixo $FLAGS_BASE -l 4 -O opt0.5 -X mcts0,1100 >> logs/$LOGFILE
echo '-l 4 -O opt0.5 -X mcts0,1200' ; ./bin/quixo $FLAGS_BASE -l 4 -O opt0.5 -X mcts0,1200 >> logs/$LOGFILE
echo '-l 4 -O opt0.5 -X mcts0,1300' ; ./bin/quixo $FLAGS_BASE -l 4 -O opt0.5 -X mcts0,1300 >> logs/$LOGFILE
echo '-l 4 -O opt0.5 -X mcts0,1400' ; ./bin/quixo $FLAGS_BASE -l 4 -O opt0.5 -X mcts0,1400 >> logs/$LOGFILE
echo '-l 4 -O opt0.5 -X mcts0,1500' ; ./bin/quixo $FLAGS_BASE -l 4 -O opt0.5 -X mcts0,1500 >> logs/$LOGFILE
echo '-l 4 -O opt0.5 -X mcts0,1600' ; ./bin/quixo $FLAGS_BASE -l 4 -O opt0.5 -X mcts0,1600 >> logs/$LOGFILE
echo '-l 4 -O opt0.5 -X mcts0,1700' ; ./bin/quixo $FLAGS_BASE -l 4 -O opt0.5 -X mcts0,1700 >> logs/$LOGFILE
echo '-l 4 -O opt0.5 -X mcts0,1800' ; ./bin/quixo $FLAGS_BASE -l 4 -O opt0.5 -X mcts0,1800 >> logs/$LOGFILE
echo '-l 4 -O opt0.5 -X mcts0,1900' ; ./bin/quixo $FLAGS_BASE -l 4 -O opt0.5 -X mcts0,1900 >> logs/$LOGFILE
echo '-l 4 -O opt0.5 -X mcts0,2000' ; ./bin/quixo $FLAGS_BASE -l 4 -O opt0.5 -X mcts0,2000 >> logs/$LOGFILE
echo
echo

################################################################################
################################################################################
# X opt O mcts
echo '-l 3 -X opt0.0 -O mcts0,1100' ; ./bin/quixo $FLAGS_BASE -l 3 -X opt0.0 -O mcts0,1100 >> logs/$LOGFILE
echo '-l 3 -X opt0.0 -O mcts0,1200' ; ./bin/quixo $FLAGS_BASE -l 3 -X opt0.0 -O mcts0,1200 >> logs/$LOGFILE
echo '-l 3 -X opt0.0 -O mcts0,1300' ; ./bin/quixo $FLAGS_BASE -l 3 -X opt0.0 -O mcts0,1300 >> logs/$LOGFILE
echo '-l 3 -X opt0.0 -O mcts0,1400' ; ./bin/quixo $FLAGS_BASE -l 3 -X opt0.0 -O mcts0,1400 >> logs/$LOGFILE
echo '-l 3 -X opt0.0 -O mcts0,1500' ; ./bin/quixo $FLAGS_BASE -l 3 -X opt0.0 -O mcts0,1500 >> logs/$LOGFILE
echo '-l 3 -X opt0.0 -O mcts0,1600' ; ./bin/quixo $FLAGS_BASE -l 3 -X opt0.0 -O mcts0,1600 >> logs/$LOGFILE
echo '-l 3 -X opt0.0 -O mcts0,1700' ; ./bin/quixo $FLAGS_BASE -l 3 -X opt0.0 -O mcts0,1700 >> logs/$LOGFILE
echo '-l 3 -X opt0.0 -O mcts0,1800' ; ./bin/quixo $FLAGS_BASE -l 3 -X opt0.0 -O mcts0,1800 >> logs/$LOGFILE
echo '-l 3 -X opt0.0 -O mcts0,1900' ; ./bin/quixo $FLAGS_BASE -l 3 -X opt0.0 -O mcts0,1900 >> logs/$LOGFILE
echo '-l 3 -X opt0.0 -O mcts0,2000' ; ./bin/quixo $FLAGS_BASE -l 3 -X opt0.0 -O mcts0,2000 >> logs/$LOGFILE
echo

echo '-l 3 -X opt0.01 -O mcts0,1100' ; ./bin/quixo $FLAGS_BASE -l 3 -X opt0.01 -O mcts0,1100 >> logs/$LOGFILE
echo '-l 3 -X opt0.01 -O mcts0,1200' ; ./bin/quixo $FLAGS_BASE -l 3 -X opt0.01 -O mcts0,1200 >> logs/$LOGFILE
echo '-l 3 -X opt0.01 -O mcts0,1300' ; ./bin/quixo $FLAGS_BASE -l 3 -X opt0.01 -O mcts0,1300 >> logs/$LOGFILE
echo '-l 3 -X opt0.01 -O mcts0,1400' ; ./bin/quixo $FLAGS_BASE -l 3 -X opt0.01 -O mcts0,1400 >> logs/$LOGFILE
echo '-l 3 -X opt0.01 -O mcts0,1500' ; ./bin/quixo $FLAGS_BASE -l 3 -X opt0.01 -O mcts0,1500 >> logs/$LOGFILE
echo '-l 3 -X opt0.01 -O mcts0,1600' ; ./bin/quixo $FLAGS_BASE -l 3 -X opt0.01 -O mcts0,1600 >> logs/$LOGFILE
echo '-l 3 -X opt0.01 -O mcts0,1700' ; ./bin/quixo $FLAGS_BASE -l 3 -X opt0.01 -O mcts0,1700 >> logs/$LOGFILE
echo '-l 3 -X opt0.01 -O mcts0,1800' ; ./bin/quixo $FLAGS_BASE -l 3 -X opt0.01 -O mcts0,1800 >> logs/$LOGFILE
echo '-l 3 -X opt0.01 -O mcts0,1900' ; ./bin/quixo $FLAGS_BASE -l 3 -X opt0.01 -O mcts0,1900 >> logs/$LOGFILE
echo '-l 3 -X opt0.01 -O mcts0,2000' ; ./bin/quixo $FLAGS_BASE -l 3 -X opt0.01 -O mcts0,2000 >> logs/$LOGFILE
echo

echo '-l 3 -X opt0.05 -O mcts0,1100' ; ./bin/quixo $FLAGS_BASE -l 3 -X opt0.05 -O mcts0,1100 >> logs/$LOGFILE
echo '-l 3 -X opt0.05 -O mcts0,1200' ; ./bin/quixo $FLAGS_BASE -l 3 -X opt0.05 -O mcts0,1200 >> logs/$LOGFILE
echo '-l 3 -X opt0.05 -O mcts0,1300' ; ./bin/quixo $FLAGS_BASE -l 3 -X opt0.05 -O mcts0,1300 >> logs/$LOGFILE
echo '-l 3 -X opt0.05 -O mcts0,1400' ; ./bin/quixo $FLAGS_BASE -l 3 -X opt0.05 -O mcts0,1400 >> logs/$LOGFILE
echo '-l 3 -X opt0.05 -O mcts0,1500' ; ./bin/quixo $FLAGS_BASE -l 3 -X opt0.05 -O mcts0,1500 >> logs/$LOGFILE
echo '-l 3 -X opt0.05 -O mcts0,1600' ; ./bin/quixo $FLAGS_BASE -l 3 -X opt0.05 -O mcts0,1600 >> logs/$LOGFILE
echo '-l 3 -X opt0.05 -O mcts0,1700' ; ./bin/quixo $FLAGS_BASE -l 3 -X opt0.05 -O mcts0,1700 >> logs/$LOGFILE
echo '-l 3 -X opt0.05 -O mcts0,1800' ; ./bin/quixo $FLAGS_BASE -l 3 -X opt0.05 -O mcts0,1800 >> logs/$LOGFILE
echo '-l 3 -X opt0.05 -O mcts0,1900' ; ./bin/quixo $FLAGS_BASE -l 3 -X opt0.05 -O mcts0,1900 >> logs/$LOGFILE
echo '-l 3 -X opt0.05 -O mcts0,2000' ; ./bin/quixo $FLAGS_BASE -l 3 -X opt0.05 -O mcts0,2000 >> logs/$LOGFILE
echo

echo '-l 3 -X opt0.1 -O mcts0,1100' ; ./bin/quixo $FLAGS_BASE -l 3 -X opt0.1 -O mcts0,1100 >> logs/$LOGFILE
echo '-l 3 -X opt0.1 -O mcts0,1200' ; ./bin/quixo $FLAGS_BASE -l 3 -X opt0.1 -O mcts0,1200 >> logs/$LOGFILE
echo '-l 3 -X opt0.1 -O mcts0,1300' ; ./bin/quixo $FLAGS_BASE -l 3 -X opt0.1 -O mcts0,1300 >> logs/$LOGFILE
echo '-l 3 -X opt0.1 -O mcts0,1400' ; ./bin/quixo $FLAGS_BASE -l 3 -X opt0.1 -O mcts0,1400 >> logs/$LOGFILE
echo '-l 3 -X opt0.1 -O mcts0,1500' ; ./bin/quixo $FLAGS_BASE -l 3 -X opt0.1 -O mcts0,1500 >> logs/$LOGFILE
echo '-l 3 -X opt0.1 -O mcts0,1600' ; ./bin/quixo $FLAGS_BASE -l 3 -X opt0.1 -O mcts0,1600 >> logs/$LOGFILE
echo '-l 3 -X opt0.1 -O mcts0,1700' ; ./bin/quixo $FLAGS_BASE -l 3 -X opt0.1 -O mcts0,1700 >> logs/$LOGFILE
echo '-l 3 -X opt0.1 -O mcts0,1800' ; ./bin/quixo $FLAGS_BASE -l 3 -X opt0.1 -O mcts0,1800 >> logs/$LOGFILE
echo '-l 3 -X opt0.1 -O mcts0,1900' ; ./bin/quixo $FLAGS_BASE -l 3 -X opt0.1 -O mcts0,1900 >> logs/$LOGFILE
echo '-l 3 -X opt0.1 -O mcts0,2000' ; ./bin/quixo $FLAGS_BASE -l 3 -X opt0.1 -O mcts0,2000 >> logs/$LOGFILE
echo

echo '-l 3 -X opt0.2 -O mcts0,1100' ; ./bin/quixo $FLAGS_BASE -l 3 -X opt0.2 -O mcts0,1100 >> logs/$LOGFILE
echo '-l 3 -X opt0.2 -O mcts0,1200' ; ./bin/quixo $FLAGS_BASE -l 3 -X opt0.2 -O mcts0,1200 >> logs/$LOGFILE
echo '-l 3 -X opt0.2 -O mcts0,1300' ; ./bin/quixo $FLAGS_BASE -l 3 -X opt0.2 -O mcts0,1300 >> logs/$LOGFILE
echo '-l 3 -X opt0.2 -O mcts0,1400' ; ./bin/quixo $FLAGS_BASE -l 3 -X opt0.2 -O mcts0,1400 >> logs/$LOGFILE
echo '-l 3 -X opt0.2 -O mcts0,1500' ; ./bin/quixo $FLAGS_BASE -l 3 -X opt0.2 -O mcts0,1500 >> logs/$LOGFILE
echo '-l 3 -X opt0.2 -O mcts0,1600' ; ./bin/quixo $FLAGS_BASE -l 3 -X opt0.2 -O mcts0,1600 >> logs/$LOGFILE
echo '-l 3 -X opt0.2 -O mcts0,1700' ; ./bin/quixo $FLAGS_BASE -l 3 -X opt0.2 -O mcts0,1700 >> logs/$LOGFILE
echo '-l 3 -X opt0.2 -O mcts0,1800' ; ./bin/quixo $FLAGS_BASE -l 3 -X opt0.2 -O mcts0,1800 >> logs/$LOGFILE
echo '-l 3 -X opt0.2 -O mcts0,1900' ; ./bin/quixo $FLAGS_BASE -l 3 -X opt0.2 -O mcts0,1900 >> logs/$LOGFILE
echo '-l 3 -X opt0.2 -O mcts0,2000' ; ./bin/quixo $FLAGS_BASE -l 3 -X opt0.2 -O mcts0,2000 >> logs/$LOGFILE
echo

echo '-l 3 -X opt0.5 -O mcts0,1100' ; ./bin/quixo $FLAGS_BASE -l 3 -X opt0.5 -O mcts0,1100 >> logs/$LOGFILE
echo '-l 3 -X opt0.5 -O mcts0,1200' ; ./bin/quixo $FLAGS_BASE -l 3 -X opt0.5 -O mcts0,1200 >> logs/$LOGFILE
echo '-l 3 -X opt0.5 -O mcts0,1300' ; ./bin/quixo $FLAGS_BASE -l 3 -X opt0.5 -O mcts0,1300 >> logs/$LOGFILE
echo '-l 3 -X opt0.5 -O mcts0,1400' ; ./bin/quixo $FLAGS_BASE -l 3 -X opt0.5 -O mcts0,1400 >> logs/$LOGFILE
echo '-l 3 -X opt0.5 -O mcts0,1500' ; ./bin/quixo $FLAGS_BASE -l 3 -X opt0.5 -O mcts0,1500 >> logs/$LOGFILE
echo '-l 3 -X opt0.5 -O mcts0,1600' ; ./bin/quixo $FLAGS_BASE -l 3 -X opt0.5 -O mcts0,1600 >> logs/$LOGFILE
echo '-l 3 -X opt0.5 -O mcts0,1700' ; ./bin/quixo $FLAGS_BASE -l 3 -X opt0.5 -O mcts0,1700 >> logs/$LOGFILE
echo '-l 3 -X opt0.5 -O mcts0,1800' ; ./bin/quixo $FLAGS_BASE -l 3 -X opt0.5 -O mcts0,1800 >> logs/$LOGFILE
echo '-l 3 -X opt0.5 -O mcts0,1900' ; ./bin/quixo $FLAGS_BASE -l 3 -X opt0.5 -O mcts0,1900 >> logs/$LOGFILE
echo '-l 3 -X opt0.5 -O mcts0,2000' ; ./bin/quixo $FLAGS_BASE -l 3 -X opt0.5 -O mcts0,2000 >> logs/$LOGFILE
echo
echo

# O opt X mcts
echo '-l 3 -O opt0.0 -X mcts0,1100' ; ./bin/quixo $FLAGS_BASE -l 3 -O opt0.0 -X mcts0,1100 >> logs/$LOGFILE
echo '-l 3 -O opt0.0 -X mcts0,1200' ; ./bin/quixo $FLAGS_BASE -l 3 -O opt0.0 -X mcts0,1200 >> logs/$LOGFILE
echo '-l 3 -O opt0.0 -X mcts0,1300' ; ./bin/quixo $FLAGS_BASE -l 3 -O opt0.0 -X mcts0,1300 >> logs/$LOGFILE
echo '-l 3 -O opt0.0 -X mcts0,1400' ; ./bin/quixo $FLAGS_BASE -l 3 -O opt0.0 -X mcts0,1400 >> logs/$LOGFILE
echo '-l 3 -O opt0.0 -X mcts0,1500' ; ./bin/quixo $FLAGS_BASE -l 3 -O opt0.0 -X mcts0,1500 >> logs/$LOGFILE
echo '-l 3 -O opt0.0 -X mcts0,1600' ; ./bin/quixo $FLAGS_BASE -l 3 -O opt0.0 -X mcts0,1600 >> logs/$LOGFILE
echo '-l 3 -O opt0.0 -X mcts0,1700' ; ./bin/quixo $FLAGS_BASE -l 3 -O opt0.0 -X mcts0,1700 >> logs/$LOGFILE
echo '-l 3 -O opt0.0 -X mcts0,1800' ; ./bin/quixo $FLAGS_BASE -l 3 -O opt0.0 -X mcts0,1800 >> logs/$LOGFILE
echo '-l 3 -O opt0.0 -X mcts0,1900' ; ./bin/quixo $FLAGS_BASE -l 3 -O opt0.0 -X mcts0,1900 >> logs/$LOGFILE
echo '-l 3 -O opt0.0 -X mcts0,2000' ; ./bin/quixo $FLAGS_BASE -l 3 -O opt0.0 -X mcts0,2000 >> logs/$LOGFILE
echo

echo '-l 3 -O opt0.01 -X mcts0,1100' ; ./bin/quixo $FLAGS_BASE -l 3 -O opt0.01 -X mcts0,1100 >> logs/$LOGFILE
echo '-l 3 -O opt0.01 -X mcts0,1200' ; ./bin/quixo $FLAGS_BASE -l 3 -O opt0.01 -X mcts0,1200 >> logs/$LOGFILE
echo '-l 3 -O opt0.01 -X mcts0,1300' ; ./bin/quixo $FLAGS_BASE -l 3 -O opt0.01 -X mcts0,1300 >> logs/$LOGFILE
echo '-l 3 -O opt0.01 -X mcts0,1400' ; ./bin/quixo $FLAGS_BASE -l 3 -O opt0.01 -X mcts0,1400 >> logs/$LOGFILE
echo '-l 3 -O opt0.01 -X mcts0,1500' ; ./bin/quixo $FLAGS_BASE -l 3 -O opt0.01 -X mcts0,1500 >> logs/$LOGFILE
echo '-l 3 -O opt0.01 -X mcts0,1600' ; ./bin/quixo $FLAGS_BASE -l 3 -O opt0.01 -X mcts0,1600 >> logs/$LOGFILE
echo '-l 3 -O opt0.01 -X mcts0,1700' ; ./bin/quixo $FLAGS_BASE -l 3 -O opt0.01 -X mcts0,1700 >> logs/$LOGFILE
echo '-l 3 -O opt0.01 -X mcts0,1800' ; ./bin/quixo $FLAGS_BASE -l 3 -O opt0.01 -X mcts0,1800 >> logs/$LOGFILE
echo '-l 3 -O opt0.01 -X mcts0,1900' ; ./bin/quixo $FLAGS_BASE -l 3 -O opt0.01 -X mcts0,1900 >> logs/$LOGFILE
echo '-l 3 -O opt0.01 -X mcts0,2000' ; ./bin/quixo $FLAGS_BASE -l 3 -O opt0.01 -X mcts0,2000 >> logs/$LOGFILE
echo

echo '-l 3 -O opt0.05 -X mcts0,1100' ; ./bin/quixo $FLAGS_BASE -l 3 -O opt0.05 -X mcts0,1100 >> logs/$LOGFILE
echo '-l 3 -O opt0.05 -X mcts0,1200' ; ./bin/quixo $FLAGS_BASE -l 3 -O opt0.05 -X mcts0,1200 >> logs/$LOGFILE
echo '-l 3 -O opt0.05 -X mcts0,1300' ; ./bin/quixo $FLAGS_BASE -l 3 -O opt0.05 -X mcts0,1300 >> logs/$LOGFILE
echo '-l 3 -O opt0.05 -X mcts0,1400' ; ./bin/quixo $FLAGS_BASE -l 3 -O opt0.05 -X mcts0,1400 >> logs/$LOGFILE
echo '-l 3 -O opt0.05 -X mcts0,1500' ; ./bin/quixo $FLAGS_BASE -l 3 -O opt0.05 -X mcts0,1500 >> logs/$LOGFILE
echo '-l 3 -O opt0.05 -X mcts0,1600' ; ./bin/quixo $FLAGS_BASE -l 3 -O opt0.05 -X mcts0,1600 >> logs/$LOGFILE
echo '-l 3 -O opt0.05 -X mcts0,1700' ; ./bin/quixo $FLAGS_BASE -l 3 -O opt0.05 -X mcts0,1700 >> logs/$LOGFILE
echo '-l 3 -O opt0.05 -X mcts0,1800' ; ./bin/quixo $FLAGS_BASE -l 3 -O opt0.05 -X mcts0,1800 >> logs/$LOGFILE
echo '-l 3 -O opt0.05 -X mcts0,1900' ; ./bin/quixo $FLAGS_BASE -l 3 -O opt0.05 -X mcts0,1900 >> logs/$LOGFILE
echo '-l 3 -O opt0.05 -X mcts0,2000' ; ./bin/quixo $FLAGS_BASE -l 3 -O opt0.05 -X mcts0,2000 >> logs/$LOGFILE
echo

echo '-l 3 -O opt0.1 -X mcts0,1100' ; ./bin/quixo $FLAGS_BASE -l 3 -O opt0.1 -X mcts0,1100 >> logs/$LOGFILE
echo '-l 3 -O opt0.1 -X mcts0,1200' ; ./bin/quixo $FLAGS_BASE -l 3 -O opt0.1 -X mcts0,1200 >> logs/$LOGFILE
echo '-l 3 -O opt0.1 -X mcts0,1300' ; ./bin/quixo $FLAGS_BASE -l 3 -O opt0.1 -X mcts0,1300 >> logs/$LOGFILE
echo '-l 3 -O opt0.1 -X mcts0,1400' ; ./bin/quixo $FLAGS_BASE -l 3 -O opt0.1 -X mcts0,1400 >> logs/$LOGFILE
echo '-l 3 -O opt0.1 -X mcts0,1500' ; ./bin/quixo $FLAGS_BASE -l 3 -O opt0.1 -X mcts0,1500 >> logs/$LOGFILE
echo '-l 3 -O opt0.1 -X mcts0,1600' ; ./bin/quixo $FLAGS_BASE -l 3 -O opt0.1 -X mcts0,1600 >> logs/$LOGFILE
echo '-l 3 -O opt0.1 -X mcts0,1700' ; ./bin/quixo $FLAGS_BASE -l 3 -O opt0.1 -X mcts0,1700 >> logs/$LOGFILE
echo '-l 3 -O opt0.1 -X mcts0,1800' ; ./bin/quixo $FLAGS_BASE -l 3 -O opt0.1 -X mcts0,1800 >> logs/$LOGFILE
echo '-l 3 -O opt0.1 -X mcts0,1900' ; ./bin/quixo $FLAGS_BASE -l 3 -O opt0.1 -X mcts0,1900 >> logs/$LOGFILE
echo '-l 3 -O opt0.1 -X mcts0,2000' ; ./bin/quixo $FLAGS_BASE -l 3 -O opt0.1 -X mcts0,2000 >> logs/$LOGFILE
echo

echo '-l 3 -O opt0.2 -X mcts0,1100' ; ./bin/quixo $FLAGS_BASE -l 3 -O opt0.2 -X mcts0,1100 >> logs/$LOGFILE
echo '-l 3 -O opt0.2 -X mcts0,1200' ; ./bin/quixo $FLAGS_BASE -l 3 -O opt0.2 -X mcts0,1200 >> logs/$LOGFILE
echo '-l 3 -O opt0.2 -X mcts0,1300' ; ./bin/quixo $FLAGS_BASE -l 3 -O opt0.2 -X mcts0,1300 >> logs/$LOGFILE
echo '-l 3 -O opt0.2 -X mcts0,1400' ; ./bin/quixo $FLAGS_BASE -l 3 -O opt0.2 -X mcts0,1400 >> logs/$LOGFILE
echo '-l 3 -O opt0.2 -X mcts0,1500' ; ./bin/quixo $FLAGS_BASE -l 3 -O opt0.2 -X mcts0,1500 >> logs/$LOGFILE
echo '-l 3 -O opt0.2 -X mcts0,1600' ; ./bin/quixo $FLAGS_BASE -l 3 -O opt0.2 -X mcts0,1600 >> logs/$LOGFILE
echo '-l 3 -O opt0.2 -X mcts0,1700' ; ./bin/quixo $FLAGS_BASE -l 3 -O opt0.2 -X mcts0,1700 >> logs/$LOGFILE
echo '-l 3 -O opt0.2 -X mcts0,1800' ; ./bin/quixo $FLAGS_BASE -l 3 -O opt0.2 -X mcts0,1800 >> logs/$LOGFILE
echo '-l 3 -O opt0.2 -X mcts0,1900' ; ./bin/quixo $FLAGS_BASE -l 3 -O opt0.2 -X mcts0,1900 >> logs/$LOGFILE
echo '-l 3 -O opt0.2 -X mcts0,2000' ; ./bin/quixo $FLAGS_BASE -l 3 -O opt0.2 -X mcts0,2000 >> logs/$LOGFILE
echo

echo '-l 3 -O opt0.5 -X mcts0,1100' ; ./bin/quixo $FLAGS_BASE -l 3 -O opt0.5 -X mcts0,1100 >> logs/$LOGFILE
echo '-l 3 -O opt0.5 -X mcts0,1200' ; ./bin/quixo $FLAGS_BASE -l 3 -O opt0.5 -X mcts0,1200 >> logs/$LOGFILE
echo '-l 3 -O opt0.5 -X mcts0,1300' ; ./bin/quixo $FLAGS_BASE -l 3 -O opt0.5 -X mcts0,1300 >> logs/$LOGFILE
echo '-l 3 -O opt0.5 -X mcts0,1400' ; ./bin/quixo $FLAGS_BASE -l 3 -O opt0.5 -X mcts0,1400 >> logs/$LOGFILE
echo '-l 3 -O opt0.5 -X mcts0,1500' ; ./bin/quixo $FLAGS_BASE -l 3 -O opt0.5 -X mcts0,1500 >> logs/$LOGFILE
echo '-l 3 -O opt0.5 -X mcts0,1600' ; ./bin/quixo $FLAGS_BASE -l 3 -O opt0.5 -X mcts0,1600 >> logs/$LOGFILE
echo '-l 3 -O opt0.5 -X mcts0,1700' ; ./bin/quixo $FLAGS_BASE -l 3 -O opt0.5 -X mcts0,1700 >> logs/$LOGFILE
echo '-l 3 -O opt0.5 -X mcts0,1800' ; ./bin/quixo $FLAGS_BASE -l 3 -O opt0.5 -X mcts0,1800 >> logs/$LOGFILE
echo '-l 3 -O opt0.5 -X mcts0,1900' ; ./bin/quixo $FLAGS_BASE -l 3 -O opt0.5 -X mcts0,1900 >> logs/$LOGFILE
echo '-l 3 -O opt0.5 -X mcts0,2000' ; ./bin/quixo $FLAGS_BASE -l 3 -O opt0.5 -X mcts0,2000 >> logs/$LOGFILE
echo
echo

echo '=== DONE ==='
