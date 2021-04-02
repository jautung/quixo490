#!/bin/bash
DATE=$(date +'%Y-%m-%d-%H%M%S')

L=3
NUM_USED_UNTIL=0

OPT_COMP_FLAGS='-p opt-compute'$NUM_USED_UNTIL' -l '$L' -T 16 -L 1000'
OPT_COMP_FLAGS_NOSPACE="$(echo -e "${OPT_COMP_FLAGS}" | tr -d '[:space:]')"
LOGFILE="${DATE}${OPT_COMP_FLAGS_NOSPACE}-callgrind.log"

export OPT_COMPUTE_ERROR_CHECKING=0
export OPT_COMPUTE_SPEED_CHECKING=0
export OPT_COMPUTE_MEMORY_CHECKING=0

clear
make purge
make clean # necessary when one of the OPT_COMPUTE exports change
make
clear

echo '--- SETTINGS ---'
echo 'flags:' $OPT_COMP_FLAGS
echo 'ERROR_CHECKING:' $OPT_COMPUTE_ERROR_CHECKING
echo 'SPEED_CHECKING:' $OPT_COMPUTE_SPEED_CHECKING
echo 'MEMORY_CHECKING:' $OPT_COMPUTE_MEMORY_CHECKING
echo

echo '--- COMPUTING ---'
valgrind --tool=callgrind --simulate-cache=yes ./bin/quixo $OPT_COMP_FLAGS
callgrind_annotate --auto=yes callgrind.out.* > logs/$LOGFILE
rm callgrind.out.* vgcore.*
echo

echo '--- COMPUTING COMPLETE; HEAD LOGFILE (logs/'$LOGFILE') ---'
head -n 16 logs/$LOGFILE

echo '=== DONE ==='
