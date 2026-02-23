/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClassVolumed.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fclivaz <fclivaz@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/09 20:14:56 by fclivaz           #+#    #+#             */
/*   Updated: 2026/02/23 20:13:37 by fclivaz          ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLASS_VOLUMED_HPP
# define CLASS_VOLUMED_HPP

#include "AClassPopup.hpp"

#define VOLUME_POPUP_NAME "volpopup"

class Volumed : public Popupd
{
	private:
		bool	_mute;

	public:
		Volumed(char mode);
		~Volumed();

};

#endif // !CLASS_VOLUMED_HPP
