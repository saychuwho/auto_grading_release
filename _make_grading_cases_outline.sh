#!/bin/bash

# make grading cases outline : make grading cases outline .cpp files

# Set hw information.
HW_LIST="./hw_info.txt"

declare -a HW_INFO
while read value; do
    HW_INFO+=($value)
done < $HW_LIST

HW_NAME=${HW_INFO[0]}
HW_INFO_PROB_NUM=${HW_INFO[1]}
HW_INFO_PROB_START=2
HW_INFO_CASE_START=$((HW_INFO_PROB_START+HW_INFO_PROB_NUM))
HW_INFO_LEN=${#HW_INFO[@]}

# Set HW_PROB, HW_PROB_CASE
declare -a HW_PROB
declare -a HW_PROB_CASE

for ((prob_num=$HW_INFO_PROB_START; prob_num<$HW_INFO_CASE_START; prob_num++)); do
    HW_PROB+=( ${HW_INFO[prob_num]} )
    case_index=$((prob_num+HW_INFO_PROB_NUM))
    HW_PROB_CASE+=( ${HW_INFO[case_index]} )
done

for ((prob_num=0; prob_num<$HW_INFO_PROB_NUM; prob_num++)); do
    prob_name="${HW_PROB[prob_num]}"
    case_len="${HW_PROB_CASE[prob_num]}"

    for ((case_num=1; case_num<$((case_len+1)); case_num++)); do
        > "./grading_cases/${HW_NAME}_${prob_name}_case_${case_num}.cpp"
        > "./grading_cases/${HW_NAME}_${prob_name}_case_${case_num}_answer.cpp"
    done
done