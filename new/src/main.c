#include "../ft_ls.h"


static char *get_string(char **tab, char **used)
{
	int i = 0;
	char *lower = NULL;
    char *save = NULL;

    while (tab && tab[i])
	{
        if (already_use(tab[i], used) == 0)
        {
            if (lower == NULL)
                lower = tab[i];
            if (my_strcmp(lower, tab[i]) >= 0)
            {
                lower = tab[i];
                if (save)
                    free(save);
                save = ft_strdup(tab[i]);
            }
        }
        i++;
	}
    return (save);
}

static char **sort_dir(char **tab)
{
    int i = 0;
    char **save = NULL;
    char *tmp = NULL;
    while (tab && tab[i])
    {
        tmp = get_string(tab, save);
        if (tmp != NULL)
            save = ft_realloc_str(save, tmp);
        free(tmp);
        i++;
    }
    free_all(tab);
    return(save);
}

t_list * char_tab_to_lst(char** tab)
{
    t_list *new = NULL;
    for (int i = 0; tab && tab[i]; i++)
        ft_lstadd_back(&new, ft_lstnew(ft_strdup(tab[i])));
    free_all(tab);
    return (new);
}

int main (int argc, char** argv)
{
    if (argc == 1)
    {
        ls_no_args();
        return (1);
    }
    enum e_flag *used = malloc(sizeof(int) * 6);
    if (!used)
    {
        ft_putstr_fd("Error malloc failed\n", 2);
        return (1);
    }
    void *flag_ptr = used;
    used = parse_flag(argv, used);
    if (used == NULL)
    {
        ft_putstr_fd("parse flag return -1", 2);
        free(flag_ptr);
        return (1);
    }
    int flag_nb = get_flag(flag_ptr);
    free(flag_ptr);
    char **dir_lst = get_dir(&argv[1]);
    dir_lst = parse_dir_lst(dir_lst);
    if (dir_lst == NULL)
        dir_lst = ft_realloc_str(dir_lst, ".");
    dir_lst = sort_dir(dir_lst);

    t_list *cc = char_tab_to_lst(dir_lst);
    if (flag_nb >=  R_OPTION)
    {
        flag_nb -= R_OPTION;
        search_recurcive_dir(cc, flag_nb, 0);
        ft_lstclear(&cc, free);
        return (0);
    }
    return (0);
}
