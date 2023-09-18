#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include <grp.h>
#include <pwd.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "src/libft/libft.h"

#define     BUFFER_LEN 10000
#define     PRINT_SIZE 8000

#define     L_FLAG_CHAR 'l'
#define     R_FLAG_CHAR 'R'
#define     REVERSE_FLAG_CHAR 'r'
#define     A_FLAG_CHAR 'a'
#define     T_FLAG_CHAR 't'

#define     BLOCK        'b'
#define     CHARACTER    'c'
#define     DIRECTORY    'd'
#define     FIFO         'p'
#define     SYMLINK      'l'
#define     REGULAR      '-'
#define     SOCKET       's'
#define     UNDIFINED    '?'

#define NEW 0
#define OLD 1

enum e_flag  {
    UNKNOW=0,
    L_OPTION=1,
    A_OPTION=2,
    T_OPTION=4,
    REVERSE_OPTION=8,
    R_OPTION=16,
};

typedef struct s_file 
{
    char        type;
    int         perm;
    int         nb_link;
    long        user_id;
    long        group_id;
    long long   size;
    long long   total_size;
    long long   nb_block;
    time_t      last_change;
    long        n_time;
    char        *name;
} t_file;

typedef struct s_buff
{
	char		buffer[BUFFER_LEN];
    int	        i;
}	t_buff;

extern t_buff g_buff;

// TODO -t :
//  -- fix don't list empty dir

//  -sort function by change_time (newer first)
// Check flag order ()
//      - a : ( done ) ( toBeChecked )
//      - t : ( done ), need to check 6 month 
//      - r : ( done )
//      - R : ( done )
//      - l :  ( done )

/////////////////////////////
#include <stdio.h>///////////
/////////////////////////////

// utils.c
int         is_point_dir(char *path, int flag_nb);
char        *ft_strjoin_free(char *s1, char *s2, char option);
int         lower_strcmp(char *s1, char *s2);
char         is_directory(const char *path);
int         last_char_is_slash(char *str);
void	    new_lstclear(t_list **lst, void (*del)(void*));
char        *join_parent_name(char* parent_name, char* path);
void display_file_lst(t_list *lst);

// flag_gestion.c
int         get_flag(enum e_flag *flag);
enum e_flag *check_for_flag(int argc, char **argv);

// parse.c
t_list      *get_all_file_name(const char *directory_name, int flag_nb);
t_list      *get_dir_args(char **argv);
t_list      *get_all_file_struct(t_file *file, int flag_nb);

// ft_ls.c
void        ls_l_one_dir(t_file *file, int flag_nb);
void        ls_one_dir(t_file *file, int flag_nb);

// recurcive.c
void        search_recurcive_dir(t_list *dir_lst, int flag_nb);

// l_options.c
char        get_type(struct stat sb);
t_file      *fill_file_struct(struct stat sb, char *path);
void        fill_buffer_l_option(t_file file, int* space);
void        display_file_struct(t_file file);
int *get_all_space(t_list *lst);



// time gestion.c
char        *get_printable_date(time_t *time);

// sort.c
void        sort_lst(t_list *lst, int flag_nb);
void        free_node_ptr(t_list **lst);
void        reverse_lst(t_list *lst, t_list **new);

// buffer.c
void        print_and_clear();
void        fill_buffer(char *str);
void        fill_buffer_char(char c);
void        store_in_buffer(t_list *lst, int flag_nb);
void        finish_print_buffer();
