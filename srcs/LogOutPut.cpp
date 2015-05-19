// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   LogOutPut.cpp                                        :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: ftaffore <ftaffore@student.42.fr>          +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2015/04/27 19:41:02 by ftaffore          #+#    #+#             //
//   Updated: 2015/04/27 19:41:03 by ftaffore         ###   ########.fr       //
//                                                                            //
// ************************************************************************** //

#include "LogOutPut.hpp"

LogOutPut::LogOutPut(std::string fileLog) : _logFile(fileLog) {
	if ((this->_fdLog = open(this->_logFile.c_str(), O_CREAT | O_WRONLY, S_IWUSR | S_IRUSR | S_IRGRP | S_IROTH)) < 0) {
		write(2, "ERROR : log file cant be open.\n", 31);
		throw std::exception();
	}
}

LogOutPut::~LogOutPut(void)  {
	close(this->_fdLog);
}

void			LogOutPut::putLogFile(std::string str) {
	write (this->_fdLog, str.c_str(), str.size());
}

void			LogOutPut::putLogFileError(std::string str) {
	std::string newStr = "ERROR : " + str;
	write (this->_fdLog, newStr.c_str(), newStr.size());
}

void			LogOutPut::putStdout(std::string str) {
	write (1, str.c_str(), str.size());
}

void			LogOutPut::putStderr(std::string str) {
	std::string newStr = "ERROR : " + str;
	write (2, newStr.c_str(), newStr.size());
}

void			LogOutPut::putAll(std::string str) {
	write (1, str.c_str(), str.size());
	write (this->_fdLog, str.c_str(), str.size());
}

void			LogOutPut::putAllError(std::string str) {
	std::string newStr = "ERROR : " + str;
	write (2, newStr.c_str(), newStr.size());
	write (this->_fdLog, newStr.c_str(), newStr.size());
}
