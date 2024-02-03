#include "../include/basic_define.h"
#include "../include/define_enum.h"
#include "../libft/libft.h"

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
* 00010110 -> Starting value has Flag2, Flag3 and Flag5 set
* & 00000100 -> Perform & with Flag3
* = 00000100 -> Result is equal to Flag3
* check if flag_val enable in flags
// __always_inline int has_flag(int flags, int flag_val)
*/

t_int8 has_flag(int flags, int flag_val) {
   return ((flags & flag_val) == flag_val);
}

t_int8 flag_already_present(int flags, int flag_val) {
    return (has_flag(flags, flag_val));
}

void display_flags(int flags) 
{
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

/*
* 00010110 -> Our current value has Flag2, Flag3 and Flag5 set
* ^ 00000100 -> Perform XOR with Flag3
* = 00010010 -> New value does not have Flag3 set
* Flip target flag if == 1 -> 0 and vice versa
*/
// void flip_flag(int *flags, int flag_val)
// {
//     if (flags)
//         *flags ^= flag_val;
// }
/**
 * check if flag_val enable in flags give flag_val with flag1 | flag2 ... 
*/
// int has_any_flag(int flags, int flag_val)
// {
//     return (flags & flag_val) != 0;
// }
