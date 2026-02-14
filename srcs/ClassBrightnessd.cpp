/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClassBrightnessd.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fclivaz <fclivaz@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/09 21:10:13 by fclivaz           #+#    #+#             */
/*   Updated: 2026/02/14 16:05:43 by fclivaz          ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ClassBrightnessd.hpp"
#include "../includes/plumberd.hpp"

Brightnessd::Brightnessd(char mode) : Popupd(BRIGHTNESS_POPUP_NAME), _bctl(REGEX_STR)
{
	if (mode == MODE_CLIENT)
		return;
}

Brightnessd::~Brightnessd()
{}

int	Brightnessd::set_brightness(const std::string& amount, const std::string& device)
{
	int			fd[2] = {0, 0};
	int			ret;
	pid_t		pid;
	size_t		bytes;
	char		buf[2048] = {0};
	std::smatch	smack;
	std::string	tmp;

	if (parse_amount(amount))
		throw std::runtime_error("Badly formatted amount.");

	// TODO: Watch out for device!
	const char	*const cmd[] = {"brightnessctl", "set", amount.c_str(), "-d", device.c_str(), NULL};

	if (pipe(fd) < 0)
		throw std::runtime_error(std::format("Unable to create pipes: {}", strerror(errno)));

	pid = fork();

	if (pid == 0)
	{
		close(fd[0]);
		dup2(fd[1], STDOUT_FILENO);
		if (execvp(cmd[0], (char* const*)cmd) == -1)
		{
			std::cerr << "Unable to execute " << cmd[0] << ": " << strerror(errno) << std::endl;
			exit(-1);
		}
	}
	else if (pid < 0)
		throw std::runtime_error(std::format("Unable to fork: {}", strerror(errno)));

	waitpid(pid, &ret, 0);

	if (ret < 0)
		throw std::runtime_error(std::format("brightnessctl exited with status: {}", ret));

	bytes = read(fd[0], buf, sizeof(buf));

	if (bytes <= 0)
		throw std::runtime_error(std::format("Unable to read: {}", strerror(errno)));

	buf[bytes] = 0;
	tmp = buf;
	close(fd[1]);

	if (std::regex_search(tmp, smack, _bctl))
		_level = atoi(smack.str().c_str());
	else
		throw std::runtime_error(std::format("Unable to find match in: {}", tmp));

	return this->_level;
}
