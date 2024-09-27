#!/bin/bash

# _result_moss : make MOSS report

# code from https://github.com/fearside/ProgressBar
# 1. Create ProgressBar function
# 1.1 Input is currentState($1) and totalState($2)
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
printf "\rProgress : [${_fill// /#}${_empty// /-}] ${_progress}%%"

}

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



# real report part
MOSS_REPORT="./result_moss.md"

if [ -f $MOSS_REPORT ]; then
    rm $MOSS_REPORT
fi

echo "# MOSS result of all problems in ${HW_NAME}" >> $MOSS_REPORT
echo "" >> $MOSS_REPORT
echo "click the link below to see each problem's MOSS report" >> $MOSS_REPORT
echo "" >> $MOSS_REPORT

PROGRESS_ITER=1
for ((prob_num=0; prob_num<$HW_INFO_PROB_NUM; prob_num++)); do
    prob_name="${HW_PROB[prob_num]}"

    moss_report_link=$(./_moss.pl -l cc ./submission_by_problem/${prob_name}/*.cpp | grep "http")
    echo "${prob_name} : ${moss_report_link}" >> $MOSS_REPORT

    ProgressBar ${PROGRESS_ITER} ${HW_INFO_PROB_NUM}
    PROGRESS_ITER=$((PROGRESS_ITER+1))
done