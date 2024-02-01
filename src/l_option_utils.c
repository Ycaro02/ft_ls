#include "../include/ft_ls.h"

void convert_ato(char* perm, int nbr, int index) // array to octal
{
        int             b_size = 8;
        long int        n = nbr;
        char            *base = "01234567";

        index -= 1;
        if (n / b_size != 0)
            convert_ato(perm, n / b_size, index);
        perm[index] = base[n % b_size];
}

static char *remove_char(char *str, char c, int *err)
{
    int i = 0;
    char* tmp;

    tmp = NULL;
    while (str && str[i] && str[i] == c)
        i++;
    if (i < 3 && i != 0) {
        tmp = ft_strdup(&str[i]);
        if (!tmp) {
            *err = MALLOC_ERR;
            return (NULL);
        }
        free(str);
    }
    return (tmp);
}

char *get_perm(int nbr)
{
    char *perm;
    int   err;

    err = 0;
    perm = malloc(sizeof(char) * 4);
    if (!perm)
        return (NULL);
    perm[0] = '8'; 
    perm[1] = '8'; 
    perm[2] = '8'; 
    perm[3] = '\0';
    convert_ato(perm, nbr, 3);
    char *tmp = remove_char(perm, '8', &err);
    if (err == MALLOC_ERR)
        return (NULL);
    if (tmp)
        return (tmp);
    return (perm);
}

/*
 * fill buffer with permision take flag for no display
*/
void fill_buffer_perm(char c, int *is_exec, int display_flag)
{
    int nb = c - 48;
    char r = '-';
    char w = '-';
    char x = '-';

    // char spe = '-';

    if (nb & 4)
        r = 'r';
    if (nb & 2)
        w = 'w';
    if (nb & 1) {
        x = 'x';
        *is_exec = 0;   
    }
    if (display_flag == 1) {
        fill_buffer_char(r);
        fill_buffer_char(w);
        fill_buffer_char(x);
    }
}

char get_type(struct stat sb)
{
    switch (sb.st_mode & S_IFMT) 
    {
        case S_IFBLK:  
            return (BLOCK);
        case S_IFCHR:  
            return (CHARACTER);
        case S_IFDIR:
            return (DIRECTORY);
        case S_IFIFO:  
            return (FIFO);
        case S_IFLNK:  
            return (SYMLINK);
        case S_IFREG:  
            return (REGULAR);
        case S_IFSOCK: 
            return (SOCKET);
        default:
            return (UNDIFINED);
    }
    return (UNDIFINED);
}