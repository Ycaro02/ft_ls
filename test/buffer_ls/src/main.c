#include "../ft_ls.h"

t_buff g_buff;

void finish_print_buffer()
{
    if (g_buff.i != 0)
        write(1, g_buff.buffer, g_buff.i);
}

void ft_ls(char **argv, int flag_nb)
{
    t_list *dir_lst = get_dir_args(&argv[1]);
    t_list *new = NULL;
    if (!dir_lst)
    {
        ls_no_args(flag_nb);
        ft_lstclear(&dir_lst, free);
        finish_print_buffer();
        return ;
    }
    sort_by_name(dir_lst);
    if (flag_nb & REVERSE_OPTION)
    {
        reverse_lst(dir_lst, &new);
        ft_lstclear(&dir_lst, free);
        dir_lst = new;
    }
    if (flag_nb & R_OPTION)
        search_recurcive_dir(dir_lst, flag_nb);
    else
    {
        t_list *current = dir_lst;
        while (current)
        {
            ls_one_dir(current->content, flag_nb);
            current = current->next;
        }
    }
    finish_print_buffer();
    ft_lstclear(&dir_lst, free);
}

int main (int argc, char** argv)
{
    (void)argc;
    enum e_flag *flag = check_for_flag(argv);
    if (!flag)
        return(2);
    int flag_nb = get_flag(flag);
    free(flag);
    ft_ls(argv, flag_nb);
    return (0);
}