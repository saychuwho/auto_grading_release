#!/bin/bash

# run_extra : run _run_extra_onlyone.sh multiple times based on student_list_regrade.txt

# code from https://github.com/fearside/ProgressBar
# 1. Create ProgressBar function
# 1.1 Input is currentState($1) and totalState($2) 
# 1.2 modify ProgressBar extra_print($3)
function ProgressBar {
# Process data
    let _progress=(${1}*100/${2}*100)/100
    let _done=(${_progress}*4)/10
    let _left=40-$_done
# Build progressbar string lengths
    _fill=$(printf "%${_done}s")
    _empty=$(printf "%${_left}s")

# 1.2 Build progressbar strings and print the ProgressBar line
# 1.2.1 Output example:                           
# 1.2.1.1 Progress : [########################################] 100%
printf "\rProgress : [${_fill// /#}${_empty// /-}] ${_progress}%% ${3}     "

}

# 0. Pre steps 
# Prevent running sample.sh when it already executed.
PROGNAME=$(basename $0)
if [ ! -f "./.runlock" ]; then
    echo "$PROGNAME: ./run.sh did not executed. Execute $PROGNAME after run ./run.sh" >&2
    exit 1
fi

# declare constants
REGRADE_LIST="./student_list_regrade.txt"
PROGRESS_START=1
PROGRESS_TOTAL_STUDENT=$(cat $REGRADE_LIST | wc -l)



# hw1 extra - check recurssion
printf "\n0-extra. hw1 extra - check recurssion\n"

./_prob_2_recurssion_detector.sh



# run multiple _run_extra_onlyone.sh
printf "\n1. run multiple _run_extra_onlyone.sh\n"

PROGRESS_ITER=${PROGRESS_START}
while read sid; do
    tmp_sid=$(echo "$sid" | grep -oe '^[0-9]*')
    if [ -z $tmp_sid ]; then
        continue
    fi

    ProgressBar ${PROGRESS_ITER} ${PROGRESS_TOTAL_STUDENT} ${tmp_sid}
    PROGRESS_ITER=$((PROGRESS_ITER+1))

    ./_run_extra_onlyone.sh $tmp_sid

done < $REGRADE_LIST
ProgressBar ${PROGRESS_ITER} ${PROGRESS_TOTAL_STUDENT} ${tmp_sid}

echo ""

# re-execute _result_score.py
python3 _result_score.py

printf "\n2. run _result_moss.sh\n"
# re-execute _result_moss.sh
./_result_moss.sh

echo ""