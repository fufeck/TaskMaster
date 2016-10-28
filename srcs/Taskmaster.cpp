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

	this->_cmds[START] = &Taskmaster::_start;
	this->_cmds[RESTART] = &Taskmaster::_restart;
	this->_cmds[STOP] = &Taskmaster::_stop;
	this->_cmds[STATUS] = &Taskmaster::_status;
	this->_cmds[RELOAD] = &Taskmaster::_reload;
}

Taskmaster::~Taskmaster() {
	//	Lorsqu'une touche est frappée, je détruis le segment (le segment n'est pas détruit tant qu'au moins un processus est lié au segment)
	this->_delProgram();
	delete this->_logOutPut;
}



/**********************/
/*******COMMANDS*******/
/**********************/

void				Taskmaster::exit(void) {
	std::cout << "DEL PROG" << std::endl;
	this->_delProgram();
	std::cout << "DELETED PROG" << std::endl;
	// delete this->_logOutPut;
}

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
	if (!this->_parse.getStart()) {
		this->_parse.reloadFile();
	}
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
		this->_logOutPut->putAllError("'" + nameProgram + "' cant be add\n");
	}
	return ;
}

void						Taskmaster::run(void) {
	std::string 			line = "";
	int 					flags = fcntl(0, F_GETFL, 0);
	char					buff[4096];

	fcntl(0, F_SETFL, flags | O_NONBLOCK);
	this->_reload();
	this->_logOutPut->putStdout(PRONPT);
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
				this->_logOutPut->putAllError("command '" + cmd + "' unknow.\n");
				this->_logOutPut->putStdout(PRONPT);
			}
		}
		for (PList::iterator it = this->_programs.begin(); it != this->_programs.end(); ++it) {
			it->second->checkProcess();
		}
		usleep(500000);
	}

}
