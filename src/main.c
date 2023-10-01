#include "../include/ft_ls.h"

t_buff g_buff;

int ls(t_list * lst, int flag_nb,  int (*ls_function)(t_file*, int, int, int*), int* error)
{
    t_list *current = lst;
    int lst_len = get_lst_len(lst) - 1;
    int err = 0;
    while (current)
    {
        err = ls_function(current->content, flag_nb, lst_len, error);
        if (err == MALLOC_ERR)
            break ;
        current = current->next;
    }
    if (flag_nb & D_OPTION && !(flag_nb & L_OPTION))
        fill_buffer_char('\n');
    return (err);
}

static int ls_only_dir(t_list *dir_lst, int flag_nb)
{
    t_list *current = dir_lst;
    int *space = get_all_space(dir_lst, flag_nb);
    int err = 0;
    while (current)
    {
        err = fill_buffer_l_option(*(t_file *)current->content, space, flag_nb);
        if (err == MALLOC_ERR)
        {
            free(space);
            return (MALLOC_ERR);
        }
        current = current->next;
    }
    free(space);
    return (0);
}

void call_ls(t_list *dir_lst, int flag_nb, int *error)
{
    int err;
    
    err = 0;
    if (flag_nb & R_OPTION && !(flag_nb & D_OPTION))
        err = search_recurcive_dir(dir_lst, flag_nb, error);
    else if (flag_nb & L_OPTION && flag_nb & D_OPTION)
        err = ls_only_dir(dir_lst, flag_nb);
    else if (flag_nb & L_OPTION)
        err = ls(dir_lst, flag_nb, ls_l_one_dir, error);
    else
        err = ls(dir_lst, flag_nb, ls_one_dir, error);
    if (!(flag_nb & D_OPTION) || (flag_nb & L_OPTION && flag_nb & D_OPTION))
        new_lstclear(&dir_lst, free);
    else
        free_node_ptr(&dir_lst);
    if (err == MALLOC_ERR)
    {
        ft_printf_fd(2, "Malloc error exit\n");
        exit(MALLOC_ERR);
    }
    finish_print_buffer();
}

int ft_ls(char **argv, int flag_nb, int* error)
{
    t_list *dir_lst;
    
    dir_lst = get_dir_args(&argv[1], error);
    if (!dir_lst && *error == MALLOC_ERR)
        return (print_error("Malloc error\n", NULL, MALLOC_ERR, 1));
    else if (!dir_lst)
        return (*error);
    sort_lst(dir_lst, flag_nb);
    if (!dir_lst)
        return (print_error("Malloc error\n", NULL, MALLOC_ERR, 1));
    if (flag_nb & REVERSE_OPTION)
    {
        if (safe_reverse_lst(&dir_lst, error, flag_nb) == MALLOC_ERR)
        {
            new_lstclear(&dir_lst, free);
            return (print_error("Malloc error\n", NULL, MALLOC_ERR, 1));
        }
    }
    call_ls(dir_lst, flag_nb, error);
    return (*error);
}

static int check_display_help(int argc, char**argv)
{
    if (argc >= 2)
    {
        if (ft_strcmp(argv[1], "--help") == 0)
        {
            ft_printf_fd(1, HELP_STR);
            return (0);
        }
    }
    return (1);
}

int main (int argc, char **argv)
{
    int error;
    int flag_nb;

    error = 0;
    if (check_display_help(argc, argv) == 0)
        return (0);
    ft_bzero(g_buff.buffer, BUFFER_LEN - 1);
    enum e_flag *flag = check_for_flag(argc, argv);
    if (!flag)
        return(MALLOC_ERR);
    flag_nb = get_flag(flag);
    free(flag);
    error = ft_ls(argv, flag_nb, &error);

    return (error);
}