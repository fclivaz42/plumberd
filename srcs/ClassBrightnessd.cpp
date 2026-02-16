/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClassBrightnessd.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fclivaz <fclivaz@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/09 21:10:13 by fclivaz           #+#    #+#             */
/*   Updated: 2026/02/16 21:22:54 by fclivaz          ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ClassBrightnessd.hpp"
#include "../includes/plumberd.hpp"

Brightnessd::Brightnessd(char mode) : Popupd(BRIGHTNESS_POPUP_NAME)
{
	if (mode == MODE_CLIENT)
		return;
}

Brightnessd::~Brightnessd()
{}
