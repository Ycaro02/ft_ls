#ifndef FT_LS_H
# define FT_LS_H

# include <stdlib.h>
# include <unistd.h>
# include <dirent.h>
# include <string.h>
# include <grp.h>
# include <pwd.h>
# include <time.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <stdio.h> // perror
# include "../libft/libft.h"
# include "define_enum.h"

# include <sys/ioctl.h>


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
// refactor str_trim_pattern //

// utils.c
int         get_stdout_width();
void        update_error(int *error);
int         print_error(char *msg, char* str, int error_type, int use_perror);
int         get_lst_len(t_list *lst);
int         is_point_dir(char *path, int flag_nb);
int         last_char_is_slash(char *str);
void	    new_lstclear(t_list **lst, void (*del)(void*));
char        *join_parent_name(char* parent_name, char* path);
void        display_file_lst(t_list *lst);

// flag_gestion.c
int         get_flag(enum e_flag *flag);
enum e_flag *check_for_flag(int argc, char **argv);

// parse.c
t_list      *get_all_file_name(const char *directory_name, int flag_nb);
t_list      *get_all_file_struct(t_file *file, int flag_nb, int* error);
t_list      *get_dir_args(char **argv, int *error);

// ft_ls.c
int        ls_l_one_dir(t_file *file, int flag_nb, int lst_len, int* error);
int        ls_one_dir(t_file *file, int flag_nb, int lst_len, int* error);

// recurcive.c
int        search_recurcive_dir(t_list *dir_lst, int flag_nb, int *error);

// l_options.c
char        get_type(struct stat sb);
t_file      *fill_file_struct(struct stat sb, char *path, char* parent);
int         fill_buffer_l_option(t_file file, int* space);
int         write_file_name(t_file file, int is_exec, int option);
char        *get_perm(int nbr);
void        insert_space(int nb);

// manage_space.c
int			*get_all_space(t_list *lst);
int         get_nb_space(t_list *lst, int(*get_len_info)(t_file));

// time gestion.c
char        **get_printable_date(time_t *time);

// sort.c
void        sort_lst(t_list *lst, int flag_nb);
void        free_node_ptr(t_list **lst);
int         safe_reverse_lst(t_list **lst,  int* error);


// buffer.c
void        fill_buffer(char *str);
int         fill_l_buffer(t_list *lst);
void        fill_buffer_char(char c);
void        fill_buffer_color(char *str, enum e_color color);
int         store_in_buffer(t_list *lst, int flag_nb);
void        print_and_clear();
void        finish_print_buffer();


// manage_column.c
int         fill_buffer_with_column(char **tab, int nb_raw, t_list **lst);
char        **check_manage_colum(t_list *lst, int *err, int *value);

#endif