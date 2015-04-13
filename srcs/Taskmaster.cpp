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
	// init parse
	this->_logFile = this->_parse.getLogFile();
	// init log file
	this->_cmds[START] = &Taskmaster::start;
	this->_cmds[RESTART] = &Taskmaster::restart;
	this->_cmds[STOP] = &Taskmaster::stop;
	this->_cmds[STATUS] = &Taskmaster::status;
	this->_cmds[RELOAD] = &Taskmaster::reload;
}


/**********************/
/*******COMMANDS*******/
/**********************/

void				Taskmaster::start(std::string const &processName == "") {

	if (cmd == "") {
		for (ProcessList::iterator it = this->_process.begin(); it != this->_process.end(); ++it) {
			it->start();
		}
	} else {
		if (this->_process.find(processName) != this->_process.end()) {
			this->_process[processName].start();
		} else {
			std::cerr << "ERROR : process name '" << processName << "' unknow" << std::endl;
		}
	}
}

void				Taskmaster::restart(std::string const &processName == "") {

	if (cmd == "") {
		for (ProcessList::iterator it = this->_process.begin(); it != this->_process.end(); ++it) {
			it->restart();
		}
	} else {
		if (this->_process.find(processName) != this->_process.end()) {
			this->_process[processName].restart();
		} else {
			std::cerr << "ERROR : process name '" << processName << "' unknow" << std::endl;
		}
	}
}

void				Taskmaster::stop(std::string const &processName == "") {

	if (cmd == "") {
		for (ProcessList::iterator it = this->_process.begin(); it != this->_process.end(); ++it) {
			it->stop();
		}
	} else {
		if (this->_process.find(processName) != this->_process.end()) {
			this->_process[processName].stop();
		} else {
			std::cerr << "ERROR : process name '" << processName << "' unknow" << std::endl;
		}
	}
}

void				Taskmaster::status(std::string const &processName == "") {

	if (cmd == "") {
		for (ProcessList::iterator it = this->_process.begin(); it != this->_process.end(); ++it) {
			it->status();
		}
	} else {
		if (this->_process.find(processName) != this->_process.end()) {
			this->_process[processName].status();
		} else {
			std::cerr << "ERROR : process name '" << processName << "' unknow" << std::endl;
		}
	}
}

void				Taskmaster::reload(std::string const &processName == "") {
	this->_parse.reloadFile();

	if (cmd == "") {
		MStrStr 	infoAllProcess = this->_parse.getAllProcess();

		for (PList::iterator it = this->_process.begin(); it != this->_process.end(); ++it) {
			if (infoAllProcess.find(it->first) != infoAllProcess.end())
				it->reload(infoAllProcess[it->first]);
			else
				this->deleteProcess(it->second);
		}
		for (MStrStr::iterator it = this->_process.begin(); it != this->_process.end(); ++it) {
			if (this->_process.find(it->first) == this->_process.end())
				this->createProcess(it->first, it->second);
		}
	} else {
		if (this->_process.find(processName) != this->_process.end()) {
			this->_process[processName].reload();
		} else {
			std::cerr << "ERROR : process name '" << processName << "' unknow" << std::endl;
		}
	}
}

void				Taskmaster::_exit(void) {
	this->deleteProcess();
	return ;
}

/*************************/
/***********RUN***********/
/*************************/

void				Taskmaster::deleteProcess(std::string const &name) {

}

void				Taskmaster::createProcess(std::string const &name, StrStr const &infoNewProcess) {

	//autoStart
}

void				Taskmaster::autoStart(std::string const &processName == "") {

	if (cmd == "") {
		for (ProcessList::iterator it = this->_process.begin(); it != this->_process.end(); ++it) {
			if (it->getAutostart() == true && it->getState() == STOP) {
				it->start();
			}
		}
	} else {
		if (this->_process.find(processName) != this->_process.end()) {
			if (it->getAutostart() == true && it->getState() == STOP) {
				this->_process[processName].start();
			}
		} else {
			std::cerr << "ERROR : process name unknow" << std::endl;
		}
	}
	return ;
}

void						Taskmaster::run(void) {
	std::string 			line;

	this->reload("");
	std::cout << "Taskmaster$>";
	std::getline(std::cin, line);
	while (line != EXIT) {
		std::stringstream 	ss;
		std::string 		command;
		std::string 		params;

		ss.str(line);
		ss >> command;
		ss >> params;
		if (this->_cmds.find(command) != this->_cmds.end()) {
			commandFunc 		func = this->_cmds[command];
			(this->*func)(params);
		} else {
			std::cerr << "ERROR : command '" << command << "'' unknow." << std::endl;
		}
		std::cout << "Taskmaster$>";
		std::getline(std::cin, line);
	}
	this->_exit();
	return ;
}
