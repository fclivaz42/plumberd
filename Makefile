# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: fclivaz <fclivaz@student.42lausanne.ch>    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/07/22 14:16:50 by fclivaz           #+#    #+#              #
#    Updated: 2026/02/23 19:07:39 by fclivaz          ###   LAUSANNE.ch        #
#                                                                              #
# **************************************************************************** #

export MAKEFLAGS += --silent

NAME	= plumberd

SHELL	= /bin/bash

ifeq (,$(shell which sccache &>/dev/null))
CXX		= c++
else
CXX		= sccache c++
endif

DEPS	=	${INCL}/plumberd.hpp\
			${INCL}/AClassPopup.hpp\
			${INCL}/ClassVolumed.hpp\
			${INCL}/ClassBrightnessd.hpp\
			${INCL}/ClassPlumberdServer.hpp\
			${SRCS}

SRCS	=	${SUBDIR}/plumberd.cpp\
			${SUBDIR}/AClassPopup.cpp\
			${SUBDIR}/ClassVolumed.cpp\
			${SUBDIR}/ClassBrightnessd.cpp\
			${SUBDIR}/ClassPlumberdActions.cpp\
			${SUBDIR}/ClassPlumberdServer.cpp

SUBDIR	= srcs

SRCTREE	= $(shell find srcs -type d)

OBJ		= $(SRCS:${SUBDIR}/%.cpp=${OBJDIR}/%.o)

OBJDIR	= obj

OBJTREE	= $(SRCTREE:srcs%=${OBJDIR}%)

SERVICES = -D SILVER=1

CXXFLAGS= -O2 -std=c++23 -Wall -Werror -Wextra -march=native -mtune=native -pipe ${SERVICES}

DBFLAGS	= DEBUG=1 ${SERVICES} -std=c++23 -g3 -fsanitize=thread -O0

LFLAGS = -I${INCL}

LDFLAGS	= -g3 -I${INCL}

INCL	= includes

CRED = \033[1;31m
CGRN = \033[1;32m
CYEL = \033[1;33m
RSET = \033[0m

all:	${NAME}

${NAME}:	${DEPS}
		@if [[ $(if $(filter r,${MAKECMDGOALS}),1,0) == "1" ]]; then \
			printf "├──────────\n"; \
		else \
			printf "┌──────────\n"; \
		fi
		@${MAKE} ${OBJ}
		@printf "│\tSources for ${CGRN}${NAME}${RSET} done.\n"
		@${CXX} ${OBJ} -o ${NAME} ${LFLAGS}
		@strip ${NAME}
		@printf "├──────────\n├─>>> ${CGRN}${NAME}${RSET} compiled!\n└──────────\n"

${OBJDIR}/%.o:	${SUBDIR}/%.cpp | ${OBJDIR}
				@if [[ $(if $(filter dbg,${MAKECMDGOALS}),1,0) == "1" ]]; then \
					printf "│\t > ${CYEL}DEBUG${RSET} Compiling ${CYEL}$<${RSET} with ${CYEL}${DBFLAGS}${RSET} and ${CYEL}${LDFLAGS}${RSET}...\r"; \
					${CXX} -D ${DBFLAGS} -c $< -o $@ ${LDFLAGS}; \
				else \
					printf "│\t > Compiling ${CYEL}$<${RSET} for ${CGRN}${NAME}${RSET}...\r"; \
					${CXX} ${CXXFLAGS} -c $< -o $@; \
				fi
				@printf "\33[2K"

${OBJDIR}:
		@printf "│ Compiling ${CGRN}${NAME}${RSET}...\n"
		@printf "│\tCreating ${CYEL}${OBJTREE}${RSET} for ${CGRN}${NAME}${RSET}\n"
		@mkdir -p ${OBJTREE}

debug:
		@printf "┌──────────\n"
		@rm -rf obj
		@rm -rf ${NAME}
		@${MAKE} ${OBJ} dbg
		@printf "│\t${CYEL}DEBUG${RSET} Sources for ${CGRN}${NAME}${RSET} done.\n"
		@${CXX} ${OBJ} -D ${DBFLAGS} -o ${NAME} ${LFLAGS} ${LDFLAGS}
		@printf "├──────────\n├─>>> ${CYEL}DEBUG${RSET} ${CGRN}${NAME}${RSET} compiled!\n└──────────\n"

clean:
		@if [[ $(if $(filter r,${MAKECMDGOALS}),1,0) == "1" ]]; then \
			printf "├──────────\n"; \
		else \
			printf "┌──────────\n"; \
		fi
		@if [ -d "./${OBJDIR}" ]; then \
			printf "│\tRemoving ${CRED}${OBJDIR}${RSET} for ${CYEL}${NAME}${RSET}\n"; \
			rm -rf ${OBJDIR}; \
		else \
			if [[ $(if $(filter fclean,${MAKECMDGOALS}),1,0) == "1" ]]; then \
				printf "│\t${CYEL}${OBJDIR}${RSET} already removed!\n├──────────\n"; \
			else \
				printf "│\t${CYEL}${OBJDIR}${RSET} already removed!\n└──────────\n"; \
			fi; \
		fi
		@if [ -d "./${NAME}.dSYM" ]; then \
			printf "│\tRemoving ${CYEL}${NAME}.dSYM${RSET}\n"; \
			rm -rf ${NAME}.dSYM; \
		fi

fclean: clean
		@if [ -e "./${NAME}" ]; then \
			if [[ $(if $(filter r,${MAKECMDGOALS}),1,0) == "1" ]]; then \
				printf "│\tRemoving ${CYEL}${NAME}${RSET}\n"; \
			else \
				printf "│\tRemoving ${CYEL}${NAME}${RSET}\n└──────────\n"; \
			fi; \
			rm -f ${NAME}; \
		else \
			if [[ $(if $(filter r,${MAKECMDGOALS}),1,0) == "1" ]]; then \
				printf "│\t${CYEL}${NAME}${RSET} already removed!\n├──────────\n"; \
			else \
				printf "│\t${CYEL}${NAME}${RSET} already removed!\n└──────────\n"; \
			fi; \
		fi

re:
		@printf "\n┌──────────\n│ Cleaning and ${CGRN}recompiling${RSET}...\n"
		@${MAKE} fclean r
		@${MAKE} all r

test: debug

.PHONY: all fclean clean re

$(eval r:;@:)
$(eval dbg:;@:)
