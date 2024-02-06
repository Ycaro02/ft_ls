#include "../include/ft_ls.h"

t_buff g_buff;

static int check_display_help(int argc, char**argv)
{
    if (argc >= 2) {
        if (ft_strcmp(argv[1], "--help") == 0) {
            ft_printf_fd(1, HELP_STR);
            return (0);
        }
    }
    return (1);
}

int main (int argc, char **argv)
{
    t_context c;

    ft_bzero(&c, sizeof(t_context));

    if (check_display_help(argc, argv) == 0)
        return (0);
    ft_bzero(g_buff.buffer, BUFFER_LEN - 1);
    c.flag_nb = parse_flag(argc, argv, &c.special_error);
    if (c.flag_nb == -1)
        return (2);
    int ret = ft_ls(argv, &c);
    finish_print_buffer();
    return (ret);
}