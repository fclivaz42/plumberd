/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClassPlumberdServer.cpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fclivaz <fclivaz@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/07 20:12:55 by fclivaz           #+#    #+#             */
/*   Updated: 2026/02/14 16:10:12 by fclivaz          ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ClassPlumberdServer.hpp"
#include "../includes/plumberd.hpp"

int sig = 0;

void set_signal(int rsig)
{
	sig = rsig;
}

InfoServer::InfoServer(const pid_t pid, const std::string& socket_path) : _pid(pid), _volctl(MODE_SERVER), _brictl(MODE_SERVER)
{
	bzero(&(this->_sockadr), sizeof(struct sockaddr_un));
	this->_sockadr.sun_family = AF_UNIX;
	strncpy(this->_sockadr.sun_path, socket_path.c_str(), sizeof(this->_sockadr.sun_path) - 1);
	std::freopen("/tmp/infod.log", "w", stdout);
	std::freopen("/tmp/infod.err", "w", stderr);
}

int	InfoServer::boilerplate()
{
	this->_sfd = socket(AF_UNIX, SOCK_STREAM, 0);
	if (this->_sfd <= 0)
	{
		std::cerr << ERR_SOCK << std::endl;
		return ERR_SOCKET;
	}
	if (bind(this->_sfd, (struct sockaddr *) &(this->_sockadr), sizeof(struct sockaddr_un)) < 0)
	{
		std::cerr << ERR_BIND << std::endl;
		return ERR_SOCKET;
	}
	if (listen(this->_sfd, 64) < 0)
	{
		std::cerr << ERR_LISTEN << std::endl;
		return ERR_SOCKET;
	}

	_volcli.reserve(4);
	_bricli.reserve(4);
	_clients.reserve(4);
	return SUCCESS;
}

int InfoServer::loop()
{
	std::cout << "looping! yippie :)" << std::endl;

	signal(SIGINT, set_signal);
	signal(SIGQUIT, set_signal);
	signal(SIGTERM, set_signal);
	signal(SIGUSR1, set_signal);
	signal(SIGUSR2, set_signal);

	_clients.push_back({_sfd, POLLIN, 0});

	while (poll(_clients.data(), _clients.size(), -1) > 0)
	{
		if (_clients[0].revents & POLLIN)
			std::cout << "server pollin" << std::endl;
			// accept_incoming();
		for (size_t i = 1; i < _clients.size(); i++)
		{
			// if (_clients[i].revents & POLLIN)
			// 	client_read(); which will figure out if the client is [e]xecuting or [s]ubscribing.
			// 	Send reply to execution and close immediately, close subscribed only on disconnect.
			// else if (_clients[i].revents & POLLOUT) // dont really check for pollout though lmao
			// 	forward();
		}
	}
	switch (sig)
	{
		default:
			std::cerr << "ERROR: poll failure: " << strerror(errno) << std::endl;
			return ERR_SERVER;
		case SIGINT:
			std::cout << "SIGINT ";
			break;
		case SIGQUIT:
			std::cout << "SIGQUIT ";
			break;
		case SIGTERM:
			std::cout << "SIGTERM ";
			break;
		case SIGUSR1:
			std::cout << "SIGUSR1 ";
			break;
		case SIGUSR2:
			std::cout << "SIGUSR2 ";
			break;
	}
	std::cout << "received. Terminating..." << std::endl;
	return SUCCESS;
}

InfoServer::~InfoServer()
{
	if (unlink(this->_sockadr.sun_path) < 0)
		std::cerr << strerror(errno) << " " << this->_sockadr.sun_path << std::endl;
	close(this->_sfd);
}
