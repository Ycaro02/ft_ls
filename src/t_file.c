#include "../include/ft_ls.h"

int get_quote(t_file *file) /* old space quote for -l option */
{
    return (file->quote == NORMAL_CHAR ? 0 : 1);
}

int fill_name_and_quote(t_file *file, char *path, char *parent, t_file_context *file_c, int l_option)
{
    file->name = ft_strdup(path);
    if (!file->name)
        return (MALLOC_ERR);
    if (parent) {
        file->parrent = ft_strdup(parent);
        if (!file->parrent) {
            free(file->name);
            return (MALLOC_ERR);
        }
    }
    else
        file->parrent = NULL;
    file->quote = check_for_quote(file->name);
    if (l_option == 1)
        file_c->space[S_NAME_QUOTE] = get_quote(file);
    return (0);
}

int check_for_quote(char *str)
{
    int quote = quotes_required(str);
    return (quote > NOEFFECT_CHAR ? quote : NORMAL_CHAR);
}

/*
    Need to change all l option storage logic, if l_option build line here
        - create line structure 
        - fill it with all actual "write info" 
            - update int Array Space in concequences
        - just need to fill buffer in new write
    This change avoid double allocation for all string and special function call
    like getpwuid ...
*/
/* int * space */

/**
 * Call in: 
 *          default_file_struct -> check_args -> np
 *          check_args -> np
 *          check_for_fill_struct in main ls function one -> np
 *          create_new_file(recurcive routine) -> like cmd args create
 *          display_symlink -> np for this 
*/
/* maybe we can remove parent and path and fill it after call, 3 call in parse, one in recurcive and one in write name no realy a pain */
t_file *fill_file_struct(struct stat *sb, int symlink, t_context *c, t_file_context *file_c)
{
    t_file *file;
    int l_option = has_flag(c->flag_nb, L_OPTION);
 
    file = ft_calloc(sizeof(t_file), 1);
    if (!file)
        return (NULL);
    file->total_size = -1;
    file->type = symlink == TRUE ? SYMLINK : get_type(*sb);

    // file->sb = sb; /* STOP FREE SB */

    file->perm = sb->st_mode;
    file->size = sb->st_size;
    file->nb_link = sb->st_nlink;
    file->last_status_change = sb->st_ctim;
    file->last_access = sb->st_atim;
    file->last_change = sb->st_mtim;
    file->user_id = sb->st_uid;
    file->group_id = sb->st_gid;
    file->nb_block = sb->st_blocks;
    file->rdev = sb->st_rdev;

    if (fill_name_and_quote(file, file_c->path, file_c->path, file_c, l_option) == MALLOC_ERR) {
        ft_printf_fd(2, "Malloc error fill file struct\n");
        free(sb);
        return (NULL);
    }   

     if (l_option) {
        // printf("%sGot file_c manage space here%s\n",GREEN, RESET);
        build_file_line(file, c, file_c, symlink);
    }


    return (file);
}
