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


# include <sys/sysmacros.h> // major minor macro for C file
# include <sys/ioctl.h>  // ioctl for manage_column
# include <sys/xattr.h>  // extended attr 
# include "../rsc/acl/acl.h" // classic <sys/acl.h> not present on 42 computer, same for -lacl
# include "basic_define.h"
# include "../libft/libft.h"
# include "../libft/list/linked_list.h"

# include "define_enum.h"
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

/**
 * File structure, contain lot of struct stat field
*/
typedef struct s_file 
{
    t_int8      type;
    mode_t      perm;
    nlink_t     nb_link;
    uid_t       user_id;
    gid_t       group_id;
    t_int64     size;
    t_int64     total_size;
    blkcnt_t    nb_block;
    dev_t       rdev;
    t_timespec  last_status_change;
    t_timespec  last_access;
    t_timespec  last_change;
    char        *name;
    char        *parrent;
    t_int8      quote;
} t_file;



    /* special error for only '-' manage case:
         exit code -2, 
         don't active default search in current dir (".")
         display if another args found (dir or file) 
    */
/**
 * Execution context
 * error : Exit code error 
 *  - 0 for nothing
 *  - 1 for for subdirectory access error
 *  - 2 commande line file access error
 * special_error: Store if special error found in cmd line to adapt display
 *  - for only '-' manage case: 
 *      - if no valid args -> exit code -2, don't active default search in current dir (".")
 *      - else display error and another args normaly
 * flag_nb: Flag enable (ls OPTION) see e_flag in define_enum.h
*/
typedef struct s_context {
    t_int8      error;              /* Exit error code */
    t_int8      special_error;      /* special error to manage -- and special display */
    int         flag_nb;            /* flags value (ls option) */
} t_context;

/** 
 * File context
 * call_flag: Call flag used in hub ls function
 *  - 0 for file display
 *  - 1 for dir without file before
 *  - 2 for dir with file or another dir before
 * idx: Index of file in lst
 * lst_len: File's list len
*/
typedef struct s_file_context {
    int         call_flag;
    int         idx;
    int         lst_len;
    // int *space;
} t_file_context;

/**
 * Buffer to avoid multiple useless call of write
*/
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

//-------------------------------
//      t_file.c               //
//-------------------------------
t_file      *fill_file_struct(struct stat *sb, char *path, char *parent, int symlink_bool);
char        *perm_to_string(mode_t mode, char type);
int         check_for_quote(char *str);
//-------------------------------
//      utils.c                 //
//-------------------------------
void        file_lstclear(t_list **lst, void (*del)(void*)); /* can be remove with create free file */
int         ft_strlen_word(char *s);
int         get_stdout_width();
void        update_error(t_int8 *error);
int         is_point_dir(char *path, int flag_nb, int display);
char        *join_parent_name(char* parent_name, char* path);
//-------------------------------
//      flag_gestion.c          //
//-------------------------------
int         parse_flag(int argc, char **argv, t_int8 *special_err);
//-------------------------------
//      parse.c                //
//-------------------------------
t_list      *get_all_file_struct(t_file *file, int flag_nb, t_int8 *error);
t_list      *get_dir_args(char **argv, t_list **simple_file, t_int8 *args_found, t_context *c);
//-------------------------------
//      ft_ls.c                //
//-------------------------------
int         ls_only_file_L(t_list *lst, int flag_nb);
void        display_quote(int quote);
int         ls_l_one_dir(t_file *file, t_context *c, t_file_context *file_c);
int         ls_one_dir(t_file *file, t_context *c, t_file_context *file_c);
//-------------------------------
//      recurcive.c            //
//-------------------------------
int         search_recurcive_dir(t_list *dir_lst, t_context *c, int call_flag);
//-------------------------------
//      l_options.c            //
//-------------------------------
int         fill_buffer_l_option(t_file file, int* space, int nb_flag);
void        write_user_name(long user_id, int space, int flag_nb);
void        write_group_name(long group_id, int space, int flag_nb);
//-------------------------------
//      write_file_name.c      //
//-------------------------------
int         write_file_name(t_file file, int flag_nb, int space);
//-------------------------------
//      l_option_utils.c       //
//-------------------------------
void        fill_buffer_perm(char c, int *is_exec, int display_flag);
char        get_type(struct stat sb);
//-------------------------------
//      manage_space.c          //
//-------------------------------
int         get_nb_space(t_list *lst, int(*get_len_by_info)(t_file));
int         get_len_name_quote(t_file file);
int		    *get_all_space(t_list *lst, int flag_nb);
//-------------------------------
//      time_gestion.c          //
//-------------------------------
char        **get_printable_date(t_timespec last_change);
//-------------------------------
//      sort.c                  //
//-------------------------------
void        sort_lst(t_list **lst, int flag_nb);
void        safe_reverse_lst(t_list **lst, int flag_nb);
int         is_special_char(char c);
//-------------------------------
//      buffer.c                //
//-------------------------------
void        fill_buffer(char *str);
int         fill_l_buffer(t_list *lst, int flag_nb, int call_flag);
void        fill_buffer_char(char c);
void        fill_buffer_color(char *str, enum e_color color, int flag_nb, int space, int quote);
int         store_in_buffer(t_list *lst, int flag_nb);
void        print_and_clear();
void        finish_print_buffer();
void        multiple_fill_buff(char *s1, char*s2, char *s3, char *s4);
void        fill_color(enum e_color color);
//-------------------------------
//      build_coloumn.c         //
//-------------------------------
int         manage_column(t_list *lst, int space_quote, int flag);
//-------------------------------
//      manage_column.c         //
//-------------------------------
int         *get_max_by_column(t_list *lst, int nb_column, int nb_line);
int         **create_column_array (t_list *lst, int nb_column_max, int nb_line);
//-------------------------------
//      list_xattr.c           //
//-------------------------------
int         diplay_xattr_acl(t_file *file);
int         list_xattr(char *path, char *list);
int         check_lst_acl(t_list *lst);
int         check_acl(t_file *file);
//-------------------------------
//      manage_perm.c           //
//-------------------------------
char        *perm_to_string(mode_t mode, char type);
t_int8      is_full_perm(mode_t mode);
t_int8      is_executable_file(mode_t mode);

//-------------------------------
//    Basic flag gestion.c     //
//-------------------------------
void        display_flags(int flags);
void        set_flag(int *flags, int flag_val);
void        unset_flag(int *flags, int flag_val);
t_int8      has_flag(int flags, int flag_val);
t_int8      flag_already_present(int flags, int flag_val);
// main.c to move
struct stat *check_for_stat(char* name, int flag, int *save);
int         quotes_required(char *str);

#endif /* FT_LS_H */