#!/bin/bash

# setup : set shells to be executable & install needed things 

dos2unix ./run.sh
dos2unix ./reset.sh
dos2unix ./student_list.txt
dos2unix ./_report_print.sh
dos2unix ./_result_score.py
dos2unix ./run_extra.sh
dos2unix ./_result_moss.sh
dos2unix ./_convert_file_name.py
dos2unix ./_run_extra_onlyone.sh
dos2unix ./run_output.sh

chmod 755 ./run.sh
chmod 755 ./reset.sh
chmod 755 ./_report_print.sh
chmod 755 ./run_extra.sh
chmod ug+x ./_moss.pl
chmod 755 ./_result_moss.sh
chmod 755 ./_run_extra_onlyone.sh
chmod 755 ./run_output.sh