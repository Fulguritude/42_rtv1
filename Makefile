# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: tduquesn <tduquesn@42.fr>                  +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2017/11/17 21:33:56 by tduquesn          #+#    #+#              #
#    Updated: 2018/08/31 22:19:30 by fulguritu        ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME	:=	rtv1
LFT		:=	libft.a


PLATFORM:=	LINUX
CC		:=	gcc
CFLAGS	:=	-Wall -Werror -Wextra #-O3

ifeq ($(PLATFORM),LINUX)
DBFLAGS =	-fsanitize=address
LIBASAN =	-lasan
LOC_LX	=	/usr/lib/x86_64-linux-gnu/
LIB_SUFF=	_Linux
LOC_LMLX=	../
LIBS	=	$(LIBASAN) -lm -L$(LOC_LMLX) -lmlx$(LIB_SUFF) -L$(LFTDIR) -lft -L$(LOC_LX) -lX11 -lXext
else
DBFLAGS =
LIBASAN =
LOC_LX	=	/usr/X11/lib
LIB_SUFF=
LOC_LMLX=	../../other_src/ #remove at 42
LIBS	=	-L$(LOC_LMLX) -lmlx -framework OpenGL -framework AppKit -L$(LFTDIR) -lft
endif


HDRDIR	:=	./
LFTDIR	:=	./libft/
TSTDIR	:=	./tests/
SRCDIR	:=	./

HDRS	:=	$(NAME).h
SRCS	:=	camera.c		\
			coordinates.c	\
			event_key.c		\
			image_utils.c
			

OBJS	:=	$(SRCS:.c=.o)

MAIN	:=	main_$(NAME).c
OBJ_MAIN:=	$(MAIN:.c=.o)

#TST_DIR		:=	unit_tests
#TST_MAIN	:=	$(TST_DIR)/unit_tests.c
#TST_EXEC	:=	test.out

RESET	:=	"\033[0m"
RED		:=	"\033[0;31m"
GREEN	:=	"\033[0;32m"

$(NAME): $(LFTDIR)$(LFT) $(OBJS) $(OBJ_MAIN) $(HDRS)
	@echo "Compiling "$@": "$@" -> \c"$(RED)
	@$(CC) $(CFLAGS) $(DBFLAGS) $(OBJS) $(OBJ_MAIN) $(LIBS) -o $@
	@echo $(GREEN)"OK!"$(RESET)

%.o: $(SRCDIR)%.c $(HDRS)
	@$(CC) $(CFLAGS) -c $< -I$(HDRDIR)

#dependencies are taken care of in libft's makefile.
$(LFTDIR)$(LFT):
	@rm -f $@
	$(MAKE) -C $(LFTDIR) $(LFT)

all: $(NAME)

clean:
	@rm -f $(OBJS) $(OBJ_MAIN)

fclean:clean
	$(MAKE) -C $(LFTDIR) fclean
	@rm -f $(NAME)

re:fclean all

#test:$(LFTDIR)$(LFT) $(OBJS) $(HDRS)
#	@$(CC) $(CFLAGS) $(DBFLAGS) $(OBJS) $(TST_MAIN) $(LIBS) -o $(TST_EXEC)
#	./$(TST_EXEC)
#	@rm -f $(TST_MAIN:.c=.o) $(TST_EXEC)

mf_debug:
	@cat -e -t -v Makefile

.PHONY: all clean re test vlqtest capstest
