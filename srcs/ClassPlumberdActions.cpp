/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClassPlumberdActions.cpp                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fclivaz <fclivaz@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/23 18:30:16 by fclivaz           #+#    #+#             */
/*   Updated: 2026/02/23 20:27:54 by fclivaz          ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ClassPlumberdServer.hpp"
#include "../includes/plumberd.hpp"

std::tuple<char, char, std::string> InfoServer::parse_read(size_t *i)
{
	char		command, service;
	const char	*input = this->_buf;
	std::string	 parameters;

	if (strchr(input, ' ') - input != 1)
		throw ERR_WRONG_COMMAND;

	command = input[0];

	if ((command == EXECUTE && (strchr(&input[2], ' ') - &input[2] != 1)) || (command == LISTEN && strlen(&input[2]) > 1))
		throw ERR_WRONG_SERVICE;

	service = input[2];
	// Not a problem since buf will always be MAXBUFSIZE + 1
	parameters = &input[4];

	if (DEBUG)
	{
		std::clog << "Command: " << command << std::endl;
		std::clog << "Service: " << service << std::endl;
		std::clog << "Parameters: " << parameters << std::endl;
		std::clog << "i: " << *i << std::endl;
	}
	return { command, service, parameters };
}

int	InfoServer::subscribe(size_t *i, const char service)
{
	std::string	response;

	_subbed_clients[service].push_back(_clients[*i].fd);
	switch (static_cast<combinations>(service))
	{
		case VOLUME:
			response = std::format("{}", this->_volctl.get_level());
			break;

		case BRIGHTNESS:
			response = std::format("{}", this->_brictl.get_level());
			break;

		case BATTERY:
			break;

		case BLUETOOTH:
			break;

		case WIRELESS:
			break;

		default:
			throw ERR_UNKNOWN_SERVICE;
	}
	send(_clients[*i].fd, response.c_str(), response.size(), MSG_NOSIGNAL);
	return SUCCESS;
}

int	InfoServer::process_query(size_t *i)
{
	try
	{
		std::tuple<char, char, std::string>	parsed(this->parse_read(i));
		switch (static_cast<commands>(std::get<0>(parsed)))
		{
			case LISTEN:
				if (DEBUG)
					std::cout << "listening to service " << std::get<1>(parsed) << std::endl;
				subscribe(i, std::get<1>(parsed));
				break;

			case EXECUTE:
				if (DEBUG)
					std::cout << "Executing command on device " << std::get<1>(parsed) << " with parameters "  << std::get<2>(parsed) << std::endl;
				// execute();
				break;

			default:
				throw ERR_UNKNOWN_COMMAND;
		}
	}
	catch (const error_codes ecode)
	{
		int			fd = _clients[*i].fd;
		const char	*err = error_strings[ecode];

		send(fd, err, strlen(err), MSG_NOSIGNAL);
		this->close_erase_rollback(i);
		return ecode;
	}
	return SUCCESS;
}
