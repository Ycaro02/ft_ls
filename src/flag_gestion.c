#include "../ft_ls.h"

int get_flag(enum e_flag *flag)
{
    int i = 0;
    int nb = 0;
    while (flag && flag[i] != UNKNOW)
    {
        nb += flag[i];
        i++;
    }   
    return (nb);
}


int already_add(enum e_flag *tab, enum e_flag to_check)
{
    int i = 0;
    while (i < 6)
    {
        if (tab[i] == to_check)
            return (1);
        i++;
    }
    return (0);
}

int flag_already_add(char c, enum e_flag *used)
{
    if (c == L_FLAG_CHAR && already_add(used, L_OPTION) == 1)
        return (1);
    else if (c == R_FLAG_CHAR && already_add(used, R_OPTION) == 1)
        return (1);
    else if (c == REVERSE_FLAG_CHAR && already_add(used, REVERSE_OPTION) == 1)
        return (1);
    else if (c == A_FLAG_CHAR && already_add(used, A_OPTION) == 1)
        return (1);
    else if (c == T_FLAG_CHAR && already_add(used, T_OPTION) == 1)
        return (1);
    return (0);
}

static void put_flag_error(char c)
{
   ft_putstr_fd("\nft_ls: unrecognized option ", 2);
    char cc[2];
    cc[0] = c;
    cc[1] = '\0';
    ft_putstr_fd(cc, 2);
    ft_putstr_fd("\n", 2);
}

static enum e_flag fill_used_flag(enum e_flag *tab, enum e_flag flag)
{
    int i = 0;
    while (tab && tab[i] != UNKNOW)
        i++;
    tab[i] =  flag;
    return (flag);
}

int add_flag(char c, enum e_flag *used)
{
    int tmp = flag_already_add(c, used);
    if (tmp == 1)
        return (0);
    if (c == L_FLAG_CHAR)
        return (fill_used_flag(used, L_OPTION));
    else if (c == R_FLAG_CHAR)
        return (fill_used_flag(used, R_OPTION));
    else if (c == REVERSE_FLAG_CHAR)
        return (fill_used_flag(used, REVERSE_OPTION));
    else if (c == A_FLAG_CHAR)
        return (fill_used_flag(used, A_OPTION));
    else if (c == T_FLAG_CHAR)
        return (fill_used_flag(used, T_OPTION));
    put_flag_error(c);
    return (-1);
}

enum e_flag *parse_flag(char **argv, enum e_flag *used)
{
    int i = 0;
    while (i < 6)
    {
        used[i] = UNKNOW;
        i++;
    }
    i = 0;
    while (argv && argv[i])
    {
        if (argv[i][0] == '-')
        {
            int j = 1;
            if (argv[i][1] == '\0')
                printf("ft_ls: cannot access '%s': No such file or directory\n", argv[i]); // special case
            while (argv[i] && argv[i][j])
            {
                int check = add_flag(argv[i][j], used);
                if (check == -1)
                    return (NULL);
                j++;
            }
        }
        i++;
    }
    return(used);
}

enum e_flag *check_for_flag(char **argv)
{
    enum e_flag *used = malloc(sizeof(int) * 6);
    if (!used)
    {
        ft_putstr_fd("Error malloc failed\n", 2);
        return (NULL);
    }
    void *flag_ptr = used;
    used = parse_flag(argv, used);
    if (used == NULL)
    {
        free(flag_ptr);
        return (NULL);
    }
    return (used);
}