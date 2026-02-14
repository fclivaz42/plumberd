/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClassBrightnessd.hpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fclivaz <fclivaz@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/09 18:27:28 by fclivaz           #+#    #+#             */
/*   Updated: 2026/02/14 16:03:38 by fclivaz          ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLASS_BRIGHTNESSD_HPP
# define CLASS_BRIGHTNESSD_HPP

#include "AClassPopup.hpp"

#include <cerrno>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <sys/wait.h>

#define REGEX_STR "[0-9]{0,3}%"
#define BRIGHTNESS_POPUP_NAME "brightpopup"

class Brightnessd : Popupd
{
	private:
		const std::regex		_bctl;

	public:
		Brightnessd(char mode);
		~Brightnessd();

		int	set_brightness(const std::string& amount, const std::string& device);
};

#endif // !CLASS_BRIGHTNESSD_HPP
