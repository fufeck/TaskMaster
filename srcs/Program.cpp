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
#include <iostream>
#include <iomanip>
#include <sstream>

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

	this->_mapUmask.push_back(std::vector<int>(7));
	this->_mapUmask.push_back(std::vector<int>(7));
	this->_mapUmask.push_back(std::vector<int>(7));
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

	if ((end = open(fileName.c_str(), O_WRONLY | O_CREAT | O_APPEND, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)) > 0) {
		dup2(end, start);
	}

}

void 								Program::_executeProgram(void) {
	char 							*command = strdup(this->_feature.getCommand().c_str());
	char  							**environ = this->_feature.getEnv();

	if (environ != NULL) {
		execle("/bin/sh", "sh", "-c", command, NULL, environ);
		this->_logOutPut->putLogFileError(strerror(errno));
	} else {
		execle("/bin/sh", "sh", "-c", command, (char*)0);
	}
	exit(1);
}


void								Program::_runProgram(void) {
	std::stringstream 				ss;
	pid_t							pid;

	if ((pid = fork()) < 0) {
		this->_logOutPut->putLogFileError("fork fail.");
		return ;
	}
	if (pid == 0) {
		this->_setUmask();
		this->_setDirectory();
		this->_redirectLogfile(this->_feature.getStdoutLogfile(), 1);
		if (this->_feature.getRedirectStderr()) {
			this->_redirectLogfile(this->_feature.getStderrLogfile(), 2);
		} else {
			close(2);
		}

		this->_executeProgram();
	}
	// this->status();
	ss << this->_feature.getProcessName() << " : START with pid " << pid << std::endl;
	this->_logOutPut->putAll(ss.str());
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
	if (this->_state == STOPPED || this->_state == RESTART) {
		this->_process.clear();
		this->_state = RUNNING;
		time(&(this->_lastTime));
		for (int i = 0; i < this->_feature.getNumProcs(); i++) {
			this->_runProgram();
		}
	}
}

void								Program::restart(void) {
	if (this->_state != STOPPED) {
		this->_state = RESTART;
		this->stop();
	}
}

void								Program::stop(void) {
	std::stringstream 				ss;
	this->_nbRestart = this->_feature.getStartRetries();

	if (this->_state == RUNNING || this->_state == RESTART) {
		for (std::vector<Process>::iterator it = this->_process.begin(); it != this->_process.end(); it++) {
			if (it->isRunning == true) {
				kill(it->pid, this->_feature.getStopSignal());
				ss << this->_feature.getProcessName() << " : with pid " << it->pid << " is STOPPING" << std::endl;
				this->_logOutPut->putAll(ss.str());
			}
		}
		this->_checkState();
		if (this->_state == STOPPED) {
			this->_process.clear();
		}
	}
}

void									Program::status(void) {
	std::stringstream 					ss;
	std::string 						status = "";
	struct tm 							*timeInfo = localtime(&(this->_lastTime));

	ss << this->_feature.getProgramName() << " : ";
	if (this->_state == RUNNING) {
		ss << "RUNNING, ";
	} else if (this->_state == STOPPED || this->_state == RESTART) {
		ss << "STOPPED, ";
	} else {
		ss << "ERROR, ";
	}
	ss << std::setw(2) << std::setfill('0') << timeInfo->tm_hour << ":" << std::setw(2) << std::setfill('0') << timeInfo->tm_min << ":" << std::setw(2) << std::setfill('0') << timeInfo->tm_sec;
	if (this->_process.size() > 0 && this->_state != STOPPED) {
		ss << ", with pid ";
		for (std::vector<Process>::iterator it = this->_process.begin(); it != this->_process.end(); it++) {
			if (it->isRunning == true)
				ss << it->pid << " ";
		}
	}
	ss << std::endl;
	this->_logOutPut->putAll(ss.str());
	return ;
}

void								Program::reload(ProgramFeature const & newFeature) {
	cmpFeature						lvlReload = (this->_feature == newFeature);

	this->_feature = newFeature;
	if (lvlReload == MUST_RESTART && this->_state == RUNNING) {
		this->_nbRestart = 0;
		this->restart();
	}
}

void									Program::_checkState(void) {
	bool								programRunning = false;

	for (std::vector<Process>::iterator it = this->_process.begin(); it != this->_process.end(); it++) {
		int 			ret = 0;
		if (it->isRunning == true) {
			if (waitpid(it->pid, &ret, WNOHANG) != -1)
				it->returnCode = ret / 256;
			if (waitpid(it->pid, &ret, WNOHANG) == -1) {
				it->isRunning = false;
				time(&it->endTime);
			}
		}
		if (it->isRunning == true)
			programRunning = true;
	}
	if (this->_state == RUNNING && programRunning == false) {
		this->_state = STOPPED;
		time(&(this->_lastTime));
		this->_logOutPut->putStdout("\n");
		this->status();
		this->_logOutPut->putStdout(PRONPT);
	} else if (this->_state == RESTART && programRunning == false) {
		this->_logOutPut->putStdout("\n");
		this->status();
		this->start();
		this->_logOutPut->putStdout(PRONPT);
	}
}

bool									Program::_checkExitCodes(void) {
	v_int 								exitCodesFeatures = this->_feature.getExitcodes();

	for (std::vector<Process>::iterator it = this->_process.begin(); it != this->_process.end(); it++) {
		bool 							exitCode = false;
		for (v_int::const_iterator ex = exitCodesFeatures.begin(); ex != exitCodesFeatures.end(); ex++) {
			if (*ex == it->returnCode) {
				exitCode = true;
			}
		}
		if (exitCode == false)
			return false;
	}
	return true;
}

bool									Program::_checkStartSuccess(void) {

	for (std::vector<Process>::iterator it = this->_process.begin(); it != this->_process.end(); it++) {
		double 		t = difftime(it->beginTime, it->endTime);
		t = (t >= 0) ? (t) : (-t);
		if (t < this->_feature.getStartSuccessTime())
			return false;
	}
	return true;
}

void									Program::_checkAutoRestart(void) {
	if (this->_nbRestart < this->_feature.getStartRetries()) {
		if (this->_feature.getAutoRestart() == ALL_THE_TIME) {
			this->_logOutPut->putStdout("\n");
			this->start();
			this->_logOutPut->putStdout(PRONPT);
			this->_nbRestart++;
		} else if (this->_feature.getAutoRestart() == UNEXPECTED) {
			bool								exitCodes = this->_checkExitCodes();
			bool								startsuccess = this->_checkStartSuccess();

			if (exitCodes == false || startsuccess == false) {
				this->_logOutPut->putStdout("\n");
				this->start();
				this->_logOutPut->putStdout(PRONPT);
				this->_nbRestart++;
			}
		} 
	} else {
		this->_process.clear();
	}
}

void 									Program::checkProcess(void) {
	this->_checkState();
	if (this->_state == STOPPED)
		this->_checkAutoRestart();
}
