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

void    print_and_clear()
{
    write(1, g_buff.buffer, g_buff.i);
    ft_bzero(g_buff.buffer, g_buff.i);
    g_buff.i = 0;
}

void fill_buffer(char *str)
{
    if (!str)
        return ;
    int i = 0;
    while (str[i])
    {
        g_buff.buffer[g_buff.i] = str[i];
        i++;
        ( g_buff.i)++;
        if (g_buff.i > PRINT_SIZE || (str[i] == '\n' && g_buff.i > PRINT_SIZE * 0.5))
            print_and_clear();
    }
}

static void store_in_buffer(t_list *lst, int flag_nb)
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
        fill_buffer(current->content);
        if (current->next)
            fill_buffer("  ");
        current = current->next;
    }
    fill_buffer("\n");
    ft_lstclear(&new, free);
}

void ls_no_args(int flag_nb)
{
    t_list *lst = get_all_file_name(".", flag_nb);
    if (lst == NULL)
    {
        ft_putstr_fd("Error for read current directory\n", 1);
        return ;
    }
    store_in_buffer(lst, flag_nb);
    ft_lstclear(&lst, free);
}

void ls_one_dir(char *str, int flag_nb)
{
    (void)flag_nb;
    fill_buffer(str);
    fill_buffer(":\n");
    t_list *lst = get_all_file_name(str, flag_nb);
    if (lst)
    {
        store_in_buffer(lst, flag_nb);
        ft_lstclear(&lst, free);
    }
    fill_buffer("\n");
}