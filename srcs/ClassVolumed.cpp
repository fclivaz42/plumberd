/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClassVolumed.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fclivaz <fclivaz@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/09 20:39:17 by fclivaz           #+#    #+#             */
/*   Updated: 2026/02/14 16:06:27 by fclivaz          ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ClassVolumed.hpp"
#include "../includes/plumberd.hpp"

Volumed::Volumed(char mode) : Popupd(VOLUME_POPUP_NAME)
{
	if (mode == MODE_CLIENT)
		return;
}

Volumed::~Volumed()
{}

int	Volumed::set_volume(const std::string& amount)
{
	if (parse_amount(amount))
		throw std::runtime_error("Badly formatted amount.");
	return this->_level;
}

int	Volumed::switch_mute()
{

	return this->_level;
}
