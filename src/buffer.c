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

/**
 * fill buffer with long format info
 * args: lst of t_file to display
 * flag_nb: flag option
 * file_c: file_context
    * call_flag: call flag context
    * space: int array of space for each column
*/
int fill_l_buffer(t_list *lst, t_context *c, t_file_context *file_c)
{
    t_list  *current = lst;
    int     lst_len = ft_lstsize(lst), i = 0, error = 0;
    /*CALL get_all_space HERE  */
    // int     *space = get_all_space(current, c->flag_nb);
    // int *new_space = file_c->space;

    while (current) {
        // printf("%sIn fill Line for: |%s|%s: \n",GREEN, ((t_file * )current->content)->name, RESET);
        // for (int i = 0; i <= S_MAJOR_SIZE; ++i) {
        //     printf("%sLine de [%d]|%s|%s -> ",CYAN, i, ((t_file * )current->content)->line[i], RESET);
        //     printf("%sNEW Space de [%d]|%d|%s, ",YELLOW, i, new_space[i], RESET);
        //     printf("%sOLD Space de [%d]|%d|%s\n",RED, i, space[i], RESET);
        // }
        error = fill_buffer_l_option(((t_file *)current->content), c, file_c); // change to int return for malloc check
        if (error == MALLOC_ERR)
            break ;
        ++i;
        if (i < lst_len)
            fill_buffer("\n");
        current = current->next;
    }
    // free(space);
    if (file_c->call_flag != 0)
        file_lstclear(&lst, free);
    return (error);
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


static int check_lst_quote(t_list *lst)
{
    t_list *current = lst; 
    while (current)
    {
        if (((t_file *) current->content)->quote > NOEFFECT_CHAR)
            return (1);
        current = current->next;
    }
    return (0);
}

int store_in_buffer(t_list *lst, t_context *c, t_file_context *file_c)
{
    int     err = 0;
    /* check for quote in lst and give bool */
    // int quote_space = get_nb_space(lst, get_len_name_quote);

    int quote_space = check_lst_quote(lst);

    err = manage_column(lst, quote_space, c, file_c);
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