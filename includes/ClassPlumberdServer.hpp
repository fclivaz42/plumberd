/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClassPlumberdServer.hpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fclivaz <fclivaz@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/07 19:58:12 by fclivaz           #+#    #+#             */
/*   Updated: 2026/02/23 20:27:13 by fclivaz          ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLASS_PLUMBERD_SERVER_HPP
# define CLASS_PLUMBERD_SERVER_HPP

#include "ClassBrightnessd.hpp"
#include "ClassVolumed.hpp"
#include "plumberd.hpp"
#include <sys/socket.h>
#include <sys/poll.h>
#include <sys/un.h>
#include <fcntl.h>
#include <csignal>
#include <cstdio>
#include <ostream>
#include <vector>
#include <tuple>
#include <unordered_map>

#define	ERR "[Error]: "

class InfoServer
{
	private:
		const pid_t									_pid;
		int											_sfd;
		char										_buf[MAXBUFSIZE + 1];
		Volumed										_volctl;
		Brightnessd									_brictl;
		struct sockaddr_un							_sockadr;
		std::vector<struct pollfd>					_clients;
		std::unordered_map<char, std::vector<int> >	_subbed_clients;

		int											connect();
		int											accept_incoming();
		int											client_read(size_t *i);
		int											execute();
		int											forward();
		int											subscribe(size_t *id, const char service);
		int											process_query(size_t *i);
		std::tuple<char, char, std::string>			parse_read(size_t *i);

		inline void close_erase_rollback(size_t *i)
		{
			for (auto& [key, value] : _subbed_clients)
			{
				const auto found = std::find(value.begin(), value.end(), _clients[*i].fd);
				if (found != value.end())
					value.erase(found);
			}
			close(_clients[*i].fd);
			_clients.erase(_clients.begin() + *i);
			*i = *i - 1;
		}

	public:
		InfoServer(const pid_t pid, const std::string& socket_path);
		~InfoServer();

		int	boilerplate();
		int	loop();
};

#endif // !CLASS_PLUMBERD_SERVER_HPP
