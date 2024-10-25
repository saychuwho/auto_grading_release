import os
import re

def has_dir(path):
    with os.scandir(path) as entries:
        for entry in entries:
            # do not care directory name "__MACOSX"
            if entry.is_dir() and not entry.name == "__MACOSX":
                return True
            
    return False


def __find_function_block(file_content: list, function_pattern, function_pattern_alter=None):
    inside_function = False
    function_name = ""
    function_block = ""
    brace_count = 0
    brace_open_count = 0

    for line in file_content:
        if not inside_function:
            if function_pattern in line or (function_pattern_alter != None and function_pattern_alter in line):
                # exclude class pattern that have ; inside
                if f"{function_pattern_alter};" in line: continue
                
                inside_function = True

                function_name = line.strip()
                function_block = function_name + "\n"

                # check if function is end at here
                if '{' in function_name:
                    brace_count += line.count('{')
                    brace_open_count += line.count('{')
                    brace_count -= line.count('}')
                    if brace_count == 0:
                        inside_function = False
                        break

        else:
            tmp_line = line
            function_block += tmp_line

            brace_count += line.count('{')
            brace_open_count += line.count('{')
            brace_count -= line.count('}')

            if brace_open_count > 0 and brace_count == 0:
                inside_function = False
                break

    return function_block



def find_function(data: str, func_return_type: str, func_name: str, is_path=True) -> str:

    # make function pattern
    function_pattern = f"{func_return_type} {func_name}("           # default - find functions
    function_pattern_alter = f"{func_return_type} {func_name} ("    # default - with space
    if len(func_return_type) == 0:
        function_pattern = f"{func_name}("                          # use it to find constructor inside class
        function_pattern_alter = f"{func_name} ("                   # use it to find constructor : alter
    elif func_return_type == "class":
        function_pattern = f"{func_return_type} {func_name} "       # use it to find class
        function_pattern_alter = f"{func_return_type} {func_name}"
        

    if is_path: # if path is send, read contents from file
        with open(data, 'r', errors="replace") as cpp_file: cpp_file_content = cpp_file.readlines()
    else:       # if content(like class) send, spilt it.
        cpp_file_content = [x + '\n' for x in data.split('\n')]

    function_block = __find_function_block(cpp_file_content, function_pattern, function_pattern_alter)
        
    return function_block


def find_member_functions(data:str, class_name: str, log_write, is_path=True):
    func_list = []


    function_pattern = re.compile(
        rf'^([a-zA-Z_][a-zA-Z0-9_:<>\s*&]*\s+)?({class_name}\s*::)\s*(~?[a-zA-Z_][a-zA-Z0-9_]*)\s*\(',
        re.MULTILINE
    )

    if is_path: # if path is send, read contents from file
        with open(data, 'r', errors="replace") as cpp_file: cpp_file_content = cpp_file.read()

    matches = function_pattern.finditer(cpp_file_content)

    with open(data, 'r', errors="replace") as cpp_file: cpp_file_content = cpp_file.readlines()

    for match in matches:
        func_pattern = match.group()
        log_write(f"\n\nfunc_pattern {class_name}:\n{func_pattern}")
        func_list.append(__find_function_block(cpp_file_content, func_pattern))                    
        
    return func_list


def find_line(data:str, pattern: str, is_path=True):
    res_line = ""

    if is_path: # if path is send, read contents from file
        with open(data, 'r', errors="replace") as cpp_file: cpp_file_content = cpp_file.readlines()
    else:       # if content(like class) send, spilt it.
        cpp_file_content = data.split('\n')

    for line in cpp_file_content:
        if pattern in line:
            res_line = line
            return res_line
    
    return res_line



def is_file_empty(file: str):
    with open(file, 'r') as f:
        if f.readline():
            return False
        else:
            return True
        

""" code from ChatGPT """
def progress_bar(iteration, total, prefix='', suffix='', decimals=1, length=50, fill='#', print_end='\r'):
    percent = ("{0:." + str(decimals) + "f}").format(100 * (iteration / float(total)))
    filled_length = int(length * iteration // total)
    bar = fill * filled_length + '-' * (length - filled_length)
    print(f'\r{prefix} |{bar}| {percent}% {suffix}', end=print_end)
    # Print New Line on Complete
    if iteration == total:
        print()