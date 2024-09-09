#!/bin/bash

# sample : sample shell script for development
# when string uses wildcard, I marked it as 'USING WILDCARD'


# 0. Pre steps 
# set hw informations / check if sample.sh already executed
# Prevent running sample.sh when it already executed.
PROGNAME=$(basename $0)
if [ -f "./.runlock" ]; then
    echo "$PROGNAME: $PROGNAME already executed. Run ./reset.sh to reset." >&2
    exit 1
fi

# make lockfile
> ".runlock"

# Set hw information.
HW_LIST="./hw_info.txt"
STUDENT_LIST="./student_list.txt"
STUDENT_LIST_SUBMITTED="./student_list_submitted.txt"

LOG_FILE="./run_log.txt"

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

# Print information of HW
echo "< $HW_NAME scoring system >"
echo "> # of problem : ${HW_INFO_PROB_NUM}"
for ((prob_num=0; prob_num<$HW_INFO_PROB_NUM; prob_num++)); do
    printf "> Problem ${HW_PROB[prob_num]}: \n"

    case_len=${HW_PROB_CASE[prob_num]}

    for ((case_num=0; case_num<${case_len}; case_num++)); do
        printf "\tcase $((case_num+1))\n"
    done

done

# Print same thing on ./run_log.txt
echo "< $HW_NAME scoring system >" >> $LOG_FILE
echo $(date) >> $LOG_FILE
echo "> # of problem : ${HW_INFO_PROB_NUM}" >> $LOG_FILE
for ((prob_num=0; prob_num<$HW_INFO_PROB_NUM; prob_num++)); do
    printf "> Problem ${HW_PROB[prob_num]}: \n" >> $LOG_FILE

    case_len=${HW_PROB_CASE[prob_num]}

    for ((case_num=0; case_num<${case_len}; case_num++)); do
        printf "\tcase $((case_num+1))\n" >> $LOG_FILE
    done

done


# 1. unzip sample submission

printf "\n1. Unzip submissions\n"
printf "\n1. Unzip submissions\n" >> $LOG_FILE

while read sid; do
    tmp_sid=$(echo "$sid" | grep -oe '^[0-9]*')

    # USING WILDCARD
    unzip_file=./student_submission/${HW_NAME}_*_${tmp_sid}.zip

    printf ">> student id : ${tmp_sid}\n" >> $LOG_FILE

    if [ -f $unzip_file ]; then
        unzip "$unzip_file" -d "./student_submission/${tmp_sid}" > /dev/null
        printf "${tmp_sid}\n" >> "./student_list_submitted.txt" 
        printf ">>> unzip success\n" >> $LOG_FILE
    else
        printf "${tmp_sid}\n" >> "./student_list_not_submitted.txt"
        printf ">>> no zip file submitted\n" >> $LOG_FILE
    fi

done < $STUDENT_LIST


# 2. combine submission and case main

printf "\n2. combine submission and cases\n"
printf "\n2. combine submission and cases\n" >> $LOG_FILE

while read sid; do
    tmp_sid=$(echo "$sid" | grep -oe '^[0-9]*')
    mkdir "./outputs/${tmp_sid}"

    # copy student submission to make case main
    for ((prob_num=0; prob_num<$HW_INFO_PROB_NUM; prob_num++)); do
        prob_name="${HW_PROB[prob_num]}"
        case_len="${HW_PROB_CASE[prob_num]}"

        submission_file_name=$(ls ./student_submission/${tmp_sid}/ | grep -E "_${prob_name}_")
        submission_file="./student_submission/${tmp_sid}/${submission_file_name}"
        
        printf "\n> student id: ${tmp_sid}\n" >> $LOG_FILE

        for ((case_num=1; case_num<$((case_len+1)); case_num++)); do
            output_file="./outputs/${tmp_sid}/${HW_NAME}_${prob_name}_case_${case_num}_${tmp_sid}.cpp"
            grading_case="./grading_cases/${HW_NAME}_${prob_name}_case_${case_num}.cpp"
            
            if [ -f $submission_file ]; then
                echo ">> ${HW_NAME}_${prob_name}: file submitted" >> $LOG_FILE

                cat "${submission_file}" >> "${output_file}"
                printf "\n" >> "${output_file}"
                cat "${grading_case}" >> "${output_file}"
            else
                echo ">> ${HW_NAME}_${prob_name}: file not submitted" >> $LOG_FILE
            fi
        done

    done

done < $STUDENT_LIST_SUBMITTED



# 3. compile cases and make outputs

printf "\n3. compile cases and make outputs, make diff file\n"
printf "\n3. compile cases and make outputs, make diff file\n" >> $LOG_FILE

while read sid; do
    tmp_sid=$(echo "$sid" | grep -oe '^[0-9]*')

    printf "\n> student id: ${tmp_sid}"
    printf "\n> student id: ${tmp_sid}\n" >> $LOG_FILE

    for ((prob_num=0; prob_num<$HW_INFO_PROB_NUM; prob_num++)); do
        prob_name=${HW_PROB[prob_num]}
        case_len=${HW_PROB_CASE[prob_num]}
        
        echo ">> HW ${prob_name} compile starts" >> $LOG_FILE
       
        for ((case_num=1; case_num<$((case_len+1)); case_num++)); do
            
            printf ">>> case ${case_num} > " >> $LOG_FILE

            grading_case="./grading_cases/${HW_NAME}_${prob_name}_case_${case_num}.output"
            output_file="./outputs/${tmp_sid}/${HW_NAME}_${prob_name}_case_${case_num}_${tmp_sid}"
            
            if [ -f "${output_file}.cpp" ]; then
                g++ -o "${output_file}.out" "${output_file}.cpp" > "${output_file}_compile_result.txt" 2>&1
            else
                printf "E: file does not exist\n" >> $LOG_FILE
                continue
            fi
            
            if [ "$(cat ${output_file}_compile_result.txt | wc -l)" -ge 1 ]; then
                printf " E: compile error\n" >> $LOG_FILE
            else
                "${output_file}.out" > "${output_file}_output.txt" 
                printf " compile success\n" >> $LOG_FILE
                diff -u --strip-trailing-cr "${grading_case}" "${output_file}_output.txt" >> "${output_file}_output_diff.txt"
            fi

        done

    done

done < $STUDENT_LIST_SUBMITTED



printf "\n\n4. score using outputs\n"
printf "\n4. score using outputs\n" >> $LOG_FILE


while read sid; do
    tmp_sid=$(echo "$sid" | grep -oe '^[0-9]*')

    printf "> student id: ${tmp_sid}\n" >> $LOG_FILE

    result_json="./outputs/${tmp_sid}/${tmp_sid}_result.json"
    # printf "student id: ${tmp_sid}\n" >> "$result_json"
    
    printf "{\n" >> $result_json

    for ((prob_num=0; prob_num<$HW_INFO_PROB_NUM; prob_num++)); do
        prob_name="${HW_PROB[prob_num]}"
        case_len="${HW_PROB_CASE[prob_num]}"

        output_file="./outputs/${tmp_sid}/${HW_NAME}_${prob_name}_case_1_${tmp_sid}"
        if [ ! -f "${output_file}_compile_result.txt" ]; then
            printf "\"${prob_name}\" : \"file-not-submitted\",\n" >> "$result_json"
        else
            printf "\"${prob_name}\": \"file-submitted\",\n" >> "$result_json"
        

            for ((case_num=1; case_num<$((case_len+1)); case_num++)); do

                output_file="./outputs/${tmp_sid}/${HW_NAME}_${prob_name}_case_${case_num}_${tmp_sid}"
                
                if [ "$(cat ${output_file}_compile_result.txt | wc -l)" -ge 1 ]; then
                    printf "\"${prob_name}-case-${case_num}\" : \"compile-error\",\n" >> "$result_json"
                elif [ "$(cat ${output_file}_output_diff.txt | wc -l)" -ge 1 ]; then
                    printf "\"${prob_name}-case-${case_num}\" : \"fail\",\n" >> "$result_json"
                else
                    printf "\"${prob_name}-case-${case_num}\" : \"pass\",\n" >> "$result_json"
                fi

            done
        fi

    done
    printf "\"dummy\" : \"dummy\"\n" >> "$result_json"
    printf "}\n" >> "$result_json"

done < $STUDENT_LIST_SUBMITTED



# 5. Print Reports using print.sh

printf "\n5. Print reports using _report_print.sh\n"
printf "\n5. Print reports using _report_print.sh\n" >> $LOG_FILE

while read sid; do
    tmp_sid=$(echo "$sid" | grep -oe '^[0-9]*')

    printf "> student id: ${tmp_sid}\n" >> $LOG_FILE
    ./_report_print.sh $tmp_sid
done < $STUDENT_LIST


# 6. Score student's submission based on result file and make one result.csv

printf "\n6. Score student submission based on result file and make one result.csv\n"
printf "\n6. Score student submission based on result file and make one result.csv\n" >> $LOG_FILE

python _result_score.py


printf "\n<<< FINISHED >>>\n"
printf "\n<<< FINISHED >>>\n" >> $LOG_FILE