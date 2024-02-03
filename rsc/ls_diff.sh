#!/bin/bash

export LC_COLLATE=en_US.utf8 && ls ${1} ${2} | tr -d "\'\" " > real_ls_out
./ft_ls ${1} ${2} | tr -d "\'\" " > myls_out
diff myls_out real_ls_out
echo $?
