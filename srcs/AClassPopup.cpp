/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AClassPopup.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fclivaz <fclivaz@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/09 20:25:36 by fclivaz           #+#    #+#             */
/*   Updated: 2026/02/23 17:56:48 by fclivaz          ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/AClassPopup.hpp"

Popupd::Popupd(const std::string& name) : _name(name)
{
}

Popupd::~Popupd()
{}

int Popupd::get_level() const
{
	return this->_level;
}

int Popupd::close_popup()
{

	return 0;
}

int Popupd::force_close()
{
	if (_open)
		return system(std::format("eww close {}", this->_name).c_str());
	return 0;
}
