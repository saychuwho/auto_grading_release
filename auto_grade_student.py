import json
import os
import subprocess
import re
import shutil

from functions import has_dir, find_function, is_file_empty, find_member_functions, find_line
from report_print import ReportPrint


class AutoGradeStudent:
    def __init__(self, s_id: int, 
                 log_file: str, 
                 hw_info_dict: dict, 
                 hw_pattern_dict: dict,
                 zip_submitted = False):
        # information of grading
        self.s_id = s_id
        self.log_file = log_file
        self.hw_name = hw_info_dict['hw_name']
        self.prob_names = hw_info_dict['prob_name']     # contains hw's prob_name
        self.case_num = hw_info_dict['case_num']        # contains case num per prob_name
        self.function_num = hw_info_dict['func_num']    # contains function number per prob_name
        self.class_num = hw_info_dict['class_num']      # contains class number per prob_name
        self.hw_pattern = hw_pattern_dict
        
        # student's case status
        self.zip_submitted = zip_submitted
        self.file_submitted = {prob_name: False for prob_name in self.prob_names}
        self.compile_success = {prob_name: [False for _ in range(self.case_num[i])] for i, prob_name in enumerate(self.prob_names)}



    def log_write(self, str):
        """
        Write log string into self.log_file

        Args:
            str (str): log string to write
        """        
        with open(self.log_file, "a") as f: f.write(str)


    def unzip_submission(self):
        """
        Unzip submission that student submitted.
        Rename the zip file's name with using self.hw_name, self.s_id

        Returns:
            bool: return True if student submitted zip file.
        """        
        zip_list = os.listdir("./student_submission/")
        student_zip_res = [x for x in zip_list if re.search(fr'{self.s_id}', x)]
        
        self.log_write(f">> {self.s_id} unzip files:\n")
        
        if student_zip_res:
            student_zip = student_zip_res[0] 
            student_zip_file = f"./student_submission/{student_zip}"
        
            copy_zip_file = f'./student_submission/{self.hw_name}_{self.s_id}.zip'
            shutil.copy(student_zip_file, copy_zip_file)

            unzip_command = ['unzip','-O','UTF-8','-o', f'{copy_zip_file}', '-d', f'./student_submission/{self.s_id}']
            unzip_result = subprocess.run(unzip_command, stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)                        
            
            if unzip_result.returncode != 0:
                self.log_write(f"\tE: unzip failed")
            else:
                self.zip_submitted = True
                self.log_write(f"\tunzip success\n")
        else:
            self.log_write(f"\tE: no zip file submitted\n")

        return self.zip_submitted


    def __is_function_inside_file(self, submit_file_content, prob_name, is_class=False):
        """
        Return True if submit_file_content have prob_name's function pattern.

        Args:
            submit_file_content (List[str]): Student's submitted file content
            prob_name (str): Problem name
            is_class (bool, optional): Set True if submit_file_content uses class. Defaults to False.

        Returns:
            bool : if submit_file_content have function pattern of prob_name, return True.
        """        
        if is_class:
            func_0_0_name = self.hw_pattern[prob_name]["0-1"].split()[1]
            func_0_1_name = self.hw_pattern[prob_name]["0-2"].split()[2]
            if func_0_0_name in submit_file_content or func_0_1_name in submit_file_content:
                return True
            else:
                return False
        else:
            func_0_name = self.hw_pattern[prob_name]["0"].split()[1]
            if func_0_name in submit_file_content:
                return True
            else:
                return False



    def change_file_names(self):
        """
        Change submitted source code file name using self.hw_name, prob_name, self.sid.
        If student's submission contains folder, find .cpp files and copy it into parent directory
        Find student's problem submission with function name pattern in self.hw_pattern.
        Copy submitted source code into ./submission_by_problem. These files are used in MOSS plagirism detection.
        """        
        self.log_write(f"\n>> {self.s_id} change file name into correct format:\n")

        # copy .cpp files into student_id folder if cpp file is in folder
        student_dir = f"./student_submission/{self.s_id}"
        if has_dir(student_dir):
            self.log_write(f"\tfolder detected. copy files out from folder.\n")
            cpp_files = []
            for root, _, files in os.walk(student_dir):
                for file in files:
                    if file.endswith('.cpp'):
                        cpp_files.append(os.path.join(root, file))
            

            for cpp_file in cpp_files:
                copy_cpp_file = f"./student_submission/{self.s_id}/{os.path.basename(cpp_file)}"
                # eliminate case that copying same file
                if cpp_file != copy_cpp_file: shutil.copy(cpp_file, copy_cpp_file)


        # make file format into correct format & copy files into submission_by_problem
        for i, prob_name in enumerate(self.prob_names):
            if self.class_num[i] > 0: is_class = True
            else: is_class = False
            # make file name format into correct format
            student_cpp_files = [file for file in os.listdir(student_dir) if ".cpp" in file]
            is_exist = False
            correct_file_name = f"./student_submission/{self.s_id}/{self.hw_name}_{prob_name}_{self.s_id}.cpp"
            
            for cpp_file in student_cpp_files:
                submit_file_name = f"./student_submission/{self.s_id}/{cpp_file}"
                # use errors="replace" to handle student code encoded with EUC-KR
                with open(submit_file_name, 'r', errors="replace") as wrong_file: submit_file_content = wrong_file.read()
                is_exist = self.__is_function_inside_file(submit_file_content, prob_name, is_class)
                if is_exist: break

            if is_exist:
                shutil.copy(submit_file_name, correct_file_name)

                # change CRLF into LF in submission
                dos2unix_command = ['dos2unix', correct_file_name]
                subprocess.run(dos2unix_command, stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)

                self.log_write(f"\trewrite {prob_name} file name into correct format.\n")
                self.file_submitted[prob_name] = True
            else:
                self.log_write(f"\tE: {prob_name} sorce code not submitted")
                continue

            # copy files into submission_by_problem
            if not os.path.isdir(f"./submission_by_problem/{prob_name}"): os.mkdir(f"./submission_by_problem/{prob_name}")
            submission_by_problem_file_name = f"./submission_by_problem/{prob_name}/{self.hw_name}_{prob_name}_{self.s_id}.cpp"
            shutil.copy(correct_file_name, submission_by_problem_file_name)


        self.log_write("\n")


    def __write_functions(self, iterator, output_file, grading_header, prob_name, submission_file, grading_case, is_class=False):
        combine_f = open(output_file, 'w')
        
        # write header
        combine_f.write(grading_header)
        combine_f.write("\n\n")

        # class case: write class declaration and class member function
        if is_class:
            for k in range(self.class_num[iterator]):
                # write class declaration
                class_name = self.hw_pattern[f"{prob_name}-class"][str(k)].split()[1]
                class_declare = find_function(submission_file, "class", class_name)
                combine_f.write(class_declare)

                # SPECIAL LOGIC FOR HW2
                if class_name == "Product": 
                    combine_f.write(find_line(submission_file, "int Product::nextId"))
                    combine_f.write(find_line(submission_file, "int Product :: nextId"))
                    combine_f.write(find_line(submission_file, "int Product::nextID"))


                self.log_write(f"\n\nclass block\nclass_name: {class_name}:\n{class_declare}\n")

                combine_f.write("\n\n")

                # write class member functions if it exist
                class_member_func_list = find_member_functions(submission_file, class_name, log_write=self.log_write)

                self.log_write(f"\n\nclass_member_func_list:\n{class_member_func_list}\n")

                if len(class_member_func_list) != 0:
                    for member_block in class_member_func_list:
                        if len(member_block) != 0:
                            combine_f.write(member_block)

                            self.log_write(f"\n\n{k} member block:\n{member_block}\n")

                            combine_f.write("\n\n")

        # find functions in submitted code and combine it in output file.
        else:
            for k in range(self.function_num[iterator]):
                func_return_type = self.hw_pattern[prob_name][str(k)].split()[0]
                func_name = self.hw_pattern[prob_name][str(k)].split()[1]
                func_block = find_function(submission_file, func_return_type, func_name)
                
                combine_f.write(func_block)
                combine_f.write("\n\n")

        # write grading case's main function
        if grading_case != None:
            combine_f.write(grading_case)
            combine_f.write("\n\n")

        combine_f.close()


    def combine_submission(self):
        """
        Make source code to compile.
        Find functions in student's submission with self.hw_pattern to make compiling code.
        Add problem's header in ./grading_cases 
        """        
        self.log_write(f"\n>> {self.s_id} combine submission to make case main\n")
        
        # make directory if it is not exist
        if not os.path.isdir(f"./outputs/{self.s_id}"): os.mkdir(f"./outputs/{self.s_id}")

        # make compiling source code per case
        for i, prob_name in enumerate(self.prob_names):
            if self.class_num[i] > 0: is_class = True
            else: is_class = False  

            submission_file=f"./student_submission/{self.s_id}/{self.hw_name}_{prob_name}_{self.s_id}.cpp"
            
            if is_class: self.log_write(f"\t>> class submission\n")
            else: self.log_write(f"\t>> default submission\n")

            for j in range(self.case_num[i]):
                output_file = f"./outputs/{self.s_id}/{self.hw_name}_{prob_name}_case_{j+1}_{self.s_id}.cpp"
                grading_case_file = f"./grading_cases/{self.hw_name}_{prob_name}_case_{j+1}.cpp"
                grading_header_file = f"./grading_cases/{self.hw_name}_{prob_name}_header.cpp"
                with open(grading_case_file, 'r') as f: grading_case = f.read()
                with open(grading_header_file, 'r') as f: grading_header = f.read()

                # combine file if student submitted code for problem
                if self.file_submitted[prob_name]:
                
                    self.__write_functions(i, output_file, grading_header, prob_name,
                                        submission_file, grading_case, is_class=is_class)
                    
                    # remove zero witdh no-break space : This issue happens often
                    with open(output_file, 'r') as combine_f: combine_code = combine_f.read()
                    combine_code = combine_code.replace('\xEF\xBB\xBF', '')
                    with open(output_file, 'w') as combine_f: combine_f.write(combine_code)

                    self.log_write(f"\t>>>> {self.s_id}: combined {prob_name}-{j+1}\n")
                else:
                    self.log_write(f"\t>>>> {self.s_id}: not submitted {prob_name}\n")


            # SPECIAL LOGIC FOR HW2 - copy class member function file to submission_by_problem
            tmp_output_file = f"./outputs/{self.s_id}/{self.hw_name}_{prob_name}_{self.s_id}_class_member.cpp"
            tmp_grading_header_file = f"./grading_cases/{self.hw_name}_{prob_name}_header.cpp"
            with open(tmp_grading_header_file, 'r') as f: tmp_grading_header = f.read()
            if self.file_submitted[prob_name]:
                self.__write_functions(iterator=0, output_file=tmp_output_file, grading_header=tmp_grading_header,
                                       prob_name="1", submission_file=submission_file, grading_case=None, is_class=True)
                
                # remove zero witdh no-break space : This issue happens often
                with open(output_file, 'r') as combine_f: combine_code = combine_f.read()
                combine_code = combine_code.replace('\xEF\xBB\xBF', '')
                with open(output_file, 'w') as combine_f: combine_f.write(combine_code)

                if not os.path.isdir(f"./submission_by_problem/{prob_name}-class"): os.mkdir(f"./submission_by_problem/{prob_name}-class")
                submission_by_problem_file_name = f"./submission_by_problem/{prob_name}-class/{self.hw_name}_{prob_name}_{self.s_id}.cpp"
                shutil.copy(tmp_output_file, submission_by_problem_file_name)
                

        self.log_write(f"\n")
        


    def compile_case_make_output(self, is_print=True):
        """        
        Compile compiling code combined before.
        Execute output program and make diff file for grading.
    
        Args:
            is_print (bool, optional): options to print which student's submission is compiled. Defaults to True.
        """        
                  
        self.log_write(f"\n>> {self.s_id} compile cases and make outputs, make diff file\n")
        
        for i, prob_name in enumerate(self.prob_names):
            self.log_write(f"\t{self.s_id} : {prob_name}\n")
            for j in range(self.case_num[i]):

                if is_print: print(f"\r>>>> {prob_name}-case-{j+1}\t", end="")        
                
                self.log_write(f"\t\t{self.s_id} : case {j+1} (")

                grading_case_output_file = f"./grading_cases/{self.hw_name}_{prob_name}_case_{j+1}_answer.txt"
                output_file = f"./outputs/{self.s_id}/{self.hw_name}_{prob_name}_case_{j+1}_{self.s_id}"

                # compile combined source code
                if self.file_submitted[prob_name]:
                    gcc_command = ['g++', '-w', '-o', f"{output_file}.out", f"{output_file}.cpp"]
                    subprocess_result = subprocess.run(gcc_command, capture_output=True, text=True)
                    with open(f"{output_file}_compile_result.txt", 'w') as f: f.write(subprocess_result.stderr)
                    
                    if len(subprocess_result.stderr) > 0: self.log_write(": compile error\n")
                    else:
                        self.compile_success[prob_name][j] = True 
                        self.log_write(": compile success\n")
                else: 
                    self.log_write(": file does not exist\n")
                    continue

                # run output and get result
                run_output_command = ['timeout', '10s', f"{output_file}.out"]
                subprocess_result = subprocess.run(run_output_command, capture_output=True, text=True)
                with open(f"{output_file}_output.txt", 'w') as f: f.write(subprocess_result.stdout)
                
                # make diff file between output result and grading case answer
                diff_command = ['diff', '-uZB', '--strip-trailing-cr', grading_case_output_file, f"{output_file}_output.txt"]
                subprocess_result = subprocess.run(diff_command, capture_output=True, text=True)
                with open(f"{output_file}_output_diff.txt", 'w') as f: f.write(subprocess_result.stdout)

        if is_print: print(f"\r>>>> Done               ")

        self.log_write("\n")



    def score_using_output(self):
        """
        Make json file that contains information of graded result.
        Grading policy: 
            If compile result file is not empty, then this case is compile-error.
            If diff file is empty, then this case is pass. Else, case is fail.
        """        
        self.log_write(f"\n>> {self.s_id} score using outputs\n")

        student_score_json_file = f"./outputs/{self.s_id}/{self.s_id}_result.json"
        student_score = {}

        # write score information in student_score.json
        for i, prob_name in enumerate(self.prob_names):
            self.log_write(f"\t{self.s_id} : {prob_name}\n")
            
            output_file = f"./outputs/{self.s_id}/{self.hw_name}_{prob_name}_case_1_{self.s_id}"
            if not os.path.isfile(f"{output_file}_compile_result.txt"):
                student_score[f"{prob_name}"] = "file-not-submitted"
            else:
                student_score[f"{prob_name}"] = "file-submitted"
                for j in range(self.case_num[i]):
                    self.log_write(f"\t\t{self.s_id} : case {j+1}")
                    output_file = f"./outputs/{self.s_id}/{self.hw_name}_{prob_name}_case_{j+1}_{self.s_id}"

                    if not is_file_empty(f"{output_file}_compile_result.txt"):
                        student_score[f"{prob_name}-case-{j+1}"] = "compile-error"
                    elif not is_file_empty(f"{output_file}_output_diff.txt"):
                        student_score[f"{prob_name}-case-{j+1}"] = "fail"
                    else:
                        student_score[f"{prob_name}-case-{j+1}"] = "pass"
                self.log_write("\n")
        
        with open(student_score_json_file, "w") as json_file: json.dump(student_score, json_file, indent=4)

        self.log_write("\n")


    def print_reports(self):
        """
        Generate markdown reports using ReportPrint
        """        
        self.log_write(f"\n>> {self.s_id} print reports for student\n")
        
        report_print = ReportPrint(self.s_id, self.hw_name, self.prob_names, self.case_num, 
                                   self.class_num, self.zip_submitted)
        self.log_write(f">>>> print report into markdown\n")
        report_print.print_report()

        self.log_write("\n")


    def run_output(self, prob_name, case_num):
        output_file = f"./outputs/{self.s_id}/{self.hw_name}_{prob_name}_case_{case_num}_{self.s_id}"
        
        if not os.path.isfile(f"{output_file}.out"): return False

        run_output_command = ['timeout', '10s', f"{output_file}.out"]
        subprocess.run(run_output_command)

        return True
        
    