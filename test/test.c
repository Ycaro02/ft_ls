#include <stdio.h>
#include <stdlib.h>
#include <sys/acl.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    acl_t acl = acl_get_file(argv[1], ACL_TYPE_ACCESS);
    if (acl == NULL) {
        perror("acl_get_file");
        exit(EXIT_FAILURE);
    }

    char *text = acl_to_text(acl, NULL);
    printf("%s\n", text);

    acl_free(text);
    acl_free(acl);

    return 0;
}

// int ft_strlen_word(char *s)
// {
// 	int i =0;
// 	while (s && s[i] && s[i] != ' ')
// 		i++;
// 	return (i);
// }

// int main(int argc, char**argv)
// {
// 	char value[LEN];
// 	char list[LEN];
// 	int i  = 1;
// 	int j = 0;
// 	if (argc < 2)
// 		return (1);
// 	while (i < argc)
// 	{
// 		bzero(list, LEN);
// 		int listen = listxattr(argv[i], list, LEN);
// 		if (listen == -1)
// 			return (1);	
// 		printf("name = %s, len = %d\n", list, listen);

// 		while (j < listen)
// 		{
// 			bzero(value, LEN);
// 			printf("\n&list[%d] = %s\n", j, &list[j]);
// 			int res = getxattr (argv[i],  &list[j], value, LEN);
// 			if (res == -1)
// 			{
// 				printf("Eroor get x\n");
// 				return (1);
// 			}
// 			printf("value = %s,", value);
// 			printf("len = %d\n", res);
// 			j += ft_strlen_word(&list[j]) + 1;
// 		}
// 		i++;
// 	}
// 	return (0);
// }
