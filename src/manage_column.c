#include "../include/ft_ls.h"

/** Alloc array
 * Alloc int ** array to contain orderer file id to display 
 * Ret: ptr on allocated double array
*/
static int **alloc_array(int nb_line, int max_per_line)
{
    int **array = ft_calloc(sizeof(int *), nb_line + 1); /* +1 for NULL*/
    if (!array)
        return (NULL);
    for (int i = 0; i < nb_line; i++)
    {
        array[i] = ft_calloc(sizeof(int), max_per_line + 1);
        if (!array[i]){
            free_incomplete_array((void **)array, i);
            return (NULL);
        }
        array[i][max_per_line] = -1; /* Borne value */
    }
    return (array);
}

/** Create_column_array
 * Create double int array with id of each file, with the brut force column information
 * Args:    lst : ptr on list to display
 *          nb_column_max : max name per line (nb max column)
 *          nb_line: nb of line
 * Ret: ptr on allocated double int array
*/
int **create_column_array (t_list *lst, int nb_column_max, int nb_line)
{
    int file_idx = 0, current_line = 0, current_column = 0;
    int **array = alloc_array(nb_line, nb_column_max);
    if (!array)
        return (NULL);
    while (lst) {
        array[current_line][current_column] = file_idx;
        ++current_line;
        if (current_line == nb_line) {
            ++current_column;
            current_line = 0;
        }
        ++file_idx;
        lst = lst->next;
    }
    return (array);
}