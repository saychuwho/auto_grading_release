import argparse
import os
import json

from auto_grader import AutoGrader
from functions import progress_bar


def exit_with_message(message: str, grader: AutoGrader):
    print(message)
    grader.log_write(message)
    exit(1)


lock_file = "./.runlock"
had_run = os.path.isfile(lock_file)

# argument define
parser = argparse.ArgumentParser()
parser.add_argument('--option', help='option for run', type=str, default='all')

args = parser.parse_args()

option = args.option

grader = AutoGrader()

grader.print_grader_start()

# Grade all student't submission - default option
if option == 'all':
    print("<<< Grade All >>>")
    grader.log_write("<<< Grade ALL >>>")
    if had_run: exit_with_message("E: already run Grade All option", grader)
    
    with open(lock_file, 'w') as f: f.write('lock')
    grader.unzip_all()
    grader.file_format_correction_all()
    grader.combine_submission_all()
    grader.compile_output_all()
    grader.score_all()
    grader.print_report_all()
    grader.result_score()
    # grader.result_moss()

    print("<<< FINISHED >>>")
    grader.log_write("<<< FINISHED >>>\n")

# Regrade student's submission failed before
elif option == 'regrade':
    print("<<< Regrade Mode >>>\n")
    grader.log_write("<<< Regrade Mode >>>\n")
    if not had_run: exit_with_message("E: Did not run Grade All option", grader)

    # regrade zip-not-submitted
    with open('./result_zip_not_submitted_list.csv', 'r') as f: regrade_zip = f.readlines()    
    # regrade else
    with open('./student_list_regrade.txt', 'r') as f: regrade_list = list(map(int, f.readlines()))
    
    counter = 1
    total_count = len(regrade_zip) + len(regrade_list) - 1

    for s_id_str in regrade_zip:
        if s_id_str[0] == 's': continue
        s_id = int(s_id_str.strip().strip(','))
        
        progress_bar(counter+1, total_count, prefix="Progress: Regrade", suffix=f"{s_id}   ", length=50)

        grader.regrade_student(s_id, True)
        counter += 1

    for s_id in regrade_list:
        progress_bar(counter+1, total_count, prefix="Progress: Regrade ", suffix=f"{s_id}   ", length=50)
        grader.regrade_student(s_id)
        counter += 1

    grader.dump_student_list()

    grader.result_score()

    print()

    # grader.result_moss()

    print("<<< FINISHED >>>")
    grader.log_write("<<< FINISHED >>>\n")


# Reset the state.
elif option == "reset":
    print("<<< Reset Mode >>>\n")
    if not had_run: exit_with_message("E: Did not run Grade All option", grader)

    grader.reset()

    print("<<< FINISHED >>>")
    grader.log_write("<<< FINISHED >>>\n")


elif option == "run_output":
    print("<<< Run Output Mode >>>\n")

    if not had_run: exit_with_message("E: Did not run Grade All option", grader)
    
    run_output_option_list = input("put s_id prob_name case_num: ").split()

    if len(run_output_option_list) != 3:
        exit_with_message("E: invalid run_output_option", grader)
    
    s_id = int(run_output_option_list[0])
    prob_name = run_output_option_list[1]
    case_num = int(run_output_option_list[2])

    if not s_id in grader.student_list:
        exit_with_message("E: invalid student id", grader)
    if not prob_name in grader.prob_names:
        exit_with_message("E: invalid prob name", grader)
    if case_num < 1 or case_num > grader.case_num[grader.prob_names.index(prob_name)]:
        exit_with_message("E: invalid case_num", grader)

    print(f">> output of {s_id}'s {grader.hw_name} prob {prob_name} case {case_num}\n")

    grader.run_output(s_id, prob_name, case_num)

    print("<<< FINISHED >>>")
    grader.log_write("<<< FINISHED >>>\n")
    
elif option == "add_func_pattern":
    print("<<< Add Function Pattern in hw_pattern.json >>>")
    with open('./hw_info.json', 'r') as f: hw_info_dict = json.load(f)
    with open('./hw_pattern.json', 'r') as f: hw_pattern_dict = json.load(f)

    get_input = input("put prob_name class func_return_type func_name\nif function is not in class, put 'none' in class\n: ")

    prob_name, class_name, func_return_type, func_name = get_input.split()

    if not prob_name in grader.prob_names:
        exit_with_message("E: invalid prob name", grader)
    
    func_num = int(hw_info_dict["function_num"])
    hw_info_dict["function_num"] = str(func_num + 1)
    hw_pattern_dict[prob_name][f"{func_num}-1"] = f"{func_return_type} {class_name}::{func_name}"
    hw_pattern_dict[prob_name][f"{func_num}-2"] = f"{class_name} {func_return_type} {func_name}"

    with open('./hw_info.json', 'w') as f: json.dump(hw_info_dict, f, indent=4)
    with open('./hw_pattern.json', 'w') as f: json.dump(hw_pattern_dict, f, indent=4)

    print("<<< FINISHED >>>")
    grader.log_write("<<< FINISHED >>>\n")



else:
    print("E: Usage: python3 main.py [--option all/regrade/reset]")
    grader.log_write("E: Usage: python3 main.py [--option all/regrade/reset]\n")
    exit(1)
