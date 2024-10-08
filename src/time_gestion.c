#include "../include/ft_ls.h"

static int get_patern_index(char *str, int *start, int pos, char c)
{
    int find = 0;
    int count = 0;
    int i = 0;
    if (pos == 1)
        find = 1;
    while (str && str[i])
    {
        if (str[i] == c && find == 0)
        {
            count++;
            if (count == pos)
            {
                *start = i;
                find = 1;
            }
            i++;
            while (str[i] == c)
                i++;
        }
        else if (str[i] == c && find == 1)
            break;
        else
            i++;
    }
    return (i);
}

static char *str_trim_patern(char *str, char c, int pos, int to_free)
{
    int start = 0;
    int stop = 0;

    stop = get_patern_index(str, &start, pos, c);
    int len = ft_strlen(str) - (stop - start);
    char *new = malloc(sizeof(char) * len + 1);
    int i = 0;
    int j = 0;
    if (pos != 1)
        start++;
    while (i < len)
    {
        if (j < start || (j > start && j > stop))
        {
            if (str[j] != '\n')
            {
                new[i] = str[j];
                i++;
            }
        }
        j++;
    }
    new[i] = '\0';
    if (to_free != 0)
        free(str);
    return (new);
}

static void remove_last_nchar(char* str, int nb)
{
    int len = ft_strlen(str);
    int i = len - 1;
    while (nb != 0) {
        str[i] = '\0';
        nb--;
        i--;
    }
}
/**
 * Check if file is older than 6 month or more than 6 moth in future
 * (current - six_month) > last_change : older than 6 month
 * (current + six_month) < last_change: more than 6 month in futurre
*/
int check_six_month(time_t last_change)
{
    time_t current = time(NULL);
    time_t six_month = (60 * 60 * 24 * 30 * 3) + (60 * 60 * 24 * 31 * 3);

    if ((current - six_month) > last_change || (current + six_month) < last_change)
        return (OLD);
    return (NEW);
}

char **get_printable_date(t_timespec last_change)
{
    char *str = ctime(&last_change.tv_sec);
    char * new = NULL;
    int old;

    old = check_six_month(last_change.tv_sec);
    if (old == NEW) {
        new = str_trim_patern(str_trim_patern(str, ' ', 1, 0), ' ', 3, 1);
        if (!new)
            return (NULL);
        remove_last_nchar(new, 3);
    }
    else
        new = str_trim_patern(str_trim_patern(str, ' ', 3 , 0), ' ', 1, 1);
    if (!new)
        return (NULL);
    char **tab;
    tab = ft_split(new, ' ');
    free(new);
    return (tab);
}