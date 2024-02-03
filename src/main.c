#include "../include/ft_ls.h"

t_buff g_buff;

// int ls(t_list *lst, int flag_nb,  int (*ls_function)(t_file*, int, int, t_int8*, int, int), t_int8* error, int call_flag)
int ls(t_list *lst, t_context *c, int (*ls_function)(t_file*, t_context*, t_file_context*), int call_flag)
{
    if (!lst)
        return (42);
    t_list *current = lst;
    int err = 0;
    t_file_context file_c;
    file_c.idx = 0;
    file_c.lst_len = ft_lstsize(lst);
    file_c.call_flag = call_flag;
    
    while (current) {
        err = ls_function(current->content, c, &file_c);
        ++(file_c.idx);
        if (err == MALLOC_ERR)
            break ;
        current = current->next;
    }
    return (err);
}

static int ls_only_dir(t_list *dir_lst, int flag_nb)
{
    t_list  *current = dir_lst;
    int     *space = get_all_space(dir_lst, flag_nb), err = 0;
    int     lst_len = ft_lstsize(dir_lst), i = 0;

    while (current) {
        err = fill_buffer_l_option(*(t_file *)current->content, space, flag_nb);
        if (err == MALLOC_ERR) {
            break ;
        }
        ++i;
        if (i != lst_len)
            fill_buffer("\n");
        current = current->next;
    }
    free(space);
    return (err);
}


// static void call_ls(t_list *dir_lst, int flag_nb, t_int8 *error, int call_flag)
static void call_ls(t_list *dir_lst, t_context *c, int call_flag)
{
    int err = 0;

    if (call_flag != 0 && has_flag(c->flag_nb, R_OPTION) && !has_flag(c->flag_nb, D_OPTION))
        err = search_recurcive_dir(dir_lst, c, call_flag);
    
    
    else if (call_flag != 0 && has_flag(c->flag_nb, L_OPTION) && has_flag(c->flag_nb, D_OPTION))
        err = ls_only_dir(dir_lst, c->flag_nb);
    else if (has_flag(c->flag_nb, L_OPTION) && call_flag != 0)
        err = ls(dir_lst, c, ls_l_one_dir, call_flag); /* HERE */
    else if (has_flag(c->flag_nb, L_OPTION) && call_flag == 0)
        err = ls_only_file_L(dir_lst, c->flag_nb);
    else
        err = ls(dir_lst, c, ls_one_dir, call_flag); /* HERE */

    file_lstclear(&dir_lst, free);
    if (err == MALLOC_ERR) {
        ft_printf_fd(2, "Malloc error exit\n");
        exit(MALLOC_ERR);
    }
}

inline static void basic_sort_lst(t_list **lst, int flag, t_int8 *error)
{
    sort_lst(*lst, flag);
    if (has_flag(flag , REVERSE_OPTION))
        safe_reverse_lst(lst, error, flag);
}

// static int ft_ls(char **argv, int flag_nb, t_int8* error, t_int8 special_err)
static int ft_ls(char **argv, t_context *c)
{
    t_list  *dir_lst, *simple_file = NULL;
    int     call_value = 0;
    t_int8  args_found = c->special_error;
    
    // dir_lst = get_dir_args(&argv[1], &c->error, c->flag_nb, &simple_file, &args_found);
    dir_lst = get_dir_args(&argv[1], &simple_file, &args_found, c);
    
    /* Error management */
    if (!dir_lst && c->error == MALLOC_ERR) {
        ft_printf_fd (2, "Malloc Error ft_ls\n");
        return (MALLOC_ERR);
    }

    if (dir_lst) {
        if (has_flag(c->flag_nb, D_OPTION)) {
            t_list *new = NULL;
            new = ft_lstjoin(dir_lst, simple_file);
            if (new) {
                basic_sort_lst(&new, c->flag_nb, &c->error);
                call_ls(new, c, call_value);
                return (c->error);
            }
        }
        basic_sort_lst(&dir_lst, c->flag_nb, &c->error);
    }

    if (simple_file){
        basic_sort_lst(&simple_file, c->flag_nb, &c->error);
        call_ls(simple_file, c, call_value);
        ++call_value;
        if (has_flag(c->flag_nb, L_OPTION) && dir_lst)
            fill_buffer("\n");
    }

    /*  display new band before ls call if:
        args found (invalid args),
        no simple file found (call value == 0)
        lst_size != 1 */
    if (dir_lst){
        if (args_found && call_value == 0 && ft_lstsize(dir_lst) == 1) {
            t_file *file = dir_lst->content;
            int quote = quotes_required(file->name);
            if (quote > NOEFFECT_CHAR)
                display_quote(quote);
            fill_buffer(file->name);
            if (quote > NOEFFECT_CHAR)
                display_quote(quote);
            fill_buffer(":\n");
        }
        call_ls(dir_lst, c, call_value + 1);
    }

    if (c->special_error == 1) /* set return cmd value for special err */
        c->error = 2;
    
    return (c->error);
}

static int check_display_help(int argc, char**argv)
{
    if (argc >= 2) {
        if (ft_strcmp(argv[1], "--help") == 0) {
            ft_printf_fd(1, HELP_STR);
            return (0);
        }
    }
    return (1);
}

t_context init_context(){
    t_context context;

    context.error = 0;
    context.special_error = 0;
    context.flag_nb = 0;
    return (context);
}

int main (int argc, char **argv)
{
    /* special error for only '-' manage case:
         exit code -2, 
         don't active default search in current dir (".")
         display if another args found (dir or file) 
    */
    // int error = 0, special_error = 0, flag_nb = 0;
    t_context c = init_context();

    if (check_display_help(argc, argv) == 0)
        return (0);
    ft_bzero(g_buff.buffer, BUFFER_LEN - 1);

    c.flag_nb = parse_flag(argc, argv, &c.special_error);
    if (c.flag_nb == -1)
        return (2);
    // c.error = ft_ls(argv, c.flag_nb, &c.error, c.special_error);
    c.error = ft_ls(argv, &c);
    finish_print_buffer();

    return (c.error);
}

int quotes_required(char *str)
{
    int i = 0;
    int ret = NORMAL_CHAR;
    int tmp = NORMAL_CHAR;

    while (str && str[i]) {
        tmp = is_special_char(str[i]);
        if (tmp == DIEZE_CHAR)
            tmp = i == 0 ? ADD_SIMPLE_QUOTE_CHAR : NORMAL_CHAR;
        else if (tmp == BRACKET_CHAR) 
            tmp = (ft_strlen(str) == 1 ? ADD_SIMPLE_QUOTE_CHAR : NORMAL_CHAR);
        if (tmp != NORMAL_CHAR) {
            if (ret < ADD_DOUBLE_QUOTE_CHAR)
                ret = tmp;
        }
        ++i;
    }
    return (ret);
}

struct stat *check_for_stat(char* name, int flag, int *save_symlink)
{
    struct stat *sb = ft_calloc(sizeof(struct stat), 1);

    if (!sb)
        return (NULL);

    if (lstat(name, sb) == -1) {
        // printf("%sName: %s%s\n",RED, name, RESET);
        free(sb);
        return (NULL);
    }
    /* store symlink type before call stat */
    *save_symlink = get_type(*sb) == SYMLINK;

    if (!has_flag(flag, L_OPTION))
        if (stat(name, sb) == -1){
            free(sb);
            return (NULL);
        }
    return (sb);
}