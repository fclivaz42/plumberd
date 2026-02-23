/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClassBrightnessd.hpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fclivaz <fclivaz@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/09 18:27:28 by fclivaz           #+#    #+#             */
/*   Updated: 2026/02/23 20:14:02 by fclivaz          ###   LAUSANNE.ch       */
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

#define BRIGHTNESS_POPUP_NAME "brightpopup"

class Brightnessd : public Popupd
{
	public:
		Brightnessd(char mode);
		~Brightnessd();
};

#endif // !CLASS_BRIGHTNESSD_HPP
