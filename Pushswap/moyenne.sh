#!/bin/bash

if [ $# -ne 4 ]; then
    echo -e "Usage: $0 MIN MAX ITER LIM_SUP\n  Remind : LIM_SUP(100) = 700 & LIM_SUP(500) = 5500"
    exit 1
fi

MIN=$1
MAX=$2
ITER=$3
LIM=$4

if [ $MIN -gt $MAX ]; then
    echo "Error: MIN must be less than or equal to MAX"
    exit 1
fi

MOY=0
NB_SUP_LIM=0
VAL_MAX=0
VAL_MIN=1000000000

echo -e "Limit = $LIM\n"

echo "Calcul en cours..."

for i in $(seq 1 $ITER); do
	VAL=$( ./push_swap $(for i in $(seq $MIN $MAX | shuf); do
    echo -n $i ''
done)| wc -l)
#	echo "  Val($i) = $VAL"
	MOY=`expr $MOY + $VAL`
	if [ $VAL -gt $LIM ];
	then
		echo -e "  \e[0;31m$VAL\e[0m"
		NB_SUP_LIM=`expr $NB_SUP_LIM + 1`
	fi

	if [ `expr $i % 20` -eq 0 ];
	then
		echo "Etape $i / $ITER"
	fi
	if [ $VAL -gt $VAL_MAX ];
	then
		VAL_MAX=$VAL
	fi
	if [ $VAL -lt $VAL_MIN ];
	then
		VAL_MIN=$VAL
	fi
done

MOY=`expr $MOY / $ITER`

echo -e "\n======================\n\n\e[30;1mCONCLUSION :\e[0m\n"
echo "Complexite moyenne: $MOY"
echo -e "Complexite max : $VAL_MAX"
echo -e "Complexite min : $VAL_MIN\n"
echo "NB d'elem dont la complexite est sup a la lim : $NB_SUP_LIM / $ITER"