// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   Process.cpp                                        :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: ftaffore <ftaffore@student.42.fr>          +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2015/04/13 12:51:47 by ftaffore          #+#    #+#             //
//   Updated: 2015/04/13 12:51:48 by ftaffore         ###   ########.fr       //
//                                                                            //
// ************************************************************************** //

#include "Process.hpp"

Process::Process(ProcessFeature processFeature) {

	//init var
	this->_state = STOPPED;
	this->_feature = processFeature;

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



	this->_initProcess(infoProcess);
}

/***********************/
/****EXECUTE*PROCESS****/
/***********************/

void 								Process::_setUmask(void) {
	mode_t							mask = 0;

	mask = (this->_mapUmask[0][this->_umask[0]] | this->_mapUmask[1][this->_umask[1]] | this->_mapUmask[2][this->_umask[2]]);
	umask(mask);
}

void 								Process::_setDirectory(void) {
	if (chdir(this->_directory) < 0) {
//		this->_logFile << this->_name << " => ERROR : directory is incorrect." << std::endl;
		std::cerr << this->_name << " => ERROR : directory is incorrect." << std::endl;
		throw std::exception();
	}
}

void 								Process::_executeProcess(void) {
	//execl(const char * path, const char * arg, ...);
	//this->_logFile << this->_name << " => ERROR : execute command." << std::endl;
	std::cerr << this->_name << " => ERROR : execute command." << std::endl;
	throw std::exception();
}

void								Process::_threadFunc(void) {
	t_pid							pid;
	int 							ret;	

	if ((pid = fork()) < 0) {
		//this->_logFile << this->_name << " : fork fail." << std::endl;
		std::cerr << this->_name << " : fork fail." << std::endl;
		throw std::exception();
	}
	if (pid == 0) {
		this->_setUmask();
		this->_setDirectory();
		this->_redirectLogfile();
		this->_excuteProcess();
	}
	else {
		this->_mutes.lock();
		this->_pidChild = pid;
		this->_state = RUNNING;
		//this->_logFile << this->_name << " RUNNING with pid " << pid << std::endl;
		this->_mutes.unlock();
		wait(&ret);
		this->_mutes.lock();
		this->_pidChild = 0;
		this->_state = STOPPED;
		//this->_logFile << this->_name << " STOPPED" << pid << std::endl;
		this->_mutes.unlock();
	}
	exit(1);
}

/***********************/
/********COMMAND********/
/***********************/


void								autostart(void) {
	if (this->_autostart == true)
		this->_start();
}

void								start(void) {
	this->_mutex.lock();
	if (this->_state == STOPPED) {
		//START PROCESS (thread + fork)
		std::thread 				th(&Process::threadFunc);
	} else if (this->_state == RUNNING) {
		//this->_logFile << this->_name << " is already running." << std::endl;
	} else if (this->_state == ERROR) {
		//this->_logFile << this->_name << " dont be running." << std::endl;
	}
}

void								restart(void) {

	if (this->_state == RUNNING)
		this->_stop();
	if (this->_state == STOPPED)
		this->_start();
}

void								stop(void) {
	if (this->_state == RUNNING) {
		//STOP PROCESS (kill fork + delete thread)
		kill(this->_pidChild, this->_stopsignal);
		if (this->_state != STOPPED) {
			//this->_logFile << this->_name << " can not be stopped." << std::endl;
		}
	} else if (this->_state == STOPPED) {
		//this->_logFile << this->_name << " is already stopped." << std::endl;
	} else if (this->_state == ERROR) {
		//this->_logFile << this->_name << " process have an error." << std::endl;
	}
}

void								status(void) {
	if (this->_name != "")
		std::cout << this->_name;
	else
		std::cout << "unknow"
	std::cout << "	";
	for (iterator::std::map<std::string, eState> it = this->_mapState.begin(); it != this->_mapState.end(); it++)
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
}

void								reload(void) {
	return ;
}

void								exit(void) {
	if (this->_state == RUNNING)
		this->_stop();
}
