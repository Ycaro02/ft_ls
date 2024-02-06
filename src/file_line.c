#include "../include/ft_ls.h"

// S_PERM,
// S_USER,
// S_GROUP,
// S_SIZE,
// S_LINK,
// S_MONTH,
// S_DAY,
// S_HOUR,

static char *get_perm_string(t_file *file, int *space, int symlink)
{
    char    *tmp = perm_to_string(file->perm, file->type);
    if (!tmp)
        return (NULL);   
    /* ACL management */
    int ret = check_acl(file);
    if (ret == 0)
        tmp = ft_strjoin_free(tmp, "+", 'f');
    else if (ret == MALLOC_ERR)
        return (NULL);
    // insert_space(space - (10 + (ret == 0)));
    int local_space = 10 + (ret == 0); /* 10 for classic len + return bool for acl '+'*/
    // int local_space = 10; /* 10 for classic len + return bool for acl '+'*/
    if (symlink != -1 && local_space > space[S_PERM])
        space[S_PERM] = local_space;
    return (tmp);
}

static char *update_ltoa_value(t_int64 value, int *space, int idx, int symlink)
{
    char *str = ft_ltoa(value);
    if (!str)
        return (NULL);
    int nb = ft_strlen(str);
    if (symlink != -1 && nb > space[idx])
        space[idx] = nb;
    return (str);
}

static char *update_grp_name(t_file *file, int *space, int idx, int symlink)
{
    struct group* group = getgrgid(file->group_id);
    if (!group)
        return (update_ltoa_value(file->group_id, space, idx, symlink));
    char *str = ft_strdup(group->gr_name);
    int len = ft_strlen(str);
    if (symlink != -1 && len > space[idx])
        space[idx] = len;
    return (str);
}

static char *update_user_name(t_file *file, int *space, int idx, int symlink)
{
    struct passwd* user = getpwuid(file->user_id);
    if (!user)
        return (update_ltoa_value(file->user_id, space, idx, symlink));
    char *str = ft_strdup(user->pw_name);
    int len = ft_strlen(str);
    if (symlink != -1 && len > space[idx])
        space[idx] = len;
    return (str);
}

static char *update_ultoa_value(t_int64 value, int *space, int idx, int symlink)
{
    char *str = ft_ultoa(value);
    if (!str)
        return (NULL);
    int nb = ft_strlen(str);
    if (idx == S_MINOR_SIZE)
        nb += 2; /* add ', ' for minor */
    // printf("alo %s len: %d\n", str, (int)ft_strlen(str));
    if (symlink != -1 && nb > space[idx])
        space[idx] = nb;
    return (str);
}

static int update_date_value(t_file *file, int *space, int symlink) /* need to take wich time to evaluate in args */
{
    char **tmp = get_printable_date(file->last_change);
    if (!tmp)
        return (MALLOC_ERR);
        /* check date */
    file->line[S_MONTH] = ft_strdup(tmp[0]);
    file->line[S_DAY]   = ft_strdup(tmp[1]);
    file->line[S_HOUR]  = ft_strdup(tmp[2]);

    /* can while on fun call here */
    int tmp_len = ft_strlen(file->line[S_MONTH]);
    if (symlink != -1 && tmp_len > space[S_MONTH])
        space[S_MONTH] = tmp_len;

    tmp_len = ft_strlen(file->line[S_DAY]);
    if (symlink != -1 && tmp_len > space[S_DAY])
        space[S_DAY] = tmp_len;

    tmp_len = ft_strlen(file->line[S_HOUR]);
    if (symlink != -1 && tmp_len > space[S_HOUR])
        space[S_HOUR] = tmp_len;
    ft_free_tab(tmp);
    return (0);
}

/* if symlink == -1 don't update space, -2 for special no x perm */
int build_file_line(t_file *file, t_context *c, t_file_context *file_c, int symlink)
{
    // int max = S_MINOR_SIZE;
    file->line = ft_calloc(sizeof(char *), S_MAJOR_SIZE + 1);
    if (!file->line)
        return (MALLOC_ERR);

    if (symlink == -2) {
        file->type = -2;
        file->line[S_PERM] = ft_strdup("??????????");
        file->line[S_GROUP] = ft_strdup("?");
        file->line[S_USER] = ft_strdup("?");
        file->line[S_LINK] = ft_strdup("?");
        file->line[S_SIZE] = ft_strdup("?            ?");
        ft_bzero(file_c->space, sizeof(int) * S_MAX);
        file_c->space[S_MONTH] = 5;
        return (0);
    }

    file->line[S_PERM] = get_perm_string(file, file_c->space, symlink);
    if (has_flag(c->flag_nb, N_OPTION)) {
        file->line[S_USER] =  update_ltoa_value(file->user_id, file_c->space, S_USER, symlink);
        file->line[S_GROUP] = update_ltoa_value(file->group_id, file_c->space, S_GROUP, symlink);
    } else {
        file->line[S_USER] =  update_user_name(file, file_c->space, S_USER, symlink); 
        file->line[S_GROUP] = update_grp_name(file, file_c->space, S_GROUP, symlink);
    }

    if (file->type == CHARACTER || file->type == BLOCK) { /* store classic size, no major minor */
        file->line[S_MINOR_SIZE] = update_ultoa_value(minor(file->rdev), file_c->space, S_MINOR_SIZE, symlink);
        file->line[S_MAJOR_SIZE] = update_ultoa_value(major(file->rdev), file_c->space, S_MAJOR_SIZE, symlink);
    } 
    else
        file->line[S_SIZE] = update_ultoa_value(file->size, file_c->space, S_SIZE, symlink);
    file->line[S_LINK] = update_ultoa_value(file->nb_link, file_c->space, S_LINK, symlink);

    if (update_date_value(file, file_c->space, symlink) == MALLOC_ERR)
        return (MALLOC_ERR);
    return (0);
}