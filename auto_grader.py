import json
import subprocess
import os
import shutil
import glob
from datetime import datetime

from functions import progress_bar
from auto_grade_student import AutoGradeStudent


class AutoGrader():
    def __init__(self):
        hw_info_file = "./hw_info.json"
        hw_pattern_file = "./hw_pattern.json"
        hw_info_score_file = "./hw_info_score.json"
        student_list_file = "./student_list.json"

        with open(hw_info_file, "r") as f: hw_info_dict = json.load(f)
        with open(hw_pattern_file, "r") as f: hw_pattern_dict = json.load(f)
        with open(hw_info_score_file, "r") as f: hw_info_score_dict = json.load(f)
        with open(student_list_file, "r") as f: student_list_dict = json.load(f)

        self.hw_name = hw_info_dict["hw_name"]
        self.prob_names = hw_info_dict["prob_name"].split()
        self.case_num = list(map(int, hw_info_dict["case_num"].split()))
        self.function_num = list(map(int, hw_info_dict["function_num"].split()))
        self.class_num = list(map(int, hw_info_dict["class_num"].split()))

        self.hw_info_dict = {"hw_name": self.hw_name,
                             "prob_name": self.prob_names,
                             "case_num": self.case_num,
                             "func_num": self.function_num,
                             "class_num": self.class_num}
        self.hw_pattern_dict = hw_pattern_dict
        self.hw_info_score_dict = hw_info_score_dict
        self.student_list_dict = student_list_dict
        self.student_list = list(map(int, list(student_list_dict.keys())))
        self.auto_grade_student_dict = {}

        now = datetime.now()
        formatted_time = now.strftime("%Y%m%d-%H%M")
        self.log_file = f"./log/run_log_{formatted_time}.txt"

        for s_id in self.student_list:
            self.auto_grade_student_dict[s_id] = AutoGradeStudent(s_id, self.log_file, 
                                                                  self.hw_info_dict, self.hw_pattern_dict,
                                                                  zip_submitted=True if self.student_list_dict[str(s_id)] == "zip-submitted" else False) 


        self.progress_total = len(self.auto_grade_student_dict.keys())

        # write information in self.log_file
        with open(self.log_file, "w") as f:
            log_init_str = f"< {self.hw_name} scoring system>\n"
            log_init_str += f"> # of problem : {len(self.prob_names)}\n"
            for i, prob_name in enumerate(self.prob_names):
                log_init_str += f"> Problem {prob_name}:\n"
                for j in range(self.case_num[i]):
                    log_init_str += f"\tcase {j+1}"
                log_init_str += "\n"
            log_init_str += "\n"
            f.write(log_init_str)


    def print_grader_start(self):
        print(f"< {self.hw_name} scoring system>")
        print(f"> # of problem : {len(self.prob_names)}")
        for i, prob_name in enumerate(self.prob_names):
            print(f"> Problem {prob_name}:", end="")
            for j in range(self.case_num[i]):
                print(f"\tcase {j+1}", end="")
            print()
        print("\n")


    def log_write(self, str):
        with open(self.log_file, "a") as f: f.write(str)


    def unzip_all(self):
        print("\n1. Unzip submissions")
        self.log_write("\n1. Unzip submissions")

        for i, s_id in enumerate(self.auto_grade_student_dict.keys()):
            progress_bar(i+1, self.progress_total, prefix="Progress ", suffix=f"{s_id}", length=50)

            student_grader: AutoGradeStudent = self.auto_grade_student_dict[s_id]
            zip_submitted = student_grader.unzip_submission()
            self.student_list_dict[str(s_id)] = "zip-submitted" if zip_submitted else "zip-not-submitted"

            
        with open("./student_list.json", "w") as f: json.dump(self.student_list_dict, f, indent=4)


    
    def file_format_correction_all(self):
        print("\n2. Change submitted files into correct format & Copy files into submission_by_problem")
        self.log_write("\n2. Change submitted files into correct format & Copy files into submission_by_problem")

        for i, s_id in enumerate(self.auto_grade_student_dict.keys()):
            progress_bar(i+1, self.progress_total, prefix="Progress ", suffix=f"{s_id}", length=50)
            
            student_grader: AutoGradeStudent = self.auto_grade_student_dict[s_id]
            if student_grader.zip_submitted: student_grader.change_file_names()


    def combine_submission_all(self):
        print("\n3. combine submission and cases")
        self.log_write("\n3. combine submission and cases")

        for i, s_id in enumerate(self.auto_grade_student_dict.keys()):
            progress_bar(i+1, self.progress_total, prefix="Progress ", suffix=f"{s_id}", length=50)
            
            student_grader: AutoGradeStudent = self.auto_grade_student_dict[s_id]
            if student_grader.zip_submitted: student_grader.combine_submission()

            
    def compile_output_all(self):
        print("\n4. Compile cases and make outputs, make diff file")
        self.log_write("\n4. Compile cases and make outputs, make diff file")

        for i, s_id in enumerate(self.auto_grade_student_dict.keys()):
            # progress_bar(i+1, self.progress_total, prefix="Progress ", suffix=f"{s_id}", length=50)
            
            student_grader: AutoGradeStudent = self.auto_grade_student_dict[s_id]
            if student_grader.zip_submitted: 
                print(f"\n>> student id : {s_id} / Progress ({i+1}/{self.progress_total})")    
                student_grader.compile_case_make_output()


    def score_all(self):
        print("\n5. score using outputs")
        self.log_write("\n5. score using outputs")

        for i, s_id in enumerate(self.auto_grade_student_dict.keys()):
            progress_bar(i+1, self.progress_total, prefix="Progress ", suffix=f"{s_id}", length=50)
            
            student_grader: AutoGradeStudent = self.auto_grade_student_dict[s_id]
            if student_grader.zip_submitted: student_grader.score_using_output()



    def print_report_all(self):
        print("\n6. Print reports")
        self.log_write("\n6. Print reports")

        for i, s_id in enumerate(self.auto_grade_student_dict.keys()):
            progress_bar(i+1, self.progress_total, prefix="Progress ", suffix=f"{s_id}", length=50)
            
            student_grader: AutoGradeStudent = self.auto_grade_student_dict[s_id]
            student_grader.print_reports()

    
    
    def result_score(self):
        # write information inside csv file
        result_csv_file="./result.csv" # result.txt information .csv
        write_score_file = "./result_score.csv" # score information .csv
        json_score_file = "./hw_info_score.json" # score information is in this .json file

        zip_not_submitted_list="./result_zip_not_submitted_list.csv"
        file_not_submitted_list="./result_file_not_submitted_list.csv"
        compile_error_list="./result_compile_error.csv"
        fail_list="./result_fail_list.csv"

        regrade_list = "./student_list_regrade.txt"

        f_result = open(result_csv_file, 'w')
        f_score = open(write_score_file, 'w')

        f_zip_not_submitted_list = open(zip_not_submitted_list, 'w')
        f_file_not_submitted_list = open(file_not_submitted_list, 'w')
        f_compile_error_list = open(compile_error_list, 'w')
        f_fail_list = open(fail_list, 'w')
        f_regrade_list = open(regrade_list, 'w')

        with open(json_score_file, 'r') as f:
            score_data=json.load(f)


        # write header of information in csv file
        csv_header = "student_id,"
        for i, prob_name in enumerate(self.prob_names):
            csv_header += f"{prob_name},"
            for j in range(self.case_num[i]):
                csv_header += f"{prob_name}-case-{j+1},"
        csv_header += "total-score,"
        csv_header += "\n"

        f_result.write(csv_header)
        f_score.write(csv_header)


        # write second csv header
        csv_header_2 = "student_id,\n"
        f_zip_not_submitted_list.write(csv_header_2)
        
        csv_header_2 = "student_id,prob_name,\n"
        f_file_not_submitted_list.write(csv_header_2)

        csv_header_2 = "student_id,prob_name,case_num,\n"
        f_compile_error_list.write(csv_header_2)
        f_fail_list.write(csv_header_2)


        # write student's result, score
        for s_id in self.student_list:
            student_result_file = f"./outputs/{s_id}/{s_id}_result.json"
            student_result = f"{s_id},"
            student_score_result = f"{s_id},"
            student_score_sum = 0

            # student did not submitted zip file
            if not os.path.isfile(student_result_file):
                student_zip_not_submitted = False
                student_result += "zip-file-not-submitted," * (sum(self.case_num)+1+len(self.prob_names))

                for i, prob_name in enumerate(self.prob_names):
                    prob_score = score_data[f"{prob_name}"]["zip-file-not-submitted"]
                    student_score_result += f"{prob_score},"
                    student_score_sum += prob_score
                    
                    for j in range(self.case_num[i]):
                        case_score = score_data[f"{prob_name}"]["zip-file-not-submitted"]
                        student_score_result += f"{case_score},"
                        student_score_sum += case_score
                
                student_score_result += f"{student_score_sum},"

                # write zip_not_submitted_list
                f_zip_not_submitted_list.write(f"{s_id},\n")


            # student did submitted zip file
            else:
                is_regrade = False
                with open(student_result_file, 'r') as f:
                    student_case_result = json.load(f)

                for i, prob_name in enumerate(self.prob_names):
                    prob_output = student_case_result[f"{prob_name}"]
                    student_result += f"{prob_output},"

                    prob_score = score_data[f"{prob_name}"][prob_output]
                    student_score_result += f"{prob_score},"
                    student_score_sum += prob_score
                    
                    for j in range(self.case_num[i]):
                        prob_output = student_case_result[f"{prob_name}"]
                        if prob_output == "file-not-submitted":
                            student_result += f"{prob_output},"

                            case_score = score_data[f"{prob_name}"][prob_output]
                            student_score_result += f"{case_score},"
                            student_score_sum += case_score

                            # write file_not_submitted_list
                            f_file_not_submitted_list.write(f"{s_id},{prob_name},\n")
                            is_regrade = True
                            break             
                        
                        else:
                            case_output = student_case_result[f"{prob_name}-case-{j+1}"]
                            student_result += f"{case_output},"
                            
                            case_score = score_data[f"{prob_name}-case-{j+1}"][case_output]
                            student_score_result += f"{case_score},"
                            student_score_sum += case_score

                            # write compile_error_list, fail_list
                            if case_output == "compile-error":
                                f_compile_error_list.write(f"{s_id},{prob_name},{j+1},\n")
                                is_regrade = True
                            elif case_output == "fail":
                                f_fail_list.write(f"{s_id},{prob_name},{j+1}\n")
                                is_regrade = True
                
                if is_regrade: f_regrade_list.write(f'{s_id}\n')

                student_result += ","
                student_score_result += f"{student_score_sum},"


            student_result += "\n"
            student_score_result += "\n"
            f_result.write(student_result)
            f_score.write(student_score_result)


        f_result.close()
        f_score.close()
        f_zip_not_submitted_list.close()
        f_file_not_submitted_list.close()
        f_compile_error_list.close()
        f_fail_list.close()
        f_regrade_list.close()


    def result_moss(self):
        moss_report_file = "./result_moss.md"
        if os.path.isfile(moss_report_file): os.remove(moss_report_file)

        write_str_moss = f"# MOSS result of all problems in {self.hw_name}\n\n"
        write_str_moss += "click the link below to see each problems's MOSS report\n\n"

        for prob_name in self.prob_names:
            print(f">> Progress: {prob_name}")

            moss_command = f'./_moss.pl -l cc ./submission_by_problem/{prob_name}/*.cpp'
            subprocess_result = subprocess.run(moss_command, shell=True, capture_output=True, text=True)
            link_str = subprocess_result.stdout.split('\n')[-2]
            write_str_moss += f"{prob_name} : {link_str}\n"
        
        write_str_moss += "\n"

        # SPECIAL LOGIC - HW2: give MOSS class member file too
        print(f">> Progress: 1-class member")
        moss_command = f'./_moss.pl -l cc ./submission_by_problem/1-class/*.cpp'
        subprocess_result = subprocess.run(moss_command, shell=True, capture_output=True, text=True)
        link_str = subprocess_result.stdout.split('\n')[-2]
        write_str_moss += f"1-class : {link_str}\n"

        with open(moss_report_file, "w") as f: f.write(write_str_moss)


    def dump_student_list(self):
        with open("./student_list.json", "w") as f: json.dump(self.student_list_dict, f, indent=4)


    def regrade_student(self, s_id: int, is_zip=False):
        student_grader: AutoGradeStudent = self.auto_grade_student_dict[s_id]
        if is_zip:
            zip_submitted = student_grader.unzip_submission()
            self.student_list_dict[str(s_id)] = "zip-submitted" if zip_submitted else "zip-not-submitted"
            
            if self.student_list_dict[str(s_id)] == "zip-submitted":
                student_grader.change_file_names()
        
        if self.student_list_dict[str(s_id)] == "zip-submitted":
            student_grader.combine_submission(True)
            student_grader.compile_case_make_output(False)
            student_grader.score_using_output()
            student_grader.print_reports()


    def reset(self):
        counter = 1
        print(">> Remove student's submission, output")
        self.log_write(">> Remove student's submission, output\n")
        # Remove student's submission, output
        for s_id in self.student_list:
            progress_bar(counter+1, len(self.student_list)+1, prefix="Progress: ", suffix=f"{s_id}", length=50)
            
            if os.path.isdir(f"./student_submission/{s_id}"): shutil.rmtree(f"./student_submission/{s_id}")
            if os.path.isdir(f"./outputs/{s_id}"): shutil.rmtree(f"./outputs/{s_id}")

            if os.path.isfile(f"./student_submission/{self.hw_name}_{s_id}.zip"):
                os.remove(f"./student_submission/{self.hw_name}_{s_id}.zip")
            
            counter += 1

        # Remove report files
        print(">> Remove Report files")
        dir_list = glob.glob('./submission_by_problem/*/')
        for dir_path in dir_list: shutil.rmtree(dir_path)
        dir_list = glob.glob('./reports/not_submitted/*')
        for dir_path in dir_list: shutil.rmtree(dir_path)
        dir_list = glob.glob('./reports/submitted/*')
        for dir_path in dir_list: shutil.rmtree(dir_path)

        # Remove Result .csv files
        print(">> Remove Result .csv files")
        result_list = glob.glob('./result*.csv')
        for result_csv in result_list: 
            if os.path.isfile(result_csv): os.remove(result_csv)

        if os.path.isfile("./result_moss.md"): os.remove("./result_moss.md")
        
        # Reset student list
        print(">> Reset student_list.json")
        student_list_file = "./student_list.json"
        with open(student_list_file, "r") as f: student_list_dict = json.load(f)
        student_list = list(map(int, list(student_list_dict.keys())))
        rewrite_student_dict = {s_id: "zip-not-submitted" for s_id in student_list}
        with open(student_list_file, "w") as f: json.dump(rewrite_student_dict, f, indent=4)

        # Extra works - remove log, lock
        print(">> Extra work - remove lock")
        if os.path.isfile('./student_list_regrade.txt'): os.remove('./student_list_regrade.txt')
        # if os.path.isfile("./run_log.txt"): os.remove("./run_log.txt")
        if os.path.isfile("./.runlock"): os.remove("./.runlock")


    def run_output(self, s_id, prob_name, case_num):
        student_grader: AutoGradeStudent = self.auto_grade_student_dict[s_id]
        return student_grader.run_output(prob_name, case_num)
        


def main():
    grader = AutoGrader()

    grader.print_grader_start()

    grader.unzip_all()
    grader.file_format_correction_all()
    grader.combine_submission_all()
    grader.compile_output_all()
    grader.score_all()
    grader.print_report_all()
    grader.result_score()
    grader.result_moss()


if __name__ == "__main__":
    main()