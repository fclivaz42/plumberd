/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AClassPopup.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fclivaz <fclivaz@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/09 20:25:34 by fclivaz           #+#    #+#             */
/*   Updated: 2026/02/23 20:14:40 by fclivaz          ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ACLASS_POPUP_HPP
# define ACLASS_POPUP_HPP

#include <regex>
#include <string>
#include <iostream>
#include <stdexcept>

class Popupd
{
	private:
		const std::string		_name;
		bool					_open;

	protected:
		int	_level = 0;

	public:
		Popupd(const std::string& name);
		virtual ~Popupd() = 0;
		int		get_level() const;
		int		open_popup();
		int		close_popup(); // TODO: std::thread this, conditional variable, check proceed on exit
		int		force_close();
};

#endif // !ACLASS_POPUP_HPP
