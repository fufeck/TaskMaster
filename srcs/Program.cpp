// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   Program.cpp                                        :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: ftaffore <ftaffore@student.42.fr>          +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2015/04/13 12:51:47 by ftaffore          #+#    #+#             //
//   Updated: 2015/04/13 12:51:48 by ftaffore         ###   ########.fr       //
//                                                                            //
// ************************************************************************** //

#include "Program.hpp"

Program::Program(ProgramFeature programFeature) {

	//init var
	this->_state = STOPPED;
	this->_feature = programFeature;

	//init map
	this->_mapState["STOPPED"] = STOPPED;
	this->_mapState["RUNNING"] = RUNNING;
	this->_mapState["ERROR"] = ERROR;

	this->_mapUmask[0][0] = 0;
	this->_mapUmask[0][1] = S_IRUSR;
	this->_mapUmask[0][2] = S_IWUSR;
	this->_mapUmask[0][3] = S_IRUSR | S_IWUSR;
	this->_mapUmask[0][4] = S_IXUSR;
	this->_mapUmask[0][5] = S_IXUSR | S_IRUSR;
	this->_mapUmask[0][6] = S_IXUSR | S_IWUSR;
	this->_mapUmask[0][7] = S_IRWXU;

	this->_mapUmask[1][0] = 0;
	this->_mapUmask[1][1] = S_IRGRP;
	this->_mapUmask[1][2] = S_IWGRP;
	this->_mapUmask[1][3] = S_IRGRP | S_IWGRP;
	this->_mapUmask[1][4] = S_IXGRP;
	this->_mapUmask[1][5] = S_IXGRP | S_IRGRP;
	this->_mapUmask[1][6] = S_IXGRP | S_IWGRP;
	this->_mapUmask[1][7] = S_IRWXG;

	this->_mapUmask[2][0] = 0;
	this->_mapUmask[2][1] = S_IROTH;
	this->_mapUmask[2][2] = S_IWOTH;
	this->_mapUmask[2][3] = S_IROTH | S_IWOTH;
	this->_mapUmask[2][4] = S_IXOTH;
	this->_mapUmask[2][5] = S_IXOTH | S_IROTH;
	this->_mapUmask[2][6] = S_IXOTH | S_IWOTH;
	this->_mapUmask[2][7] = S_IRWXO;
	this->autostart();
}
/*
Program::Program(Program const &other) {
	this->_state = STOPPED;
	this->_feature = other.getFeature();

	//init map
	this->_mapState["STOPPED"] = STOPPED;
	this->_mapState["RUNNING"] = RUNNING;
	this->_mapState["ERROR"] = ERROR;

	this->_mapUmask[0][0] = 0;
	this->_mapUmask[0][1] = S_IRUSR;
	this->_mapUmask[0][2] = S_IWUSR;
	this->_mapUmask[0][3] = S_IRUSR | S_IWUSR;
	this->_mapUmask[0][4] = S_IXUSR;
	this->_mapUmask[0][5] = S_IXUSR | S_IRUSR;
	this->_mapUmask[0][6] = S_IXUSR | S_IWUSR;
	this->_mapUmask[0][7] = S_IRWXU;

	this->_mapUmask[1][0] = 0;
	this->_mapUmask[1][1] = S_IRGRP;
	this->_mapUmask[1][2] = S_IWGRP;
	this->_mapUmask[1][3] = S_IRGRP | S_IWGRP;
	this->_mapUmask[1][4] = S_IXGRP;
	this->_mapUmask[1][5] = S_IXGRP | S_IRGRP;
	this->_mapUmask[1][6] = S_IXGRP | S_IWGRP;
	this->_mapUmask[1][7] = S_IRWXG;

	this->_mapUmask[2][0] = 0;
	this->_mapUmask[2][1] = S_IROTH;
	this->_mapUmask[2][2] = S_IWOTH;
	this->_mapUmask[2][3] = S_IROTH | S_IWOTH;
	this->_mapUmask[2][4] = S_IXOTH;
	this->_mapUmask[2][5] = S_IXOTH | S_IROTH;
	this->_mapUmask[2][6] = S_IXOTH | S_IWOTH;
	this->_mapUmask[2][7] = S_IRWXO;
}

Program&						Program::operator=(ProgramFeature const & other) {
	this->_feature = other.getFeature();
	return (*this);
}
*/
eState								Program::getState(void) const {
	return this->_state;
}

ProgramFeature						Program::getFeature(void) const {
	return this->_feature;
}


/***********************/
/****EXECUTE*PROCESS****/
/***********************/

void 								Program::_setUmask(void) {
	mode_t							mask = 0;

	mask = (this->_mapUmask[0][this->_feature.getUmask()[0]] | this->_mapUmask[1][this->_feature.getUmask()[1]] | this->_mapUmask[2][this->_feature.getUmask()[2]]);
	umask(mask);
}

void 								Program::_setDirectory(void) {
	if (chdir(this->_feature.getDirectory().c_str()) < 0) {
//		this->_logFile << this->_name << " => ERROR : directory is incorrect." << std::endl;
		std::cerr << this->_feature.getProgramName() << " => ERROR : directory is incorrect." << std::endl;
		throw std::exception();
	}
}

void 								Program::_executeProgram(void) {
	//execl(const char * path, const char * arg, ...);
	//this->_logFile << this->_name << " => ERROR : execute command." << std::endl;
	std::cerr << this->_feature.getProgramName() << " => ERROR : execute command." << std::endl;
	throw std::exception();
}

void 								Program::_redirectLogfile(std::string const &fileName, int start) {
	int 							p[2];
	int 							end;

	if ((end = open(fileName.c_str(), O_WRONLY | O_CREAT)) < 0) {
		std::cerr << this->_feature.getProgramName() << " => ERROR : '" << fileName << "' cant be open." << std::endl;
		return ;
	}
	if (pipe(p) < 0) {
		std::cerr << this->_feature.getProgramName() << " => ERROR : pipe fail." << std::endl;
		return ;
	}
	dup2(start, p[1]);
	dup2(p[0], end);
}

void								Program::runProgram(void) {
	pid_t							pid;
	int 							ret;	

	if ((pid = fork()) < 0) {
		//this->_logFile << this->_name << " : fork fail." << std::endl;
		std::cerr << this->_feature.getProgramName() << " : fork fail." << std::endl;
		throw std::exception();
	}
	if (pid == 0) {
		this->_setUmask();
		this->_setDirectory();
		this->_redirectLogfile(this->_feature.getStdoutLogfile(), 1);
		if (this->_feature.getRedirectStderr())
			this->_redirectLogfile(this->_feature.getStderrLogfile(), 2);
		this->_executeProgram();
	}
	else {
		this->_mutex.lock();
		this->_pidChild = pid;
		this->_state = RUNNING;
		//this->_logFile << this->_name << " RUNNING with pid " << pid << std::endl;
		this->_mutex.unlock();
		wait(&ret);
		this->_mutex.lock();
		this->_pidChild = 0;
		this->_state = STOPPED;
		//this->_logFile << this->_name << " STOPPED" << pid << std::endl;
		this->_mutex.unlock();
	}
	exit(0);
}

/***********************/
/********COMMAND********/
/***********************/


void								Program::autostart(void) {
	if (this->_feature.getAutoStart() == true)
		this->start();
}

void								Program::start(void) {

	this->_mutex.lock();
	if (this->_state == STOPPED) {
		//START PROCESS (thread + fork)
		std::thread 				t(&Program::runProgram, this);

		t.join();
	} else if (this->_state == RUNNING) {
		//this->_logFile << this->_name << " is already running." << std::endl;
	} else if (this->_state == ERROR) {
		//this->_logFile << this->_name << " dont be running." << std::endl;
	}
	this->_mutex.unlock();
}

void								Program::restart(void) {

	this->_mutex.lock();
	if (this->_state == RUNNING) {
		this->_mutex.unlock();
		this->stop();
	}
	if (this->_state == STOPPED) {
		this->_mutex.unlock();
		this->start();
	} else {
		this->_mutex.unlock();
	}
}

void								Program::stop(void) {
	this->_mutex.lock();
	if (this->_state == RUNNING) {
		//STOP PROCESS (kill fork + delete thread)
		this->_mutex.unlock();
		kill(this->_pidChild, this->_feature.getStopSignal());
		sleep(2);
		this->_mutex.lock();
		if (this->_state != STOPPED) {
			std::cerr << this->_feature.getProgramName() << " cant be stopped." << std::endl;
		} else {
			std::cout << this->_feature.getProgramName() << " is stopped." << std::endl;
		}
	} else if (this->_state == STOPPED) {
		std::cout << this->_feature.getProgramName() << " is already stopped." << std::endl;
	} else if (this->_state == ERROR) {
		std::cout << this->_feature.getProgramName() << " have an error." << std::endl;
	}
	this->_mutex.unlock();
}

void								Program::status(void) {
	this->_mutex.lock();
	std::cout << this->_feature.getProgramName();
	std::cout << "	";
	for (std::map<std::string, eState>::const_iterator it = this->_mapState.begin(); it != this->_mapState.end(); it++)
		if (it->second == this->_state)
			std::cout << it->first;
	std::cout << "	";
	if (this->_state == RUNNING) {
		std::cout << "pid " << this->_pidChild;
		std::cout << ", uptime " << static_cast<double>(clock() - this->_lastTime) / 1000000.0;
	} else if (this->_state == ERROR) {
		std::cout << this->_lastTime;
	}
	std::cout << std::endl;
	this->_mutex.unlock();
}

void								Program::reload(ProgramFeature const & newFeature) {
	this->_mutex.lock();
	cmpFeature						lvlReload = (this->_feature == newFeature);

	if (lvlReload == MUST_RESTART) {
		this->_mutex.unlock();
		this->stop();
		this->_feature = newFeature;
		this->start();
	} else if (lvlReload == NO_RESTART) {
		this->_feature = newFeature;
		this->_mutex.unlock();
	} else {
		this->_mutex.unlock();
	}
	return ;
}
