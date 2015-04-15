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
	std::string 		logFileName = this->_parse.getLogFileName();
	this->_logFile.open(logFileName);
	if (!this->_logFile.is_open()) {
		std::cerr << "ERROR file : log file '" << logFileName << " cant be opened." << std::endl;
	}
	// init log file
	this->_cmds[START] = &Taskmaster::_start;
	this->_cmds[RESTART] = &Taskmaster::_restart;
	this->_cmds[STOP] = &Taskmaster::_stop;
	this->_cmds[STATUS] = &Taskmaster::_status;
	this->_cmds[RELOAD] = &Taskmaster::_reload;
}


/**********************/
/*******COMMANDS*******/
/**********************/

void				Taskmaster::_start(std::string const &processName) {

	if (processName == "") {
		for (PList::iterator it = this->_process.begin(); it != this->_process.end(); ++it) {
			it->second->start();
		}
	} else {
		if (this->_process.find(processName) != this->_process.end()) {
			this->_process[processName]->start();
		} else {
			std::cerr << "ERROR : process name '" << processName << "' unknow" << std::endl;
		}
	}
}

void				Taskmaster::_restart(std::string const &processName) {

	if (processName == "") {
		for (PList::iterator it = this->_process.begin(); it != this->_process.end(); ++it) {
			it->second->restart();
		}
	} else {
		if (this->_process.find(processName) != this->_process.end()) {
			this->_process[processName]->restart();
		} else {
			std::cerr << "ERROR : process name '" << processName << "' unknow" << std::endl;
		}
	}
}

void				Taskmaster::_stop(std::string const &processName) {

	if (processName == "") {
		for (PList::iterator it = this->_process.begin(); it != this->_process.end(); ++it) {
			it->second->stop();
		}
	} else {
		if (this->_process.find(processName) != this->_process.end()) {
			this->_process[processName]->stop();
		} else {
			std::cerr << "ERROR : process name '" << processName << "' unknow" << std::endl;
		}
	}
}

void				Taskmaster::_status(std::string const &processName) {

	if (processName == "") {
		for (PList::iterator it = this->_process.begin(); it != this->_process.end(); ++it) {
			it->second->status();
		}
	} else {
		if (this->_process.find(processName) != this->_process.end()) {
			this->_process[processName]->status();
		} else {
			std::cerr << "ERROR : process name '" << processName << "' unknow" << std::endl;
		}
	}
}

void				Taskmaster::_reload(std::string const &processName) {
	this->_parse.reloadFile();

	if (processName == "") {
		mStrStr 	infoAllProcess = this->_parse.getAllProcess();

		for (PList::iterator it = this->_process.begin(); it != this->_process.end(); ++it) {
			if (infoAllProcess.find(it->first) != infoAllProcess.end())
				it->second->reload(infoAllProcess[it->first]);
			else
				this->_deleteProcess(it->first);
		}
		for (mStrStr::iterator it = infoAllProcess.begin(); it != infoAllProcess.end(); ++it) {
			if (this->_process.find(it->first) == this->_process.end())
				this->_createProcess(it->first, it->second);
		}
	} else {
		StrStr 	infoProcess = this->_parse.getProcess(processName);
		if (this->_process.find(processName) != this->_process.end()) {
			this->_process[processName]->reload(infoProcess);
		} else {
			std::cerr << "ERROR : process name '" << processName << "' unknow" << std::endl;
		}
	}
}

void				Taskmaster::_exit(void) {
	this->_deleteProcess();
	return ;
}

/*************************/
/***********RUN***********/
/*************************/

void				Taskmaster::_autoStart(std::string const &processName) {

	if (processName == "") {
		for (PList::iterator it = this->_process.begin(); it != this->_process.end(); ++it) {
			it->second->autostart();
		}
	} else {
		if (this->_process.find(processName) != this->_process.end()) {
			this->_process[processName]->autostart();
		} else {
			std::cerr << "ERROR : process name unknow" << std::endl;
		}
	}
	return ;
}

void				Taskmaster::_deleteProcess(std::string const &processName) {
	if (processName == "") {
		return ;
	} else {
		return ;
	}
	return ;
}

void				Taskmaster::_createProcess(std::string const &name, StrStr const &infoNewProcess) {
	Process 		*newProcess = new Process(name, this->_logFile, infoNewProcess);

	this->_process[name] = newProcess;
}


void						Taskmaster::run(void) {
	std::string 			line;

	this->_reload();
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
			Cmd 			func = this->_cmds[command];
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
