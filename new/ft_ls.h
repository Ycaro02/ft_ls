#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <stdio.h>
#include "src/libft/libft.h"
#include <stdio.h>
#include <string.h>

#include <sys/types.h>
#include <sys/stat.h>

#define L_FLAG_CHAR 'l'
#define R_FLAG_CHAR 'R'
#define REVERSE_FLAG_CHAR 'r'
#define A_FLAG_CHAR 'a'
#define T_FLAG_CHAR 't'

enum e_flag  {
    UNKNOW=0,
    L_OPTION=1,
    A_OPTION=2,
    T_OPTION=4,
    REVERSE_OPTION=8,
    R_OPTION=16,
};

/// TODO -t : time, -a : hiden_file, -l: all info
//// Remplace all of possible char** array by linked list

// utils.c
int         is_point_dir(char *path);
char        *ft_strjoin_free(char *s1, char *s2, char option);
void        free_lst(t_list *lst);
int         lower_strcmp(char *s1, char *s2);
int         is_directory(const char *path);

//flag_gestion.c
int         get_flag(enum e_flag *flag);
enum e_flag *check_for_flag(char **argv);

//parse.c
t_list      *get_all_file_name(const char *directory_name, int hiden_file);
t_list      *get_dir_no_hiden(char **argv);

// ft_ls.c
void        ls_no_args();
void        ls_one_dir(char *str, int flag_nb);

//recurcive.c
void        search_recurcive_dir(t_list *dir_lst, int flag_nb);
void        sort_by_name(t_list *lst);