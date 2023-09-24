#include "../include/ft_ls.h"

void    print_and_clear()
{
    write(1, g_buff.buffer, g_buff.i);
    ft_bzero(g_buff.buffer, g_buff.i);
    g_buff.i = 0;
}

int fill_l_buffer(t_list *lst)
{
    t_list *current = lst;
    int *space;

    current = lst;
    space = get_all_space(current);
    if (!space)
        return (MALLOC_ERR);
    while (current)
    {
        fill_buffer_l_option(*(t_file *)current->content, space); // change to int return for malloc check
        current = current->next;
    }
    free(space);
    new_lstclear(&lst, free);
    return (0);
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
        (g_buff.i)++;
        if (g_buff.i > PRINT_SIZE || (str[i] == '\n' && g_buff.i > PRINT_SIZE * 0.5))
            print_and_clear();
    }
}

void fill_color(enum e_color color)
{
    if (color == E_RED)
        fill_buffer(RED);
    if (color == E_BLUE)
        fill_buffer(BLUE);
    if (color == E_GREEN)
        fill_buffer(GREEN);
    if (color == E_YELLOW)
        fill_buffer(YELLOW);
    if (color == E_PURPLE)
        fill_buffer(PURPLE);
    if (color == E_CYAN)
        fill_buffer(CYAN);
}

void fill_buffer_color(char *str, enum e_color color)
{
    if (!str)
        return ;
    fill_color(color);
    fill_buffer(str);
    fill_buffer(RESET);
}

void fill_buffer_char(char c)
{
    g_buff.buffer[g_buff.i] = c;
    (g_buff.i)++;
    if (g_buff.i > PRINT_SIZE || (c == '\n' && g_buff.i > PRINT_SIZE * 0.5))
        print_and_clear();
}

static int check_buffer_perm(int perm, int to_check)
{
    char    *tmp;
    int     i;
    int     ret;

    i = 0;
    ret = 1;
    tmp = get_perm(perm);
    if (!tmp)
        return (MALLOC_ERR);
    while (tmp[i])
    {
        if ((tmp[i] - 48) & to_check)
        {
            ret = 0;
            break;
        }
        i++;
    }
    free(tmp);
    return (ret);
}

int store_in_buffer(t_list *lst, int flag_nb)
{
    t_list *current;
    int is_exec = 0;

    if (flag_nb & REVERSE_OPTION)
        if (safe_reverse_lst(&lst, NULL) == MALLOC_ERR)
            return (MALLOC_ERR);
    current = lst;
    while (current)
    {
        t_file *file = current->content;
        is_exec = check_buffer_perm(file->perm, 1);
        if (is_exec == MALLOC_ERR)
            return (MALLOC_ERR);
        if (write_file_name(*file, is_exec, 0) == MALLOC_ERR)
            return (MALLOC_ERR);
        if (current->next)
            fill_buffer("  ");
        current = current->next;
    }
    fill_buffer("\n");
    new_lstclear(&lst, free);
    return (0);
}

void finish_print_buffer()
{
    if (g_buff.i != 0)
        write(1, g_buff.buffer, g_buff.i);
}