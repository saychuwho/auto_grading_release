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
parser.add_argument('--option', nargs='*', help='option for run', type=str, default=['all'])

args = parser.parse_args()

option = args.option[0]

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
        progress_bar(counter+1, total_count, prefix="Progress: Regrade", suffix=f"{s_id}   ", length=50)
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
    
    if len(args.option) != 4:
        exit_with_message("E: invalid run_output_option", grader)
    
    s_id = int(args.option[1])
    prob_name = args.option[2]
    case_num = int(args.option[3])

    if not s_id in grader.student_list:
        exit_with_message("E: invalid student id", grader)
    if not prob_name in grader.prob_names:
        exit_with_message("E: invalid prob name", grader)
    if case_num < 1 or case_num > grader.case_num[grader.prob_names.index(prob_name)]:
        exit_with_message("E: invalid case_num", grader)

    print(f">> output of {s_id}'s {grader.hw_name} prob {prob_name} case {case_num}\n")

    result = grader.run_output(s_id, prob_name, case_num)

    if not result: exit_with_message(f"E: No output for student {s_id} {prob_name}-case-{case_num}", grader)

    print("<<< FINISHED >>>")
    grader.log_write("<<< FINISHED >>>\n")
    

elif option == "MOSS":
    print("<<< run MOSS analysis Mode >>>")
    
    if not had_run: exit_with_message("E: Did not run Grade All option", grader)

    grader.result_moss()

    print("<<< FINISHED >>>")
    grader.log_write("<<< FINISHED >>>\n")


else:
    print("E: Usage: python3 main.py [--option all/regrade/reset]")
    grader.log_write("E: Usage: python3 main.py [--option all/regrade/reset]\n")
    exit(1)
