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
        // printf("Fill_l_buffer: i->[%d], lst_len[%d], call [%d]\n",i, lst_len, call_flag);
        if (i < lst_len)
            fill_buffer("\n");
        current = current->next;
    }
    free(space);
    if (call_flag != 0)
        file_lstclear(&lst, free);
    return (0);
}

void fill_buffer(char *str)
{
    if (!str)
        return ;
    int i = 0;
    while (str[i]) {
        g_buff.buffer[g_buff.i] = str[i];
        ++i;
        ++(g_buff.i);
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
    if (color == E_FILL_GREEN)
        fill_buffer(FILL_GREEN);
    if (color == E_FILL_YELLOW)
        fill_buffer(FILL_YELLOW);
    if (color == E_FILL_RED)
        fill_buffer(FILL_RED);
    if (color == E_YELLOW_BLACK)
        fill_buffer(FILL_YELBLACK);
}

void fill_buffer_color(char *str, enum e_color color, int flag_nb, int space, int quote)
{
    char c = ' ', color_bool = 0, quote_status = -1; /* -1 for nothing, 0 for space, 1 for real quote */

    if (!str)
        return ;
    /* detect quote type */
    if (space != 0) {
        if (quote > NOEFFECT_CHAR)
            c = quote == ADD_SIMPLE_QUOTE_CHAR ? '\'' : '\"';
        quote_status = quote > NOEFFECT_CHAR ? 1 : 0;
    }

    /* if no quote but need to print space, display it before color */
    if (quote_status == 0)
        fill_buffer_char(c);
    if (color != E_NONE && has_flag(flag_nb, COLOR_OPTION)) {
        color_bool = 1;
        fill_color(color);
    }

    /* if quote display it after color */
    if (quote_status == 1)
        fill_buffer_char(c);
    /* display name */
    fill_buffer(str);
    if (quote_status == 1)
        fill_buffer_char(c);
    
    if (color_bool)
        fill_buffer(RESET);
    if (quote_status == 0)
        fill_buffer_char(c);
}

void fill_buffer_char(char c)
{
    g_buff.buffer[g_buff.i] = c;
    ++(g_buff.i);
    if (g_buff.i > PRINT_SIZE || (c == '\n' && g_buff.i > PRINT_SIZE * 0.5))
        print_and_clear();
}


int store_in_buffer(t_list *lst, int flag_nb)
{
    int     err = 0;
    /* check for quote in lst and give bool */
    int quote_space = get_nb_space(lst, get_len_name_quote); 

    err = manage_column(lst, quote_space, flag_nb);
    if (err == MALLOC_ERR) {
        file_lstclear(&lst, free);
        return (MALLOC_ERR);
    }
    return (err);
}

/** finish_print_buffer
 * Function to empty the buffer before exiting the program
*/
void finish_print_buffer()
{
    if (g_buff.i != 0) {
        write(1, g_buff.buffer, g_buff.i);
        if (g_buff.buffer[g_buff.i] != '\n')
            write(1, "\n", 1);
    }
}

/* ACL attr */
//         if (has_flag(flag_nb, Z_OPTION)) {
//             fill_buffer_char('\n');
//             diplay_xattr_acl(file);
//             fill_buffer_char('\n');
//         }