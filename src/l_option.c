#include "../include/ft_ls.h"

void insert_space(int nb)
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

void display_symlink(struct stat *sb, int flag_nb, char *new, int sym_bool)
{
    t_file *file = fill_file_struct(sb, new, NULL, sym_bool);
    if (file) {
        write_file_name(*file, flag_nb, -1);
        free(file->name);
        free(file);
    } 
    free(sb);
}


static char* remove_last_word(char *str, char sep)
{
    int i = (int)ft_strlen(str);
    int save_len = i;

    while (i > 0) {
        if (i == save_len && str[i] == sep)
            --i;
        else if (str[i] == sep)
            break ;
        --i;
    }
    str[i] = '\0';
    char *new = ft_calloc(sizeof(char), i+1);
    ft_strcpy(new, str, i+1);
    return (new);
}

static void stat_symlink(char* buff, char *parrent_path, char* path, int flag_nb)
{
    int         sym_bool = 0;
    char        *new = NULL;
    struct stat *sb = check_for_stat(buff, flag_nb, &sym_bool);

    // ft_printf_fd(2, "%sFor path: %s, parent |%s| buff|%s|%s\n", CYAN, path, parrent_path, buff, RESET);
    /* check for buff real path */
    if (sb)
        display_symlink(sb, flag_nb, buff, sym_bool);
    else { /* else check for add parrent path or / before */
        if (parrent_path && ft_strcmp(parrent_path, "/") == 0)
            new = ft_strjoin(parrent_path, buff);
        else {
            if (parrent_path && parrent_path[ft_strlen(parrent_path) -1] != '/') 
                new = ft_strjoin(parrent_path, "/");
            new = ft_strjoin_free(new, buff, 'f');
        }
        sb = check_for_stat(new, flag_nb, &sym_bool);
        /* check for new path with parrent */
        if (sb)
            display_symlink(sb, flag_nb, buff, sym_bool);
        else { /* else check to remove last word of path, this will search in current dir */
            if (new)
                free(new);
            new = remove_last_word(path, '/');
            sb = check_for_stat(new, flag_nb, &sym_bool);
            if (sb)
                display_symlink(sb, flag_nb, buff, sym_bool);
            else
                multiple_fill_buff(" ", buff, NULL, NULL);
        }
        free(new);
    }
}

static int write_symlink(char *path, char *parrent_path, int flag_nb, int space)
{
    char    buff[500];
    char    *tmp;
    int quote = quotes_required(path);

    fill_buffer_color(path, E_CYAN, flag_nb, space, quote);
    if (has_flag(flag_nb, L_OPTION))
    {
        
        if (parrent_path && path[0] != '/') /* avoid /bin/bin case*/
            tmp = join_parent_name(parrent_path, path);
        else
            tmp = ft_strjoin(path, "");

        if (!tmp)
            return (MALLOC_ERR);
        if (space == 0)
            fill_buffer_char(' ');
        fill_buffer("->");
        int ret = readlink(tmp, buff, 199);
        if (ret == -1)
            perror("readlink");
        else {
            buff[ret] = '\0';
            stat_symlink(buff, parrent_path, path, flag_nb);
        }
        free(tmp);
    }
    return (0);
}


static int is_full_perm(mode_t mode)
{
    return ((mode & S_IRWXO) == S_IRWXO && (mode & S_IRWXG) == S_IRWXG &&(mode & S_IRWXU) == S_IRWXU);
}

int write_file_name(t_file file, int flag_nb, int space)
{
    // char c = ' ';
    int perm_color = E_NONE;

    // if (space != 0) {
    //     // printf("for %s quote = %d\n", file.name, file.quote);
    //     if (file.quote != NORMAL_CHAR)
    //         c = file.quote == ADD_SIMPLE_QUOTE_CHAR ? '\'' : '\"';
    //     fill_buffer_char(c);
    // }


    if (file.perm & S_IXOTH || file.perm & S_IXGRP || file.perm & S_IXUSR)
        perm_color = E_GREEN;

    if (is_full_perm(file.perm))
        perm_color = E_FILL_GREEN;

    if (file.type == FIFO)
        perm_color = E_YELLOW_BLACK;
    
    if (file.perm & S_ISUID)
        perm_color = E_FILL_RED;
    else if (file.perm & S_ISGID)
        perm_color = E_FILL_YELLOW;
    
    if(file.type == SYMLINK && space != -1) {
        if (write_symlink(file.name, file.parrent, flag_nb, space) == MALLOC_ERR)
            return (MALLOC_ERR);
    }
    else if (file.type == SYMLINK)
        fill_buffer_color(file.name, E_CYAN, flag_nb, space, file.quote);
    else if (file.type == CHARACTER || file.type == BLOCK)
        fill_buffer_color(file.name, E_YELLOW, flag_nb, space, file.quote);
    else if (perm_color == E_FILL_GREEN)
        fill_buffer_color(file.name, perm_color, flag_nb, space, file.quote);
    else if (file.type == DIRECTORY)
        fill_buffer_color(file.name, E_BLUE, flag_nb, space, file.quote);
    else
        fill_buffer_color(file.name, perm_color, flag_nb, space, file.quote);

    // if (space != 0 && file.quote != NORMAL_CHAR)
    // if (space != 0)
    //     fill_buffer_char(c);
    
    if (!has_flag(flag_nb, L_OPTION) && space == 0)
        fill_buffer(" ");
    //     fill_buffer_char('\n');
    // else
    return (0);
}


static int write_perm(t_file file, int *is_exec, int space)
{
    char    *tmp = perm_to_string(file.perm, file.type);
    fill_buffer(tmp);
    free(tmp);
    (void)is_exec;
    /* ACL management */
    int ret = check_acl(&file);
    if (ret == 0)
        fill_buffer_char('+');
    else if (ret == MALLOC_ERR)
        return (ret);
    insert_space(space - (10 + (ret == 0)));
    return (0);
}

static int write_size(t_file file, int *space)
{
    char *tmp;
    // printf("size: %d minor:%d major: %d\n", space[S_SIZE], space[S_MINOR_SIZE], space[S_MAJOR_SIZE]);
    if (file.type == CHARACTER)
    {
        // printf("for %s size :%ld\n", file.name, (space[S_MAJOR_SIZE] - ft_strlen(tmp)));
        char *tmp = ft_ultoa(major(file.rdev));
        char* tmp2 = ft_ultoa(minor(file.rdev));
        if (space[S_MAJOR_SIZE] + space[S_MINOR_SIZE] > space[S_SIZE])
        {
            insert_space(space[S_MAJOR_SIZE] - ft_strlen(tmp));
            fill_buffer(tmp);
            fill_buffer(", ");
            insert_space((space[S_MINOR_SIZE])- ft_strlen(tmp2));
            fill_buffer(tmp2);
        }
        else
        {
            int pad_len = space[S_MINOR_SIZE] + ft_strlen(tmp);
            insert_space(space[S_SIZE] - pad_len);
            fill_buffer(tmp);
            fill_buffer(", ");
            insert_space(space[S_MINOR_SIZE] - ft_strlen(tmp2) - 2);
            fill_buffer(tmp2);
        }
        free(tmp);
        free(tmp2);
        fill_buffer_char(' ');
        // ft_printf_fd(2, "MINOR :%d\n", minor(file.rdev));
        // ft_printf_fd(2, "MAJOR :%d\n", major(file.rdev));
        return (0);
    }
    else
    {
        tmp = ft_ltoa(file.size);
        if (!tmp)
            return (MALLOC_ERR);
        if (space[S_MAJOR_SIZE] + space[S_MINOR_SIZE] > space[S_SIZE])
            insert_space(space[S_MAJOR_SIZE] + space[S_MINOR_SIZE] + 2 - ft_strlen(tmp));
        else
        {
            // if ((int)ft_strlen(tmp) != space[S_SIZE])
            if ((int)ft_strlen(tmp) == space[S_SIZE])
                insert_space(space[S_SIZE] - 1 - ft_strlen(tmp)); // DEVIL - 1
            else
                insert_space(space[S_SIZE] - ft_strlen(tmp)); // DEVIL - 1
        }
        fill_buffer(tmp);
        free(tmp);
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
    int is_exec;

    is_exec = 1;
    fill_buffer_char(file.type);
    if (write_perm(file, &is_exec, space[S_PERM]) == MALLOC_ERR\
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
