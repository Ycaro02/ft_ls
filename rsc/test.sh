#!/bin/bash

DIFF_LS="./rsc/ls_diff.sh"

BASIC_FLAG="-alr"

BASIC_RECURCE="-aR"

LONG_RECURCE="-aRl"

LS="./ft_ls"

T_DIR="test"

NULL="/dev/null"

echo Test diff
${DIFF_LS} ${BASIC_FLAG} ${T_DIR}
${DIFF_LS} ${BASIC_FLAG} ${T_DIR} /
${DIFF_LS} ${BASIC_RECURCE} ${T_DIR}
${DIFF_LS} ${LONG_RECURCE} ${T_DIR}
${DIFF_LS} -lar / /dev
# Test mixed argument dir + simple file for l and no l option
${DIFF_LS} -a / Makefile
${DIFF_LS} -al / Makefile
# Test mixed argument dir +  incorect file for l and no l option
${DIFF_LS} -al / sda 2> $NULL
${DIFF_LS} -a / sda 2> $NULL

echo Test Exit code 2
mkdir -p noperm && chmod 000 noperm
${LS} dsa > "${NULL}" 2> "${NULL}"; echo Exit code $?
${LS} dsa / > "${NULL}" 2> "${NULL}"; echo Exit code $?
${LS} dsa Makefile > "${NULL}" 2> "${NULL}"; echo Exit code $?
${LS} dsa Makefile / > "${NULL}" 2> "${NULL}"; echo Exit code $?

${LS} -l dsa / > "${NULL}" 2> "${NULL}"; echo Exit code $?
${LS} -l dsa > "${NULL}" 2> "${NULL}"; echo Exit code $?
${LS} -l dsa Makefile > "${NULL}" 2> "${NULL}"; echo Exit code $?
${LS} -l dsa Makefile / > "${NULL}" 2> "${NULL}"; echo Exit code $?

${LS} noperm > "${NULL}" 2> "${NULL}"; echo Exit code $?
${LS} noperm / > "${NULL}" 2> "${NULL}"; echo Exit code $?
${LS} noperm Makefile > "${NULL}" 2> "${NULL}"; echo Exit code $?
${LS} noperm Makefile / > "${NULL}" 2> "${NULL}"; echo Exit code $?

${LS} -l noperm > "${NULL}" 2> "${NULL}"; echo Exit code $?
${LS} -l noperm / > "${NULL}" 2> "${NULL}"; echo Exit code $?
${LS} -l noperm Makefile > "${NULL}" 2> "${NULL}"; echo Exit code $?
${LS} -l noperm Makefile / > "${NULL}" 2> "${NULL}"; echo Exit code $?


echo Test Exit code 1
${LS} -R > "${NULL}" 2> "${NULL}"; echo Exit code $?
${LS} -R . > "${NULL}" 2> "${NULL}"; echo Exit code $?
${LS} -R Makefile . > "${NULL}" 2> "${NULL}"; echo Exit code $?

${LS} -R -l . > "${NULL}" 2> "${NULL}"; echo Exit code $?
${LS} -R -l > "${NULL}" 2> "${NULL}"; echo Exit code $?
${LS} -R -l Makefile . > "${NULL}" 2> "${NULL}"; echo Exit code $?

${LS} -R > "${NULL}" 2> "${NULL}"; echo Exit code $?
${LS} -R . > "${NULL}" 2> "${NULL}"; echo Exit code $?
${LS} -R Makefile . > "${NULL}" 2> "${NULL}"; echo Exit code $?

${LS} -R -l > "${NULL}" 2> "${NULL}"; echo Exit code $?
${LS} -R -l . > "${NULL}" 2> "${NULL}"; echo Exit code $?
${LS} -R -l Makefile . > "${NULL}" 2> "${NULL}"; echo Exit code $?


echo Test Exit code 0
${LS} -R -l Makefile > "${NULL}" 2> "${NULL}"; echo Exit code $?
${LS} -R Makefile > "${NULL}" 2> "${NULL}"; echo Exit code $?
${LS} -R Makefile > "${NULL}" 2> "${NULL}"; echo Exit code $?
${LS} -R -l Makefile > "${NULL}" 2> "${NULL}"; echo Exit code $?
${LS} -R test > "${NULL}" 2> "${NULL}"; echo Exit code $?
${LS} -Rl test > "${NULL}" 2> "${NULL}"; echo Exit code $?


# sudo rm -rf noperm