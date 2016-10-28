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
#include <iostream>
#include <iomanip>

LogOutPut::LogOutPut(std::string fileLog) : _logFile(fileLog) {
	if ((this->_fdLog = open(this->_logFile.c_str(), O_CREAT | O_WRONLY, S_IWUSR | S_IRUSR | S_IRGRP | S_IROTH)) < 0) {
		write(2, "ERROR : log file cant be open.\n", 31);
		throw std::exception();
	}
	lseek(this->_fdLog, 0, SEEK_END);

	std::string 	str(this->_getTime("[INFO] : Taskmaster start\n"));
	write(this->_fdLog, str.c_str(), str.size());
}

LogOutPut::~LogOutPut(void)  {
	this->exit();
}

void 				LogOutPut::exit(void) {
	std::string 	str(this->_getTime("[INFO] : Taskmaster stop"));
	write(this->_fdLog, str.c_str(), str.size());
	close(this->_fdLog);
}
std::string 		LogOutPut::_getTime(std::string str) {
	time_t 			t = time(NULL);
	struct 			tm *ti = localtime(&t);
	std::stringstream 	ss;

	ss << "[" << std::setw(2) << std::setfill('0') << ti->tm_mday << "/" << std::setw(2) << std::setfill('0') << (ti->tm_mon + DECAL_MONTH) << "/" << (ti->tm_year + DECAL_YEAR);
	ss << "-" << std::setw(2) << std::setfill('0') << ti->tm_hour << ":" << std::setw(2) << std::setfill('0') << ti->tm_min << ":" << std::setw(2) << std::setfill('0') << ti->tm_sec << "] ";
	ss << str;
	return (ss.str());
}

void				LogOutPut::putLogFile(std::string log) {
	std::string 	str(this->_getTime("[INFO] : " + log));
	write(this->_fdLog, str.c_str(), str.size());
}

void				LogOutPut::putLogFileError(std::string log) {
	std::string 	str(this->_getTime("[ERROR] : " + log));
	write (this->_fdLog, str.c_str(), str.size());
}

void				LogOutPut::putStdout(std::string log) {
	write (1, log.c_str(), log.size());
}

void				LogOutPut::putStderr(std::string log) {
	std::string 	newStr = "[ERROR] : " + log;
	write (2, newStr.c_str(), newStr.size());
}

void				LogOutPut::putAll(std::string log) {
	std::string 	str(this->_getTime("[INFO] : " + log));

	write(1, log.c_str(), log.size());
	write(this->_fdLog, str.c_str(), str.size());
}

void				LogOutPut::putAllError(std::string log) {
	std::string 	newStr = "[ERROR] : " + log;
	std::string 	str(this->_getTime(newStr));

	write (2, newStr.c_str(), newStr.size());
	write (this->_fdLog, newStr.c_str(), newStr.size());
}
