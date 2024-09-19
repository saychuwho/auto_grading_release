#!/bin/bash

# sample reset : reset all process from sample.sh

# Prevent running sample_reset.sh when there are nothing to reset.
PROGNAME=$(basename ${0})
if [ ! -f "./.runlock" ]; then
    echo "$PROGNAME: E: Nothing to reset." >&2
    exit 1
fi

STUDENT_LIST="./student_list.txt"

while read sid; do
    tmp_sid=$(echo "$sid" | grep -oe '^[0-9]*')
    rm -rf "./student_submission/${tmp_sid}"
    rm -rf "./outputs/${tmp_sid}"
done < $STUDENT_LIST

rm ./reports/not_submitted/*.md
rm ./reports/submitted/*.md


# remove student_list_submitted.txt
rm *_submitted.txt

# remove result.csv
rm "./result.csv"
rm "./result_score.csv"

# remove log
rm "./run_log.txt"

# remove .samplelock
rm "./.runlock"
