#include "../ft_ls.h"

static void print_dir(t_list *lst)
{
    while (lst)
    {
        ft_putstr_fd(lst->content, 1);
        if (lst->next)
            ft_putstr_fd(" ", 1);
        lst = lst->next;
    }
    ft_putstr_fd("\n", 1);
}

void ls_no_args()
{
    t_list *lst = get_all_file_name(".", 1);
    if (lst == NULL)
    {
        ft_putstr_fd("Error for read current directory\n", 1);
        return ;
    }
    print_dir(lst);
    ft_lstclear(&lst, free);
}

void ls_one_dir(char *str, int flag_nb)
{
    (void)flag_nb;
    ft_putstr_fd(str, 1);
    ft_putstr_fd(":\n", 1);
    t_list *lst = get_all_file_name(str, 1);
    if (lst)
    {
        print_dir(lst);
        ft_lstclear(&lst, free);
    }
    ft_putstr_fd("\n", 1);
}