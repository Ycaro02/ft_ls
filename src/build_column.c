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
        current = current->next;
    }
    return (NULL);
}

/**
 * Compute total len of lst file
 * Args:    lst: ptr target lst
 * ret : total file len + (2 * len) for space: maybe can adapt with bool quote +1 or +2
*/
static long long get_total_len(t_list *list, int quote_bool)
{
    long long total;
    t_file *file;
    
    total = 0;
    while (list) {
        file = list->content;
        total += ft_strlen(file->name);
        total += (1 + quote_bool);
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
// get the max column len for specific column and test value
static int get_max_len_by_index(int start, int test ,int max, int* tab)
{
    int nb = 0;
    int i = start;
    while (i < (start + test))
    {
        if (i < max)
        {
            if (nb < tab[i])
                nb = tab[i];
        }
        i++;
    }
    return (nb);
}

static int brut_test(int i, int test, int tab_len, int *all_len, int *local_space, int quote_bool)
{
    int ret = -1;
    int column = 0;
    // printf("in brut test i = %d test = %d ", i ,test);
    while (i < tab_len - test) {
        if (ret == -1) {
            local_space[column] = get_max_len_by_index(i, test, tab_len, all_len);
            ret = local_space[column];
            ret += (1 + quote_bool);
            column++;
        }
        if (i + test <= tab_len) {
            local_space[column] = get_max_len_by_index(i + test, test, tab_len, all_len);
            ret += local_space[column];
            ret += (1 + quote_bool);
            column++;
        }
        else
            break ;
        i += test;
    }
    ret += 2; // brut pad value
    return (ret);
}

static int test_all(int test, int* all_len, int tab_len, int stdout_w, int bool_quote)
{
    int *local_space = ft_calloc(sizeof(int), tab_len);
    int ret = -1;
    int i = 0;
    while (i < test) {
        ret = brut_test(i, test, tab_len, all_len, local_space, bool_quote);
        // printf("Ret = %d for [%d] width: %d\n", ret, test, stdout_w);
        if (ret >= stdout_w - 2)
            break ;
        i++;
    }
    if (ret >= stdout_w - 2)
        ret = -1;
    else
        ret = 0;
    free(local_space);
    return (ret);
}

/** get_nb_line
 * Start brut force with test value, test if we can display all one 2 line if not increment test
 * args:    stdout_w: width of stdout, 
 *          lst: ptr on linked list to display 
 * ret: number of line
*/
static int get_nb_line(int stdout_w, int *all_len, int len, int bool_quote)
{
    if (!all_len)
        return (MALLOC_ERR);
    int test = 2; // test value for nb_line, brute force it
    int ret = -1;

    if (stdout_w <= 0)
        stdout_w = 80;

    while (ret != 0) {
        // ft_printf_fd(2, "width %d\n", stdout_w);
        ret = test_all(test, all_len, len, stdout_w, bool_quote);
        test++;
    }
    return (test - 1);
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
 * Entry point, compute and return double char ** (bad just need to return index tab/lst -> int**)
*/
char **check_manage_colum(t_list *lst, int *err, int *value, int space_quote)
{
    char    **tab = NULL; /* go to int */
    // int     **array = NULL; 
    
    int     stdout_width = get_stdout_width(), nb_line = 0, max_per_line = 1, lst_len = get_lst_len(lst);
    int     *tab_max_unit = NULL, *all_len = get_all_len(lst, lst_len);

    if (!all_len){
        ft_printf_fd(2, "Error alloc all_len manage column\n");
        return (NULL);
    }
    nb_line = get_nb_line(stdout_width, all_len, lst_len, space_quote);
    if (nb_line == MALLOC_ERR) {
        *err = MALLOC_ERR;
        return (NULL);
    }

    *value = nb_line; // second return need to kept nb_line
    max_per_line  = (int)(lst_len / nb_line) + (lst_len % nb_line != 0); // add 1 if (lst_len % nb_line != 0)
    
    if (get_total_len(lst, space_quote) > (long long)stdout_width) { /* not necesary ? always call ? */
        tab_max_unit = get_max_by_column(lst, max_per_line, nb_line);
        if (!tab_max_unit) {
            free(all_len);
            *err = MALLOC_ERR;
            return (NULL);
        }
        /* new call for int ** here */
        // array = create_column_array(lst, tab_max_unit, max_per_line, nb_line, space_quote);
        tab = manage_column(lst, tab_max_unit, max_per_line, nb_line, space_quote);
        if (!tab)
            *err = MALLOC_ERR;
        free(tab_max_unit);
    }

    free(all_len);
    
    return (tab);
}

/* Protection after max_per_raw affectation, not sure is mandatory
        if (max_per_raw <= 0)
            max_per_raw = 1; */