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

#define BUFFER_LEN 10000
#define PRINT_SIZE 8000

enum e_flag  {
    UNKNOW=0,
    L_OPTION=1,
    A_OPTION=2,
    T_OPTION=4,
    REVERSE_OPTION=8,
    R_OPTION=16,
};

typedef struct s_buff
{
	char			buffer[BUFFER_LEN];
	int	            i;
}	t_buff;

extern t_buff g_buff;

// TODO -t : time(last change), -l: all info : 
// For -l:
// -total weight of dir :                       add all sb.st_size
// - first char : type :                        like dir -> detect type.c
// - perm : owner:samegroup:another :           ft_putnbr_base(sb.st_mode & 0777, "01234567"); 
// - number link stat:                          sb.st_nlink
// - owner name :                               getpwuid:struct user, (long) sb.st_uid, 
// - owner groups ?? :                          getgrgid:groupe_id struct, (long) sb.st_gid); 
// - file weight :                              sb.st_size
// - time ( in working )
//          -detect if last change time is more than 6 month
//          -change time display if file's last change is 6 month or greather, done in time_gestion.c 
// - name (done)

// For -t:
//  -sort function by change_time (newer first)
// Check flag order ()
//      - a : ( done ) ( toBeChecked )
//      - t : update sort logic order, check flag -> alpha or time sort -> check reverse 
//      - r : ( done )
//      - R : ( done )
//      - l : in working

// utils.c
int         is_point_dir(char *path, int flag_nb);
char        *ft_strjoin_free(char *s1, char *s2, char option);
int         lower_strcmp(char *s1, char *s2);
int         is_directory(const char *path);

//flag_gestion.c
int         get_flag(enum e_flag *flag);
enum e_flag *check_for_flag(char **argv);

//parse.c
t_list      *get_all_file_name(const char *directory_name, int flag_nb);
t_list      *get_dir_args(char **argv);

// ft_ls.c
void        reverse_lst(t_list *lst, t_list** new);
void        ls_no_args(int);
void        ls_one_dir(char *str, int flag_nb);

//recurcive.c
void        search_recurcive_dir(t_list *dir_lst, int flag_nb);
void        sort_by_name(t_list *lst);