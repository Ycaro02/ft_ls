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
    UNKNOW=-1,
    L_OPTION=1,
    A_OPTION=4,
    T_OPTION=10,
    REVERSE_OPTION=20,
    R_OPTION=50,
};

// utils.c
void        free_all(char **array);
void        print_tab(char **tab);
char        **ft_realloc_str(char **strs, char *str);
int         count_char_tab(char **tab);
int         already_use(char *str, char** used);
int      is_directory(const char *path);
char    *ft_strjoin_free(char *s1, char *s2, char option);
int my_strcmp(char *s1, char *s2);


//flag_gestion.c
int         already_add(enum e_flag *tab, enum e_flag to_check);
void        fill_used_flag(enum e_flag *tab, enum e_flag flag);
int         get_flag(enum e_flag *flag);
int         check_flag(char c, enum e_flag *used);
int         add_flag(char c, enum e_flag *used);
enum e_flag *parse_flag(char **argv, enum e_flag *used);

//parse.c
char        **get_all_file_name(const char *directory_name);
char        **get_dir(char **argv);
char        *get_lower_string(char **tab, char **used);
char    **parse_dir_lst(char **dir_lst);

// ft_ls.c
void        classic_ls(char **tab);
void        reverse_ls(char **tab);
void        ls_no_args();
void        ft_list_dir(char **tab, int flag_nb);
void        ft_ls(char **dir_lst, int flag_nb, int);
void        ls_one_dir(char *str, int flag_nb);


//recurcive.c
void        search_recurcive_dir(char **dir_lst, int flag_nb, int first);