#include "../include/ft_ls.h"

void multiple_fill_buff(char *s1, char*s2, char *s3, char *s4)
{
    if (s1)
        fill_buffer(s1);
    if (s2)
        fill_buffer(s2);
    if (s3)
        fill_buffer(s3);
    if (s4)
        fill_buffer(s4);
}

void    print_and_clear()
{
    write(1, g_buff.buffer, g_buff.i);
    ft_bzero(g_buff.buffer, g_buff.i);
    g_buff.i = 0;
}

int fill_l_buffer(t_list *lst, int flag_nb, int call_flag)
{
    t_list *current = lst;
    int *space;
    int lst_len = ft_lstsize(lst), i = 0;
    

    current = lst;
    space = get_all_space(current, flag_nb);
    if (!space)
        return (MALLOC_ERR);
    // printf("Yo call: %d\n", call_flag);
    while (current) {
        fill_buffer_l_option(*((t_file *)current->content), space, flag_nb); // change to int return for malloc check
        ++i;
        if (i != lst_len)
            fill_buffer("\n");
        current = current->next;
    }
    free(space);
    if (call_flag != 0)
        new_lstclear(&lst, free);
    return (0);
}

void fill_buffer(char *str)
{
    if (!str)
        return ;
    int i = 0;
    while (str[i]) {
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

void fill_buffer_color(char *str, enum e_color color, int flag_nb)
{
    if (!str)
        return ;
    if (color != E_NONE && has_flag(flag_nb, COLOR_OPTION)) {
        fill_color(color);
        fill_buffer(str);
        fill_buffer(RESET);
    }
    else
        fill_buffer(str);
}

void fill_buffer_char(char c)
{
    g_buff.buffer[g_buff.i] = c;
    (g_buff.i)++;
    if (g_buff.i > PRINT_SIZE || (c == '\n' && g_buff.i > PRINT_SIZE * 0.5))
        print_and_clear();
}

static int classic_store(t_list *lst, int flag_nb)
{
    int is_exec = 0;
    t_list *current = lst;
    while (current)
    {
        t_file *file = current->content;
        is_exec = check_file_perm(file->perm, 1);
        if (is_exec == MALLOC_ERR)
            return (MALLOC_ERR);
        /* TOCHECK */
        if (write_file_name(*file, is_exec, flag_nb , 1) == MALLOC_ERR)
            return (MALLOC_ERR);
        if (has_flag(flag_nb, Z_OPTION)) {
            fill_buffer_char('\n');
            diplay_xattr_acl(file);
            fill_buffer_char('\n');
        }
        else if (current->next)
            fill_buffer(" ");
        current = current->next;
    }
    new_lstclear(&lst, free);
    return (0);
}

int store_in_buffer(t_list *lst, int flag_nb)
{
    int     err = 0;
    int     nb_raw = 0;
    char    **tab = NULL;
    
    if (has_flag(flag_nb, REVERSE_OPTION))
        if (safe_reverse_lst(&lst, NULL, flag_nb) == MALLOC_ERR)
            return (MALLOC_ERR);

    /* check for quote in lst and give bool */
    int quote_space = get_nb_space(lst, get_len_name_quote); 
    tab = check_manage_colum(lst, &err, &nb_raw, quote_space);
    if (err == MALLOC_ERR) {
        new_lstclear(&lst, free);
        return (MALLOC_ERR);
    }
    else if (tab != NULL && !has_flag(flag_nb, Z_OPTION))
        return (fill_buffer_with_column(tab, nb_raw, &lst, flag_nb, quote_space));
    if (tab)
        ft_free_tab(tab);
    return (classic_store(lst, flag_nb));
}

void finish_print_buffer()
{
    if (g_buff.i != 0) {
        write(1, g_buff.buffer, g_buff.i);
        if (g_buff.buffer[g_buff.i] != '\n')
            write(1, "\n", 1);
    }
}