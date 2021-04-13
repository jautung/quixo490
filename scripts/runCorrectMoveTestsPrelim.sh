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
echo '-l 3 -X random' ; ./bin/quixo $FLAGS_BASE -l 3 -X random >> logs/$LOGFILE ; while [ $? -ne 0 ] ; do ./bin/quixo $FLAGS_BASE -l 3 -X random >> logs/$LOGFILE ; done
echo '-l 3 -X heuris-simple' ; ./bin/quixo $FLAGS_BASE -l 3 -X heuris-simple >> logs/$LOGFILE ; while [ $? -ne 0 ] ; do ./bin/quixo $FLAGS_BASE -l 3 -X heuris-simple >> logs/$LOGFILE ; done
echo '-l 3 -X mcts0,100' ; ./bin/quixo $FLAGS_BASE -l 3 -X mcts0,100 >> logs/$LOGFILE ; while [ $? -ne 0 ] ; do ./bin/quixo $FLAGS_BASE -l 3 -X mcts0,100 >> logs/$LOGFILE ; done
echo '-l 3 -X mcts0,200' ; ./bin/quixo $FLAGS_BASE -l 3 -X mcts0,200 >> logs/$LOGFILE ; while [ $? -ne 0 ] ; do ./bin/quixo $FLAGS_BASE -l 3 -X mcts0,200 >> logs/$LOGFILE ; done
echo '-l 3 -X mcts0,500' ; ./bin/quixo $FLAGS_BASE -l 3 -X mcts0,500 >> logs/$LOGFILE ; while [ $? -ne 0 ] ; do ./bin/quixo $FLAGS_BASE -l 3 -X mcts0,500 >> logs/$LOGFILE ; done
echo '-l 3 -X mcts0,1000' ; ./bin/quixo $FLAGS_BASE -l 3 -X mcts0,1000 >> logs/$LOGFILE ; while [ $? -ne 0 ] ; do ./bin/quixo $FLAGS_BASE -l 3 -X mcts0,1000 >> logs/$LOGFILE ; done
echo '-l 3 -X opt' ; ./bin/quixo $FLAGS_BASE -l 3 -X opt >> logs/$LOGFILE ; while [ $? -ne 0 ] ; do ./bin/quixo $FLAGS_BASE -l 3 -X opt >> logs/$LOGFILE ; done
echo
echo

# -l 4
echo '-l 4 -X random' ; ./bin/quixo $FLAGS_BASE -l 4 -X random >> logs/$LOGFILE ; while [ $? -ne 0 ] ; do ./bin/quixo $FLAGS_BASE -l 4 -X random >> logs/$LOGFILE ; done
echo '-l 4 -X heuris-simple' ; ./bin/quixo $FLAGS_BASE -l 4 -X heuris-simple >> logs/$LOGFILE ; while [ $? -ne 0 ] ; do ./bin/quixo $FLAGS_BASE -l 4 -X heuris-simple >> logs/$LOGFILE ; done
echo '-l 4 -X mcts0,100' ; ./bin/quixo $FLAGS_BASE -l 4 -X mcts0,100 >> logs/$LOGFILE ; while [ $? -ne 0 ] ; do ./bin/quixo $FLAGS_BASE -l 4 -X mcts0,100 >> logs/$LOGFILE ; done
echo '-l 4 -X mcts0,200' ; ./bin/quixo $FLAGS_BASE -l 4 -X mcts0,200 >> logs/$LOGFILE ; while [ $? -ne 0 ] ; do ./bin/quixo $FLAGS_BASE -l 4 -X mcts0,200 >> logs/$LOGFILE ; done
echo '-l 4 -X mcts0,500' ; ./bin/quixo $FLAGS_BASE -l 4 -X mcts0,500 >> logs/$LOGFILE ; while [ $? -ne 0 ] ; do ./bin/quixo $FLAGS_BASE -l 4 -X mcts0,500 >> logs/$LOGFILE ; done
echo '-l 4 -X mcts0,1000' ; ./bin/quixo $FLAGS_BASE -l 4 -X mcts0,1000 >> logs/$LOGFILE ; while [ $? -ne 0 ] ; do ./bin/quixo $FLAGS_BASE -l 4 -X mcts0,1000 >> logs/$LOGFILE ; done
echo '-l 4 -X opt' ; ./bin/quixo $FLAGS_BASE -l 4 -X opt >> logs/$LOGFILE ; while [ $? -ne 0 ] ; do ./bin/quixo $FLAGS_BASE -l 4 -X opt >> logs/$LOGFILE ; done
echo
echo

# -l 5
echo '-l 5 -X random' ; ./bin/quixo $FLAGS_BASE -l 5 -X random >> logs/$LOGFILE ; while [ $? -ne 0 ] ; do ./bin/quixo $FLAGS_BASE -l 5 -X random >> logs/$LOGFILE ; done
echo '-l 5 -X heuris-simple' ; ./bin/quixo $FLAGS_BASE -l 5 -X heuris-simple >> logs/$LOGFILE ; while [ $? -ne 0 ] ; do ./bin/quixo $FLAGS_BASE -l 5 -X heuris-simple >> logs/$LOGFILE ; done
echo '-l 5 -X mcts0,100' ; ./bin/quixo $FLAGS_BASE -l 5 -X mcts0,100 >> logs/$LOGFILE ; while [ $? -ne 0 ] ; do ./bin/quixo $FLAGS_BASE -l 5 -X mcts0,100 >> logs/$LOGFILE ; done
echo '-l 5 -X mcts0,200' ; ./bin/quixo $FLAGS_BASE -l 5 -X mcts0,200 >> logs/$LOGFILE ; while [ $? -ne 0 ] ; do ./bin/quixo $FLAGS_BASE -l 5 -X mcts0,200 >> logs/$LOGFILE ; done
echo '-l 5 -X mcts0,500' ; ./bin/quixo $FLAGS_BASE -l 5 -X mcts0,500 >> logs/$LOGFILE ; while [ $? -ne 0 ] ; do ./bin/quixo $FLAGS_BASE -l 5 -X mcts0,500 >> logs/$LOGFILE ; done
echo '-l 5 -X mcts0,1000' ; ./bin/quixo $FLAGS_BASE -l 5 -X mcts0,1000 >> logs/$LOGFILE ; while [ $? -ne 0 ] ; do ./bin/quixo $FLAGS_BASE -l 5 -X mcts0,1000 >> logs/$LOGFILE ; done
echo '-l 5 -X opt' ; ./bin/quixo $FLAGS_BASE -l 5 -X opt >> logs/$LOGFILE ; while [ $? -ne 0 ] ; do ./bin/quixo $FLAGS_BASE -l 5 -X opt >> logs/$LOGFILE ; done
echo
echo

# -l 5 cache persist
echo '-l 5 -X mcts-cache-persist0,100' ; ./bin/quixo $FLAGS_BASE -l 5 -X mcts-cache-persist0,100 >> logs/$LOGFILE ; while [ $? -ne 0 ] ; do ./bin/quixo $FLAGS_BASE -l 5 -X mcts-cache-persist0,100 >> logs/$LOGFILE ; done
echo '-l 5 -X mcts-cache-persist0,200' ; ./bin/quixo $FLAGS_BASE -l 5 -X mcts-cache-persist0,200 >> logs/$LOGFILE ; while [ $? -ne 0 ] ; do ./bin/quixo $FLAGS_BASE -l 5 -X mcts-cache-persist0,200 >> logs/$LOGFILE ; done
echo '-l 5 -X mcts-cache-persist0,500' ; ./bin/quixo $FLAGS_BASE -l 5 -X mcts-cache-persist0,500 >> logs/$LOGFILE ; while [ $? -ne 0 ] ; do ./bin/quixo $FLAGS_BASE -l 5 -X mcts-cache-persist0,500 >> logs/$LOGFILE ; done
echo '-l 5 -X mcts-cache-persist0,1000' ; ./bin/quixo $FLAGS_BASE -l 5 -X mcts-cache-persist0,1000 >> logs/$LOGFILE ; while [ $? -ne 0 ] ; do ./bin/quixo $FLAGS_BASE -l 5 -X mcts-cache-persist0,1000 >> logs/$LOGFILE ; done
echo
echo

echo '=== DONE ==='
