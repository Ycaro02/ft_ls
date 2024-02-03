#include "../include/ft_ls.h"

inline static void read_write_perm(mode_t mode, char *str, int first, int second)
{
    if (mode & first)
        str[0] = 'r';
    if (mode & second)
        str[1] = 'w';
}

inline static char detect_exe_perm(mode_t mode, int first, int second)
{
    char exec = '-';
    char special = 's';

    if (second == __S_ISVTX)
        special = 't';
    if (mode & first)
        exec = 'x';
    if (mode & second) {
        if (exec == 'x')
            exec = special;
        else
            exec = special - 32;
    }
    return (exec);
}

/**
 * Convert mode_t to allocated char *
*/
char *perm_to_string(mode_t mode, char type)
{
    char *perm = ft_calloc(sizeof(char), 11);
    if (!perm)
        return (NULL);

    for (int i = 0; i < 11; ++i)
        perm[i] = '-';
    perm[10] = '\0';
    perm[0] = type;

    /* Read/Write by owner */
    read_write_perm(mode, &perm[1], S_IRUSR, S_IWUSR);
    /* Execute by owner and Set User ID exe */
    perm[3] = detect_exe_perm(mode, S_IXUSR, S_ISUID);
    /* Read/Write by group */
    read_write_perm(mode, &perm[4], S_IRGRP, S_IWGRP);
    /* Execute by group and Set Group ID exe */
    perm[6] = detect_exe_perm(mode, S_IXGRP, S_ISGID);
    /* Read/Write by other */
    read_write_perm(mode, &perm[7], S_IROTH, S_IWOTH);
    /* Execute by other and Sticky (save swaped text) */
    perm[9] = detect_exe_perm(mode, S_IXOTH, __S_ISVTX);
    return (perm);
}

/**
 * Check if chmod 777
*/
t_int8 is_full_perm(mode_t mode)
{
    return (has_flag(mode, S_IRWXO) && has_flag(mode, S_IRWXG) && has_flag(mode, S_IRWXU));
}

/**
 * Check if one of x perm is enable
*/
t_int8 is_executable_file(mode_t mode)
{
    return ((mode & S_IXOTH || mode & S_IXGRP || mode & S_IXUSR));
}