#!/bin/bash
export TIME='
> real time       : %E
> max rss         : %M Kbytes
> ave rss         : %t Kbytes
> ave total mem   : %K Kbytes
> num page faults : %F major, %R minor
> filesystem I/O  : %I inputs, %O outputs'

export OPT_COMPUTE_ERROR_CHECKING=0
export OPT_COMPUTE_SPEED_CHECKING=0
export OPT_COMPUTE_MEMORY_CHECKING=0
export MCTS_CACHE_HIT_CHECK=0

clear
make clean
make
clear

echo '--- COMPUTING ---'
/usr/bin/time ./bin/quixo -p opt-compute -l 5 -T 16 -L 1000

echo '=== DONE ==='
