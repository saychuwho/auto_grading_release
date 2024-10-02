#!/bin/bash

# sample reset : reset all process from sample.sh

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

# Prevent running sample_reset.sh when there are nothing to reset.
PROGNAME=$(basename ${0})
if [ ! -f "./.runlock" ]; then
    echo "$PROGNAME: E: Nothing to reset." >&2
    exit 1
fi

STUDENT_LIST="./student_list.txt"

echo "< $HW_NAME scoring system - reset >"

# declare progress bar variable
PROGRESS_START=1
PROGRESS_TOTAL_STUDENT=$(cat $STUDENT_LIST | wc -l)

PROGRESS_ITER=${PROGRESS_START}
while read sid; do
    tmp_sid=$(echo "$sid" | grep -oe '^[0-9]*')
    rm -rf "./student_submission/${tmp_sid}"
    rm -rf "./outputs/${tmp_sid}"

    ProgressBar ${PROGRESS_ITER} ${PROGRESS_TOTAL_STUDENT}
    PROGRESS_ITER=$((PROGRESS_ITER+1))
done < $STUDENT_LIST
echo ""

# remove student_list_submitted.txt
rm *_submitted.txt

# remove tmpzip files
rm ./student_submission/*_tmpzip_*.zip

# remove submission_by_problem content
rm -rf ./submission_by_problem/*/

# remove report files
rm -rf ./reports/not_submitted/*
rm -rf ./reports/submitted/*

# remove result files
rm "./result.csv"
rm "./result_score.csv"
rm ./result_*.md
rm ./result_*.txt

# remove student_list_regrade.txt and make new one
rm "./student_list_regrade.txt"
printf "# student_list_regrade : put student id want to regrade using run_extra.sh" > "./student_list_regrade.txt"

# remove log
rm "./run_log.txt"

# remove .samplelock
rm "./.runlock"
