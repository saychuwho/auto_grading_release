#!/bin/bash

# extract student csv - extract student number & name from user_list & make student_list.txt

IFS=','

FILE_NAME1="./student_list.csv"
FILE_NAME2="./student_list.txt"

while read -r blank1 sid name blank2 email who blank3; do
    if [ "$who" = "학생" ]; then
        echo "$sid,$name,$email,$who" >> $FILE_NAME1
        printf "$sid" >> $FILE_NAME2
    fi
done < user_list.csv

