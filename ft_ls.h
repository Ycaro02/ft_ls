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

# define RED "\033[1;31m"
# define GREEN "\033[1;32m"
# define YELLOW "\033[1;33m"
# define BLUE "\033[1;34m"
# define PURPLE "\033[1;35m"
# define CYAN "\033[1;36m"
# define RESET "\033[0m"

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

enum e_color  {
    E_RED,
    E_GREEN,
    E_YELLOW,
    E_BLUE,
    E_PURPLE,
    E_CYAN,
};

typedef struct s_file 
{
    char        type;
    int         perm;
    nlink_t     nb_link;
    uid_t       user_id;
    gid_t       group_id;
    off_t       size;
    off_t       total_size;
    blkcnt_t    nb_block;
    time_t      last_change;
    char        *name;
    char        *parrent;
} t_file;

typedef struct s_buff
{
	char		buffer[BUFFER_LEN];
    int	        i;
}	t_buff;

extern t_buff g_buff;

// TODO -t :
//  -- fix don't list empty dir
// for get space -l need to separate date date with 3 separate colum and count for each
// refactor str_trim_pattern

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
int         get_lst_len(t_list *lst);
int         is_point_dir(char *path, int flag_nb);
char        *ft_strjoin_free(char *s1, char *s2, char option);
int         lower_strcmp(char *s1, char *s2);
char         is_directory(const char *path);
int         last_char_is_slash(char *str);
void	    new_lstclear(t_list **lst, void (*del)(void*));
char        *join_parent_name(char* parent_name, char* path);
void        display_file_lst(t_list *lst);

// flag_gestion.c
int         get_flag(enum e_flag *flag);
enum e_flag *check_for_flag(int argc, char **argv);

// parse.c
t_list      *get_all_file_name(const char *directory_name, int flag_nb);
t_list      *get_dir_args(char **argv);
t_list      *get_all_file_struct(t_file *file, int flag_nb);

// ft_ls.c
void        ls_l_one_dir(t_file *file, int flag_nb, int lst_len);
void        ls_one_dir(t_file *file, int flag_nb, int lst_len);

// recurcive.c
void        search_recurcive_dir(t_list *dir_lst, int flag_nb);

// l_options.c
char        get_type(struct stat sb);
t_file      *fill_file_struct(struct stat sb, char *path, char* parent);
void        fill_buffer_l_option(t_file file, int* space);
void        display_file_struct(t_file file);
int			*get_all_space(t_list *lst);
void        write_file_name(t_file file, int is_exec, int option);


// time gestion.c
char        *get_printable_date(time_t *time);

// sort.c
void        sort_lst(t_list *lst, int flag_nb);
void        free_node_ptr(t_list **lst);
void        reverse_lst(t_list *lst, t_list **new);

// buffer.c
void        fill_buffer(char *str);
void        fill_buffer_char(char c);
void        store_in_buffer(t_list *lst, int flag_nb);
void        print_and_clear();
void        finish_print_buffer();
void        fill_buffer_color(char *str, enum e_color color);
