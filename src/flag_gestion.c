#include "../include/ft_ls.h"

void display_flags(int flags);

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

t_int8 flag_already_present(int flags, int flag_val)
{
    return (has_flag(flags, flag_val) ? TRUE : FALSE);
}



static int manage_bonus_flag(int nb)
{
    if ((has_flag(nb, G_OPTION) && !has_flag(nb, L_OPTION))
    || (has_flag(nb, N_OPTION) && !has_flag(nb, L_OPTION)))
        set_flag(&nb, L_OPTION);
    if (has_flag(nb, F_OPTION)) {
        if (!has_flag(nb, A_OPTION))
            set_flag(&nb, A_OPTION);
        if (has_flag(nb, COLOR_OPTION))
            unset_flag(&nb, COLOR_OPTION);
    }
    return (nb);
}

int get_flag_value(char c) 
{
    int i = 0;
    int flag = -1;

    while (i < NB_FLAG) {
        if (c == ALL_FLAG[i]) {
            flag = (1 << i);
            break;
        }
        ++i;
    }
    return (flag);
}


/* PUBLIC MAIN */

int parse_flag(int argc, char **argv, int *special_err)
{
    int i = 1, flags = 0, tmp_value = 0;

    while (i < argc)
    {
        if (argv[i][0] == '-') {
            if (argv[i][1] == '\0') {   /* special case ugly */
                ft_printf_fd(2, "ft_ls: cannot access '%s': No such file or directory\n", argv[i]); // special case
                *special_err = 1;
            }
            else if (argv[i][1] == '-' && argv[i][2] == '\0') {
                ++i;
                continue;
            }
            else {
                for (int j = 1; argv[i][j]; ++j) 
                {
                    tmp_value =  get_flag_value(argv[i][j]);
                    if (tmp_value == -1) {
                        ft_printf_fd(2, "ft_ls: invalid option -- '%c'\nTry './ft_ls --help' for more information.\n", argv[i][j]);
                        return (-1);
                    }
                    if (flag_already_present(flags, tmp_value) == FALSE)
                        set_flag(&flags, tmp_value);
                }
            }
                
        }
        ++i;
    }
    flags = manage_bonus_flag(flags);
    // display_flags(flags);
    return (flags);
}


void display_flags(int flags) {
    ft_printf_fd(2, "Flags: [%d] : ", flags);
    if (has_flag(flags, L_OPTION))
        ft_printf_fd(2, "%s[%s]%s, ", GREEN, "L_OPTION", RESET);
    if (has_flag(flags, A_OPTION))
        ft_printf_fd(2, "%s[%s]%s, ", YELLOW, "A_OPTION", RESET);
    if (has_flag(flags, T_OPTION))
        ft_printf_fd(2, "%s[%s]%s, ", RED, "T_OPTION", RESET);
    if (has_flag(flags, REVERSE_OPTION))
        ft_printf_fd(2, "%s[%s]%s, ", PURPLE, "REVERSE_OPTION", RESET);
    if (has_flag(flags, R_OPTION))
        ft_printf_fd(2, "%s[%s]%s, ", GREEN, "R_OPTION", RESET);
    if (has_flag(flags, Z_OPTION))
        ft_printf_fd(2, "%s[%s]%s, ", YELLOW, "Z_OPTION", RESET);
    if (has_flag(flags, U_OPTION))
        ft_printf_fd(2, "%s[%s]%s, ", RED, "U_OPTION", RESET);
    if (has_flag(flags, C_OPTION))
        ft_printf_fd(2, "%s[%s]%s, ", PURPLE, "C_OPTION", RESET);
    if (has_flag(flags, G_OPTION))
        ft_printf_fd(2, "%s[%s]%s, ", GREEN, "G_OPTION", RESET);
    if (has_flag(flags, F_OPTION))
        ft_printf_fd(2, "%s[%s]%s, ", YELLOW, "F_OPTION", RESET);
    if (has_flag(flags, D_OPTION))
        ft_printf_fd(2, "%s[%s]%s, ", RED, "D_OPTION", RESET);
    if (has_flag(flags, N_OPTION))
        ft_printf_fd(2, "%s[%s]%s, ", PURPLE, "N_OPTION", RESET);
    if (has_flag(flags, COLOR_OPTION))
        ft_printf_fd(2, "%s[%s]%s, ", GREEN, "COLOR_OPTION", RESET);
    ft_printf_fd(2, "\n");
}


/*--------------------------------------OLD FLAG GESTION----------------------------------------------------*/
// int get_flag(enum e_flag *flag)
// {
//     int i = 0;
//     int flags = 0;
//     while (flag && flag[i] != UNKNOW)
//     {
//         // flags += flag[i];
//         set_flag(&flags, flag[i]);
//         i++;
//     }
//     flags = manage_bonus_flag(flags);
//     return (flags);
// }

// int already_add(enum e_flag *tab, enum e_flag to_check)
// {
//     int i = 0;
//     while (i < 6)
//     {
//         if (tab[i] == to_check)
//             return (1);
//         i++;
//     }
//     return (0);
// }


// static enum e_flag fill_used_flag(enum e_flag *tab, enum e_flag flag)
// {
//     int i = 0;
//     while (tab && tab[i] != UNKNOW)
//         i++;
//     tab[i] =  flag;
//     return (flag);
// }


/**
 * Need to be rtefactor, no need to store in array just check flags value 
*/
// int flag_already_add(char c, t_eflag *used)
// {
//     int i = 0;
//     int option = 0;
//     while (i < NB_FLAG)
//     {
//         if (c == ALL_FLAG[i])
//         {
//             option = (1 << i);
//             if (already_add(used, option) == 1)
//                 return (1);
//         }
//         i++;
//     }
//     return (0);
// }


// int add_flag(char c, enum e_flag *used)
// {
//     int i = 0;
//     int option = 0;
//     if (flag_already_add(c, used) == 1)
//         return (0);
//     while (i < NB_FLAG)
//     {
//         if (c == ALL_FLAG[i])
//         {
//             option = (1 << i);
//             return (fill_used_flag(used, option) == 1);
//         }
//         i++;
//     }
    // ft_printf_fd(2, "ft_ls: unrecognized option %c\n", c);
    // return (-1);
// }

// static int check_for_add_flag(char *str, enum e_flag *used)
// {
//     int i;

//     i = 1;
//     while (str && str[i])
//     {
//         int check = add_flag(str[i], used);
//         if (check == -1)
//             return (1);
//         i++;
//     }  
//     return (0);
// }

// enum e_flag *parse_flag(int argc, char **argv, enum e_flag *used)
// {
//     int i = 0;
//     while (i < 6)
//     {
//         used[i] = UNKNOW;
//         i++;
//     }
//     i = 0;
//     while (i < argc)
//     {
//         if (argv[i][0] == '-')
//         {
//             if (argv[i][1] == '\0')
//                 ft_printf_fd(2, "ft_ls: cannot access '%s': No such file or directory\n", argv[i]); // special case
//             if (check_for_add_flag(argv[i], used) == 1)
//                 return (NULL);
//         }
//         i++;
//     }
//     return(used);
// }

// enum e_flag *check_for_flag(int argc, char **argv)
// {
//     enum e_flag *used = ft_calloc(sizeof(int), NB_FLAG);
//     if (!used)
//     {
//         perror("Malloc");
//         return (NULL);
//     }
//     void *flag_ptr = used;
//     used = parse_flag(argc, argv, used);
//     if (!used)
//     {
//         free(flag_ptr);
//         return (NULL);
//     }
//     return (used);
// }


/*--------------------------------------END----------------------------------------------------*/


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