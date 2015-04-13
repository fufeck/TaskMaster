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

Process::Process(std::ifstream logFile, StrStr const &infoProcess) {
	this->_initProcess(infoProcess);
}

void			_initProcess(StrStr const &infoProcess) {
	for (StrStr::iterator it = this->_process.begin(); it != this->_process.end(); ++it) {
		if (it->first == COMMAND) {
			this->_command = it->second;
		} else if (it->first == NAME) {
			this->_name = it->second;
		} else if (it->first == DIRECTORY) {
			this->_directory = it->second;
		} else if (it->first == UMASK) {
			this->_umask = it->second;
		} else if (it->first == NUM_PROCS) {
			std::stringstream 	ss;
			ss.str(it->second);
			ss >> this->_numprocs;
		} else if (it->first == AUTOSTART) {
			if (this->_mapTruth.find(it->second) != this->_mapTruth.end()) {
				this->_autostart = this->_mapTruth[it->second];
			} else {
				this->_logFile << "ERROR : autoStart is not valid" << std::endl;
			}
		} else if (it->first == AUTORESTART) {
			if (this->_mapRestart.find(it->second) != this->_mapRestart.end()) {
				this->_autorestart = this->_mapRestart[it->second];
			} else {
				this->_logFile << "ERROR : autoRestart is not valid" << std::endl;
			}
		} else if (it->first == EXIT_CODES) {
			std::string 		str = replace(it->second.begin(),it->second.end(), ",", " ");
			std::stringstream 	ss;
			ss.str(str);

			int 				tmp;
			while (ss >> tmp) {
				this->_exitcodes.push_back(tmp);
			}
		} else if (it->first == STOPSIGNAL) {
			if (this->_mapExit.find(it->second) != this->_mapExit.end()) {
				this->_stopsignal = this->_mapExit[it->second];
			} else {
				this->_logFile << "ERROR : stopSignal is not valid" << std::endl;
			}
		} else if (it->first == STOP_WAIT_SECS) {
			std::stringstream 	ss;
			ss.str(it->second);
			ss >> this->_stopwaitsecs;
		} else if (it->first == REDIRECT_SDTERR) {
			if (this->_mapTruth.find(it->second) != this->_mapTruth.end()) {
				this->_redirect_stderr = this->_mapTruth[it->second];
			} else {
				this->_logFile << "ERROR : redirect_stderr is not valid" << std::endl;
			}
		} else if (it->first == STDOUT_LOGFILE) {
			this->_stdoutlogfile = it->second;
		} else if (it->first == STDERR_LOGFILE) {
			this->_stdoutlogfile = it->second;
		} else if (it->first == VARENV) {
			this->_stdoutlogfile = it->second;
		}
	}
}