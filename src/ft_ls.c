#include "../ft_ls.h"

void reverse_lst(t_list *lst, t_list **new)
{
    t_list *current = lst;
    while (current)
    {
        ft_lstadd_front(new, ft_lstnew(ft_strdup(current->content)));
        current = current->next; 
    }
}

static void print_dir(t_list *lst, int flag_nb)
{
    t_list *current = NULL;
    t_list *new = NULL;
    if (flag_nb >= REVERSE_OPTION)
    {
        reverse_lst(lst, &new);
        current = new;
    }
    else
        current = lst;
    while (current)
    {
        ft_putstr_fd(current->content, 1);
        if (current->next)
            ft_putstr_fd("  ", 1);
        current = current->next;
    }
    ft_putstr_fd("\n", 1);
    ft_lstclear(&new, free);
}

void ls_no_args(int flag_nb)
{
    t_list *lst = get_all_file_name(".", 1);
    if (lst == NULL)
    {
        ft_putstr_fd("Error for read current directory\n", 1);
        return ;
    }
    print_dir(lst, flag_nb);
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
        print_dir(lst, flag_nb);
        ft_lstclear(&lst, free);
    }
    ft_putstr_fd("\n", 1);
}