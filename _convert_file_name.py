# convert file name : convert wrong file format into correct file format

import sys
import os
import json
import shutil


student_id = sys.argv[1]
prob_num_input = sys.argv[2] # prob_num from shell should be here. not prob_name in shell.


# get hw information
hw_info_file = "./hw_info.txt"
f = open(hw_info_file, 'r')
hw_info = f.readlines()
hw_info = [x.strip() for x in hw_info]
f.close()

hw_name = hw_info[0]
hw_info_prob_num = int(hw_info[1])
hw_info_prob_start = 2
hw_info_case_start = hw_info_prob_start + hw_info_prob_num
hw_info_len = len(hw_info)

hw_prob = []
hw_prob_case = []

for prob_num in range(2, hw_info_case_start):
    hw_prob.append(hw_info[prob_num])
    case_index = prob_num + hw_info_prob_num
    hw_prob_case.append(int(hw_info[case_index]))

json_pattern_file = "./hw_pattern.json"
with open(json_pattern_file, 'r') as f:
    pattern_data=json.load(f)


# make list of files.
submission_folder = f"./student_submission/{student_id}/"
cpp_list = [file for file in os.listdir(submission_folder) if ".cpp" in file]


# find cpp file that has certain function name
prob_name = hw_prob[int(prob_num_input)]
func_name = pattern_data[prob_name]
correct_file_name = f"./student_submission/{student_id}/{hw_name}_{prob_name}_tmpfile_{student_id}.cpp"

is_exist = False
wrong_file_name = ""
for cpp_file in cpp_list:
    wrong_file_name = f"./student_submission/{student_id}/{cpp_file}"
    with open(wrong_file_name, 'r') as wrong_file:
        wrong_file_content = wrong_file.read()

        if func_name in wrong_file_content:
            is_exist = True
            break
        else:
            continue


if is_exist:
    shutil.copy(wrong_file_name, correct_file_name)
    print(f"{student_id} :  wrong file format detected at prob:{prob_name}. change filename into correct file format.")
else:
    print(f"{student_id} :  no file containing function pattern in prob:{prob_name}.")