#!/bin/bash

VAL_OUT="valgrind_output"

VAL="valgrind --leak-check=full --log-file=${VAL_OUT}"

LS="./ft_ls"

ALL_FLAG="-latrRzucgfdnG"

CHECK="All heap blocks were freed -- no leaks are possible"

NULL="/dev/null"


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
echo VALGRIND CHECK DONE