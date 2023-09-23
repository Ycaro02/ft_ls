#include "../include/ft_ls.h"

t_buff g_buff;


int ls(t_list * lst, int flag_nb,  int (*ls_function)(t_file*, int, int, int*), int* error)
{
    t_list *current = lst;
    int lst_len = get_lst_len(lst);
    int err = 0;
    while (current)
    {
        err = ls_function(current->content, flag_nb, lst_len, error);
        if (err == MALLOC_ERR)
            break ;
        current = current->next;
    }
    return (err);
}

void call_ls(t_list *dir_lst, int flag_nb, int *error)
{
    int err = 0;
    if (flag_nb & R_OPTION)
        err = search_recurcive_dir(dir_lst, flag_nb, error);
    else if (flag_nb & L_OPTION)
        err = ls(dir_lst, flag_nb, ls_l_one_dir, error);
    else
        err = ls(dir_lst, flag_nb, ls_one_dir, error);
    new_lstclear(&dir_lst, free);
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
    t_list *new;
    
    new = NULL;
    dir_lst = get_dir_args(&argv[1], error);
    if (!dir_lst)
        return (print_error("Malloc error\n", NULL, MALLOC_ERR, 1));
    sort_lst(dir_lst, flag_nb);
    if (!dir_lst)
        return (print_error("Malloc error\n", NULL, MALLOC_ERR, 1));
    if (flag_nb & REVERSE_OPTION)
    {
        reverse_lst(dir_lst, &new);
        if (!new)
        {
            new_lstclear(&dir_lst, free);
            return (print_error("Malloc error\n", NULL, MALLOC_ERR, 1));
        }
        free_node_ptr(&dir_lst);
        dir_lst = new;
    }
    call_ls(dir_lst, flag_nb, error);
    return (*error);
}

int main (int argc, char **argv)
{
    int error;
    int flag_nb;

    error = 0;
    enum e_flag *flag = check_for_flag(argc, argv);
    if (!flag)
        return(MALLOC_ERR);
    flag_nb = get_flag(flag);
    free(flag);
    ft_ls(argv, flag_nb, &error);
    return (error);
}