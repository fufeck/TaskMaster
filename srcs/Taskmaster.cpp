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

Taskmaster::Taskmaster(int ac, char **av) : _end(false), _parse(ac, av) {
	// init parse
	std::string 		logFileName = this->_parse.getLogFileName();
	this->_logFile.open(logFileName);
	if (!this->_logFile.is_open()) {
		std::cerr << "ERROR file : log file '" << logFileName << " cant be opened." << std::endl;
		throw std::exception();
	}
	// init log file
	this->_cmds[START] = &Taskmaster::_start;
	this->_cmds[RESTART] = &Taskmaster::_restart;
	this->_cmds[STOP] = &Taskmaster::_stop;
	this->_cmds[STATUS] = &Taskmaster::_status;
	this->_cmds[RELOAD] = &Taskmaster::_reload;
}

Taskmaster::~Taskmaster() {}

/**********************/
/*******COMMANDS*******/
/**********************/

void				Taskmaster::_start(std::string const &programName) {

	if (programName == "") {
		for (PList::iterator it = this->_programs.begin(); it != this->_programs.end(); ++it) {
			if (it->second->start()) {
				this->_logFile << "[" << it->first << "] : start.";
			} else {
				this->_logFile << "ERROR : [" << it->first << "] : cant be start.";
				std::cerr << "ERROR : [" << it->first << "] : cant be start." << std::endl;
			}
		}
	} else {
		if (this->_programs.find(programName) != this->_programs.end()) {
			if (this->_programs[programName]->start()) {
				this->_logFile << "[" << programName << "] : start.";
			} else {
				this->_logFile << "ERROR : [" << programName << "] : cant be start.";
				std::cerr << "ERROR : [" << programName << "] : cant be start." << std::endl;
			}
		} else {
			this->_logFile << "ERROR : program's name '" << programName << "' unknow" << std::endl;
			std::cerr << "ERROR : program's name '" << programName << "' unknow" << std::endl;
		}
	}
}

void				Taskmaster::_restart(std::string const &programName) {

	if (programName == "") {
		for (PList::iterator it = this->_programs.begin(); it != this->_programs.end(); ++it) {
			if (it->second->restart()) {
				this->_logFile << "[" << it->first << "] : restart.";
			} else {
				this->_logFile << "ERROR : [" << it->first << "] : cant be restart.";
				std::cerr << "ERROR : [" << it->first << "] : cant be restart." << std::endl;
			}
		}
	} else {
		if (this->_programs.find(programName) != this->_programs.end()) {
			if (this->_programs[programName]->restart()) {
				this->_logFile << "[" << programName << "] : restart.";
			} else {
				this->_logFile << "ERROR : [" << programName << "] : cant be restart.";
				std::cerr << "ERROR : [" << programName << "] : cant be restart." << std::endl;
			}
		} else {
			this->_logFile << "ERROR : program's name '" << programName << "' unknow" << std::endl;
			std::cerr << "ERROR : program's name '" << programName << "' unknow" << std::endl;
		}
	}
}

void				Taskmaster::_stop(std::string const &programName) {

	if (programName == "") {
		for (PList::iterator it = this->_programs.begin(); it != this->_programs.end(); ++it) {
			if (it->second->stop()) {
				this->_logFile << "[" << it->first << "] : restart.";
			} else {
				this->_logFile << "ERROR : [" << it->first << "] : cant be restart.";
				std::cerr << "ERROR : [" << it->first << "] : cant be restart." << std::endl;
			}
		}
	} else {
		if (this->_programs.find(programName) != this->_programs.end()) {
			if (this->_programs[programName]->restart()) {
				this->_logFile << "[" << programName << "] : stoped.";
			} else {
				this->_logFile << "ERROR : [" << programName << "] : cant be stopped.";
				std::cerr << "ERROR : [" << programName << "] : cant be stopped." << std::endl;
			}
		} else {
			this->_logFile << "ERROR : program's name '" << programName << "' unknow" << std::endl;
			std::cerr << "ERROR : program's name '" << programName << "' unknow" << std::endl;
		}
	}
}

void				Taskmaster::_status(std::string const &programName) {

	if (programName == "") {
		for (PList::iterator it = this->_programs.begin(); it != this->_programs.end(); ++it) {
			std::cout << it->second->status() << std::endl;
		}
	} else {
		if (this->_programs.find(programName) != this->_programs.end()) {
			std::cout << this->_programs[programName]->status() << std::endl;
		} else {
			this->_logFile << "ERROR : program's name '" << programName << "' unknow" << std::endl;
			std::cerr << "ERROR : program's name '" << programName << "' unknow" << std::endl;
		}
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
			if (this->_programs.find(programName) != this->_programs.end()) {
				this->_programs[programName]->reload(program);
			} else {
				this->_addProgram(programName, program);
			}
		} else {
			if (this->_programs.find(programName) != this->_programs.end()) {
				this->_delProgram(programName);
			} else {
				this->_logFile << "ERROR : program's name '" << programName << "' unknow" << std::endl;
				std::cerr << "ERROR : program's name '" << programName << "' unknow" << std::endl;
			}
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
		Program 		*newProgram = new Program(newProgramFeature);
		this->_programs[nameProgram] = newProgram;		
	} catch (std::exception & e) {
		this->_logFile << "ERROR : '" << nameProgram << "' cant be add" << std::endl;
		std::cerr << "ERROR : '" << nameProgram << "' cant be add" << std::endl;
	}
	return ;
}

void				Taskmaster::_exit(void) {
	//this->_mutex.lock();
	this->_delProgram();
	this->_logFile.close();
	this->_end = true;
	//this->_mutex.unlock();
	return ;
}
/*
void						Taskmaster::runCheck(void) {

	this->_mutex.lock();
	while (!this->_end) {
		this->_mutex.unlock();
		for (PList::iterator it = this->_programs.begin(); it != this->_programs.end(); it++) {
			this->_mutex.lock();
			it->second->timerCheck();
			this->_mutex.unlock();
		}
		this->_mutex.lock();
	}
	this->_mutex.unlock();
}
*//*
void						Taskmaster::run(void) {
	std::string 			line;

	int flags = fcntl(0, F_GETFL, 0);
	fcntl(0, F_SETFL, flags | O_NONBLOCK);

	this->_reload();
	std::cout << "$Taskmaster>";
	std::getline(std::cin, line);
	while (line != EXIT) {

		if (line.size() != 0) {
			std::stringstream 	ss;
			std::string 		cmd;
			std::string 		params;

			ss.str(line);
			ss >> cmd;
			ss >> params;
			if (this->_cmds.find(cmd) != this->_cmds.end()) {
				CmdFunc 		func = this->_cmds[cmd];
				(this->*func)(params);
			} else {
				std::cerr << "ERROR : command '" << cmd << "' unknow." << std::endl;
			}
			std::cout << "$Taskmaster>";
		}
		for (PList::iterator it = this->_programs.begin(); it != this->_programs.end(); ++it) {
			it->second->checkProcess();
		}
		sleep(1);
		std::getline(std::cin, line);
	}
	this->_exit();
}

void 						Taskmaster::initSelect(fd_set & rd) {
	FD_ZERO(&rd);
	FD_SET(0, &rd);
	select(1, &rd, NULL, NULL, NULL);
}
*/
void						Taskmaster::run(void) {
	std::string 			line = "";

	std::cout << "$Taskmaster>";
	sleep(1);
	this->_reload();
	while (line != EXIT) {
		fd_set					rd;	
		FD_ZERO(&rd);
		FD_SET(0, &rd);
		select(1, &rd, NULL, NULL, NULL);
		if (FD_ISSET(0, &rd)) {
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
			} else if (line != EXIT) {
				std::cerr << "ERROR : command '" << cmd << "' unknow." << std::endl;
			}
			if (line != EXIT)
				std::cout << "$Taskmaster>";
		}
		for (PList::iterator it = this->_programs.begin(); it != this->_programs.end(); ++it) {
			it->second->checkProcess();
		}
		sleep(1);
	}
	this->_exit();
}

/*

void						Taskmaster::run(void) {
	std::thread				thCheck(&Taskmaster::runCheck, this);
	std::thread				thCmd(&Taskmaster::runCmd, this);

	thCheck.join();
	thCmd.join();

	return ;
}
*/