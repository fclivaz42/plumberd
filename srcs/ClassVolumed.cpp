/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClassVolumed.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fclivaz <fclivaz@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/09 20:39:17 by fclivaz           #+#    #+#             */
/*   Updated: 2026/02/16 21:21:17 by fclivaz          ###   LAUSANNE.ch       */
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
