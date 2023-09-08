#include "../ft_ls.h"

int main (int argc, char** argv)
{
    if (argc == 1)
    {
        ls_no_args();
        return (1);
    }
    char **dir_lst = get_dir(&argv[1]);
    if (dir_lst == NULL)
        dir_lst = ft_realloc_str(dir_lst, ".");
    enum e_flag *used = malloc(sizeof(int) * 6);
    if (!used)
    {
        printf("Error malloc failed\n");
        return (1);
    }
    void *flag_ptr = used;
    used = parse_flag(argv, used);
    if (used == NULL)
    {
        printf("parse flag return -1");
        free_all(dir_lst);
        free(flag_ptr);
        return (1);
    }
    int flag_nb = get_flag(flag_ptr);
    free(flag_ptr);
    ft_ls(dir_lst, flag_nb);
    return (0);
}