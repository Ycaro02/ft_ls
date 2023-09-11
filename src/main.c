#include "../ft_ls.h"

int main (int argc, char** argv)
{
    (void)argc;
    enum e_flag *flag = check_for_flag(argv);
    if (!flag)
        return(2);
    int flag_nb = get_flag(flag);
    free(flag);

    t_list *dir_lst = get_dir_no_hiden(&argv[1]);
    t_list *new = NULL;
    if (!dir_lst)
    {
        ls_no_args(flag_nb);
        ft_lstclear(&dir_lst, free);
        return (0);
    }
    sort_by_name(dir_lst);

    if (flag_nb >=  R_OPTION)
    {
        flag_nb -= R_OPTION;
        if (flag_nb >= REVERSE_OPTION)
        {
            reverse_lst(dir_lst, &new);
            ft_lstclear(&dir_lst, free);
            dir_lst = new;
        }
        search_recurcive_dir(dir_lst, flag_nb);
        ft_lstclear(&dir_lst, free);
        return (0);
    }
    else
    {
        if (flag_nb >= REVERSE_OPTION)
        {
            reverse_lst(dir_lst, &new);
            ft_lstclear(&dir_lst, free);
            dir_lst = new;
        }
        t_list *current = dir_lst;
        while (current)
        {
            ls_one_dir(current->content, flag_nb);
            current = current->next;
        }
        ft_lstclear(&dir_lst, free);

    }
    return (0);
}
