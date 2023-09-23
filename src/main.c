#include "../include/ft_ls.h"

t_buff g_buff;

void ls(t_list * lst, int flag_nb,  void (*ls_function)(t_file*, int, int))
{
    t_list *current = lst;
    int lst_len = get_lst_len(lst);
    while (current)
    {
        ls_function(current->content, flag_nb, lst_len);
        current = current->next;
    }
}

void ft_ls(char **argv, int flag_nb, int *error)
{
    t_list *dir_lst;
    

    dir_lst = get_dir_args(&argv[1], error);
    if (!dir_lst)
        return ;
    sort_lst(dir_lst, flag_nb);
    t_list *new = NULL;
    if (flag_nb & REVERSE_OPTION)
    {
        reverse_lst(dir_lst, &new);
        free_node_ptr(&dir_lst);
        dir_lst = new;
    }
    if (flag_nb & R_OPTION)
        search_recurcive_dir(dir_lst, flag_nb);
    else if (flag_nb & L_OPTION)
        ls(dir_lst, flag_nb, ls_l_one_dir);
    else
        ls(dir_lst, flag_nb, ls_one_dir);
    finish_print_buffer();
    new_lstclear(&dir_lst, free);
}

int main (int argc, char** argv)
{
    int error;
    int flag_nb;

    error = 0;
    enum e_flag *flag = check_for_flag(argc, argv);
    if (!flag)
        return(2);
    flag_nb = get_flag(flag);
    free(flag);
    ft_ls(argv, flag_nb, &error);
    return (0);
}
