#!/bin/bash
DATE=$(date +'%Y-%m-%d-%H%M%S')
LOGFILE="${DATE}-game-length-tests.log"

N_GAMES_EACH=100
TURN_LIMIT=100
FLAGS_BASE='-p test -N '$N_GAMES_EACH' -n '$TURN_LIMIT' -v game-length-tests-mcts'

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
echo '-l 3 -X opt -O random' ; ./bin/quixo $FLAGS_BASE -l 3 -X opt -O random >> logs/$LOGFILE
echo '-l 3 -X opt -O heuris-simple' ; ./bin/quixo $FLAGS_BASE -l 3 -X opt -O heuris-simple >> logs/$LOGFILE
echo '-l 3 -X opt -O mcts0,100' ; ./bin/quixo $FLAGS_BASE -l 3 -X opt -O mcts0,100 >> logs/$LOGFILE
echo '-l 3 -X opt -O mcts0,200' ; ./bin/quixo $FLAGS_BASE -l 3 -X opt -O mcts0,200 >> logs/$LOGFILE
echo '-l 3 -X opt -O mcts0,500' ; ./bin/quixo $FLAGS_BASE -l 3 -X opt -O mcts0,500 >> logs/$LOGFILE
echo '-l 3 -X opt -O mcts0,1000' ; ./bin/quixo $FLAGS_BASE -l 3 -X opt -O mcts0,1000 >> logs/$LOGFILE
echo '-l 3 -X opt -O opt' ; ./bin/quixo $FLAGS_BASE -l 3 -X opt -O opt >> logs/$LOGFILE
echo
echo

# -l 4
echo '-l 4 -X opt -O random' ; ./bin/quixo $FLAGS_BASE -l 4 -X opt -O random >> logs/$LOGFILE
echo '-l 4 -X opt -O heuris-simple' ; ./bin/quixo $FLAGS_BASE -l 4 -X opt -O heuris-simple >> logs/$LOGFILE
echo '-l 4 -X opt -O mcts0,100' ; ./bin/quixo $FLAGS_BASE -l 4 -X opt -O mcts0,100 >> logs/$LOGFILE
echo '-l 4 -X opt -O mcts0,200' ; ./bin/quixo $FLAGS_BASE -l 4 -X opt -O mcts0,200 >> logs/$LOGFILE
echo '-l 4 -X opt -O mcts0,500' ; ./bin/quixo $FLAGS_BASE -l 4 -X opt -O mcts0,500 >> logs/$LOGFILE
echo '-l 4 -X opt -O mcts0,1000' ; ./bin/quixo $FLAGS_BASE -l 4 -X opt -O mcts0,1000 >> logs/$LOGFILE
echo '-l 4 -X opt -O opt' ; ./bin/quixo $FLAGS_BASE -l 4 -X opt -O opt >> logs/$LOGFILE
echo
echo

# -l 5
echo '-l 5 -X opt -O random' ; ./bin/quixo $FLAGS_BASE -l 5 -X opt -O random >> logs/$LOGFILE
echo '-l 5 -X opt -O heuris-simple' ; ./bin/quixo $FLAGS_BASE -l 5 -X opt -O heuris-simple >> logs/$LOGFILE
echo '-l 5 -X opt -O mcts0,100' ; ./bin/quixo $FLAGS_BASE -l 5 -X opt -O mcts0,100 >> logs/$LOGFILE
echo '-l 5 -X opt -O mcts0,200' ; ./bin/quixo $FLAGS_BASE -l 5 -X opt -O mcts0,200 >> logs/$LOGFILE
echo '-l 5 -X opt -O mcts0,500' ; ./bin/quixo $FLAGS_BASE -l 5 -X opt -O mcts0,500 >> logs/$LOGFILE
echo '-l 5 -X opt -O mcts0,1000' ; ./bin/quixo $FLAGS_BASE -l 5 -X opt -O mcts0,1000 >> logs/$LOGFILE
echo '-l 5 -X opt -O opt' ; ./bin/quixo $FLAGS_BASE -l 5 -X opt -O opt >> logs/$LOGFILE
echo
echo

# -l 5 cache persist
echo '-l 5 -X opt -O mcts-cache-persist0,100' ; ./bin/quixo $FLAGS_BASE -l 5 -X opt -O mcts-cache-persist0,100 >> logs/$LOGFILE
echo '-l 5 -X opt -O mcts-cache-persist0,200' ; ./bin/quixo $FLAGS_BASE -l 5 -X opt -O mcts-cache-persist0,200 >> logs/$LOGFILE
echo '-l 5 -X opt -O mcts-cache-persist0,500' ; ./bin/quixo $FLAGS_BASE -l 5 -X opt -O mcts-cache-persist0,500 >> logs/$LOGFILE
echo '-l 5 -X opt -O mcts-cache-persist0,1000' ; ./bin/quixo $FLAGS_BASE -l 5 -X opt -O mcts-cache-persist0,1000 >> logs/$LOGFILE
echo
echo

echo '=== DONE ==='
