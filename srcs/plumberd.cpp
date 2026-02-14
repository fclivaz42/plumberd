/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   plumberd.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fclivaz <fclivaz@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/07 19:31:36 by fclivaz           #+#    #+#             */
/*   Updated: 2026/02/14 23:53:10 by fclivaz          ###   LAUSANNE.ch       */
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

static int	send_command(char **args, const std::string& socket_path)
{
	char	exec[MAXBUFSIZE + 1] = {0};
	int		max_copy = MAXBUFSIZE;

	exec[0] = EXEC;
	max_copy -= 1;

	if (check_socket(socket_path, MODE_CLIENT))
		return ERR_SOCKET;

	for (; *args != nullptr; ++args)
	{
		exec[strlen(exec)] = ' ';
		max_copy -= strlcat(exec, *args, max_copy);
	}
	std::cout << exec << std::endl;

	return SUCCESS;
}

static int	start_server(const std::string& socket_path)
{
	int		rcode;
	pid_t	pid;

	if (check_socket(socket_path, MODE_SERVER))
		return ERR_SOCKET;

	pid = fork();
	if (pid != 0)
	{
		std::cout << "volumed starting... " << std::endl;
		return SUCCESS;
	}

	InfoServer	server(getpid(), socket_path.c_str());

	if ((rcode = server.boilerplate()) != 0)
		return rcode;
	rcode = server.loop();

	return rcode;
}

int	main(int ac, char *av[], char *env[])
{
	std::string		socket_path;

	if (ac < 2 || strlen(av[1]) != 1)
	{
		std::cerr << USAGE_PLUMBERD << START << USAGE_START << std::endl
			<< USAGE_ALTER << LISTEN << USAGE_LISTEN << std::endl
			<< USAGE_ALTER << VOL_SET << USAGE_VOLSET << std::endl
			<< USAGE_ALTER << VOL_MUT << USAGE_VOLMUT << std::endl
			<< USAGE_ALTER << BRI_SET << USAGE_BRISET << std::endl;
		return ERR_USAGE;
	}

	for (size_t i = 0; env[i] != NULL; i++)
	{
		if (! strncmp(env[i], XDG_ENV, strlen(XDG_ENV)))
		{
			socket_path = (strchr(env[i], '=') + 1);
			break;
		}
	}

	if (socket_path.size() == 0)
	{
		std::cerr << "XDG_RUNTIME_DIR not found. Exiting." << std::endl;
		return ERR_ENV;
	}

	socket_path.append(SOCKET_FILE);

	switch (av[1][0])
	{
		case START:
			if (ac == 2)
				return start_server(socket_path);
			std::cerr << USAGE_PLUMBERD << START << USAGE_START << std::endl;
			break;

		case LISTEN:
			if (ac == 3 && strlen(av[2]) == 1 && strchr("vb", av[2][0]))
				return sub(av[2][0], socket_path);
			std::cerr << USAGE_PLUMBERD << LISTEN << USAGE_LISTEN << std::endl;
			break;

		case VOL_SET:
			if (ac == 3)
				return send_command(&av[1], socket_path);
			std::cerr << USAGE_PLUMBERD << VOL_SET << USAGE_VOLSET << std::endl;
			break;

		case VOL_MUT:
			if (ac == 2)
				return send_command(&av[1], socket_path);
			std::cerr << USAGE_PLUMBERD << VOL_MUT << USAGE_VOLMUT << std::endl;
			break;

		case BRI_SET:
			if (ac == 3 || ac == 4)
				return send_command(&av[1], socket_path);
			std::cerr << USAGE_PLUMBERD << BRI_SET << USAGE_BRISET << std::endl;
			break;

		default:
			std::cerr << "Incorrect parameter." << std::endl;
	}
	return ERR_USAGE;
}
