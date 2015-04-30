// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   Taskmaster.cpp                                     :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: ftaffore <ftaffore@student.42.fr>          +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2015/04/13 12:49:41 by ftaffore          #+#    #+#             //
//   Updated: 2015/04/13 12:49:42 by ftaffore         ###   ########.fr       //
//                                                                            //
// ************************************************************************** //

#include "Taskmaster.hpp"

Taskmaster::Taskmaster(int ac, char **av) : _parse(ac, av) {
	this->_logOutPut = new LogOutPut(this->_parse.getLogFileName());

	//this->_logFile.open(logFileName);
	//if (!this->_logFile.is_open()) {
		//std::cerr << "ERROR file : log file '" << logFileName << " cant be opened." << std::endl;
		//throw std::exception();
	//}
	this->_cmds[START] = &Taskmaster::_start;
	this->_cmds[RESTART] = &Taskmaster::_restart;
	this->_cmds[STOP] = &Taskmaster::_stop;
	this->_cmds[STATUS] = &Taskmaster::_status;
	this->_cmds[RELOAD] = &Taskmaster::_reload;
}

Taskmaster::~Taskmaster() {
	this->_delProgram();
}

/**********************/
/*******COMMANDS*******/
/**********************/

void				Taskmaster::_start(std::string const &programName) {

	if (programName == "") {
		for (PList::iterator it = this->_programs.begin(); it != this->_programs.end(); ++it)
			it->second->start();
	} else {
		if (this->_programs.find(programName) != this->_programs.end())
			this->_programs[programName]->start();
		else
			this->_logOutPut->putStderr("program's name '" + programName + "' unknow\n");
	}
}

void				Taskmaster::_restart(std::string const &programName) {

	if (programName == "") {
		for (PList::iterator it = this->_programs.begin(); it != this->_programs.end(); ++it)
			it->second->restart();
	} else {
		if (this->_programs.find(programName) != this->_programs.end())
			this->_programs[programName]->restart();
		else
			this->_logOutPut->putStderr("program's name '" + programName + "' unknow\n");
	}
}

void				Taskmaster::_stop(std::string const &programName) {

	if (programName == "") {
		for (PList::iterator it = this->_programs.begin(); it != this->_programs.end(); ++it)
			it->second->stop();
	} else {
		if (this->_programs.find(programName) != this->_programs.end())
			this->_programs[programName]->stop();
		else
			this->_logOutPut->putStderr("program's name '" + programName + "' unknow\n");

	}
}

void				Taskmaster::_status(std::string const &programName) {

	if (programName == "") {
		for (PList::iterator it = this->_programs.begin(); it != this->_programs.end(); ++it)
			it->second->status();
	} else {
		if (this->_programs.find(programName) != this->_programs.end())
			this->_programs[programName]->status();
		else
			this->_logOutPut->putStderr("program's name '" + programName + "' unknow\n");
	}
}

void				Taskmaster::_reload(std::string const &programName) {
	if (!this->_parse.getStart())
		this->_parse.reloadFile();

	if (programName == "") {
		m_feature			allFeature = this->_parse.getAllProgramFeature();

		for (PList::iterator it = this->_programs.begin(); it != this->_programs.end(); ++it) {
			if (allFeature.find(it->first) != allFeature.end())
				it->second->reload(allFeature[it->first]);
			else
				this->_delProgram(it->first);
		}
		for (m_feature::iterator it = allFeature.begin(); it != allFeature.end(); ++it) {
			if (this->_programs.find(it->first) == this->_programs.end())
				this->_addProgram(it->first, it->second);
		}
	} else {
		ProgramFeature 		program = this->_parse.getProgramFeature(programName);

		if (program.isGood()) {
			if (this->_programs.find(programName) != this->_programs.end())
				this->_programs[programName]->reload(program);
			else
				this->_addProgram(programName, program);
		} else {
			if (this->_programs.find(programName) != this->_programs.end())
				this->_delProgram(programName);
			else
				this->_logOutPut->putStderr("program's name '" + programName + "' unknow\n");
		}
	}
}



/*************************/
/***********RUN***********/
/*************************/

void				Taskmaster::_delProgram(std::string const &programName) {
	if (programName == "") {
		this->_stop();
		this->_programs.erase(this->_programs.begin(), this->_programs.end());
	} else if (this->_programs.find(programName) != this->_programs.end()) {
		this->_stop(programName);
		this->_programs.erase(programName);
	}
	return ;
}

void				Taskmaster::_addProgram(std::string const &nameProgram, ProgramFeature const &newProgramFeature) {
	try {
		Program 		*newProgram = new Program(newProgramFeature, this->_logOutPut);
		this->_programs[nameProgram] = newProgram;		
	} catch (std::exception & e) {
		this->_logOutPut->putStderr("'" + nameProgram + "' cant be add\n");
	}
	return ;
}

void						Taskmaster::run(void) {
	std::string 			line = "";
	int 					flags = fcntl(0, F_GETFL, 0);
	char					buff[4096];

	fcntl(0, F_SETFL, flags | O_NONBLOCK);
	this->_logOutPut->putStdout(PRONPT);
	this->_reload();
	while (line != EXIT) {
		line = "";

		int len = 0;
		if ((len = read(0, buff, 4095)) > 0) {
			std::stringstream 	ss;
			std::string 		cmd;
			std::string 		params;

			(len > 0) ? (buff[len - 1] = '\0') : (buff[len] = '\0');
			line = buff;
			ss.str(line);
			ss >> cmd;
			ss >> params;
			if (this->_cmds.find(cmd) != this->_cmds.end()) {
				CmdFunc 		func = this->_cmds[cmd];
				(this->*func)(params);
				this->_logOutPut->putStdout(PRONPT);
			} else if (line != EXIT && line != "") {
				this->_logOutPut->putStderr("command '" + cmd + "' unknow.\n");
				this->_logOutPut->putStdout(PRONPT);
			}
		}
		for (PList::iterator it = this->_programs.begin(); it != this->_programs.end(); ++it) {
			it->second->checkProcess();
		}
		usleep(500000);
	}
}
/*
void						Taskmaster::run(void) {
	std::string 			line = "";

	this->_logOutPut->putStdout(PRONPT);
	this->_reload();
	while (line != EXIT) {
		write(1, "1\n", 2);
		fd_set					rd;	
		FD_ZERO(&rd);
		FD_SET(0, &rd);
		write(1, "2\n", 2);
		if (select(1, &rd, NULL, NULL, NULL) == 1) {

			write(1, "3\n", 2);
			if (FD_ISSET(0, &rd)) {
				write(1, "4\n", 2);
				std::stringstream 	ss;
				std::string 		cmd;
				std::string 		params;

				std::getline(std::cin, line);
				ss.str(line);
				ss >> cmd;
				ss >> params;
				if (this->_cmds.find(cmd) != this->_cmds.end()) {
					CmdFunc 		func = this->_cmds[cmd];
					(this->*func)(params);
					this->_logOutPut->putStdout(PRONPT);
				} else if (line != EXIT) {
					this->_logOutPut->putStderr("command '" + cmd + "' unknow.\n");
					this->_logOutPut->putStdout(PRONPT);
				}
			}
			write(1, "5\n", 2);
		} else {
			write(1, "6\n", 2);
			for (PList::iterator it = this->_programs.begin(); it != this->_programs.end(); ++it) {
				it->second->checkProcess();
			}
		}
		write(1, "7\n", 2);
		usleep(50000);
	}
}
*/