#include "../include/ft_ls.h"

t_buff g_buff;

// int ls(t_list *lst, t_context *c, int (*ls_function)(t_file*, t_context*, t_file_context*), int call_flag)
int ls(t_list *lst, t_context *c, t_file_context *file_c, int (*ls_function)(t_file*, t_context*, t_file_context*))
{
    t_list *current = lst;
    int err = 0;

    if (!lst)
        return (err);
    file_c->idx = 0;
    file_c->lst_len = ft_lstsize(lst);

    while (current) {
        err = ls_function(current->content, c, file_c);
        ++(file_c->idx);
        if (err == MALLOC_ERR)
            break ;
        current = current->next;
    }
    return (err);
}

static int ls_only_dir(t_list *dir_lst, t_context *c, t_file_context *file_c)
{
    t_list  *current = dir_lst;
    int     *space = file_c->space, err = 0;
    int     lst_len = ft_lstsize(dir_lst), i = 0;

    while (current) {
        err = fill_buffer_l_option(*(t_file *)current->content, space, c, file_c);
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

/** special_display_header
 *  display new band before ls call if:
 *      - args found (invalid args),
 *      - no simple file found (call value == 0)
 *      - lst_size != 1 
*/
static void special_display_header(t_list *dir_lst, int args_found, int call_value)
{
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
}

/** call_ls
 * ls HUB to choice which ls version call
*/
static void call_ls(t_list *dir_lst, t_context *c, t_file_context *file_c)
{
    int     err = 0;
    t_int8  recursive_flag = has_flag(c->flag_nb, R_OPTION);
    t_int8  l_flag = has_flag(c->flag_nb, L_OPTION);
    t_int8  onlydir_flag = has_flag(c->flag_nb, D_OPTION); 

    if (file_c->call_flag != 0 && recursive_flag && !onlydir_flag)
        err = search_recurcive_dir(dir_lst, c, file_c->call_flag); /* Call recurcive */
    else if (file_c->call_flag != 0 && l_flag && onlydir_flag)
        err = ls_only_dir(dir_lst, c, file_c); /* Call ls D + L option*/
    else if (l_flag && file_c->call_flag != 0)
        err = ls(dir_lst, c, file_c, ls_l_one_dir); /* Call ls L option */
    else if (l_flag && file_c->call_flag == 0)
        err = ls_only_file_l(dir_lst, c, file_c);  /* For mixed argument in cmd line */
    else
        err = ls(dir_lst, c, file_c, ls_one_dir); /* Call classic ls option */
   
    file_lstclear(&dir_lst, free);
    if (err == MALLOC_ERR) {
        ft_printf_fd(2, "Malloc error exit\n");
        exit(MALLOC_ERR);
    }
}

t_args *init_args(t_context *c)
{
    t_args *arg = ft_calloc(sizeof(t_args), 1);
    if (!arg) {
        ft_printf_fd(2, "Malloc error init args\n");
        return (NULL);
    }
    arg->c.error = c->error;
    arg->c.special_error = c->special_error;
    arg->c.flag_nb = c->flag_nb;
    return (arg);
}

/** ft_ls: start ls called in main
 * Parse command line argument and store directory and simple_file in separate linked list
 * Args:    - argv: argv from main
 *          - c: Pointer to the context containing flag, special_error, exit_code
 *              - c->error is set in parse_cmd_args or ls_function (call_ls) 
 * Return value: Return the appropriate exit code
*/
static int ft_ls(char **argv, t_context *c)
{
    // BUILD args here 
    t_args *arg = init_args(c);
    // t_list  *dir_lst, *simple_file = NULL;
    // t_file_context file_c; /* first file_context */
    // ft_bzero(&file_c, sizeof(t_file_context));
    t_int8  args_found = c->special_error;
    int     call_value = 0;
    
    args_found = parse_cmd_args(&argv[1], arg);
    if (args_found == MALLOC_ERR)
        return (MALLOC_ERR);

    // dir_lst = parse_cmd_args(&argv[1], &simple_file, &args_found, c, &file_c);
    /* Error management */
    if (!(arg->dir_lst) && c->error == MALLOC_ERR) {
        ft_printf_fd (2, "Malloc Error ft_ls\n");
        return (MALLOC_ERR);
    }
    
    if (arg->dir_lst) {  /* if directory found */
        if (has_flag(c->flag_nb, D_OPTION)) {
            t_list *new = NULL;
            new = ft_lstjoin(arg->dir_lst, arg->simple_file);
            if (new) {
                sort_lst(&new, c->flag_nb);
                call_ls(new, c, &arg->file_c);
                return (c->error);
            }
        }
        sort_lst(&arg->dir_lst, c->flag_nb);
    }

    if (arg->simple_file) { /* if other file found */
        sort_lst(&arg->simple_file, c->flag_nb);
        call_ls(arg->simple_file, c, &arg->file_c);
        ++call_value;
        if (has_flag(c->flag_nb, L_OPTION) && arg->dir_lst)
            fill_buffer("\n");
    }


    if (arg->dir_lst) {
        if (!has_flag(c->flag_nb, R_OPTION))
            special_display_header(arg->dir_lst, args_found, call_value);
        arg->file_c.call_flag += 1;
        call_ls(arg->dir_lst, c, &arg->file_c);
        // call_ls(dir_lst, c, &file_c, call_value + 1);
    }

    if (c->special_error == 1) /* set return cmd value for special err */
        c->error = NA_CMD_LINE_ERR;
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

// t_context init_context(){
//     t_context context;

//     context.error = 0;
//     context.special_error = 0;
//     context.flag_nb = 0;
//     return (context);
// }

int main (int argc, char **argv)
{
    t_context c;

    ft_bzero(&c, sizeof(t_context));

    if (check_display_help(argc, argv) == 0)
        return (0);
    ft_bzero(g_buff.buffer, BUFFER_LEN - 1);
    c.flag_nb = parse_flag(argc, argv, &c.special_error);
    if (c.flag_nb == -1)
        return (2);
    c.error = ft_ls(argv, &c);
    finish_print_buffer();
    return (c.error);
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

/* old call ls manage */
// if (call_flag != 0 && has_flag(c->flag_nb, R_OPTION) && !has_flag(c->flag_nb, D_OPTION))
//     err = search_recurcive_dir(dir_lst, c, call_flag); /* Call recurcive */
// else if (call_flag != 0 && has_flag(c->flag_nb, L_OPTION) && has_flag(c->flag_nb, D_OPTION))
//     err = ls_only_dir(dir_lst, c->flag_nb); /* Call ls D + L option*/
// else if (has_flag(c->flag_nb, L_OPTION) && call_flag != 0)
//     err = ls(dir_lst, c, ls_l_one_dir, call_flag); /* Call ls L option */
// else if (has_flag(c->flag_nb, L_OPTION) && call_flag == 0)
//     err = ls_only_file_l(dir_lst, c->flag_nb);  /* For mixed argument in cmd line */
// else
//     err = ls(dir_lst, c, ls_one_dir, call_flag); /* Call classic ls option */