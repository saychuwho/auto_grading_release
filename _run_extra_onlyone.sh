#!/bin/bash

# _run_extra_onlyone : run extra student's grading after run.sh executed


# 0. Pre steps 
# Prevent running sample.sh when it already executed.
PROGNAME=$(basename $0)
if [ ! -f "./.runlock" ]; then
    echo "$PROGNAME: ./run.sh did not executed. Execute $PROGNAME after run ./run.sh" >&2
    exit 1
fi

STUDENT_ID=${1}

# Get HW info
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


# check if student is in list or not.
if [ $(echo "$STUDENT_ID" | grep -E '^[0-9]{9}' | wc -w) -eq 0 ]; then # student_id length is not valid
    echo "${PROGNAME}: E: Invalid student_id length." >&2
    exit 1
elif [ $(grep "${STUDENT_ID}" ./student_list.txt | wc -w) -eq 0 ]; then # student_id is not in list
    echo "${PROGNAME}: E: Invalid student_id. Does not exist in student list." >&2
    exit 1
fi



if [ -d "./outputs/${STUDENT_ID}/" ]; then
    rm -rf "./outputs/${STUDENT_ID}/"
    mkdir "./outputs/${STUDENT_ID}"
fi


# 1. combine submission and case main

# printf "\n1. combine submission and cases\n"
# copy student submission to make case main
for ((prob_num=0; prob_num<$HW_INFO_PROB_NUM; prob_num++)); do
    prob_name="${HW_PROB[prob_num]}"
    case_len="${HW_PROB_CASE[prob_num]}"

    submission_file_name="$(ls ./student_submission/${STUDENT_ID}/ | grep -E "^${HW_NAME}_${prob_name}_" | grep -E ".cpp$")"
    submission_file="./student_submission/${STUDENT_ID}/${submission_file_name}"
    submission_by_problem_file="./submission_by_problem/${prob_name}/${HW_NAME}_${prob_name}_${tmp_sid}.cpp"

    # copy source code into submission_by_problem_file if file did not exists.
    if [ ! -f "$submission_by_problem_file" ]; then
        cp "$submission_file" "$submission_by_problem_file"
    fi

    for ((case_num=1; case_num<$((case_len+1)); case_num++)); do
        output_file="./outputs/${STUDENT_ID}/${HW_NAME}_${prob_name}_case_${case_num}_${STUDENT_ID}.cpp"
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

            cat "${submission_file}" >> "${output_file}"
            printf "\n" >> "${output_file}"
            cat "${grading_case}" >> "${output_file}"

            # remove zero witdh no-break space
            cp "${output_file}" "${output_file}.tmp"
            sed 's/\xEF\xBB\xBF//g' "${output_file}.tmp" > "${output_file}"
            rm "${output_file}.tmp"

        fi
    done

done



# 2. compile cases and make outputs
# printf "\n2. compile cases and make outputs, make diff file\n"


for ((prob_num=0; prob_num<$HW_INFO_PROB_NUM; prob_num++)); do
    prob_name=${HW_PROB[prob_num]}
    case_len=${HW_PROB_CASE[prob_num]}
    
    
    for ((case_num=1; case_num<$((case_len+1)); case_num++)); do

        grading_case_1="./grading_cases/${HW_NAME}_${prob_name}_case_${case_num}_output_1.txt"
        output_file="./outputs/${STUDENT_ID}/${HW_NAME}_${prob_name}_case_${case_num}_${STUDENT_ID}"
        

        if [ -f "${output_file}.cpp" ]; then
            g++ -w -o "${output_file}.out" "${output_file}.cpp" > "${output_file}_compile_result.txt" 2>&1
        else
            continue
        fi
        
        if [ "$(cat "${output_file}_compile_result.txt" | wc -l)" -ge 1 ]; then
            printf " E: compile error\n" > /dev/null
        else
            timeout 10s "${output_file}.out" > "${output_file}_output.txt"
            diff -uZB --strip-trailing-cr "${grading_case_1}" "${output_file}_output.txt" >> "${output_file}_output_1_diff.txt"
        fi

    done

done



# 3. score submission with compile result & output diff

# printf "\n\n3. score using outputs\n"

result_json="./outputs/${STUDENT_ID}/${STUDENT_ID}_result.json"


printf "{\n" >> $result_json

for ((prob_num=0; prob_num<$HW_INFO_PROB_NUM; prob_num++)); do
    prob_name="${HW_PROB[prob_num]}"
    case_len="${HW_PROB_CASE[prob_num]}"

    output_file="./outputs/${STUDENT_ID}/${HW_NAME}_${prob_name}_case_1_${STUDENT_ID}"
    if [ ! -f "${output_file}_compile_result.txt" ]; then
        printf "\"${prob_name}\" : \"file-not-submitted\",\n" >> "$result_json"
    else
        printf "\"${prob_name}\": \"file-submitted\",\n" >> "$result_json"
    

        for ((case_num=1; case_num<$((case_len+1)); case_num++)); do

            output_file="./outputs/${STUDENT_ID}/${HW_NAME}_${prob_name}_case_${case_num}_${STUDENT_ID}"
            
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


# 4. Print Reports using print.sh
# printf "\n4. Print reports using _report_print.sh\n"

if [ $(grep "${STUDENT_ID}" ./student_list_submitted.txt | wc -w) -eq 0 ]; then
    rm -rf ./reports/not_submitted/${STUDENT_ID}
else
    rm -rf ./reports/submitted/${STUDENT_ID}
fi
./_report_print.sh $STUDENT_ID


# printf "\n<<< FINISHED >>>\n"