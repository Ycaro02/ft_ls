#include "../ft_ls.h"


int is_point_dir(char *path)
{
    if (path && path[0] == '.')
        return (0);
    return (1);
}

static void     free_str_join(char *s1, char *s2, char option)
{
        if (option == 'f' || option == 'a')
                free(s1);
        if (option == 's' || option == 'a')
                free(s2);
}

char    *ft_strjoin_free(char *s1, char *s2, char option)
{
        int             len_new_s;
        char    *new_s;
        int             i;
        int             j;

        len_new_s = ft_strlen(s1) + ft_strlen(s2);
        new_s = malloc((len_new_s + 1) * sizeof(char));
        if (!new_s)
        {
                free_str_join(s1, s2, option);
                return (NULL);
        }
        i = 0;
        while (s1 && s1[i])
        {
                new_s[i] = s1[i];
                i++;
        }
        j = 0;
        while (s2 && s2[j] && i < len_new_s)
                new_s[i++] = s2[j++];
        free_str_join(s1, s2, option);
        new_s[i] = '\0';
        return (new_s);
}

void free_lst(t_list *lst)
{
    if (lst == NULL)
        return ;
    t_list *tmp = lst;
    while (tmp)
    {
        tmp = lst->next;
        free(lst->content);
        free(lst);
    }
}

int lower_strcmp(char *s1, char *s2)
{
    int i = 0;
    char first = '\0';
    char two = '\0';
    while (s1 && s1[i] && s2 && s2[i])
    {
        first = ft_tolower(s1[i]);
        two = ft_tolower(s2[i]);
        if (first != two)
            return (first - two);
        i++;
    }
    if (s1[i] != '\0' || s2[i] != '\0')
        return (s1[i] - s2[i]);
    return (0);
}

int      is_directory(const char *path)
{
        struct stat     sb;

        lstat(path, &sb);
        if ((sb.st_mode & S_IFMT) != S_IFDIR)
                return (1);
        return (0);
}