#!/bin/bash

SUCCESS=0
FAIL=0

GREEN="\033[0;32m"
RED="\033[0;31m"
NC="\033[0m"


run_test() {
    FLAG=$1
    FILE=$2

    echo "===== TEST $FLAG $FILE ====="

    cat $FLAG $FILE > tests/sys.out
    ./s21_cat $FLAG $FILE > tests/my.out

    if diff tests/sys.out tests/my.out > /dev/null
    then
        echo -e "${GREEN}OK${NC}"
        ((SUCCESS++))
    else
        echo -e "${RED}FAIL${NC}"
        diff tests/sys.out tests/my.out
        ((FAIL++))
    fi
}


run_test "-n" tests/test1.txt
run_test "-b" tests/test2.txt
run_test "-s" tests/test2.txt
run_test "-v" tests/test3.txt
run_test "-e" tests/test3.txt
run_test "-t" tests/test3.txt


echo ""
echo "===================="
echo "SUCCESS: $SUCCESS"
echo "FAIL: $FAIL"
echo "===================="


if [ $FAIL -eq 0 ]
then
    exit 0
else
    exit 1
fi