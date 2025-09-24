import argparse
import os
import json

from auto_grader import AutoGrader
from functions import progress_bar


def exit_with_message(message: str, grader: AutoGrader):
    print(message)
    grader.log_write(message)
    exit(1)

def main():

    # print("DEBUG: Starting main.py")

    lock_file = "./.runlock"

    # print(f"DEBUG: Lock file exists: {os.path.isfile(lock_file)}")
    # print(f"DEBUG: Lock file path: {os.path.abspath(lock_file)}")

    # argument define
    parser = argparse.ArgumentParser()
    parser.add_argument('--option', nargs='*', help='option for run', type=str, default=['all'])

    args = parser.parse_args()

    option = args.option[0]

    # print(f"DEBUG: Selected option: {option}")
    # print("DEBUG: About to import AutoGrader")

    grader = AutoGrader()

    # print("DEBUG: AutoGrader created successfully")


    grader.print_grader_start()

    # Grade all student't submission - default option
    if option == 'all':
        print("<<< Grade All >>>")
        grader.log_write("<<< Grade ALL >>>")
        
        had_run = os.path.isfile(lock_file)
        if had_run: exit_with_message("E: already run Grade All option", grader)
        
        # print(f"DEBUG: Creating lock file at: {os.path.abspath(lock_file)}")
        with open(lock_file, 'w') as f: 
            f.write('lock')
        # print(f"DEBUG: Lock file created. Exists: {os.path.isfile(lock_file)}")
        
        try:
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
            
            # # Remove lock file on successful completion
            # print(f"DEBUG: Removing lock file. Exists before removal: {os.path.exists(lock_file)}")
            # if os.path.exists(lock_file):
            #     os.remove(lock_file)
            #     print(f"DEBUG: Lock file removed. Exists after removal: {os.path.exists(lock_file)}")
                
        except Exception as e:
            grader.log_write(f"Error during execution: {str(e)}")
            print(f"Error during execution: {str(e)}")
            # Keep lock file to prevent retry without manual intervention
            raise

    # Regrade student's submission failed before
    elif option == 'regrade':
        print("<<< Regrade Mode >>>\n")
        grader.log_write("<<< Regrade Mode >>>\n")
        
        had_run = os.path.isfile(lock_file)
        if not had_run: exit_with_message("E: Did not run Grade All option", grader)

        grader.regrade_students()

        print("<<< FINISHED >>>")
        grader.log_write("<<< FINISHED >>>\n")


    # Reset the state.
    elif option == "reset":
        print("<<< Reset Mode >>>\n")
        
        had_run = os.path.isfile(lock_file)
        if not had_run: exit_with_message("E: Did not run Grade All option", grader)

        grader.reset()

        print("<<< FINISHED >>>")
        grader.log_write("<<< FINISHED >>>\n")


    elif option == "run_output":
        print("<<< Run Output Mode >>>\n")

        had_run = os.path.isfile(lock_file)
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
        
        had_run = os.path.isfile(lock_file)
        if not had_run: exit_with_message("E: Did not run Grade All option", grader)

        grader.result_moss()

        print("<<< FINISHED >>>")
        grader.log_write("<<< FINISHED >>>\n")


    elif option == "log_delete":
        print("<<< Log Delete Mode >>>")

        for file_name in os.listdir('./log/'):
            file_path = os.path.join('./log/', file_name)
            if os.path.isfile(file_path) and file_path.endswith('.txt'):
                os.remove(file_path)

        print("<<< FINISHED >>>")


    else:
        print("E: Usage: python3 main.py [--option (all/regrade/reset/run_output/MOSS)]")
        grader.log_write("E: Usage: python3 main.py [--option (all/regrade/reset/run_output/MOSS)]\n")
        exit(1)


if __name__ == "__main__":
    main()