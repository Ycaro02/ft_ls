#include "../include/ft_ls.h"

// void convert_ato(char* perm, int nbr, int index) // array to octal
// {
//         int             b_size = 8;
//         long int        n = nbr;
//         char            *base = "01234567";

//         index -= 1;
//         if (n / b_size != 0)
//             convert_ato(perm, n / b_size, index);
//         perm[index] = base[n % b_size];
// }

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