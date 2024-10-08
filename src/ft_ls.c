#include "../include/ft_ls.h"

static int ls(t_list *lst, t_context *c, t_file_context *file_c, int (*ls_function)(t_file*, t_context*, t_file_context*))
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
        err = fill_buffer_l_option((t_file *)current->content, c, file_c);
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
static void special_display_header(t_list *dir_lst, int args_found, int call_value, int r_flag)
{
    int lst_len = ft_lstsize(dir_lst);
    // printf("call: %d, args %d, r_flag %d, lst_len%d\n", call_value, args_found, r_flag, lst_len);
    if ((args_found == 1 && call_value <= 1 && lst_len == 1)\
        || r_flag) {
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
static void call_ls(t_list *lst, t_context *c, t_file_context *file_c)
{
    int     err = 0;
    t_int8  recursive_flag = has_flag(c->flag_nb, R_OPTION);
    t_int8  l_flag = has_flag(c->flag_nb, L_OPTION);
    t_int8  onlydir_flag = has_flag(c->flag_nb, D_OPTION); 

    if (file_c->call_flag != 0 && recursive_flag && !onlydir_flag)
        err = search_recurcive_dir(lst, c, file_c->call_flag); /* Call recurcive */
    else if (file_c->call_flag != 0 && l_flag && onlydir_flag)
        err = ls_only_dir(lst, c, file_c); /* Call ls D + L option*/
    else if (l_flag && file_c->call_flag != 0)
        err = ls(lst, c, file_c, ls_l_one_dir); /* Call ls L option */
    else if (l_flag && file_c->call_flag == 0)
        err = ls_only_file_l(lst, c, file_c);  /* For mixed argument in cmd line */
    else if (file_c->call_flag == 0)
        err = ls_only_file(lst, c, file_c); /* Call classic ls option */
    else
        err = ls(lst, c, file_c, ls_one_dir); /* Call classic ls option */
   
    if (l_flag && file_c->space) {
        free(file_c->space);
        file_c->space = NULL;
    }

    if (!(file_c->call_flag == 0 && !l_flag)) /* Don't free for ls only file, done in manage column */
        ft_lstclear(&lst, destroy_file);
    if (err == MALLOC_ERR) {
        ft_printf_fd(2, "Malloc error exit\n");
        exit(MALLOC_ERR);
    }
}


static t_args *init_args(t_context *c)
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
int ft_ls(char **argv, t_context *c)
{
    // BUILD args here 
    int     call_value = 0, save_error = 0;
    t_int8  args_found = c->special_error;
    t_args *arg = init_args(c);

    args_found = parse_cmd_args(&argv[1], arg);
    if (args_found == MALLOC_ERR) {
        free(arg);
        return (MALLOC_ERR);
    }
    /* Error management */
    if (!(arg->dir_lst) && c->error == MALLOC_ERR) {
        ft_printf_fd (2, "Malloc Error ft_ls\n");
        free(arg);
        return (MALLOC_ERR);
    }
    
    if (arg->dir_lst) {  /* if directory found */
        if (has_flag(c->flag_nb, D_OPTION)) {
            t_list *new = NULL;
            new = ft_lstjoin(arg->dir_lst, arg->simple_file);
            if (new) {
                sort_lst(&new, c->flag_nb);
                call_ls(new, c, &arg->file_c);
                free(arg);
                return (c->error);
            }
        }
        sort_lst(&arg->dir_lst, c->flag_nb);
        c->first_lst += 2; /* signal dir_lst present */
    }
    if (arg->c.special_error == 1) /* set return cmd value for special err */
        arg->c.error = NA_CMD_LINE_ERR;
    save_error = arg->c.error;

    if (arg->simple_file) { /* if other file found */
        sort_lst(&arg->simple_file, c->flag_nb);
        call_ls(arg->simple_file, c, &arg->file_c);
        ++call_value;
        if (has_flag(c->flag_nb, L_OPTION) && arg->dir_lst)
            fill_buffer("\n");
        c->first_lst += 1;
    }


    if (arg->dir_lst) {
        special_display_header(arg->dir_lst, args_found, call_value, has_flag(arg->c.flag_nb, R_OPTION));
        arg->file_c.call_flag += 1;
        call_ls(arg->dir_lst, c, &arg->file_c);
    }
    free(arg);
    if (save_error < 2)
        save_error = c->error;
    return (save_error);
}