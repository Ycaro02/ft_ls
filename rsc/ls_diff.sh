#!/bin/bash

export LC_COLLATE=en_US.utf8 && ls ${1} ${2} ${3} | tr -d "\'\" " > real_ls_out
./ft_ls ${1} ${2} ${3} | tr -d "\'\" " > myls_out
diff myls_out real_ls_out
echo $?

# possible to grep -v ls_out to avoid size issue but not realy clean and don't remove total size line for -l
# ./ft_ls ${1} ${2} | tr -d "\'\" " > myls_out