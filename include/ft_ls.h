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
// # include <sys/acl.h> 

# include "basic_define.h"
# include "../libft/libft.h"
# include "../libft/list/linked_list.h"
# include "ft_ls_structs.h" /* include <time.h> */

# include "define_enum.h"
//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//                                                                            //
//                                FUNCTION                                    //
//                                                                            //
//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

extern t_buff g_buff; // GLOBAL VARIABLE buffer 


//-------------------------------
//      t_file.c               //
//-------------------------------
t_file      *fill_file_struct(struct stat *sb, int symlink, t_context *c, t_file_context *file_c);
void        destroy_file(void *file_ptr);
//-------------------------------
//      utils.c                 //
//-------------------------------
int         ft_strlen_word(char *s);
int         get_stdout_width();
void        update_error(t_int8 *error);
int         is_point_dir(char *path, int flag_nb, int display);
char        *join_parent_name(char* parent_name, char* path);
struct stat *check_for_stat(char* name, int flag, int *save);
//-------------------------------
//      flag_gestion.c          //
//-------------------------------
int         parse_flag(int argc, char **argv, t_int8 *special_err);
//-------------------------------
//      parse.c                //
//-------------------------------
t_list*     get_all_file_struct(t_file *file, t_context *c, t_file_context *file_c);
t_int8      parse_cmd_args(char **argv, t_args *arg);
char        get_type(struct stat sb);

//-------------------------------
//      ft_ls.c		           //
//-------------------------------
int 		ft_ls(char **argv, t_context *c);
//-------------------------------
//      ls_function.c           //
//-------------------------------
int         ls_only_file(t_list *lst, t_context *c, t_file_context *file_c);
int         ls_only_file_l(t_list *lst, t_context *c, t_file_context *file_c);
int         ls_l_one_dir(t_file *file, t_context *c, t_file_context *file_c);
int         ls_one_dir(t_file *file, t_context *c, t_file_context *file_c);
//-------------------------------
//      recurcive.c            //
//-------------------------------
int         search_recurcive_dir(t_list *dir_lst, t_context *c, int call_flag);
//-------------------------------
//      l_options.c            //
//-------------------------------
int         fill_buffer_l_option(t_file *file, t_context *c, t_file_context *file_c);
//-------------------------------
//      write_file_name.c      //
//-------------------------------
int         write_file_name(t_file *file, t_context *c, t_file_context *file_c, int space);
//-------------------------------
//      time_gestion.c          //
//-------------------------------
char        **get_printable_date(t_timespec last_change);
//-------------------------------
//      sort.c                  //
//-------------------------------
void        sort_lst(t_list **lst, int flag_nb);
void        safe_reverse_lst(t_list **lst, int flag_nb);
//-------------------------------
//      buffer.c                //
//-------------------------------
void        fill_buffer(char *str);
int         fill_l_buffer(t_list *lst, t_context *c, t_file_context *file_c);
void        fill_buffer_char(char c);
void        fill_buffer_color(char *str, enum e_color color, int flag_nb, int space, int quote);
int         store_in_buffer(t_list *lst, t_context *c, t_file_context *file_c);
void        finish_print_buffer();
void        multiple_fill_buff(char *s1, char*s2, char *s3, char *s4);
//-------------------------------
//      build_coloumn.c         //
//-------------------------------
int         manage_column(t_list *lst, int space_quote, t_context *c, t_file_context *file_c);
int         *get_max_by_column(t_list *lst, int nb_column, int nb_line);
//-------------------------------
//      manage_column.c         //
//-------------------------------
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
int8_t 		has_flag(int flags, int flag_val);
t_int8      flag_already_present(int flags, int flag_val);
//-------------------------------
//    quote_gestion.c           //
//-------------------------------
int         is_special_char(char c);
int         quotes_required(char *str);
int         check_for_quote(char *str);
void        display_quote(t_int8 quote);
/* DEBUG */
void		display_fcontext_flag(t_file_context *file_c, char *str, int flag);
//-------------------------------
//    File line .c             //
//-------------------------------
int         build_file_line(t_file *file, t_context *c, t_file_context *file_c, int symlink);

#endif /* FT_LS_H */