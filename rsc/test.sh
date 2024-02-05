#!/bin/bash

DIFF_LS="./rsc/ls_diff.sh"

BASIC_FLAG="-alr"

BASIC_RECURCE="-aR"

LONG_RECURCE="-aRl"

T_DIR="test"


${DIFF_LS} ${BASIC_FLAG} ${T_DIR}
${DIFF_LS} ${BASIC_FLAG} ${T_DIR} /
${DIFF_LS} ${BASIC_RECURCE} ${T_DIR}
${DIFF_LS} ${LONG_RECURCE} ${T_DIR}
${DIFF_LS} -lar / /dev
# Test mixed argument dir + simple file for l and no l option
${DIFF_LS} -a / Makefile
${DIFF_LS} -al / Makefile
# Test mixed argument dir +  incorect file for l and no l option
${DIFF_LS} -al / sda; echo Exit code $?
${DIFF_LS} -a / sda; echo Exit code $?
