#include "../include/ft_ls.h"

/**
* 00000000 -> Our current flag value is 0
* | 00000100 -> Do or operation with Flag3, which has a value of 4
* = 00000100 -> The bit for Flag3 gets set to 1, flag value is now 4
* Enable target flag
*/
void set_flag(int *flags, int flag_val)
{
    if (flags)
        *flags |= flag_val;
}

/**
* 00000100 -> We get Flag3 as input to unset
* ~= 11111011 -> We flip all the bits of Flag3
* 00010110 -> Our current value had Flag2, 3 and 5 set already
* &  11111011 -> AND operation with the result of previous inversion
* =  00010010 -> Our new value only has Flag2 and 5 set
* Disable target flag
*/
void unset_flag(int *flags, int flag_val)
{
    if (flags)
        *flags &= ~flag_val;
}

/*
* 00010010 -> Our current value has Flag2 and Flag5 set
* ^ 00000100 -> Perform XOR with Flag3
* = 00010110 -> New value has Flag3 set
-----------------------------------------------------------------------
* 00010110 -> Our current value has Flag2, Flag3 and Flag5 set
* ^ 00000100 -> Perform XOR with Flag3
* = 00010010 -> New value does not have Flag3 set
* Flip target flag if == 1 -> 0 and vice versa
*/

void flip_flag(int *flags, int flag_val)
{
    if (flags)
        *flags ^= flag_val;
}

/*
* 00010110 -> Starting value has Flag2, Flag3 and Flag5 set
* & 00000100 -> Perform & with Flag3
* = 00000100 -> Result is equal to Flag3
* -----------------------------------------------------------
* 00010010 -> Starting value has Flag2 and Flag5 set
* & 00000100 -> Perform & with Flag3
* = 00000000 -> Result is equal to 0
* check if flag_val enable in flags
*/
int has_flag(int flags, int flag_val)
{
   return ((flags & flag_val) == flag_val);
}

/**
 * check if flag_val enable in flags give flag_val with flag1 | flag2 ... 
*/
int has_any_flag(int flags, int flag_val)
{
    return (flags & flag_val) != 0;
}

static int manage_bonus_option(int nb)
{
    if (nb & G_OPTION && !(nb & L_OPTION))
        nb += L_OPTION;
    if (nb & N_OPTION && !(nb & L_OPTION))
        nb += L_OPTION;
    if (nb & F_OPTION)
    {
        if (!(nb & A_OPTION))
            nb += A_OPTION;
        if (nb & COLOR_OPTION)
            nb -= COLOR_OPTION;
    }

    // if (has_flag(nb, G_OPTION) && !has_flag(nb, L_OPTION))
    //     set_flag(&nb, L_OPTION);

    return (nb);
}

int get_flag(enum e_flag *flag)
{
    int i = 0;
    int nb = 0;
    while (flag && flag[i] != UNKNOW)
    {
        nb += flag[i];
        // set_flag(&nb, flag[i]);
        i++;
    }
    nb = manage_bonus_option(nb);
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


static enum e_flag fill_used_flag(enum e_flag *tab, enum e_flag flag)
{
    int i = 0;
    while (tab && tab[i] != UNKNOW)
        i++;
    tab[i] =  flag;
    return (flag);
}
/**
 * Need to be rtefactor, no need to store in array just check flags value 
*/
int flag_already_add(char c, t_eflag *used)
{
    int i = 0;
    int option = 0;
    while (i < NB_FLAG)
    {
        if (c == ALL_FLAG[i])
        {
            option = (1 << i);
            if (already_add(used, option) == 1)
                return (1);
        }
        i++;
    }
    return (0);
}

int add_flag(char c, enum e_flag *used)
{
    int i = 0;
    int option = 0;
    if (flag_already_add(c, used) == 1)
        return (0);
    while (i < NB_FLAG)
    {
        if (c == ALL_FLAG[i])
        {
            option = (1 << i);
            return (fill_used_flag(used, option) == 1);
        }
        i++;
    }
    ft_printf_fd(2, "ft_ls: unrecognized option %c\n", c);
    return (-1);
}

static int check_for_add_flag(char *str, enum e_flag *used)
{
    int i;

    i = 1;
    while (str && str[i])
    {
        int check = add_flag(str[i], used);
        if (check == -1)
            return (1);
        i++;
    }  
    return (0);
}

enum e_flag *parse_flag(int argc, char **argv, enum e_flag *used)
{
    int i = 0;
    while (i < 6)
    {
        used[i] = UNKNOW;
        i++;
    }
    i = 0;
    while (i < argc)
    {
        if (argv[i][0] == '-')
        {
            if (argv[i][1] == '\0')
                ft_printf_fd(2, "ft_ls: cannot access '%s': No such file or directory\n", argv[i]); // special case
            if (check_for_add_flag(argv[i], used) == 1)
                return (NULL);
        }
        i++;
    }
    return(used);
}

enum e_flag *check_for_flag(int argc, char **argv)
{
    enum e_flag *used = ft_calloc(sizeof(int), NB_FLAG);
    if (!used)
    {
        perror("Malloc");
        return (NULL);
    }
    void *flag_ptr = used;
    used = parse_flag(argc, argv, used);
    if (!used)
    {
        free(flag_ptr);
        return (NULL);
    }
    return (used);
}

// int flag_already_add(char c, enum e_flag *used)
// {
//     if (c == L_FLAG_CHAR && already_add(used, L_OPTION) == 1)
//         return (1);
//     else if (c == R_FLAG_CHAR && already_add(used, R_OPTION) == 1)
//         return (1);
//     else if (c == REVERSE_FLAG_CHAR && already_add(used, REVERSE_OPTION) == 1)
//         return (1);
//     else if (c == A_FLAG_CHAR && already_add(used, A_OPTION) == 1)
//         return (1);
//     else if (c == T_FLAG_CHAR && already_add(used, T_OPTION) == 1)
//         return (1);
//     else if (c == ACL_EXT_Z_FLAG && already_add(used, Z_OPTION) == 1)
//         return (1);
//     else if (c == U_FLAG_CHAR && already_add(used, U_OPTION) == 1)
//         return (1);
//     else if (c == C_FLAG_CHAR && already_add(used, C_OPTION) == 1)
//         return (1);
//     else if (c == G_FLAG_CHAR && already_add(used, G_OPTION) == 1)
//         return (1);
//     else if (c == F_FLAG_CHAR && already_add(used, F_OPTION) == 1)
//         return (1);
//     else if (c == D_FLAG_CHAR && already_add(used, D_OPTION) == 1)
//         return (1);
//     else if (c == N_FLAG_CHAR && already_add(used, N_OPTION) == 1)
//         return (1);
//     return (0);
// }

// int add_flag(char c, enum e_flag *used)
// {
//     if (flag_already_add(c, used) == 1)
//         return (0);
//     if (c == L_FLAG_CHAR)
//         return (fill_used_flag(used, L_OPTION));
//     else if (c == R_FLAG_CHAR)
//         return (fill_used_flag(used, R_OPTION));
//     else if (c == REVERSE_FLAG_CHAR)
//         return (fill_used_flag(used, REVERSE_OPTION));
//     else if (c == A_FLAG_CHAR)
//         return (fill_used_flag(used, A_OPTION));
//     else if (c == T_FLAG_CHAR)
//         return (fill_used_flag(used, T_OPTION));
//      else if (c == ACL_EXT_Z_FLAG)
//         return (fill_used_flag(used, Z_OPTION));
//     else if (c == U_FLAG_CHAR)
//         return (fill_used_flag(used, U_OPTION));
//     else if (c == C_FLAG_CHAR)
//         return (fill_used_flag(used, C_OPTION));
//     else if (c == G_FLAG_CHAR)
//         return (fill_used_flag(used, G_OPTION));
//     else if (c == F_FLAG_CHAR)
//         return (fill_used_flag(used, F_OPTION));
//     else if (c == D_FLAG_CHAR)
//         return (fill_used_flag(used, D_OPTION));
//     else if (c == N_FLAG_CHAR)
//         return (fill_used_flag(used, N_OPTION));
//     ft_printf_fd(2, "ft_ls: unrecognized option %c\n", c);
//     return (-1);
// }