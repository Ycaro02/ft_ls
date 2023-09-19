#include "../ft_ls.h"


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

void store_in_buffer(t_list *lst, int flag_nb)
{
    t_list *current = NULL;
    t_list *new = NULL;
    if (flag_nb & REVERSE_OPTION)
    {
        reverse_lst(lst, &new);
        free_node_ptr(&lst);
        lst = new;
    }
    current = lst;
    while (current)
    {
        t_file *file = current->content;
        fill_buffer(file->name);
        if (current->next)
            fill_buffer("  ");
        current = current->next;
    }
    fill_buffer("\n");
    new_lstclear(&lst, free);
}

void finish_print_buffer()
{
    if (g_buff.i != 0)
        write(1, g_buff.buffer, g_buff.i);
}