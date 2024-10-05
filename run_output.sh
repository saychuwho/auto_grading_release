#!/bin/bash

# run output : run student's .out by problem

PROGNAME=$(basename $0)
if [ ! -f "./.runlock" ]; then
    echo "$PROGNAME: run ./run.sh first" >&2
    exit 1
fi

STUDENT_ID=$1
PROB_NAME=$2
CASE_NUM=$3

# check if student is in list or not.
if [ $(echo "$STUDENT_ID" | grep -E '^[0-9]{9}' | wc -w) -eq 0 ]; then # student_id length is not valid
    echo "${PROGNAME}: E: Invalid student_id length." >&2
    exit 1
elif [ $(grep "${STUDENT_ID}" ./student_list.txt | wc -w) -eq 0 ]; then # student_id is not in list
    echo "${PROGNAME}: E: Invalid student_id. Does not exist in student list." >&2
    exit 1
fi


# Set hw information.
HW_LIST="./hw_info.txt"
STUDENT_LIST="./student_list.txt"
STUDENT_LIST_SUBMITTED="./student_list_submitted.txt"

LOG_FILE="./run_log.txt"

# declare progress bar variable
PROGRESS_START=1
PROGRESS_TOTAL_STUDENT=$(cat $STUDENT_LIST | wc -l)

declare -a HW_INFO
while read value; do
    HW_INFO+=($value)
done < $HW_LIST

HW_NAME=${HW_INFO[0]}
HW_INFO_PROB_NUM=${HW_INFO[1]}
HW_INFO_PROB_START=2
HW_INFO_CASE_START=$((HW_INFO_PROB_START+HW_INFO_PROB_NUM))
HW_INFO_LEN=${#HW_INFO[@]}

HW_CASE_TOTAL_NUM=0

# Set HW_PROB, HW_PROB_CASE
declare -a HW_PROB
declare -a HW_PROB_CASE

for ((prob_num=$HW_INFO_PROB_START; prob_num<$HW_INFO_CASE_START; prob_num++)); do
    HW_PROB+=( ${HW_INFO[prob_num]} )
    case_index=$((prob_num+HW_INFO_PROB_NUM))
    HW_PROB_CASE+=( ${HW_INFO[case_index]} )
    HW_CASE_TOTAL_NUM=$((HW_CASE_TOTAL_NUM+HW_INFO[case_index]))
done

# execute problem cases
prob_name="${PROB_NAME}"
case_num="${CASE_NUM}"

echo "${prob_name} ${case_num}"
    
output_file="./outputs/${STUDENT_ID}/${HW_NAME}_${prob_name}_case_${case_num}_${STUDENT_ID}.out"
printf "${STUDENT_ID} - ${HW_NAME} ${prob_name} case ${case_num}\n"

if [ ! -f "$output_file" ]; then
    printf "> compile result does not exists.\n"
else
    timeout 10s "${output_file}"
fi

echo ""
