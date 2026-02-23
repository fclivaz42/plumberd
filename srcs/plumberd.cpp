/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   plumberd.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fclivaz <fclivaz@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/07 19:31:36 by fclivaz           #+#    #+#             */
/*   Updated: 2026/02/23 20:04:44 by fclivaz          ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/plumberd.hpp"
#include "../includes/ClassPlumberdServer.hpp"

static inline bool check_socket(const std::string& socket_path, char mode)
{
	bool	ret = false;

	switch (mode)
	{
		case MODE_SERVER:
			ret = ! access(socket_path.c_str(), F_OK);
			break;
		case MODE_CLIENT:
			ret = access(socket_path.c_str(), F_OK);
			break;
	}
	if (ret)
		std::cout << ( mode == MODE_CLIENT ? ERR_SOCKET_ABSENT : ERR_SOCKET_EXISTS ) << std::endl;
	return ret;
}

static int	sub(char service, const std::string& socket_path)
{
	char	listen[4] = {0};

	listen[0] = LISTEN;
	listen[1] = ' ';
	listen[2] = service;

	if (check_socket(socket_path, MODE_CLIENT))
		return ERR_SOCKET;

	std::cout << listen << std::endl;

	return SUCCESS;
}

static int	start_server(const std::string& socket_path)
{
	int		rcode;
	pid_t	pid;

	if (check_socket(socket_path, MODE_SERVER))
		return ERR_SOCKET;

	if (!DEBUG)
	{
		pid = fork();
		if (pid != 0)
		{
			std::cout << "plumberd starting... " << std::endl;
			return SUCCESS;
		}
	}

	InfoServer	server(getpid(), socket_path.c_str());

	if ((rcode = server.boilerplate()) != 0)
		return rcode;
	rcode = server.loop();

	return rcode;
}

int	main(int ac, char *av[])
{
	std::string		socket_path;
	const char		*xdg_dir = std::getenv(XDG_ENV);

	if (ac < 2 || strlen(av[1]) != 1)
	{
		std::cerr << USAGE_PLUMBERD << static_cast<char>(START) << USAGE_START << std::endl
			<< USAGE_ALTER << static_cast<char>(LISTEN) << USAGE_LISTEN << std::endl;
		return ERR_USAGE;
	}

	if (xdg_dir != nullptr)
		socket_path = xdg_dir;
	else
	{
		std::cerr << "XDG_RUNTIME_DIR not found. Falling back to /tmp." << std::endl;
		socket_path = "/tmp";
	}

	socket_path.append(SOCKET_FILE);

	switch (static_cast<commands>(av[1][0]))
	{
		case START:
			if (ac == 2)
				return start_server(socket_path);
			std::cerr << USAGE_PLUMBERD << static_cast<char>(START) << USAGE_START << std::endl;
			break;

		case LISTEN:
			if (ac == 3 && strlen(av[2]) == 1 && strchr(COMBINATIONS, av[2][0]))
				return sub(av[2][0], socket_path);
			std::cerr << USAGE_PLUMBERD << static_cast<char>(LISTEN) << USAGE_LISTEN << std::endl;
			break;

		case EXECUTE:
			break;

		default:
			std::cerr << "Incorrect parameter." << std::endl;
	}
	return ERR_USAGE;
}
