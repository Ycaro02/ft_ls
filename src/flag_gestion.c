#include "../include/ft_ls.h"

static int manage_bonus_flag(int nb)
{
    /* if g || n option need to enable L too*/
    if ((has_flag(nb, G_OPTION) && !has_flag(nb, L_OPTION))
    || (has_flag(nb, N_OPTION) && !has_flag(nb, L_OPTION)))
        set_flag(&nb, L_OPTION);
    /* f option disable a and color */
    if (has_flag(nb, F_OPTION)) {
        if (!has_flag(nb, A_OPTION))
            set_flag(&nb, A_OPTION);
        if (has_flag(nb, COLOR_OPTION))
            unset_flag(&nb, COLOR_OPTION);
    }
    return (nb);
}

static int get_flag_value(char c) 
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

    while (i < argc) {
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
                for (int j = 1; argv[i][j]; ++j) {
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
    return (flags);
}