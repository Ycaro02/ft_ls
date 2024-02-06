#!/bin/bash

DIFF_LS="./rsc/ls_diff.sh"

BASIC_FLAG="-alr"

BASIC_RECURCE="-aR"

LONG_RECURCE="-aRl"

LS="./ft_ls"

T_DIR="test"

NULL="/dev/null"

echo create special perm dir
mkdir -p noperm && chmod 000 noperm
mkdir -p no_exe_perm && touch no_exe_perm/koala1 no_exe_perm/koala2 no_exe_perm/koala3 && chmod -x no_exe_perm 
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

${DIFF_LS} no_exe_perm 2> $NULL
${DIFF_LS} no_exe_perm -l 2> $NULL
${DIFF_LS} no_perm 2> $NULL
${DIFF_LS} no_perm -l 2> $NULL


echo Test Exit code 2
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
${LS} -R -l no_exe_perm . > "${NULL}" 2> "${NULL}"; echo Exit code $?
${LS} -R no_exe_perm . > "${NULL}" 2> "${NULL}"; echo Exit code $?
${LS} -l no_exe_perm . > "${NULL}" 2> "${NULL}"; echo Exit code $?
${LS} no_exe_perm . > "${NULL}" 2> "${NULL}"; echo Exit code $?

echo Test Exit code 0
${LS} -R -l Makefile > "${NULL}" 2> "${NULL}"; echo Exit code $?
${LS} -R Makefile > "${NULL}" 2> "${NULL}"; echo Exit code $?
${LS} -R Makefile > "${NULL}" 2> "${NULL}"; echo Exit code $?
${LS} -R -l Makefile > "${NULL}" 2> "${NULL}"; echo Exit code $?
${LS} -R test > "${NULL}" 2> "${NULL}"; echo Exit code $?
${LS} -Rl test > "${NULL}" 2> "${NULL}"; echo Exit code $?



VAL_OUT="valgrind_output"

VAL="valgrind --leak-check=full --log-file=${VAL_OUT}"

ALL_FLAG="-latrRzucgfdnG"

CHECK="All heap blocks were freed -- no leaks are possible"



echo Diff valgrind test
${VAL} ${LS} ${BASIC_FLAG} ${T_DIR} > "${NULL}" 2> "${NULL}"; cat "${VAL_OUT}" | grep "${CHECK}" && echo Diff1 OK
${VAL} ${LS} ${BASIC_FLAG} ${T_DIR} / > "${NULL}" 2> "${NULL}"; cat "${VAL_OUT}" | grep "${CHECK}" && echo Diff2 OK
${VAL} ${LS} ${BASIC_RECURCE} ${T_DIR} > "${NULL}" 2> "${NULL}"; cat "${VAL_OUT}" | grep "${CHECK}" && echo Diff3 OK
${VAL} ${LS} ${LONG_RECURCE} ${T_DIR} > "${NULL}" 2> "${NULL}"; cat "${VAL_OUT}" | grep "${CHECK}" && echo Diff4 OK
${VAL} ${LS} -lar / /dev > "${NULL}" 2> "${NULL}"; cat "${VAL_OUT}" | grep "${CHECK}" && echo Diff5 OK
# Test mixed argument dir + simple file for l and no l option
${VAL} ${LS} -a / Makefile > "${NULL}" 2> "${NULL}"; cat "${VAL_OUT}" | grep "${CHECK}" && echo Diff6 OK
${VAL} ${LS} -al / Makefile > "${NULL}" 2> "${NULL}"; cat "${VAL_OUT}" | grep "${CHECK}" && echo Diff7 OK
# Test mixed argument dir +  incorect file for l and no l option
${VAL} ${LS} -al / sda > "${NULL}" 2> "${NULL}"; cat "${VAL_OUT}" | grep "${CHECK}" && echo Diff8 OK
${VAL} ${LS} -a / sda  > "${NULL}" 2> "${NULL}"; cat "${VAL_OUT}" | grep "${CHECK}" && echo Diff9 OK


echo Exit 2 valgrind test
${VAL} ${LS} dsa > "${NULL}" 2> "${NULL}"; cat "${VAL_OUT}" | grep "${CHECK}" && echo exit2-1 OK
${VAL} ${LS} dsa / > "${NULL}" 2> "${NULL}"; cat "${VAL_OUT}" | grep "${CHECK}" && echo exit2-2 OK
${VAL} ${LS} dsa Makefile > "${NULL}" 2> "${NULL}"; cat "${VAL_OUT}" | grep "${CHECK}" && echo exit2-3 OK
${VAL} ${LS} dsa Makefile / > "${NULL}" 2> "${NULL}"; cat "${VAL_OUT}" | grep "${CHECK}" && echo exit2-4 OK

${VAL} ${LS} -l dsa / > "${NULL}" 2> "${NULL}"; cat "${VAL_OUT}" | grep "${CHECK}" && echo exit2-5 OK
${VAL} ${LS} > "${NULL}" 2> "${NULL}"; cat "${VAL_OUT}" | grep "${CHECK}" && echo exit2-6 OK
${VAL} ${LS} -l dsa Makefile > "${NULL}" 2> "${NULL}"; cat "${VAL_OUT}" | grep "${CHECK}" && echo exit2-7 OK
${VAL} ${LS} -l dsa Makefile / > "${NULL}" 2> "${NULL}"; cat "${VAL_OUT}" | grep "${CHECK}" && echo exit2-8 OK

${VAL} ${LS} noperm > "${NULL}" 2> "${NULL}"; cat "${VAL_OUT}" | grep "${CHECK}" && echo exit2-9 OK
${VAL} ${LS} noperm / > "${NULL}" 2> "${NULL}"; cat "${VAL_OUT}" | grep "${CHECK}" && echo exit2-10 OK
${VAL} ${LS} noperm Makefile > "${NULL}" 2> "${NULL}"; cat "${VAL_OUT}" | grep "${CHECK}" && echo exit2-11 OK
${VAL} ${LS} noperm Makefile / > "${NULL}" 2> "${NULL}"; cat "${VAL_OUT}" | grep "${CHECK}" && echo exit2-12 OK

${VAL} ${LS} -l noperm > "${NULL}" 2> "${NULL}"; cat "${VAL_OUT}" | grep "${CHECK}" && echo exit2-13 OK
${VAL} ${LS} -l noperm / > "${NULL}" 2> "${NULL}"; cat "${VAL_OUT}" | grep "${CHECK}" && echo exit2-14 OK
${VAL} ${LS} -l noperm Makefile > "${NULL}" 2> "${NULL}"; cat "${VAL_OUT}" | grep "${CHECK}" && echo exit2-15 OK
${VAL} ${LS} -l noperm Makefile / > "${NULL}" 2> "${NULL}"; cat "${VAL_OUT}" | grep "${CHECK}" && echo exit2-16 OK

echo Exit 1 valgrind test
${VAL} ${LS} -R  > "${NULL}" 2> "${NULL}"; cat "${VAL_OUT}" | grep "${CHECK}" && echo exit1-1 OK
${VAL} ${LS} -R . > "${NULL}" 2> "${NULL}"; cat "${VAL_OUT}" | grep "${CHECK}" && echo exit1-2 OK
${VAL} ${LS} -R Makefile . > "${NULL}" 2> "${NULL}"; cat "${VAL_OUT}" | grep "${CHECK}" && echo exit1-3 OK
${VAL} ${LS} -R -l . > "${NULL}" 2> "${NULL}"; cat "${VAL_OUT}" | grep "${CHECK}" && echo exit1-4 OK
${VAL} ${LS} -R -l  > "${NULL}" 2> "${NULL}"; cat "${VAL_OUT}" | grep "${CHECK}" && echo exit1-5 OK
${VAL} ${LS} -R -l Makefile . > "${NULL}" 2> "${NULL}"; cat "${VAL_OUT}" | grep "${CHECK}" && echo exit1-6 OK
${VAL} ${LS} -R  > "${NULL}" 2> "${NULL}"; cat "${VAL_OUT}" | grep "${CHECK}" && echo exit1-7 OK
${VAL} ${LS} -R .  > "${NULL}" 2> "${NULL}"; cat "${VAL_OUT}" | grep "${CHECK}" && echo exit1-8 OK
${VAL} ${LS} -R Makefile .  > "${NULL}" 2> "${NULL}"; cat "${VAL_OUT}" | grep "${CHECK}" && echo exit1-9 OK
${VAL} ${LS} -R -l  > "${NULL}" 2> "${NULL}"; cat "${VAL_OUT}" | grep "${CHECK}" && echo exit1-10 OK
${VAL} ${LS} -R -l .  > "${NULL}" 2> "${NULL}"; cat "${VAL_OUT}" | grep "${CHECK}" && echo exit1-11 OK
${VAL} ${LS} -R -l Makefile .  > "${NULL}" 2> "${NULL}"; cat "${VAL_OUT}" | grep "${CHECK}" && echo exit1-12 OK
${VAL} ${LS} -R -l no_exe_perm .  > "${NULL}" 2> "${NULL}"; cat "${VAL_OUT}" | grep "${CHECK}" && echo exit1-13 OK
${VAL} ${LS} -R no_exe_perm .  > "${NULL}" 2> "${NULL}"; cat "${VAL_OUT}" | grep "${CHECK}" && echo exit1-14 OK
${VAL} ${LS} -l no_exe_perm .  > "${NULL}" 2> "${NULL}"; cat "${VAL_OUT}" | grep "${CHECK}" && echo exit1-15 OK
${VAL} ${LS} no_exe_perm .  > "${NULL}" 2> "${NULL}"; cat "${VAL_OUT}" | grep "${CHECK}" && echo exit1-16 OK


echo Exit 1 valgrind test
${VAL} ${LS} -R -l Makefile > "${NULL}" 2> "${NULL}"; echo exit0-1 OK
${VAL} ${LS} -R Makefile > "${NULL}" 2> "${NULL}"; echo exit0-2 OK
${VAL} ${LS} -R Makefile > "${NULL}" 2> "${NULL}"; echo exit0-3 OK
${VAL} ${LS} -R -l Makefile > "${NULL}" 2> "${NULL}"; echo exit0-4 OK
${VAL} ${LS} -R test > "${NULL}" 2> "${NULL}"; echo exit0-5 OK
${VAL} ${LS} -Rl test > "${NULL}" 2> "${NULL}"; echo exit0-6 OK


echo other valgrind test
${VAL} ${LS} . -la > "${NULL}" 2> "${NULL}"; cat "${VAL_OUT}" | grep "${CHECK}" && echo OK1
${VAL} ${LS} . -lartR > "${NULL}" 2> "${NULL}"; cat "${VAL_OUT}" | grep "${CHECK}" && echo OK2
${VAL} ${LS} . ${ALL_FLAG} > "${NULL}" 2> "${NULL}"; cat "${VAL_OUT}" | grep "${CHECK}" && echo OK3
${VAL} ${LS} / /dev /proc -lart > "${NULL}" 2> "${NULL}"; cat "${VAL_OUT}" | grep "${CHECK}" && echo OK4
${VAL} ${LS} ls test/'"ahah"' . .. rsc/acl libft test/out -d libft/ft_atoi.c > "${NULL}" 2> "${NULL}"; cat "${VAL_OUT}" | grep "${CHECK}" && echo OK5
${VAL} ${LS} -lRza > "${NULL}" 2> "${NULL}"; cat "${VAL_OUT}" | grep "${CHECK}" && echo OK6
${VAL} ${LS} Makefile sda > "${NULL}" 2> "${NULL}"; cat "${VAL_OUT}" | grep "${CHECK}" && echo OK7
${VAL} ${LS} sda > "${NULL}" 2> "${NULL}"; cat "${VAL_OUT}" | grep "${CHECK}" && echo OK8
${VAL} ${LS} sda . > "${NULL}" 2> "${NULL}"; cat "${VAL_OUT}" | grep "${CHECK}" && echo OK9
${VAL} ${LS} sda . / > "${NULL}" 2> "${NULL}"; cat "${VAL_OUT}" | grep "${CHECK}" && echo OK10

rm ${VAL_OUT}
chmod +x no_exe_perm && rm -rf no_exe_perm 