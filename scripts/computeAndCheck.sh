#!/bin/bash
DATE=$(date +'%Y-%m-%d-%H%M%S')
L=3
OPT_COMP_FLAGS='-l '$L' -T 2'

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
echo 'flags:' $OPT_COMP_FLAGS > logs/$DATE.log
echo '' >> logs/$DATE.log
/usr/bin/time --output=logs/$DATE.log --append ./bin/quixo -p opt-compute $OPT_COMP_FLAGS >> logs/$DATE.log
echo

echo '--- COMPUTING COMPLETE; TAILING LOGFILE (logs/'$DATE'.log) ---'
tail -n 9 logs/$DATE.log
echo

echo '--- COMPARING TO KNOWN CORRECT OUTPUT ---'
diff -rq data/optimal /home/accts/jc3395/cs490/repoProd/data_frozen/$L
echo

echo '=== DONE ==='
