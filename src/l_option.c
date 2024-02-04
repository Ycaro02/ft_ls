#include "../include/ft_ls.h"

inline static void insert_space(int nb)
{
    if (nb > 0) {
        while (nb != 0) {
            fill_buffer_char(' ');
            nb--;
        }
    }
}

void write_user_name(long user_id, int space, int flag_nb)
{
    if (has_flag(flag_nb, N_OPTION)) {
        char *tmp = ft_ltoa(user_id);
        insert_space(space - ft_strlen(tmp));
        fill_buffer(tmp);
        free(tmp);
    }
    else {
        struct passwd* user = getpwuid(user_id);
        if (!user) {
            char *tmp_buff = ft_ltoa(user_id);
            fill_buffer(tmp_buff);
            insert_space(space - ft_strlen(tmp_buff));
            free(tmp_buff);
        }
        else {
            fill_buffer(user->pw_name);
            insert_space(space - ft_strlen(user->pw_name));
        }
    }
    if (space != -1)
        fill_buffer_char(' ');
}

void write_group_name(long group_id, int space, int flag_nb)
{
    if (has_flag(flag_nb, N_OPTION))
    {
        char *tmp = ft_ltoa(group_id);
        insert_space(space - ft_strlen(tmp));
        fill_buffer(tmp);
        free(tmp);
    }
    else
    {
        struct group* group = getgrgid(group_id);
        if (!group)
        {
            char *tmp_buff = ft_ltoa(group_id);
            fill_buffer(tmp_buff);
            insert_space(space - ft_strlen(tmp_buff));
            free(tmp_buff);
        }
        else
        {
            fill_buffer(group->gr_name);
            insert_space(space - ft_strlen(group->gr_name));
        }
    }
    if (space != -1)
        fill_buffer_char(' ');
}

static int write_nb_link(long long nb_link, int space)
{
    char *tmp;

    tmp = ft_itoa((int)nb_link);
    if (!tmp)
        return (MALLOC_ERR);
    fill_buffer_char(' ');
    insert_space(space - ft_strlen(tmp));
    fill_buffer(tmp);
    free(tmp);
    fill_buffer_char(' ');
    return (0);
}


static int write_perm(t_file file, int space)
{
    char    *tmp = perm_to_string(file.perm, file.type);
    fill_buffer(tmp);
    free(tmp);
    /* ACL management */
    int ret = check_acl(&file);
    if (ret == 0)
        fill_buffer_char('+');
    else if (ret == MALLOC_ERR)
        return (ret);
    /* insert max space - 10 (basic perm len) + bool acl*/
    insert_space(space - (10 + (ret == 0)));
    return (0);
}

static int write_size(t_file file, int *space)
{
    char *size;
    // printf("size: %d minor:%d major: %d\n", space[S_SIZE], space[S_MINOR_SIZE], space[S_MAJOR_SIZE]);
    if (file.type == CHARACTER || file.type == BLOCK) {
        // printf("for %s size :%ld\n", file.name, (space[S_MAJOR_SIZE] - ft_strlen(size)));
        size = ft_ultoa(major(file.rdev));
        char* minor_size = ft_ultoa(minor(file.rdev));
        if (space[S_MAJOR_SIZE] + space[S_MINOR_SIZE] > space[S_SIZE]) {
            insert_space(space[S_MAJOR_SIZE] - ft_strlen(size));
            fill_buffer(size);
            fill_buffer(", ");
            insert_space((space[S_MINOR_SIZE])- ft_strlen(minor_size));
            fill_buffer(minor_size);
        }
        else {
            int pad_len = space[S_MINOR_SIZE] + ft_strlen(size);
            insert_space(space[S_SIZE] - pad_len);
            fill_buffer(size);
            fill_buffer(", ");
            insert_space(space[S_MINOR_SIZE] - ft_strlen(minor_size) - 2);
            fill_buffer(minor_size);
        }
        free(size);
        free(minor_size);
        fill_buffer_char(' ');
        // ft_printf_fd(2, "MINOR :%d\n", minor(file.rdev));
        // ft_printf_fd(2, "MAJOR :%d\n", major(file.rdev));
        return (0);
    }
    else {
        size = ft_ltoa(file.size);
        if (!size)
            return (MALLOC_ERR);
        if (space[S_MAJOR_SIZE] + space[S_MINOR_SIZE] > space[S_SIZE])
            insert_space(space[S_MAJOR_SIZE] + space[S_MINOR_SIZE] + 2 - ft_strlen(size));
        else {
            // if ((int)ft_strlen(size) != space[S_SIZE])
            if ((int)ft_strlen(size) == space[S_SIZE])
                insert_space(space[S_SIZE] - 1 - ft_strlen(size)); // DEVIL - 1
            else
                insert_space(space[S_SIZE] - ft_strlen(size)); // DEVIL - 1
        }
        fill_buffer(size);
        free(size);
    }
    fill_buffer_char(' ');
    return (0);
}

static int write_date(t_file file, int* space, int flag_nb)
{
    char **tmp = NULL;
    int i = 0;
    int j = S_MONTH;

    if (has_flag(flag_nb, U_OPTION))
        tmp = get_printable_date(file.last_access);
    else if (has_flag(flag_nb, C_OPTION))
        tmp = get_printable_date(file.last_status_change);
    else
        tmp = get_printable_date(file.last_change);
    if (!tmp) {
        ft_printf_fd(2, "Malloc error\n");
        return (MALLOC_ERR);
    }
    while (tmp[i]) {
        insert_space(space[j] - ft_strlen(tmp[i]));
        fill_buffer(tmp[i]);
        fill_buffer_char(' ');
        i++;
        j++;
    }
    ft_free_tab(tmp);
    return (0);
}

int fill_buffer_l_option(t_file file, int *space, int flag_nb)
{
    if (write_perm(file, space[S_PERM]) == MALLOC_ERR\
        || write_nb_link(file.nb_link, space[S_LINK]) == MALLOC_ERR)
        return (MALLOC_ERR);
    if (!has_flag(flag_nb, G_OPTION))
        write_user_name(file.user_id, space[S_USER], flag_nb);
    write_group_name(file.group_id, space[S_GROUP], flag_nb);
    if (write_size(file, space) == MALLOC_ERR || \
        write_date(file, space, flag_nb) == MALLOC_ERR || \
        write_file_name(file, flag_nb, space[S_NAME_QUOTE]) == MALLOC_ERR)
            return (MALLOC_ERR);
    if (has_flag(flag_nb, Z_OPTION))
        diplay_xattr_acl(&file);
    return (0);
}
