// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   LogOutPut.hpp                                        :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: ftaffore <ftaffore@student.42.fr>          +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2015/04/27 19:40:29 by ftaffore          #+#    #+#             //
//   Updated: 2015/04/27 19:40:29 by ftaffore         ###   ########.fr       //
//                                                                            //
// ************************************************************************** //

#ifndef LOG_OUT_PUT_HPP
# define LOG_OUT_PUT_HPP

#include <unistd.h>
#include <fcntl.h>
#include <string>

class LogOutPut
{
	std::string 	_logFile;
	int 			_fdLog;

public:
	LogOutPut(std::string fileLog);
	~LogOutPut();

	void			putLogFile(std::string str);
	void			putLogFileError(std::string str);
	void			putStdout(std::string str);
	void			putStderr(std::string str);
	void			putAll(std::string str);
	void			putAllError(std::string str);
};

#endif