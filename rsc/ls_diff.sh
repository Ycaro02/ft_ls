#!/bin/bash

export LC_COLLATE=en_US.utf8 && /bin/ls ${1} ${2} ${3} | tr -d "\'\" " > real_ls_out
./ft_ls ${1} ${2} ${3} | tr -d "\'\" " > myls_out
diff myls_out real_ls_out
echo Diff for ${1} ${2} ${3} : $?