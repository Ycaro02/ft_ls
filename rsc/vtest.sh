#!/bin/bash

VAL_OUT="valgrind_output"

VAL="valgrind --leak-check=full --log-file=${VAL_OUT}"

LS="./ft_ls"

ALL_FLAG="-latrRzucgfdnG"

CHECK="0 bytes in 0 blocks"

${VAL} ${LS} . -la > out && cat "${VAL_OUT}" | grep "${CHECK}"
${VAL} ${LS} . -lartR > out && cat "${VAL_OUT}" | grep "${CHECK}"
${VAL} ${LS} . ${ALL_FLAG} > out && cat "${VAL_OUT}" | grep "${CHECK}"
${VAL} ${LS} / /dev /proc -lart > out && cat "${VAL_OUT}" | grep "${CHECK}"
${VAL} ${LS} ls test/'"ahah"' . .. rsc/acl libft test/out -d libft/ft_atoi.c > out && cat "${VAL_OUT}" | grep "${CHECK}"
${VAL} ${LS} -lRza > out && cat "${VAL_OUT}" | grep "${CHECK}"
${VAL} ${LS} Makefile sda > out && cat "${VAL_OUT}" | grep "${CHECK}"
${VAL} ${LS} sda > out && cat "${VAL_OUT}" | grep "${CHECK}"
${VAL} ${LS} sda . > out && cat "${VAL_OUT}" | grep "${CHECK}"
${VAL} ${LS} sda . / > out && cat "${VAL_OUT}" | grep "${CHECK}"