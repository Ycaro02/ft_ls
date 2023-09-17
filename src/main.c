#include "../ft_ls.h"

t_buff g_buff;

void ls(t_list * lst, int flag_nb,  void (*ls_function)(t_file*, int))
{
    t_list *current = lst;
    while (current)
    {
        ls_function(current->content, flag_nb);
        current = current->next;
    }
}

void ft_ls(char **argv, int flag_nb)
{
    t_list *dir_lst;
    
    dir_lst = get_dir_args(&argv[1]);
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
    (void)argc;
    enum e_flag *flag = check_for_flag(argc, argv);
    if (!flag)
        return(2);
    int flag_nb = get_flag(flag);
    free(flag);
    ft_ls(argv, flag_nb);
    return (0);
}