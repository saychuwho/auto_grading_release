#!/bin/bash

# run : sample shell script for development
# when string uses wildcard, I marked it as 'USING WILDCARD'


# 0. Pre steps 
# set hw informations / check if run.sh already executed


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

# Print information of HW
echo "< $HW_NAME scoring system >"
echo "> # of problem : ${HW_INFO_PROB_NUM}"
for ((prob_num=0; prob_num<$HW_INFO_PROB_NUM; prob_num++)); do
    printf "> Problem ${HW_PROB[prob_num]}: "

    case_len=${HW_PROB_CASE[prob_num]}

    for ((case_num=0; case_num<${case_len}; case_num++)); do
        printf "\tcase $((case_num+1))"
    done
    echo ""
done

# Print same thing on ./run_log.txt
echo "< $HW_NAME scoring system >" >> $LOG_FILE
echo $(date) >> $LOG_FILE
echo "> # of problem : ${HW_INFO_PROB_NUM}" >> $LOG_FILE
for ((prob_num=0; prob_num<$HW_INFO_PROB_NUM; prob_num++)); do
    printf "> Problem ${HW_PROB[prob_num]}: " >> $LOG_FILE

    case_len=${HW_PROB_CASE[prob_num]}

    for ((case_num=0; case_num<${case_len}; case_num++)); do
        printf "\tcase $((case_num+1))" >> $LOG_FILE
    done
    echo "" >> $LOG_FILE
done


# 1. unzip sample submission & change submitted files into correct format files

printf "\n1. Unzip submissions\n"
printf "\n1. Unzip submissions\n" >> $LOG_FILE

PROGRESS_ITER=${PROGRESS_START}
while read sid; do
    tmp_sid=$(echo "$sid" | grep -oe '^[0-9]*')

    ProgressBar ${PROGRESS_ITER} ${PROGRESS_TOTAL_STUDENT} ${tmp_sid}
    PROGRESS_ITER=$((PROGRESS_ITER+1))

    unzip_file=./student_submission/${HW_NAME}_*_${tmp_sid}.zip # USING WILDCARD
    unzip_file_name_alter="$(ls ./student_submission | grep ${tmp_sid})"
    unzip_file_alter="./student_submission/${unzip_file_name_alter}"

    printf ">> student id : ${tmp_sid}\n" >> $LOG_FILE

    # Unzip part

    # student submitted zip file with right file name format.
    if [ -f ""$unzip_file"" ]; then
        unzip -O UTF-8 -o "$unzip_file" -d "./student_submission/${tmp_sid}" > /dev/null
        printf "${tmp_sid}\n" >> "./student_list_submitted.txt" 
        printf ">>> unzip success\n" >> $LOG_FILE
    # if student submitted wrong file name format, but student submitted some sort of zip file.
    elif [ -f "$unzip_file_alter" ]; then

        cp "$unzip_file_alter" "./student_submission/${HW_NAME}_tmpzip_${tmp_sid}.zip"
        unzip -O UTF-8 -o "$unzip_file_alter" -d "./student_submission/${tmp_sid}" > /dev/null

        printf "${tmp_sid}\n" >> "./student_list_submitted.txt" 
        printf ">>> unzip success\n" >> $LOG_FILE
    # student did not submitted zip file.
    else
        printf "${tmp_sid}\n" >> "./student_list_not_submitted.txt"
        printf ">>> no zip file submitted\n" >> $LOG_FILE
    fi
done < $STUDENT_LIST
echo ""



# declare submitted student number
PROGRESS_SUBMIT_STUDENT=$(cat $STUDENT_LIST_SUBMITTED | wc -l)

# 2. Change submitted files into correct format
printf "\n2. Change submitted files into correct format & Copy files into submission_by_problem\n"
printf "\n2. Change submitted files into correct format & Copy files into submission_by_problem\n" >> $LOG_FILE

PROGRESS_ITER=${PROGRESS_START}
while read sid; do
    tmp_sid=$(echo "$sid" | grep -oe '^[0-9]*')

    ProgressBar ${PROGRESS_ITER} ${PROGRESS_SUBMIT_STUDENT} ${tmp_sid}
    PROGRESS_ITER=$((PROGRESS_ITER+1))

    # alter submission_file when student submitted files inside folder
    is_alter=$(ls --classify ./student_submission/${tmp_sid} | grep '/$' | grep -v "MACOSX" | wc -w)
    if [ $is_alter -gt 0 ]; then
        echo "${tmp_sid}: folder detected. copy files out from folder." >> $LOG_FILE
        alter_submission_folder_name="$(ls --classify ./student_submission/${tmp_sid} | grep '/$' | grep -v "MACOSX")"
        
        if [ ! "$(ls --classify "./student_submission/${tmp_sid}/${alter_submission_folder_name}/" | grep '/$' | wc -l )" -ge 1 ]; then
            cp "./student_submission/${tmp_sid}/${alter_submission_folder_name}"* "./student_submission/${tmp_sid}/"
        fi
    fi

    
    for ((prob_num=0; prob_num<$HW_INFO_PROB_NUM; prob_num++)); do
        prob_name="${HW_PROB[prob_num]}"
        
        submission_file_name="$(ls ./student_submission/${tmp_sid}/ | grep -E "^${HW_NAME}_${prob_name}_" | grep -E ".cpp$" )"
        submission_file="./student_submission/${tmp_sid}/${submission_file_name}"
        
        # change filename into correct file name format
        if [ ! -f "$submission_file" ]; then
            python3 _convert_file_name.py ${tmp_sid} ${prob_num} >> $LOG_FILE
        fi

        # copy files into submission_by_problem
        if [ ! -d "./submission_by_problem/${prob_name}" ]; then
            mkdir "./submission_by_problem/${prob_name}"
        fi
        copy_submission_file_name="$(ls ./student_submission/${tmp_sid}/ | grep -E "^${HW_NAME}_${prob_name}_" | grep -E ".cpp$")"
        copy_submission_file="./student_submission/${tmp_sid}/${copy_submission_file_name}"
    
        if [ -f "${copy_submission_file}" ]; then
            cp "${copy_submission_file}" "./submission_by_problem/${prob_name}/${HW_NAME}_${prob_name}_${tmp_sid}.cpp"
        fi
    done

done < $STUDENT_LIST_SUBMITTED
echo ""



# # hw1 extra - check recurssion
# printf "\n2-extra. hw1 extra - check recurssion\n"
# printf "\n2-extra. hw1 extra - check recurssion\n" >> $LOG_FILE

# ./_prob_2_recurssion_detector.sh



# 3. combine submission and case main

printf "\n3. combine submission and cases\n"
printf "\n3. combine submission and cases\n" >> $LOG_FILE

PROGRESS_ITER=${PROGRESS_START}
while read sid; do
    tmp_sid=$(echo "$sid" | grep -oe '^[0-9]*')
    mkdir "./outputs/${tmp_sid}"

    ProgressBar ${PROGRESS_ITER} ${PROGRESS_SUBMIT_STUDENT} ${tmp_sid}
    PROGRESS_ITER=$((PROGRESS_ITER+1))

    # copy student submission to make case main
    for ((prob_num=0; prob_num<$HW_INFO_PROB_NUM; prob_num++)); do
        prob_name="${HW_PROB[prob_num]}"
        case_len="${HW_PROB_CASE[prob_num]}"

        submission_file_name="$(ls ./student_submission/${tmp_sid}/ | grep -E "^${HW_NAME}_${prob_name}_" | grep -E ".cpp$")"
        submission_file="./student_submission/${tmp_sid}/${submission_file_name}"
        
        # alter submission_file when student submitted files inside folder
        is_alter=$(ls ./student_submission/${tmp_sid} -l | grep '^d' | grep -oP 'hw1_[\p{L}]*_[0-9]*' | wc -w)
        if [ $is_alter -gt 0 ]; then
            alter_submission_folder_name=$(ls ./student_submission/${tmp_sid} -l | grep '^d' | grep -oP 'hw1_[\p{L}]*_[0-9]*')
            alter_submission_file_name=$(ls ./student_submission/${tmp_sid}/${alter_submission_folder_name}/ | grep -E "_${prob_name}_")
            alter_submission_file="./student_submission/${tmp_sid}/${alter_submission_folder_name}/${alter_submission_file_name}"
        fi
        
        printf "\n> student id: ${tmp_sid}\n" >> $LOG_FILE

        for ((case_num=1; case_num<$((case_len+1)); case_num++)); do
            output_file="./outputs/${tmp_sid}/${HW_NAME}_${prob_name}_case_${case_num}_${tmp_sid}.cpp"
            grading_case="./grading_cases/${HW_NAME}_${prob_name}_case_${case_num}.cpp"
            


            if [ -f "$submission_file" ]; then
                # add header file if there are no header
                is_header=$(cat "$submission_file" | grep '#include' | wc -l)
                if [ $is_header -lt 1 ]; then
                    prob_header="./grading_cases/${HW_NAME}_${prob_name}_header.cpp"
                    cat "$prob_header" >> "$output_file"
                    printf "\n" >> "${output_file}"
                fi

                # if there are main function in submission_file, delete it
                sed -i '/int\s\+main\s*(.*)/,/^}/d' "$submission_file"
                echo ">> ${HW_NAME}_${prob_name}: file submitted" >> $LOG_FILE

                cat "${submission_file}" >> "${output_file}"
                printf "\n" >> "${output_file}"
                cat "${grading_case}" >> "${output_file}"

                # remove zero witdh no-break space
                cp "${output_file}" "${output_file}.tmp"
                sed 's/\xEF\xBB\xBF//g' "${output_file}.tmp" > "${output_file}"
                rm "${output_file}.tmp"
            
            else
                echo ">> ${HW_NAME}_${prob_name}: file not submitted" >> $LOG_FILE
            fi
        done

    done
done < $STUDENT_LIST_SUBMITTED
echo ""


# 3. compile cases and make outputs

printf "\n4. compile cases and make outputs, make diff file\n"
printf "\n4. compile cases and make outputs, make diff file\n" >> $LOG_FILE

PROGRESS_ITER=${PROGRESS_START}
while read sid; do
    tmp_sid=$(echo "$sid" | grep -oe '^[0-9]*')

    printf "\n> student id: ${tmp_sid} / Progress (${PROGRESS_ITER}/${PROGRESS_SUBMIT_STUDENT})\n"
    printf "\n> student id: ${tmp_sid}\n" >> $LOG_FILE

    PROGRESS_ITER_INNER=$PROGRESS_START
    for ((prob_num=0; prob_num<$HW_INFO_PROB_NUM; prob_num++)); do
        prob_name=${HW_PROB[prob_num]}
        case_len=${HW_PROB_CASE[prob_num]}
        
        echo ">> HW ${prob_name} compile starts" >> $LOG_FILE
       
        for ((case_num=1; case_num<$((case_len+1)); case_num++)); do
            
            tmp_print="${prob_name}-case-${case_num}"
            ProgressBar ${PROGRESS_ITER_INNER} ${HW_CASE_TOTAL_NUM} ${tmp_print}
            PROGRESS_ITER_INNER=$((PROGRESS_ITER_INNER+1))

            printf ">>> case ${case_num} > " >> $LOG_FILE


            grading_case_1="./grading_cases/${HW_NAME}_${prob_name}_case_${case_num}_output_1.txt"
            output_file="./outputs/${tmp_sid}/${HW_NAME}_${prob_name}_case_${case_num}_${tmp_sid}"
            

            if [ -f "${output_file}.cpp" ]; then
                g++ -w -o "${output_file}.out" "${output_file}.cpp" > "${output_file}_compile_result.txt" 2>&1
            else
                printf "E: file does not exist\n" >> $LOG_FILE
                continue
            fi
            
            if [ "$(cat "${output_file}_compile_result.txt" | wc -l)" -ge 1 ]; then
                printf " E: compile error\n" >> $LOG_FILE
            else
                timeout 10s "${output_file}.out" > "${output_file}_output.txt"
                printf " compile success\n" >> $LOG_FILE
                diff -uZB --strip-trailing-cr "${grading_case_1}" "${output_file}_output.txt" >> "${output_file}_output_1_diff.txt"
            fi

            
        done

    done

    PROGRESS_ITER=$((PROGRESS_ITER+1))

done < $STUDENT_LIST_SUBMITTED



# 4. score submission with compile result & output diff

printf "\n\n5. score using outputs\n"
printf "\n5. score using outputs\n" >> $LOG_FILE


PROGRESS_ITER=${PROGRESS_START}
while read sid; do
    tmp_sid=$(echo "$sid" | grep -oe '^[0-9]*')

    ProgressBar ${PROGRESS_ITER} ${PROGRESS_SUBMIT_STUDENT} ${tmp_sid}
    PROGRESS_ITER=$((PROGRESS_ITER+1))

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
                
                if [ "$(cat "${output_file}_compile_result.txt" | wc -l)" -ge 1 ]; then
                    printf "\"${prob_name}-case-${case_num}\" : \"compile-error\",\n" >> "$result_json"
                elif [ "$(cat "${output_file}_output_1_diff.txt" | wc -l)" -ge 1 ]; then
                    printf "\"${prob_name}-case-${case_num}\" : \"fail\",\n" >> "$result_json"
                else
                    # hw1 extra - check recurssion
                    if [ "$prob_name" = "2_A" -o "$prob_name" = "2_B" -o "$prob_name" = "2_C" ]; then
                        if [ ! $(grep "${STUDENT_ID}" ./result_${prob_name}_no_recurssion.txt | wc -w) -eq 0 ]; then
                            printf "\"${prob_name}-case-${case_num}\" : \"no-recurssion\",\n" >> "$result_json"    
                        else
                            printf "\"${prob_name}-case-${case_num}\" : \"pass\",\n" >> "$result_json"
                        fi
                    else
                        printf "\"${prob_name}-case-${case_num}\" : \"pass\",\n" >> "$result_json"
                    fi
                fi
            done
        fi

    done
    printf "\"dummy\" : \"dummy\"\n" >> "$result_json"
    printf "}\n" >> "$result_json"



done < $STUDENT_LIST_SUBMITTED
echo ""


# 5. Print Reports using print.sh

printf "\n6. Print reports using _report_print.sh\n"
printf "\n6. Print reports using _report_print.sh\n" >> $LOG_FILE

PROGRESS_ITER=${PROGRESS_START}
while read sid; do
    tmp_sid=$(echo "$sid" | grep -oe '^[0-9]*')

    ProgressBar ${PROGRESS_ITER} ${PROGRESS_TOTAL_STUDENT} ${tmp_sid}
    PROGRESS_ITER=$((PROGRESS_ITER+1))

    printf "> student id: ${tmp_sid}\n" >> $LOG_FILE
    ./_report_print.sh $tmp_sid

    
done < $STUDENT_LIST
echo ""

# 6. Score student's submission based on result file and make one result.csv

printf "\n7. Score student submission based on result file and make one result.csv\n"
printf "\n7. Score student submission based on result file and make one result.csv\n" >> $LOG_FILE

python3 _result_score.py



# 8. Check Plagiarism with MOSS
printf "\n8. Check Plagiarism with MOSS & make report\n"
printf "\n8. Check Plagiarism with MOSS & make report\n" >> $LOG_FILE


./_result_moss.sh

printf "\n<<< FINISHED >>>\n"
printf "\n<<< FINISHED >>>\n" >> $LOG_FILE