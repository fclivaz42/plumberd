/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   plumberd.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fclivaz <fclivaz@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/07 19:54:05 by fclivaz           #+#    #+#             */
/*   Updated: 2026/02/23 19:59:08 by fclivaz          ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PLUMBERD_HPP
# define PLUMBERD_HPP

#include <cstdlib>
#include <iostream>
#include <unistd.h>

#define XDG_ENV				"XDG_RUNTIME_DIR"
#define SOCKET_FILE			"/plumberd.sock"
#define ERR_SOCK			"Could not create socket. Exiting."
#define ERR_BIND			"Could not bind to socket. Exiting."
#define ERR_LISTEN			"Could not listen on socket. Exiting."
#define ERR_SOCKET_EXISTS	"Socket file already exists, server is already running."
#define ERR_SOCKET_ABSENT	"Socket file does not exist! Is server alive?"

#define USAGE_PLUMBERD	"Usage: 'plumberd "
#define USAGE_ALTER		"     : 'plumberd "
#define USAGE_START		"' to start the server."
#define USAGE_LISTEN	" < b | v >' to listen to brightness or volume events."

#define COMMANDS		"lx"
#define COMBINATIONS	"vlbBw"

#define MODE_SERVER 0
#define MODE_CLIENT 1

#define MAXBUFSIZE 256

enum commands
{
	LISTEN = 'l',
	START = 's',
	EXECUTE = 'x'
};

enum combinations
{
	VOLUME = 'v',
	BRIGHTNESS = 'l',
	BATTERY = 'b',
	BLUETOOTH = 'B',
	WIRELESS = 'w'
};

enum error_codes
{
	SUCCESS = EXIT_SUCCESS,
	ERR_USAGE,
	ERR_SOCKET,
	ERR_SERVER,
	ERR_READ,
	ERR_WRONG_COMMAND,
	ERR_WRONG_SERVICE,
	ERR_UNKNOWN_COMMAND,
	ERR_UNKNOWN_SERVICE,
	ERR_EMPTY_COMMAND,
	MAX_COUNT
};

inline const char	*error_strings[MAX_COUNT] = {
	"Success.",
	"Wrong usage.",
	"Socket error.",
	"Server (poll) error.",
	"Server read() error.",
	"Invalid command formatting.\n",
	"Invalid service formatting.\n",
	"Unknown command.\n",
	"Unknwon service.\n",
	"Empty command sent.\n"
};

#ifndef DEBUG
# define DEBUG 0
#endif

#endif // !PLUMBERD_HPP
