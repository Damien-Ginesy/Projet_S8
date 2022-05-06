#! /bin/bash

#$1 : hostname
#$2 : port

j=0;
for (( i=0; i<$(($3 / 3)); i++ ))
    do
        bin/client $1 $2 0 2  > $j.report &
        ((j++));
        bin/client $1 $2 1 2  > $j.report &
        ((j++));
        bin/client $1 $2 2 2  > $j.report &
        ((j++));
    done
if [[ $(($3 % 3)) == "1" ]]; then
    bin/client $1 $2 0 2  $j.report &
    ((j++));
fi
if [[ $(($3 % 3)) == "2" ]]; then
    bin/client $1 $2 0 2  > $j.report &
    ((j++));
    bin/client $1 $2 0 2  > $j.report ;
    ((j++));
fi
