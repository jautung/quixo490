#!/bin/bash
DATE=$(date +'%Y-%m-%d-%H%M%S')

L=5
NUM_USED_UNTIL=24

OPT_COMP_FLAGS='-p opt-compute'$NUM_USED_UNTIL' -l '$L' -T 16 -L 1000'
OPT_COMP_FLAGS_NOSPACE="$(echo -e "${OPT_COMP_FLAGS}" | tr -d '[:space:]')"

LOGFILE="${DATE}${OPT_COMP_FLAGS_NOSPACE}.log"

export TIME='
> real time       : %E
> max rss         : %M Kbytes
> ave rss         : %t Kbytes
> ave total mem   : %K Kbytes
> num page faults : %F major, %R minor
> filesystem I/O  : %I inputs, %O outputs'

export OPT_COMPUTE_ERROR_CHECKING=0
export OPT_COMPUTE_SPEED_CHECKING=1
export OPT_COMPUTE_MEMORY_CHECKING=0

clear
make purge
make clean # necessary when one of the OPT_COMPUTE exports change
make
clear

echo '--- COMPUTING ---'
echo 'flags:' $OPT_COMP_FLAGS
echo 'flags:' $OPT_COMP_FLAGS > logs/$LOGFILE
echo 'ERROR_CHECKING:' $OPT_COMPUTE_ERROR_CHECKING
echo 'ERROR_CHECKING:' $OPT_COMPUTE_ERROR_CHECKING >> logs/$LOGFILE
echo 'SPEED_CHECKING:' $OPT_COMPUTE_SPEED_CHECKING
echo 'SPEED_CHECKING:' $OPT_COMPUTE_SPEED_CHECKING >> logs/$LOGFILE
echo 'MEMORY_CHECKING:' $OPT_COMPUTE_MEMORY_CHECKING
echo 'MEMORY_CHECKING:' $OPT_COMPUTE_MEMORY_CHECKING >> logs/$LOGFILE
echo '' >> logs/$LOGFILE
/usr/bin/time --output=logs/$LOGFILE --append ./bin/quixo $OPT_COMP_FLAGS >> logs/$LOGFILE
echo
echo '' >> logs/$LOGFILE

echo '--- COMPUTING COMPLETE; TAILING LOGFILE (logs/'$LOGFILE') ---'
tail -n 9 logs/$LOGFILE
echo

echo '--- COMPARING TO KNOWN CORRECT OUTPUT ---'
diff -rq data/optimal /home/accts/jc3395/cs490/repoProd/data_frozen/$L
diff -rq data/optimal /home/accts/jc3395/cs490/repoProd/data_frozen/$L >> logs/$LOGFILE
echo

echo '=== DONE ==='
