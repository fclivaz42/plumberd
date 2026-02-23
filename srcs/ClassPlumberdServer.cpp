/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClassPlumberdServer.cpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fclivaz <fclivaz@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/07 20:12:55 by fclivaz           #+#    #+#             */
/*   Updated: 2026/02/23 20:27:21 by fclivaz          ###   LAUSANNE.ch       */
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

	if (!DEBUG)
		std::freopen("/tmp/plumberd.log", "w", stderr);
}

int	InfoServer::boilerplate()
{
	const char *fill = COMBINATIONS;
	this->_sfd = socket(AF_UNIX, SOCK_STREAM, 0);
	if (this->_sfd <= 0)
	{
		std::clog << ERR << ERR_SOCK << std::endl;
		return ERR_SOCKET;
	}
	if (bind(this->_sfd, (struct sockaddr *) &(this->_sockadr), sizeof(struct sockaddr_un)) < 0)
	{
		std::clog << ERR << ERR_BIND << std::endl;
		return ERR_SOCKET;
	}
	if (listen(this->_sfd, 64) < 0)
	{
		std::clog << ERR << ERR_LISTEN << std::endl;
		return ERR_SOCKET;
	}

	_clients.reserve(4);
	for (size_t i = 0; fill[i]; i++)
		_subbed_clients[fill[i]].reserve(4);
	return SUCCESS;
}

int	InfoServer::accept_incoming()
{
	struct sockaddr_un	cli_addr;
	socklen_t			cli_len = sizeof(cli_addr);
	struct pollfd		cli_pollfd;

	cli_pollfd.fd = accept(_sfd, (struct sockaddr*)&cli_addr, &cli_len);
	if (cli_pollfd.fd == -1)
		return ERR_SOCKET;
	else if (cli_pollfd.fd > 0)
		if (DEBUG)
			std::clog << "New connection with fd " << cli_pollfd.fd << std::endl;
	cli_pollfd.events = POLLIN;
	cli_pollfd.revents = 0;
	if (fcntl(cli_pollfd.fd, F_SETFL, O_NONBLOCK))
	{
		close(cli_pollfd.fd);
		return ERR_SOCKET;
	}
	_clients.push_back(cli_pollfd);
	return SUCCESS;
}

int	InfoServer::client_read(size_t *i)
{
	bzero(this->_buf, MAXBUFSIZE + 1);
	ssize_t bytes = read(_clients[*i].fd, this->_buf, MAXBUFSIZE);
	if (bytes < 0)
	{
		std::clog << "Error reading client " << _clients[*i].fd << "! Disconnecting..." << std::endl;
		this->close_erase_rollback(i);
		return ERR_READ;
	}
	if (bytes == 0)
	{
		std::clog << "Disconnecting client with fd " << _clients[*i].fd << std::endl;
		this->close_erase_rollback(i);
		return SUCCESS;
	}
	else if (bytes > 0)
	{
		this->_buf[bytes] = 0;
		if (DEBUG)
			std::clog << "Recieved: " << this->_buf << std::endl;
	}
	return SUCCESS;
}

int	InfoServer::loop()
{
	signal(SIGINT, set_signal);
	signal(SIGQUIT, set_signal);
	signal(SIGTERM, set_signal);
	signal(SIGUSR1, set_signal);
	signal(SIGUSR2, set_signal);

	_clients.push_back({_sfd, POLLIN, 0});

	while (poll(_clients.data(), _clients.size(), -1) > 0)
	{
		if (_clients[0].revents & POLLIN)
			if (this->accept_incoming() != SUCCESS)
				std::clog << "Socket error on fd" << std::endl;
		for (size_t i = 1; i < _clients.size(); i++)
		{
			if (_clients[i].revents & POLLHUP || _clients[i].revents & POLLERR || _clients[i].revents & POLLNVAL)
			{
				std::clog << "Disconnecting client with fd " << _clients[i].fd << std::endl;
				this->close_erase_rollback(&i);
			}
			else if (_clients[i].revents & POLLIN)
			{
				if (this->client_read(&i) != SUCCESS)
					std::clog << "Error reading from client; disconnecting." << std::endl;
				if (this->process_query(&i) != SUCCESS)
					std::clog << "Error processing client query; disconnecting." << std::endl;
			}
		}
	}
	switch (sig)
	{
		default:
			std::clog << ERR << "Poll failure: " << strerror(errno) << std::endl;
			return ERR_SERVER;
		case SIGINT:
			std::clog << "SIGINT ";
			break;
		case SIGQUIT:
			std::clog << "SIGQUIT ";
			break;
		case SIGTERM:
			std::clog << "SIGTERM ";
			break;
		case SIGUSR1:
			std::clog << "SIGUSR1 ";
			break;
		case SIGUSR2:
			std::clog << "SIGUSR2 ";
			break;
	}
	std::clog << "received. Terminating..." << std::endl;
	for (const auto& item : _clients)
		if (item.fd != this->_sfd)
			close(item.fd);
	return SUCCESS;
}

InfoServer::~InfoServer()
{
	if (unlink(this->_sockadr.sun_path) < 0)
		std::clog << strerror(errno) << " " << this->_sockadr.sun_path << std::endl;
	close(this->_sfd);
	std::clog << "Closed!" << std::endl;
}
