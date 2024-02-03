#include "../include/ft_ls.h"

/* for last x perm (other) S_IXOTH, STICKY
        if just exec:   x
        if just sticky: T
        if sticky + x:  t
        
        for second x perm (group) S_IXGRP, GID
        if just exec:   x
        if just GID: S
        if GID + x:  s

        for first x perm (group) S_IXGRP, SUID
        if just exec:   x
        if just GID: S
        if GID + x:  s
    */

static int fill_name_and_parent(t_file *file, char *path, char *parent)
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
    return (0);
}

int check_for_quote(char *str)
{
    int quote = quotes_required(str);
    return (quote > NOEFFECT_CHAR ? quote : NORMAL_CHAR);
}


t_file *fill_file_struct(struct stat *sb, char *path, char *parent, int symlink)
{
    t_file *file;

    file = ft_calloc(sizeof(t_file), 1);
    if (!file)
        return (NULL);
    file->total_size = -1;
    file->type = symlink == TRUE ? SYMLINK : get_type(*sb);
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
    if (fill_name_and_parent(file, path, parent) == MALLOC_ERR)
        return (NULL);
    file->quote = check_for_quote(file->name);
    return (file);
}

// #if defined __USE_MISC || defined __USE_XOPEN
    // S_ISVTX /* sticky bits*/
// #endif