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


OBJ = $(SRCS:.c=.o)

NAME	= ft_ls

RM	= rm -f

LIBFT = libft/libft.a

LIBACL = acl/libacl.a

all:		${NAME}

%.o : %.c
	@$(CC) ${CFLAGS} -c $< -o $@

${NAME}:	$(OBJ)
			@echo " \033[5;36m ----- Compiling libft...  ----- \033[0m\n"
			@make -s -C libft bonus
			@echo "\033[7;32m -----  Compiling libft  ----- \033[0m\n"
			@echo " \033[5;36m ----- Compiling ft_ls project...  ----- \033[0m\n"
			@${CC} $(CFLAGS) -o $(NAME) $(OBJ) $(LIBFT) $(LIBACL)
			@echo "\033[7;32m -----  Compiling ft_ls done  ----- \033[0m\n"

clean:
			@echo "\033[7;31m\n -----  Cleaning all objects...  ----- \033[0m\n"
			@${RM} ${OBJ}
			@echo "\033[7;33m -----  Cleaning done  ----- \033[0m\n"

test_sort:
			echo Collate: |${LC_COLLATE}| Real ls:
			ls test/sort -la
			echo My ls:
			./ft_ls test/sort -laG

fclean:		clean
			@make -s -C libft fclean
			@${RM} ${NAME}

re:			fclean all


.PHONY:		all clean fclean re bonus
