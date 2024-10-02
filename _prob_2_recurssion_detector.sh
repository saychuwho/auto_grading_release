#!/bin/bash

# temp 2 B recurssion detector : recurssion detector for 2-B



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


# constant declare

STUDENT_LIST_SUBMITTED="./student_list_submitted.txt"
PROGRESS_START=1
PROGRESS_TOTAL_STUDENT=$(cat $STUDENT_LIST_SUBMITTED | wc -l)

NO_RECURSSION_LIST_A="./result_2_A_no_recurssion.txt"
NO_RECURSSION_LIST_B="./result_2_B_no_recurssion.txt"
NO_RECURSSION_LIST_C="./result_2_C_no_recurssion.txt"

if [ -f $NO_RECURSSION_LIST_A ]; then
    rm $NO_RECURSSION_LIST_A
    rm $NO_RECURSSION_LIST_B
    rm $NO_RECURSSION_LIST_C
fi

# main part
PROGRESS_ITER=${PROGRESS_START}
while read sid; do
    tmp_sid=$(echo "$sid" | grep -oe '^[0-9]*')
    
    ProgressBar ${PROGRESS_ITER} ${PROGRESS_TOTAL_STUDENT} ${tmp_sid}
    PROGRESS_ITER=$((PROGRESS_ITER+1))

    # 2-A
    submission_file_name_A="$(ls ./student_submission/${tmp_sid}/ | grep -E "^hw1_2_A_" | grep -E ".cpp$")"
    submission_file_A="./student_submission/${tmp_sid}/${submission_file_name_A}"

    if [ -f "${submission_file_A}" ]; then
        recurssion_counter=$(cat "$submission_file_A" | grep -E "funcA" | wc -l)
        if [ $recurssion_counter -le 1 ]; then
            echo "${tmp_sid}" >> $NO_RECURSSION_LIST_A
        fi    
    fi

    # 2-B
    submission_file_name_B="$(ls ./student_submission/${tmp_sid}/ | grep -E "^hw1_2_B_" | grep -E ".cpp$")"
    submission_file_B="./student_submission/${tmp_sid}/${submission_file_name_B}"

    if [ -f "${submission_file_B}" ]; then
        recurssion_counter=$(cat "$submission_file_B" | grep -E "funcB" | wc -l)
        if [ $recurssion_counter -le 1 ]; then
            echo "${tmp_sid}" >> $NO_RECURSSION_LIST_B
        fi    
    fi

    # 2-C
    submission_file_name_C="$(ls ./student_submission/${tmp_sid}/ | grep -E "^hw1_2_C_" | grep -E ".cpp$")"
    submission_file_C="./student_submission/${tmp_sid}/${submission_file_name_C}"

    if [ -f "${submission_file_C}" ]; then
        recurssion_counter=$(cat "$submission_file_C" | grep -E "funcC" | wc -l)
        if [ $recurssion_counter -le 1 ]; then
            echo "${tmp_sid}" >> $NO_RECURSSION_LIST_C
        fi    
    fi

done < $STUDENT_LIST_SUBMITTED

echo ""