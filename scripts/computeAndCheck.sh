#!/bin/bash
DATE=$(date +'%Y-%m-%d-%H%M%S')
L=4
OPT_COMP_FLAGS='-l '$L' -T 8 -s'
OPT_COMP_FLAGS_NOSPACE="$(echo -e "${OPT_COMP_FLAGS}" | tr -d '[:space:]')"
LOGFILE="${DATE}${OPT_COMP_FLAGS_NOSPACE}.log"

export TIME='
> real time       : %E
> max rss         : %M Kbytes
> ave rss         : %t Kbytes
> ave total mem   : %K Kbytes
> num page faults : %F major, %R minor
> filesystem I/O  : %I inputs, %O outputs'

clear

make purge
# make clean
make
echo

echo '--- COMPUTING ---'
echo 'flags:' $OPT_COMP_FLAGS
echo 'flags:' $OPT_COMP_FLAGS > logs/$LOGFILE
echo '' >> logs/$LOGFILE
/usr/bin/time --output=logs/$LOGFILE --append ./bin/quixo -p opt-compute $OPT_COMP_FLAGS >> logs/$LOGFILE
echo

echo '--- COMPUTING COMPLETE; TAILING LOGFILE (logs/'$LOGFILE') ---'
tail -n 9 logs/$LOGFILE
echo

echo '--- COMPARING TO KNOWN CORRECT OUTPUT ---'
diff -rq data/optimal /home/accts/jc3395/cs490/repoProd/data_frozen/$L
echo

echo '=== DONE ==='
