#include <sys/types.h>
// #include "/path/libarchive/libarchive/archive_platform_xattr.h"
#include "/usr/include/linux/posix_acl_xattr.h"
#include "/usr/include/linux/reiserfs_xattr.h"
#include "/usr/include/linux/xattr.h"
// #include "/usr/include/sys/xattr.h"
#include <stdio.h>
ssize_t listxattr (const char *chemin, char *liste, size_t taille);
int main (int argc, char** argv)
{
    if (argc <= 1)
        return (1);
    char buff[2001] = "alo";
    listxattr(argv[1], buff, 2000);
    printf("buff = %s\n", buff);
    return (0);
}