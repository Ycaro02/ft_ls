#include "../ft_ls.h"

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
        }
        else if (str[i] == c && find == 1)
            break;
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
    while (nb != 0)
    {
        str[i] = '\0';
        nb--;
        i--;
    }
}

char *get_printable_date(time_t *time, int old)
{
    char *str = ctime(time);
    char * new = NULL;
    if (old == 0)
    {
        new = str_trim_patern(str_trim_patern(str, ' ', 1, 0), ' ', 3, 1);
        if (!new)
            return (NULL);
        remove_last_nchar(new, 3);
        return (new);
    }
    new = str_trim_patern(str_trim_patern(str, ' ', 3 , 0), ' ', 1, 1);
    if (!new)
        return (NULL);
    return (new);
}


// int main (int argc, char **argv)
// {

//     int i = 1;
//     struct stat     sb;
//     while (i < argc)
//     {
//         lstat(argv[i], &sb);
//         if ((sb.st_mode & S_IFMT) != S_IFDIR)
//         {
//             printf("Not a directory");
//             return (1);
//         }
        
//         char *str = get_printable_date(&sb.st_mtime, 0);
//         printf("m_time str = |%s|for %s\n", str, argv[i]);
//         free(str);
//         str = get_printable_date(&sb.st_mtime, 1);
//         printf("m_time str = |%s|for %s\n", str, argv[i]);
//         free(str);
//         i++;
//     }
        
//     return (0);
// }

// static char *str_trim_last(char* str, char c)
// {
//     int len = ft_strlen(str);
//     len--;
//     while (str && str[len])
//     {
//         if (str[len] == c)
//             break;
//         len--;
//     }
//     int i = 0;
//     char *new = malloc(sizeof(char) * len + 1);
//     while (i < len)
//     {
//         new[i] = str[i];
//         i++;
//     }
//     new[i] = '\0';
//     free(str);
//     return (new);
// }



// char *str_trim_first_last(char* str, char c, int flag)
// {
//     int i = 0;
//     while (str && str[i])
//     {
//         if (str[i] == c)
//             break;
//         i++;
//     }
//     char *new = ft_strdup(&str[i + 1]);
//     if (flag == 0)
//         new = str_trim_last(new, c);
    
//     return (new);
// }