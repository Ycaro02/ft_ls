#include "../include/ft_ls.h"

/**
 * Basic get file lst content
 * Args:    lst: linked list pointer
 *          index: wanted node index
 * return:  Ptr on wanted lst->content if found, otherwise NULL
*/
void *get_lst_index_content(t_list *lst, int index)
{
    int i = 0;
    t_list *current = lst;

    while (current) {
        if (i == index)
            return (current->content);
        ++i;
        current = current->next;
    }
    return (NULL);
}

/**
 * Compute total len of lst file
 * Args:    lst: ptr target lst
 * ret : total file len + (2 * len) for space: maybe can adapt with bool quote +1 or +2
*/
long long get_total_len(t_list *list)
{
    long long total;
    t_file *file;
    
    total = 0;
    while (list) {
        file = list->content;
        total += ft_strlen(file->name);
        total += 2;
        list = list->next;
    }
    // total -= 2;
    return (total);
} 

/**
 * Store all len of lst file
 * Args:    lst: ptr target lst
 *          len: list len
 * ret: ptr on allocated integer array represent name len by node id
*/
static int* get_all_len(t_list *lst, int len)
{
    int i;
    int *all_len = NULL;
    
    i = 0;
    all_len = ft_calloc(sizeof(int), len);
    if (!all_len)
        return (NULL);
    while (lst) {
        t_file *file = lst->content;
        all_len[i] = ft_strlen(file->name);
        i++;
        lst = lst->next;
    }
    return (all_len);
}



/*--------------------------------------------START ALGO---------------------------------------------------------*/
// 
/** Get_max_len_by_index
 * Get the max column len for specific column and test value
 * Args:    start: position index to start for build line, increment by test
 *          test: the value tested (number of line)
 *          max: tab size, number of file in lst
 *          tab: array of all file's name len, store in list order
 * Ret: The longest word len for targeted column 
*/
static int get_max_len_by_index(int start, int test ,int max, int* tab)
{
    int nb = 0;
    int i = start;
    while (i < (start + test)) {
        if (i < max) {
            if (nb < tab[i])
                nb = tab[i];
        }
        i++;
    }
    return (nb);
}


/** Brut test
 * Real brut test function test maximum len for one line
 * Args:    i: position index to start for build line, increment by test
 *          test: the value tested (number of line)
 *          all_len: array of all file's name len, store in list order
 *          nb_file: all_len size, number of file in lst
 *          local_space: array of maximum space for this 'test' configuration
 *          bool_quote: quote present in lst dir 0 for no 1 for yes
 * Ret: Computed len for target line
*/
static int brut_test(int i, int test, int *all_len, int nb_file, int *local_space, int bool_quote)
{
    /* basic space is 2, 4 for bool quote*/
    int column = 1, ret = -1, space = 2 + (bool_quote + bool_quote);

    local_space[0] = get_max_len_by_index(i, test, nb_file, all_len);
    ret = (local_space[0] + space);

    while (i < nb_file - test) {
        if (i + test <= nb_file) {
            local_space[column] = get_max_len_by_index(i + test, test, nb_file, all_len);
            ret += (local_space[column] + space);
            ++column;
        }
        else
            break ;
        i += test;
    }
    ret += 2; // brut pad value
    return (ret);
}

/** Test_all
 * Call the brut test function and build array of local space for each tested column
 * Args:    test: the value tested (number of line)
 *          all_len: array of all file's name len, store in list order
 *          nb_file: all_len size, number of file in lst
 *          stdout_w: width of stdout
 *          bool_quote: quote present in lst dir 0 for no 1 for yes
 * Ret: -1 for impossible 0 for ok
*/
static int test_all(int test, int* all_len, int nb_file, int stdout_w, int bool_quote)
{
    int *local_space = ft_calloc(sizeof(int), nb_file);
    int ret = -1;
    int i = 0;
    while (i < test) {
        ret = brut_test(i, test, all_len, nb_file, local_space, bool_quote);
        // printf("%sRet = %d for [%d] width: %d%s\n",RED, ret, test, stdout_w, RESET);
        if (ret > stdout_w)
            break ;
        i++;
    }
    ret = ret > stdout_w ? -1 : 0;
    free(local_space);
    return (ret);
}

/** get_nb_line
 * Start brut force with test value, test if we can display all one 2 line if not increment test
 * args:    stdout_w: width of stdout, 
 *          lst: ptr on linked list to display 
 *          bool_quote: quote present in lst dir 0 for no 1 for yes
 * ret: right tested value, number of line
*/
static int get_nb_line(int stdout_w, int *all_len, int len, int bool_quote)
{
    if (!all_len)
        return (MALLOC_ERR);
    int test = 1; // test value for nb_line, brute force it
    int ret = -1;

    while (ret != 0) {
        ret = test_all(test, all_len, len, stdout_w, bool_quote);
        if (ret != 0)
            ++test;
    }
    return (test);
}
/*--------------------------------------------END ALGO---------------------------------------------------------*/

/** get_max_by_clomun
 *  Compute and store bigger size for each column, for padding/space
 * Args:    lst: ptr on linked list to display
 *          nb_column: column nb
 *          nb_line: line nb
 * ret: ptr on allocated integer array represent max by column id
*/
int *get_max_by_column(t_list *lst, int nb_column, int nb_line)
{
    int *tab = ft_calloc(sizeof(int), nb_column);
    int column = 0;
    int i = 0;
    int max = 0;
    while (lst) {
        t_file *file = lst->content;
        int tmp = ft_strlen(file->name);
        if (tmp > max)
            max = tmp;
        i++;
        if (i == nb_line ) {
            tab[column] = max;
            max = 0;
            i = 0;
            column++;
        }
        lst = lst->next;
    }
    if (i < nb_line && column < nb_column)
        tab[column] = max;
    return (tab);
}

/**
 * Display coloumn call write_file_name
 *          space_quote: quote present in lst dir 0 for no 1 for yes
*/
static void display_column(t_list *lst, int** array, int* max_per_column, int flag, int space_quote)
{
    t_file *file = NULL;

    /* i == line */
    for (int i = 0; array[i]; ++i) {
        /* j == column */
        for (int j = 0; array[i][j] != -1; ++j)  {
            /* protection against calloc value at the end of last line */
            if ((i != 0 || j != 0) && array[i][j] == 0)
                break ;
            file = get_lst_index_content(lst, array[i][j]);
            if (file) {
                int column_max = max_per_column[j];
                int nb_space = column_max - ft_strlen(file->name);

                // printf("%sArray[%d][%d]: [%d]->[%s] max col:[%d]->[%d]\n%s", CYAN, i, j, array[i][j], file->name, column_max, nb_space, RESET);
                write_file_name(*file, check_file_perm(file->perm, 1), flag, space_quote);
                /* if is not the last name of line*/
                if (array[i][j + 1] != -1) { 
                    if (space_quote == 1)
                        fill_buffer_char(' ');
                    fill_buffer_char(' ');
                    for (int k = 0; k < nb_space; ++k)
                        fill_buffer_char(' ');
                }
            }
        }
        if(array[i + 1])
            fill_buffer_char('\n');
    }
}

/**
 * Entry point, compute and return double char ** (bad just need to return index tab/lst -> int**)
*/
int manage_basic_column(t_list *lst, int *value, int space_quote, int flag)
{
    int     **array = NULL; 
    int     stdout_width = get_stdout_width(), nb_line = 0, max_per_line = 1, lst_len = get_lst_len(lst);
    int     *tab_max_unit = NULL, *all_len = get_all_len(lst, lst_len);

    if (!all_len || stdout_width <= 0) {
        ft_printf_fd(2, "Error alloc all_len manage column\n");
        return (MALLOC_ERR);
    }
    /* brut force here */
    nb_line = get_nb_line(stdout_width, all_len, lst_len, space_quote);
    if (nb_line == MALLOC_ERR)
        return (MALLOC_ERR);

    *value = nb_line; // second return need to kept nb_line
    max_per_line  = (int)(lst_len / nb_line) + (lst_len % nb_line != 0); // add 1 if (lst_len % nb_line != 0)
    
    tab_max_unit = get_max_by_column(lst, max_per_line, nb_line);
    if (!tab_max_unit) {
        free(all_len);
        new_lstclear(&lst, free);
        return (MALLOC_ERR);
    }
    array = create_column_array(lst, max_per_line, nb_line);
    if (array) 
        display_column(lst, array, tab_max_unit, flag, space_quote);
    free_incomplete_array((void **)array, nb_line);
    free(tab_max_unit);
    // }
    new_lstclear(&lst, free);
    free(all_len);
    return (0);
}

/* Protection after max_per_raw affectation, not sure is mandatory
        if (max_per_raw <= 0)
            max_per_raw = 1; */