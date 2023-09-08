#include "../ft_ls.h"

// enum e_flag  {
//     UNKNOW=-1,
//     L_OPTION=1,
//     R_OPTION=2,
//     REVERSE_OPTION=4,
//     A_OPTION=8,
//     T_OPTION=10,
// };


// int flag_gestion(char **lst, int flag)
// {
//       if (flag == 0)
//         classic_ls(lst);
//     else if (flag == L_OPTION)
//         reverse_ls(lst);
//     else if (flag == R_OPTION)
//         reverse_ls(lst);
//     else if (flag == REVERSE_OPTION)
//         reverse_ls(lst);
//     else if (flag == A_OPTION)
//         reverse_ls(lst);
//     else if (flag == T_OPTION)
//         reverse_ls(lst);
//     return (-1);
// }

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

void fill_used_flag(enum e_flag *tab, enum e_flag flag)
{
    int i = 0;
    while (tab && tab[i] != UNKNOW)
        i++;
    tab[i] =  flag;
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

int check_flag(char c, enum e_flag *used)
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

int add_flag(char c, enum e_flag *used)
{
    int tmp = check_flag(c, used);
    if (tmp == 1)
        return (0);
    if (c == L_FLAG_CHAR && already_add(used, L_OPTION) == 0)
    {
        fill_used_flag(used, L_OPTION);
        return (L_OPTION);
    }
    else if (c == R_FLAG_CHAR && already_add(used, R_OPTION) == 0)
    {
        fill_used_flag(used, R_OPTION);
        return (R_OPTION);
    }
    else if (c == REVERSE_FLAG_CHAR && already_add(used, REVERSE_OPTION) == 0)
    {
        fill_used_flag(used, REVERSE_OPTION);
        return (REVERSE_OPTION);
    }
    else if (c == A_FLAG_CHAR && already_add(used, A_OPTION) == 0)
    {
        fill_used_flag(used, A_OPTION);
        return (A_OPTION);
    }
    else if (c == T_FLAG_CHAR && already_add(used, T_OPTION) == 0)
    {
        fill_used_flag(used, T_OPTION);
        return (T_OPTION);
    }
    else
        printf("\nError end parsing flag, invalid flag found %c\n", c);
    for (int i = 0; i < 6; i++)
        printf("used de [%d] = %d\n", i, used[i]);
    return -1;
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