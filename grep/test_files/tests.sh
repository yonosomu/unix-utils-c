#!/bin/bash

SUCCESS=0
FAIL=0
COUNTER=0
DIFF=""

# Цвета
GREEN='\033[1;32m'
RED='\033[1;31m'
NC='\033[0m'


s21_command=(
    "./s21_grep"
)

sys_command=(
    "grep"
)


flags=(
    "v"
    "c"
    "l"
    "n"
)


tests=(
    "s test_files/test_0_grep.txt FLAGS"
    "for s21_grep.c s21_grep.h Makefile FLAGS"
    "for s21_grep.c FLAGS"
    "-e for -e ^int s21_grep.c s21_grep.h Makefile FLAGS"
    "-e for -e ^int s21_grep.c FLAGS"
    "-e regex -e ^print s21_grep.c FLAGS test_files/test_ptrn_grep.txt"
    "-e while -e void s21_grep.c Makefile FLAGS test_files/test_ptrn_grep.txt"
    "-e intel -e int FLAGS test_files/test_7_grep.txt"
    "-e int -e intel FLAGS test_files/test_7_grep.txt"
)


manual=(
    "-n for test_files/test_1_grep.txt test_files/test_2_grep.txt"
    "-n for test_files/test_1_grep.txt"
    "-n -e ^\} test_files/test_1_grep.txt"
    "-c -e /\ test_files/test_1_grep.txt"
    "-ce ^int test_files/test_1_grep.txt test_files/test_2_grep.txt"
    "-e ^int test_files/test_1_grep.txt"
    "-niv = test_files/test_1_grep.txt test_files/test_2_grep.txt"
    "-e"
    "-ie INT test_files/test_5_grep.txt"
    "-echar test_files/test_1_grep.txt test_files/test_2_grep.txt"
    "-ne = -e out test_files/test_5_grep.txt"
    "-iv int test_files/test_5_grep.txt"
    "-in int test_files/test_5_grep.txt"
    "-c -l aboba test_files/test_1_grep.txt test_files/test_5_grep.txt"
    "-v test_files/test_1_grep.txt -e ank"
    "-ne ) test_files/test_5_grep.txt"
    "-l for test_files/test_1_grep.txt test_files/test_2_grep.txt"
    "-e int test_files/test_4_grep.txt"
    "-e = -e out test_files/test_5_grep.txt"
    "-ne ing -e as -e the -e not -e is test_files/test_6_grep.txt"
    "-e ing -e as -e the -e not -e is test_files/test_6_grep.txt"
    "-c -e . test_files/test_1_grep.txt -e '.'"
    "-l for no_file.txt test_files/test_2_grep.txt"
    "-e int -si no_file.txt s21_grep.c no_file2.txt s21_grep.h"
)


run_test() {
    param=$(echo "$@" | sed "s/FLAGS/$var/")

    "${s21_command[@]}" $param > "${s21_command[@]}".log
    "${sys_command[@]}" $param > "${sys_command[@]}".log

    DIFF="$(diff -s "${s21_command[@]}".log "${sys_command[@]}".log)"

    let "COUNTER++"

    if [ "$DIFF" == "Files ${s21_command[@]}.log and ${sys_command[@]}.log are identical" ]
    then
        let "SUCCESS++"
        printf "%s - ${GREEN}Success${NC} %s\n" "$COUNTER" "$param"
    else
        let "FAIL++"
        printf "%s - ${RED}Fail${NC} %s\n" "$COUNTER" "$param"
    fi

    rm -f "${s21_command[@]}".log "${sys_command[@]}".log
}


printf "^^^^^^^^^^^^^^^^^^^^^^^\n"
printf "TESTS WITH NORMAL FLAGS\n"
printf "^^^^^^^^^^^^^^^^^^^^^^^\n"
printf "\n"

printf "#######################\n"
printf "MANUAL TESTS\n"
printf "#######################\n"
printf "\n"


for i in "${manual[@]}"
do
    var="-"
    run_test "$i"
done


printf "\n"
printf "#######################\n"
printf "AUTOTESTS\n"
printf "#######################\n"
printf "\n"

printf "=======================\n"
printf "1 PARAMETER\n"
printf "=======================\n"
printf "\n"


for var1 in "${flags[@]}"
do
    for i in "${tests[@]}"
    do
        var="-$var1"
        run_test "$i"
    done
done


printf "\n"
printf "=======================\n"
printf "2 PARAMETERS\n"
printf "=======================\n"
printf "\n"


for var1 in "${flags[@]}"
do
    for var2 in "${flags[@]}"
    do
        if [ "$var1" != "$var2" ]
        then
            for i in "${tests[@]}"
            do
                var="-$var1 -$var2"
                run_test "$i"
            done
        fi
    done
done


printf "\n"
printf "#######################\n"
printf "AUTOTESTS\n"
printf "#######################\n"
printf "\n"

printf "=======================\n"
printf "DOUBLE PARAMETER\n"
printf "=======================\n"
printf "\n"


for var1 in "${flags[@]}"
do
    for var2 in "${flags[@]}"
    do
        if [ "$var1" != "$var2" ]
        then
            for i in "${tests[@]}"
            do
                var="-$var1$var2"
                run_test "$i"
            done
        fi
    done
done


printf "\n"
printf "${RED}FAILED: %s${NC}\n" "$FAIL"
printf "${GREEN}SUCCESSFUL: %s${NC}\n" "$SUCCESS"
printf "ALL: %s\n" "$COUNTER"
printf "\n"