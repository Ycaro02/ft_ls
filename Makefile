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
			src/flag_gestion.c\
			src/parse.c\
			src/ft_ls.c\
			src/recurcive.c\
			src/l_option.c\
			src/time_gestion.c\
			src/sort.c\
			src/buffer.c\
			src/manage_space.c\

NAME	= ft_ls

RM	= rm -f

LIBFT = libft/libft.a

all:		${NAME}

${NAME}:	
			@echo " \033[5;36m ----- Compiling libft...  ----- \033[0m\n"
			@make -s -C libft bonus
			@echo "\033[7;32m -----  Compiling libft  ----- \033[0m\n"
			@echo " \033[5;36m ----- Compiling ft_ls project...  ----- \033[0m\n"
			@${CC} $(CFLAGS) $(SRCS) $(LIBFT) -o $(NAME)
			@echo "\033[7;32m -----  Compiling ft_ls done  ----- \033[0m\n"

clean:
			@echo "\033[7;31m\n -----  Cleaning all objects...  ----- \033[0m\n"
			@${RM} ${NAME}
			@echo "\033[7;33m -----  Cleaning done  ----- \033[0m\n"

fclean:		clean
			@make -s -C libft fclean
			@${RM} ${NAME}

re:			fclean all


.PHONY:		all clean fclean re bonus
