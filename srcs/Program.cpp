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

Program::Program(ProgramFeature programFeature, LogOutPut *logOutPut) {

	//init var
	this->_state = STOPPED;
	this->_nbRestart = 0;
	time(&(this->_lastTime));
	this->_feature = programFeature;
	this->_logOutPut = logOutPut;

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

	mask = (this->_mapUmask[0][this->_feature.getUmask()[0]] | \
		this->_mapUmask[1][this->_feature.getUmask()[1]] | \
		this->_mapUmask[2][this->_feature.getUmask()[2]]);
	umask(mask);
}

void 								Program::_setDirectory(void) {
	chdir(this->_feature.getDirectory().c_str());
}

void 								Program::_redirectLogfile(std::string const &fileName, int start) {
	int 							end;

	if ((end = open(fileName.c_str(), O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)) > 0)
		dup2(end, start);
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
	exit(1);
}


void								Program::_runProgram(void) {
	pid_t							pid;

	if ((pid = fork()) < 0) {
		std::cout << "error fork " << std::endl;
		return ;
	}
	if (pid == 0) {
		this->_setUmask();
		this->_setDirectory();
		this->_redirectLogfile(this->_feature.getStdoutLogfile(), 1);
		if (this->_feature.getRedirectStderr())
			this->_redirectLogfile(this->_feature.getStderrLogfile(), 2);
		this->_executeProgram();
	}
	this->_process.push_back(Process(pid));
}

/***********************/
/********COMMAND********/
/***********************/


void								Program::autostart(void) {
	if (this->_feature.getAutoStart() == true)
		this->start();
}

void								Program::start(void) {
	if (this->_state == STOPPED) {
		this->_process.clear();
		this->_state = RUNNING;
		time(&(this->_lastTime));
		for (int i = 0; i < this->_feature.getNumProcs(); i++) {
			this->_runProgram();
		}
	} else if (this->_state == RUNNING) {

	} else {

	}
}

void								Program::restart(void) {

	if (this->_state == RUNNING) {
		this->stop();
	}
	if (this->_state == STOPPED) {
		this->start();
	} else {

	}
}

void								Program::stop(void) {
	this->_nbRestart = this->_feature.getStartRetries();

	if (this->_state == RUNNING) {
		for (std::vector<Process>::iterator it = this->_process.begin(); it != this->_process.end(); it++) {
			if (it->isRunning == true)
				kill(it->pid, this->_feature.getStopSignal());
		}
		sleep(1);
		this->_checkState();
		if (this->_state == STOPPED) {
			this->_process.clear();
		} else {

		}
	} else if (this->_state == STOPPED) {
	
	}
}

void									Program::status(void) {
	std::string 						status = "";
	struct tm 							*timeInfo = localtime(&(this->_lastTime));

	std::cout << this->_feature.getProgramName() << " : ";
	if (this->_state == RUNNING) {
		std::cout << "RUNNING, ";
	} else if (this->_state == STOPPED) {
		std::cout << "STOPPED, ";
	} else {
		std::cout << "ERROR, ";
	}
	std::cout << timeInfo->tm_hour << ":" << timeInfo->tm_min << ":" << timeInfo->tm_sec;

	if (this->_process.size() > 0 && this->_state != STOPPED) {
		std::cout << ", with pid ";
		for (std::vector<Process>::iterator it = this->_process.begin(); it != this->_process.end(); it++) {
			if (it->isRunning == true)
				std::cout << it->pid << " ";
		}
	}
	std::cout << std::endl;
	return ;
}

void								Program::reload(ProgramFeature const & newFeature) {
	cmpFeature						lvlReload = (this->_feature == newFeature);

	this->_feature = newFeature;
	if (lvlReload == MUST_RESTART) {
		this->_nbRestart = 0;
		this->restart();
	}
}

void									Program::_checkState(void) {

	bool								programRunning = false;
	//std::cout << "s" << std::endl;
	for (std::vector<Process>::iterator it = this->_process.begin(); it != this->_process.end(); it++) {
		int 			ret = 0;
		if (it->isRunning == true) {
			if (waitpid(it->pid, &ret, WNOHANG) != -1)
				it->returnCode = ret / 256;
			if (waitpid(it->pid, &ret, WNOHANG) == -1) {
				it->isRunning = false;
				time(&it->endTime);
				//std::string str = "returnCode = " + std::to_string(it->returnCode) + "\n";
				//write(2, str.c_str(), str.size());
			}
		}
		if (it->isRunning == true)
			programRunning = true;
	}
	if (this->_state == RUNNING && programRunning == false) {
		this->_state = STOPPED;
		time(&(this->_lastTime));
	}
}

bool									Program::_checkExitCodes(void) {
	bool								exitCodes = true;
	v_int 								exitCodesFeatures = this->_feature.getExitcodes();

	//std::cout << "e" << std::endl;
	for (std::vector<Process>::iterator it = this->_process.begin(); it != this->_process.end(); it++) {
		//std::cout << "it" << std::endl;
		bool 							exitCode = false;
		for (v_int::const_iterator ex = exitCodesFeatures.begin(); ex != exitCodesFeatures.end(); ex++) {
			//std::cout << "ex" << *ex << std::endl;
			if (*ex == it->returnCode)
				exitCode = true;
		}
		if (exitCode == false)
			exitCodes = false;
	}
	//std::cout << "e" << std::endl;
	return exitCodes;	
}

bool									Program::_checkStartSucsess(void) {
	double								totalTime = 0;

	//std::cout << "ss" << std::endl;
	for (std::vector<Process>::iterator it = this->_process.begin(); it != this->_process.end(); it++) {
		double 		t = difftime(it->beginTime, it->endTime);
		totalTime += (t >= 0) ? (t) : (-t);
	}
	//std::cout << this->_feature.getStartSuccessTime() << std::endl;
	//std::cout << totalTime << std::endl;
	return (totalTime / this->_process.size() >= this->_feature.getStartSuccessTime()) ? (true) : (false);	
}

void									Program::_checkAutoRestart(void) {
	//this->status();
	//std::cout << "a" << std::endl;
	if (this->_nbRestart < this->_feature.getStartRetries()) {
		if (this->_feature.getAutoRestart() == ALL_THE_TIME) {
			//std::cout << "toujours" << std::endl;
			this->start();
			this->_nbRestart++;
		} else if (this->_feature.getAutoRestart() == UNEXPECTED) {
			//std::cout << "comprend pas" << std::endl;
			bool								exitCodes = this->_checkExitCodes();
			bool								startsuccess = this->_checkStartSucsess();

			//std::cout << exitCodes << ":" << startsuccess << std::endl << std::endl;
			if (exitCodes == false || startsuccess == false) {
				this->start();
				this->_nbRestart++;
			}/* else {
				this->_process.clear();
			}*/
			//std::cout << this->_state << std::endl;
		}/* else {
			this->_process.clear();
		}*/
	} else {
		this->_process.clear();
	}
	//this->status();
}

void 									Program::checkProcess(void) {

	this->_checkState();
	if (this->_state == STOPPED)
		this->_checkAutoRestart();
}
