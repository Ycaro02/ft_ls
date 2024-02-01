#ifndef FT_LS_H
# define FT_LS_H

# include <unistd.h>
# include <stdlib.h>
# include <stdio.h> // perror
# include <dirent.h>
# include <grp.h>
# include <pwd.h>
# include <time.h>
# include <sys/types.h>
# include <sys/stat.h>



// # define MINORBITS 8
// # define MINORMASK ((1U << MINORBITS) - 1)
// # define MAJOR(dev) ((unsigned int)(dev >> MINORBITS))
// # define MINOR(dev) ((unsigned int)(dev & MINORMASK))
// # define MINOR(dev) (dev & ) 
#include <sys/sysmacros.h> // major minor macro for C file

# include <sys/ioctl.h>  // ioctl for manage_column
# include <sys/xattr.h>  // extended attr 
# include "../acl/acl.h" // classic <sys/acl.h> not present on 42 computer, same for -lacl
# include "../libft/libft.h"

# include "define_enum.h"
# include "basic_define.h"
// # include <sys/acl.h> 


# define TOKEN_NO_CASE_SENSITIVE 0
# define TOKEN_CHECK_SPE_CHAR 1

/*
    Parse rule

If char in string -> simple quote:
---------------------------------------
    ! $ ^ & * ( ) = < > ? ; [ ] ` ~
---------------------------------------
 
 Cant use '/' exclusif for directory
Special char :

Simple quote add double quotes
 ' : add ""

Double quote add simple quotes
 " : add ''

 BRACKET_CHAR :
    - alway simple quote when alone { || }
    - remove simple quote whenn add any char include { || }: 
        - exemple: {{ : no simple quote != ## : simple quote
DIEZE_CHAR :
    - simple quote only when at idx 0
*/

enum special_char_e {
    NORMAL_CHAR,                // all other char
    NOEFFECT_CHAR,              // @ % - _ + . , : 
    BRACKET_CHAR,               // { }
    DIEZE_CHAR,                 // #
    ADD_SIMPLE_QUOTE_CHAR,      // ! $ ^ & * ( ) = < > ? ; [ ] ` ~ "
    ADD_DOUBLE_QUOTE_CHAR,      // '
};

//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//                                                                            //
//                                  STRUCT                                    //
//                                                                            //
//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

typedef struct timespec t_timespec;
typedef enum e_flag t_eflag;

typedef struct s_file 
{
    char        type;
    int         perm;
    nlink_t     nb_link;
    uid_t       user_id;
    gid_t       group_id;
    long        size;
    long        total_size;
    blkcnt_t    nb_block;
    dev_t       rdev;
    t_timespec  last_status_change;
    t_timespec  last_access;
    t_timespec  last_change;
    char        *name;
    char        *parrent;
    int         quote;
} t_file;

typedef struct s_buff
{
	char		buffer[BUFFER_LEN];
    int	        i;
}	t_buff;

//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//                                                                            //
//                                FUNCTION                                    //
//                                                                            //
//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

extern t_buff g_buff; // GLOBAL VARIABLE buffer 

int check_for_quote(char *str);
struct stat *check_for_stat(char* name, int flag, int *save);

//flag_gestion
void set_flag(int *flags, int flag_val);
void unset_flag(int *flags, int flag_val);
void flip_flag(int *flags, int flag_val);
int  has_flag(int flags, int flag_val);
int  has_any_flag(int flags, int flag_val);

int parse_flag(int argc, char **argv);

// main.c to move
int quotes_required(char *str);

//-------------------------------
//      t_file.c               //
//-------------------------------
t_file *fill_file_struct(struct stat *sb, char *path, char *parent, int symlink_bool);
//-------------------------------
//      utils.c                 //
//-------------------------------
void display_error_phrase(char *str);
int     ft_strlen_word(char *s);
int     check_file_perm(int perm, int to_check);
int     get_stdout_width();
void    update_error(int *error);
int     print_error(char *msg, char* str, int error_type, int use_perror);
int     get_lst_len(t_list *lst);
int     last_char_is_slash(char *str);
void    new_lstclear(t_list **lst, void (*del)(void*));
int     is_point_dir(char *path, int flag_nb, int display);
char    *join_parent_name(char* parent_name, char* path);
void    display_file_lst(t_list *lst);
//-------------------------------
//      flag_gestion.c          //
//-------------------------------
int     get_flag(enum e_flag *flag);
/* refactor in progress */
// t_eflag *check_for_flag(int argc, char **argv);
//-------------------------------
//      parse.c                //
//-------------------------------
t_list  *get_all_file_struct(t_file *file, int flag_nb, int* error);
t_list *get_dir_args(char **argv, int *error, int flag_nb, t_list **simple_file);
//-------------------------------
//      ft_ls.c                //
//-------------------------------
int     ls_only_file_L(t_list *lst, int flag_nb);
int     ls_l_one_dir(t_file *file, int flag_nb, int lst_len, int* error, int call_flag, int index);
int     ls_one_dir(t_file *file, int flag_nb, int lst_len, int* error, int call_flag, int index);
void    display_quote(int quote);
//-------------------------------
//      recurcive.c            //
//-------------------------------
int search_recurcive_dir(t_list *dir_lst, int flag_nb, int *error, int call_flag);
//-------------------------------
//      l_options.c            //
//-------------------------------
void    insert_space(int nb);
int     fill_buffer_l_option(t_file file, int* space, int nb_flag);
int     write_file_name(t_file file, int is_exec, int flag_nb, int space);
void    write_user_name(long user_id, int space, int flag_nb);
void    write_group_name(long group_id, int space, int flag_nb);
//-------------------------------
//      l_option_utils.c       //
//-------------------------------
char    *get_perm(int nbr);
void    fill_buffer_perm(char c, int *is_exec, int display_flag);
char    get_type(struct stat sb);
//-------------------------------
//      manage_space.c          //
//-------------------------------
int get_nb_space(t_list *lst, int(*get_len_by_info)(t_file));
int get_len_name_quote(t_file file);
int		*get_all_space(t_list *lst, int flag_nb);
//-------------------------------
//      time_gestion.c          //
//-------------------------------
char    **get_printable_date(t_timespec last_change);
//-------------------------------
//      sort.c                  //
//-------------------------------
void    sort_lst(t_list *lst, int flag_nb);
void    free_node_ptr(t_list **lst);
int     safe_reverse_lst(t_list **lst,  int* error, int flag_nb);
int     is_special_char(char c);
//-------------------------------
//      buffer.c                //
//-------------------------------
void    fill_buffer(char *str);
int     fill_l_buffer(t_list *lst, int flag_nb, int call_flag);
void    fill_buffer_char(char c);
void    fill_buffer_color(char *str, enum e_color color, int flag_nb);
int     store_in_buffer(t_list *lst, int flag_nb);
void    print_and_clear();
void    finish_print_buffer();
void    multiple_fill_buff(char *s1, char*s2, char *s3, char *s4);
void    fill_color(enum e_color color);
//-------------------------------
//      build_coloumn.c         //
//-------------------------------
int manage_basic_column(t_list *lst, int space_quote, int flag);
//-------------------------------
//      manage_column.c         //
//-------------------------------
int    fill_buffer_with_column(char **tab, int nb_line, t_list **lst, int flag_nb, int quote_space);
char **manage_column(t_list *lst, int *max_unit_len, int max_per_line, int nb_line, int space_quote);
int *get_max_by_column(t_list *lst, int nb_column, int nb_line);

int **create_column_array (t_list *lst, int nb_column_max, int nb_line);
//-------------------------------
//      list_xattr.c           //
//-------------------------------
int    diplay_xattr_acl(t_file *file);
char   *get_new_path(t_file *file);
int     list_xattr(char *path, char *list);
int     check_lst_acl(t_list *lst);
int     check_acl(t_file *file);

#endif /* FT_LS_H */