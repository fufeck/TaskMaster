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

	this->_mapUmask.push_back( std::vector<int>(7));
	this->_mapUmask.push_back( std::vector<int>(7));
	this->_mapUmask.push_back( std::vector<int>(7));
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
		this->_stderrLogfile << "ERROR [" << this->_feature.getProgramName() << "] : directory is incorrect." << std::endl;
		throw std::exception();
	}
}

void 								Program::_executeProgram(void) {
	char 							*command = strdup(this->_feature.getCommand().c_str());

	if (this->_feature.getEnv().size() > 0) {
		char 							env[this->_feature.getEnv().size()][100];
		bzero(env, this->_feature.getEnv().size() * 100);
		int i = 0;
		for (m_str::const_iterator it = this->_feature.getEnv().begin(); it != this->_feature.getEnv().end(); it++) {
			strcat(env[i], it->first.c_str());
			strcat(env[i], "=");
			strcat(env[i], it->second.c_str());
			i++;
		}
		execle("/bin/sh", "sh", "-c", command, (char*)0, env);
	} else {
		execle("/bin/sh", "sh", "-c", command, (char*)0, NULL);
	}
	this->_stderrLogfile << "ERROR [" << this->_feature.getProgramName() << "] : execle fail." << std::endl;
	exit(1);
}

void 								Program::_redirectLogfile(std::string const &fileName, int start) {
	int 							p[2];
	int 							end;

	if ((end = open(fileName.c_str(), O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)) < 0) {
		this->_stderrLogfile << "ERROR [" << this->_feature.getProgramName() << "] : '" << fileName << "' cant be open." << std::endl;
		return ;
	}
	if (pipe(p) < 0) {
		this->_stderrLogfile << "ERROR [" << this->_feature.getProgramName() << "] : pipe fail." << std::endl;
		return ;
	}
	dup2(start, p[1]);
	dup2(p[0], end);
}

void								Program::runProgram(void) {
	pid_t							pid;
	int 							ret;

	if ((pid = fork()) < 0) {
		this->_stderrLogfile << "ERROR [" << this->_feature.getProgramName() << "] : fork fail." << std::endl;
		return ;
	}
	if (pid == 0) {
		this->_setUmask();
		this->_setDirectory();
		this->_executeProgram();
	}
	else {
		this->_mutex.lock();
		this->_process.push_back(pid);
		this->_state = RUNNING;
		this->_stdoutLogfile << "[" << this->_feature.getProcessName() << "] : RUNNING with pid " << pid << std::endl;
		this->_mutex.unlock();
		wait(&ret);
		this->_mutex.lock();
		for (std::vector<pid_t>::iterator i =  this->_process.begin(); i < this->_process.end(); i++) {
			if (*i == pid)
				this->_process.erase(i);
		}
		this->_stdoutLogfile << "[" << this->_feature.getProcessName() << "] : STOPPED with pid " << pid << std::endl;
		if (this->_process.size() == 0) {
			this->_stdoutLogfile << "[" << this->_feature.getProgramName() << "] : STOPPED" << std::endl;
			this->_state = STOPPED;
		}
		this->_mutex.unlock();
	}
}

/***********************/
/********COMMAND********/
/***********************/


void								Program::autostart(void) {
	if (this->_feature.getAutoStart() == true)
		this->start();
}

bool								Program::start(void) {

	this->_mutex.lock();
	if (this->_state == STOPPED) {
		this->_mutex.unlock();
		std::thread 				t(&Program::runProgram, this);
		t.join();
	} else if (this->_state == RUNNING) {
		this->_stderrLogfile << "[" << this->_feature.getProgramName() << "] : is already running." << std::endl;
		this->_mutex.unlock();
		return false;
	} else if (this->_state == ERROR) {
		this->_stderrLogfile << "[" << this->_feature.getProgramName() << "] : dont be running." << std::endl;
		this->_mutex.unlock();
		return false;
	} else {
		this->_mutex.unlock();
	}
	return true;
}

bool								Program::restart(void) {

	this->_mutex.lock();
	if (this->_state == RUNNING) {
		this->_mutex.unlock();
		this->stop();
	}
	if (this->_state == STOPPED) {
		this->_mutex.unlock();
		return this->start();
	} else {
		this->_mutex.unlock();
	}
	return false;
}

bool								Program::stop(void) {
	this->_mutex.lock();
	if (this->_state == RUNNING) {
		//STOP PROCESS (kill fork + delete thread)
		this->_mutex.unlock();
		for (std::vector<pid_t>::iterator it = this->_process.begin(); it != this->_process.end(); it++) {
			kill(*it, this->_feature.getStopSignal());
		}
		sleep(2);
		this->_mutex.lock();
		if (this->_state != STOPPED) {
			this->_stderrLogfile << "[" << this->_feature.getProgramName() << "] : cant be stopped." << std::endl;
			this->_mutex.unlock();
			return false;
		} else {
			this->_stdoutLogfile << "[" << this->_feature.getProgramName() << "] : is stopped." << std::endl;
			this->_mutex.unlock();
			return true;
		}
	} else if (this->_state == STOPPED) {
		this->_stdoutLogfile << "[" << this->_feature.getProgramName() << "] : is already stopped." << std::endl;
		this->_mutex.unlock();
		return true;
	}
	this->_stderrLogfile << "[" << this->_feature.getProgramName() << "] : have an error." << std::endl;
	this->_mutex.unlock();
	return false ;
}

std::string								Program::status(void) {
	std::string 						status = "";

	this->_mutex.lock();
	status += this->_feature.getProgramName() + " : ";
	if (this->_state == RUNNING) {
		status += "RUNNING, ";
	} else if (this->_state == STOPPED) {
		status += "STOPPED, ";
	} else {
		status += "ERROR, ";
	}
	if (this->_state == RUNNING && this->_feature.getNumProcs() == 1) {
		status += "pid ";// + this->_process[0];
	} else if (this->_state == RUNNING && this->_feature.getNumProcs() > 0) {
		status += "pid ";// + this->_process[0];
	}

	std::cout << "uptime " << static_cast<double>(clock() - this->_lastTime) / 1000000.0;
	this->_mutex.unlock();
	return status;
}

bool								Program::reload(ProgramFeature const & newFeature) {
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
	return true;
}
