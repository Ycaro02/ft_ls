# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: nfour <nfour@student.42.fr>                +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/11/12 16:33:38 by nfour             #+#    #+#              #
#    Updated: 2024/02/06 16:43:35 by nfour            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC		= gcc

CFLAGS	= -Wall -Wextra -Werror -g

# CFLAGS	= -Wall -Wextra -Werror -g -fsanitize=address

SRCS	=	src/main.c\
			src/basic_flag_gestion.c\
			src/utils.c\
			src/recurcive.c\
			src/flag_gestion.c\
			src/parse.c\
			src/ft_ls.c\
			src/t_file.c\
			src/l_option.c\
			src/time_gestion.c\
			src/sort.c\
			src/buffer.c\
			src/manage_column.c\
			src/list_xattr.c\
			src/build_column.c\
			src/manage_perm.c\
			src/write_file_name.c\
			src/quote_gestion.c\
			src/file_line.c\
			src/ls_function.c\
			

OBJ = $(SRCS:.c=.o)

NAME	= ft_ls

RM	= rm -f

LIBFT = libft/libft.a

LIB_LIST = libft/list/linked_list.a

LIBACL = rsc/acl/libacl.a

LS_OUTPUT = real_ls_out

FT_LS_OUTPUT = myls_out

# DIFF_LS = ./rsc/ls_diff.sh

# T_FLAG = -alr

# T_DIR = test

# T_FLAG = -larR 

VALGRIND_TEST = ./rsc/vtest.sh

DIFF_TEST = ./rsc/test.sh

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

test:	${NAME}
		${DIFF_TEST}

vtest:		${NAME}
			${VALGRIND_TEST}

fclean:		clean
			@make -s -C libft fclean
			@make -s -C libft/list fclean
			@${RM} ${NAME} ${LS_OUTPUT} ${FT_LS_OUTPUT}

re:			fclean all

.PHONY:		all clean fclean re
