# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: nfour <nfour@student.42.fr>                +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/11/12 16:33:38 by nfour             #+#    #+#              #
#    Updated: 2023/09/16 10:24:18 by nfour            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC		= gcc

CFLAGS	= -Wall -Wextra -Werror -g

SRCS	=	src/main.c\
			src/basic_flag_gestion.c\
			src/utils.c\
			src/recurcive.c\
			src/flag_gestion.c\
			src/parse.c\
			src/ft_ls.c\
			src/t_file.c\
			src/l_option.c\
			src/l_option_utils.c\
			src/time_gestion.c\
			src/sort.c\
			src/buffer.c\
			src/manage_column.c\
			src/list_xattr.c\
			src/manage_space.c\
			src/build_column.c\
			src/manage_perm.c


OBJ = $(SRCS:.c=.o)

NAME	= ft_ls

RM	= rm -f

LIBFT = libft/libft.a

LIB_LIST = libft/list/linked_list.a

LIBACL = rsc/acl/libacl.a

all:		${NAME}

%.o : %.c
	@$(CC) ${CFLAGS} -c $< -o $@

${NAME}:	$(OBJ)
			@echo " \033[5;36m ----- Compiling lib...  ----- \033[0m\n"
			@make -s -C libft
			@make -s -C libft/list
			@echo "\033[7;32m -----  Compiling lib done  ----- \033[0m\n"
			@echo " \033[5;36m ----- Compiling ft_ls project...  ----- \033[0m\n"
			@${CC} $(CFLAGS) -o $(NAME) $(OBJ) $(LIBFT) $(LIBACL) $(LIB_LIST)
			@echo "\033[7;32m -----  Compiling ft_ls done  ----- \033[0m\n"

clean:
			@echo "\033[7;31m\n -----  Cleaning all objects...  ----- \033[0m\n"
			@make -s -C libft clean
			@make -s -C libft/list clean
			@${RM} ${OBJ}
			@echo "\033[7;33m -----  Cleaning done  ----- \033[0m\n"

test_sort:	${NAME}
			export LC_COLLATE=en_US.utf8 && ls test/sort -al > real_ls_out
			./ft_ls test/sort -al > myls_out
			echo MYLS && cat myls_out -e && echo REALLS && cat real_ls_out -e

vtest:		${NAME}
			valgrind ./ft_ls / -lR

fclean:		clean
			@make -s -C libft fclean
			@make -s -C libft/list fclean
			@${RM} ${NAME} real_ls_out myls_out

re:			fclean all

.PHONY:		all clean fclean re
