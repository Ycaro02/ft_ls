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


inline static void read_write_perm(mode_t mode, char *str, int first, int second)
{
    if (mode & first)
        str[0] = 'r';
    if (mode & second)
        str[1] = 'w';
}

inline static char detect_exe_perm(mode_t mode, int first, int second)
{
    char exec = '-';
    char special = 's';

    if (second == __S_ISVTX)
        special = 't';
    if (mode & first)
        exec = 'x';
    if (mode & second) {
        if (exec == 'x')
            exec = special;
        else
            exec = special - 32;
    }
    return (exec);
}

char *perm_to_string(mode_t mode, char type)
{
    char *perm = ft_calloc(sizeof(char), 11);
    if (!perm)
        return (NULL);

    for (int i = 0; i < 11; ++i)
        perm[i] = '-';
    perm[10] = '\0';
    perm[0] = type;

    /* Read/Write by owner */
    read_write_perm(mode, &perm[1], S_IRUSR, S_IWUSR);
    /* Execute by owner and Set User ID exe */
    perm[3] = detect_exe_perm(mode, S_IXUSR, S_ISUID);
    /* Read/Write by group */
    read_write_perm(mode, &perm[4], S_IRGRP, S_IWGRP);
    /* Execute by group and Set Group ID exe */
    perm[6] = detect_exe_perm(mode, S_IXGRP, S_ISGID);
    /* Read/Write by other */
    read_write_perm(mode, &perm[7], S_IROTH, S_IWOTH);
    /* Execute by other and Sticky (save swaped text) */
    perm[9] = detect_exe_perm(mode, S_IXOTH, __S_ISVTX);

    return (perm);
}

/*  Need to refact, call stat here, check file type, if link call lstat instead 
    Need to store space here to, each field will check for him
    Give space array here to check and store*/

t_file *fill_file_struct(struct stat *sb, char *path, char *parent, int symlink)
{
    t_file *file;

    file = ft_calloc(sizeof(t_file), 1);
    if (!file)
        return (NULL);
    file->total_size = -1;
    if (symlink == 0)
        file->type = get_type(*sb);
    else
        file->type = SYMLINK;
    
    // file->perm = sb->st_mode & 0777;
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