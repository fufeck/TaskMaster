// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   ProgramFeature.cpp                                 :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: ftaffore <ftaffore@student.42.fr>          +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2015/04/15 14:46:08 by ftaffore          #+#    #+#             //
//   Updated: 2015/04/15 14:46:09 by ftaffore         ###   ########.fr       //
//                                                                            //
// ************************************************************************** //

#include "ProgramFeature.hpp"

ProgramFeature::ProgramFeature(void) : _programName("unknow") {
	this->_processName = "";
	this->_command = "";
	this->_directory = ".";
	this->_umask.push_back(0);
	this->_umask.push_back(2);
	this->_umask.push_back(2);
	this->_numprocs = 1;
	this->_autostart = true;
	this->_autorestart = NEVER;
	this->_exitcodes.push_back(1);
	this->_stopsignal = SIGKILL;
	this->_stopwaitsecs = 10;
	this->_startRetries = 3;
	this->_startsuccesstime = 0;
	this->_redirect_stderr = false;
	this->_stdoutlogfile = "/tmp/stdoutLog";
	this->_stderrlogfile = "/tmp/stderrLog";

	this->_mapSet[COMMAND] = &ProgramFeature::setCommand;
	this->_mapSet[PROCESS_NAME] = &ProgramFeature::setProcessName;
	this->_mapSet[DIRECTORY] = &ProgramFeature::setDirectory;
	this->_mapSet[UMASK] = &ProgramFeature::setUmask;
	this->_mapSet[NUM_PROCS] = &ProgramFeature::setNumProcs;
	this->_mapSet[AUTOSTART] = &ProgramFeature::setAutoStart;
	this->_mapSet[AUTORESTART] = &ProgramFeature::setAutoRestart;
	this->_mapSet[EXIT_CODES] = &ProgramFeature::setExitcodes;
	this->_mapSet[STOPSIGNAL] = &ProgramFeature::setStopSignal;
	this->_mapSet[STOP_WAIT_SECS] = &ProgramFeature::setStopWaitSec;
	this->_mapSet[START_RETRIES] = &ProgramFeature::setStartRetries;
	this->_mapSet[START_SUCCESS] = &ProgramFeature::setStartSuccessTime;
	this->_mapSet[REDIRECT_SDTERR] = &ProgramFeature::setRedirectStderr;
	this->_mapSet[STDOUT_LOGFILE] = &ProgramFeature::setStdoutLogfile;
	this->_mapSet[STDERR_LOGFILE] = &ProgramFeature::setStderrLogfile;
	this->_mapSet[ENV] = &ProgramFeature::setEnv;
}

ProgramFeature::ProgramFeature(std::string const &programName) : _programName(programName) {
	this->_processName = "";
	this->_command = "";
	this->_directory = ".";
	this->_umask.push_back(0);
	this->_umask.push_back(2);
	this->_umask.push_back(2);
	this->_numprocs = 1;
	this->_autostart = true;
	this->_autorestart = NEVER;
	this->_exitcodes.push_back(1);
	this->_stopsignal = SIGKILL;
	this->_stopwaitsecs = 10;
	this->_startRetries = 3;
	this->_startsuccesstime = 0;
	this->_redirect_stderr = false;
	this->_stdoutlogfile = "/tmp/stdoutLog";
	this->_stderrlogfile = "/tmp/stderrLog";

	this->_mapSet[COMMAND] = &ProgramFeature::setCommand;
	this->_mapSet[PROCESS_NAME] = &ProgramFeature::setProcessName;
	this->_mapSet[DIRECTORY] = &ProgramFeature::setDirectory;
	this->_mapSet[UMASK] = &ProgramFeature::setUmask;
	this->_mapSet[NUM_PROCS] = &ProgramFeature::setNumProcs;
	this->_mapSet[AUTOSTART] = &ProgramFeature::setAutoStart;
	this->_mapSet[AUTORESTART] = &ProgramFeature::setAutoRestart;
	this->_mapSet[EXIT_CODES] = &ProgramFeature::setExitcodes;
	this->_mapSet[STOPSIGNAL] = &ProgramFeature::setStopSignal;
	this->_mapSet[STOP_WAIT_SECS] = &ProgramFeature::setStopWaitSec;
	this->_mapSet[START_RETRIES] = &ProgramFeature::setStartRetries;
	this->_mapSet[START_SUCCESS] = &ProgramFeature::setStartSuccessTime;
	this->_mapSet[REDIRECT_SDTERR] = &ProgramFeature::setRedirectStderr;
	this->_mapSet[STDOUT_LOGFILE] = &ProgramFeature::setStdoutLogfile;
	this->_mapSet[STDERR_LOGFILE] = &ProgramFeature::setStderrLogfile;
	this->_mapSet[ENV] = &ProgramFeature::setEnv;
}

ProgramFeature::ProgramFeature(ProgramFeature const &other) : _programName(other.getProgramName()) {
	*this = other;
	this->_mapSet[COMMAND] = &ProgramFeature::setCommand;
	this->_mapSet[PROCESS_NAME] = &ProgramFeature::setProcessName;
	this->_mapSet[DIRECTORY] = &ProgramFeature::setDirectory;
	this->_mapSet[UMASK] = &ProgramFeature::setUmask;
	this->_mapSet[NUM_PROCS] = &ProgramFeature::setNumProcs;
	this->_mapSet[AUTOSTART] = &ProgramFeature::setAutoStart;
	this->_mapSet[AUTORESTART] = &ProgramFeature::setAutoRestart;
	this->_mapSet[EXIT_CODES] = &ProgramFeature::setExitcodes;
	this->_mapSet[STOPSIGNAL] = &ProgramFeature::setStopSignal;
	this->_mapSet[STOP_WAIT_SECS] = &ProgramFeature::setStopWaitSec;
	this->_mapSet[START_RETRIES] = &ProgramFeature::setStartRetries;
	this->_mapSet[START_SUCCESS] = &ProgramFeature::setStartSuccessTime;
	this->_mapSet[REDIRECT_SDTERR] = &ProgramFeature::setRedirectStderr;
	this->_mapSet[STDOUT_LOGFILE] = &ProgramFeature::setStdoutLogfile;
	this->_mapSet[STDERR_LOGFILE] = &ProgramFeature::setStderrLogfile;
	this->_mapSet[ENV] = &ProgramFeature::setEnv;
}

ProgramFeature::~ProgramFeature(void) {}

ProgramFeature&						ProgramFeature::operator=(ProgramFeature const & other) {
	this->_programName = other.getProgramName();
	this->_processName = other.getProcessName();
	this->_command = other.getCommand();
	this->_directory = other.getDirectory();
	this->_umask = other.getUmask();
	this->_numprocs = other.getNumProcs();
	this->_autostart = other.getAutoStart();
	this->_autorestart = other.getAutoRestart();
	this->_exitcodes = other.getExitcodes();
	this->_stopsignal = other.getStopSignal();
	this->_stopwaitsecs = other.getStopWaitSec();
	this->_startRetries = other.getStartRetries();
	this->_startsuccesstime = other.getStartSuccessTime();
	this->_redirect_stderr = other.getRedirectStderr();
	this->_stdoutlogfile = other.getStdoutLogfile();
	this->_stderrlogfile = other.getStderrLogfile();
	return (*this);
}

cmpFeature							ProgramFeature::operator==(ProgramFeature const & other) const {
	if (this->_command != other.getCommand() ||
		this->_env != other.getEnv() ||
		this->_directory != other.getDirectory() ||
		this->_numprocs != other.getNumProcs() ||
		this->_umask != other.getUmask()) {
		return MUST_RESTART;
	} else if (this->_autostart != other.getAutoStart() ||
				this->_autorestart != other.getAutoRestart() ||
				this->_exitcodes != other.getExitcodes() ||
				this->_stopsignal != other.getStopSignal() ||
				this->_stopwaitsecs != other.getStopWaitSec() ||
				this->_startRetries != other.getStartRetries() ||
				this->_startsuccesstime != other.getStartSuccessTime() ||
				this->_redirect_stderr != other.getRedirectStderr() ||
				this->_stdoutlogfile != other.getStdoutLogfile() ||
				this->_stderrlogfile != other.getStderrLogfile()) {
		return NO_RESTART;
	}
	return NOTHING;
}

void								ProgramFeature::display(void) const {
	std::cout << " [" << this->_programName << "]" << std::endl;
	std::cout << "processName = " << this->_processName << std::endl;
	std::cout << "comand = " << this->_command << std::endl;
	std::cout << "directory = " << this->_directory << std::endl;
	std::cout << "umask = ";
	for (unsigned int i = 0; i < this->_umask.size(); i++) {
		std::cout << this->_umask[i] << " ";
	}
	std::cout << std::endl;
	std::cout << "numProcs = " << this->_numprocs << std::endl;
	std::cout << "autostart = " << this->_autostart << std::endl;
	std::cout << "autorestart = " << this->_autorestart << std::endl;
	std::cout << "exitcodes = ";
	for (unsigned int i = 0; i < this->_exitcodes.size(); i++) {
		std::cout << this->_exitcodes[i] << " ";
	}
	std::cout << std::endl;
	std::cout << "stopsignal = " << this->_stopsignal << std::endl;
	std::cout << "stopwaitsecs = " << this->_stopwaitsecs << std::endl;
	std::cout << "startretries = " << this->_startRetries << std::endl;
	std::cout << "startsuccesstime = " << this->_startsuccesstime << std::endl;
	std::cout << "redirect_stderr = " << this->_redirect_stderr << std::endl;
	std::cout << "stdoutlogfile = " << this->_stdoutlogfile << std::endl;
	std::cout << "stderrlogfile = " << this->_stderrlogfile << std::endl;
	std::cout << std::endl;

}

void								ProgramFeature::setCommand(std::string const & command, int nbLine) {
	if (command.size() <= 0) {
		std::cerr << "ERROR : file in line " << nbLine << " 'command' is empty" << std::endl;
	}
	this->_command = command;
}

void								ProgramFeature::setProcessName(std::string const & processName, int nbLine) {
	if (processName.size() <= 0) {
		std::cerr << "ERROR : file in line " << nbLine << " 'process_name' is empty" << std::endl;
	} else if (processName == "(program_name)") {
		this->_processName = this->_programName;
	} else {
		this->_processName = processName;
	}
}

void								ProgramFeature::setDirectory(std::string const & directory, int nbLine) {
	if (directory.size() <= 0 && directory != ".") {
		std::cerr << "ERROR : file in line " << nbLine << " 'directory' is empty" << std::endl;
	} else {
		this->_directory = directory;
	}
}

void								ProgramFeature::setUmask(std::string const & umask, int nbLine) {
	if (umask.size() != 3) {
		std::cerr << "ERROR : file in line " << nbLine << " 'umask' bad syntax" << std::endl;
	} else if (umask[0] < '0' || umask[0] > '7' || umask[1] < '0' || umask[1] > '7' || umask[2] < '0' || umask[2] > '7') {
		std::cerr << "ERROR : file in line " << nbLine << " 'umask' bad syntax" << std::endl;
	} else {
		v_int			tmp;

		this->_umask.push_back(static_cast<int>(umask[0]));
		this->_umask.push_back(static_cast<int>(umask[1]));
		this->_umask.push_back(static_cast<int>(umask[2]));
		this->_umask = tmp;
	}
}

void								ProgramFeature::setNumProcs(std::string const & numprocs, int nbLine) {
	if (numprocs.size() <= 0) {
		std::cerr << "ERROR : file in line " << nbLine << " 'numprocs' is empty" << std::endl;
	}
	int 			nb = atoi(numprocs.c_str());
	if (nb <= 0 && nb > 10) {
		std::cerr << "ERROR : file in line " << nbLine << " 'numprocs' must be between 1 and 10" << std::endl;
	} else {
		this->_numprocs = nb;
	}
}

void								ProgramFeature::setAutoStart(std::string const & autostart, int nbLine) {
	if (autostart.size() == 0 || (autostart != "true" && autostart != "false")) {
		std::cerr << "ERROR : file in line " << nbLine << " 'autostart' must be 'true' or 'false'" << std::endl;
	} else {
		this->_autostart = (autostart == "true") ? (true) : (false);
	}
}

void								ProgramFeature::setAutoRestart(std::string const & autorestart, int nbLine) {
	if (autorestart.size() == 0 || (autorestart != "allthetime" && autorestart != "never" && autorestart != "unexpected")) {
		std::cerr << "ERROR : file in line " << nbLine << " 'autorestart' must be 'allthetime' or 'never' or 'unexpected'" << std::endl;
	} else {
		this->_autorestart = (autorestart == "allthetime") ? (ALL_THE_TIME) : ((autorestart == "never") ? (NEVER) : (UNEXPECTED));
	}
}

void								ProgramFeature::setExitcodes(std::string const & exitCodes, int nbLine) {
	v_int							res;
	std::string						tmp = exitCodes;
	this->replace(tmp, ",", " ");
	std::stringstream 					ss;
	ss.str(tmp);
	int 		nb = 0;
	while (ss >> nb) {
		res.push_back(nb);
	}
	if (res.empty()) {
		std::cerr << "ERROR : file in line " << nbLine << " 'exitcodes' is empty" << std::endl;
	} else {
		this->_exitcodes.clear();
		this->_exitcodes = res;
	}
}
void								ProgramFeature::setStopSignal(std::string const & stopSignal, int nbLine) {
	static_cast<void>(stopSignal);
	static_cast<void>(nbLine);
	this->_stopsignal = SIGKILL;
}

void								ProgramFeature::setStopWaitSec(std::string const & stopwaitsecs, int nbLine) {
	if (stopwaitsecs.size() <= 0) {
		std::cerr << "ERROR : file in line " << nbLine << " 'stopwaitsecs' is empty" << std::endl;
	}
	int 			nb = atoi(stopwaitsecs.c_str());
	if (nb <= 0 && nb > 10) {
		std::cerr << "ERROR : file in line " << nbLine << " 'stopwaitsecs' must be between 1 and 10" << std::endl;
	} else {
		this->_stopwaitsecs = nb;
	}
}

void								ProgramFeature::setStartRetries(std::string const & startretries, int nbLine) {
	if (startretries.size() <= 0) {
		std::cerr << "ERROR : file in line " << nbLine << " 'startretries' is empty" << std::endl;
	}
	int 			nb = atoi(startretries.c_str());
	if (nb <= 0) {
		std::cerr << "ERROR : file in line " << nbLine << " 'startretries' is not correct" << std::endl;
	} else {
		this->_startRetries = nb;
	}
}

void								ProgramFeature::setStartSuccessTime(std::string const & startsuccesstime, int nbLine) {
	if (startsuccesstime.size() <= 0) {
		std::cerr << "ERROR : file in line " << nbLine << " 'startsuccesstime' is empty" << std::endl;
	}
	int 			nb = atoi(startsuccesstime.c_str());
	if (nb <= 0) {
		std::cerr << "ERROR : file in line " << nbLine << " 'startsuccesstime' is not correct" << std::endl;
	} else {
		this->_startsuccesstime = nb;
	}
}

void								ProgramFeature::setRedirectStderr(std::string const & redirect_stderr, int nbLine) {
	if (redirect_stderr.size() == 0 || (redirect_stderr != "true" && redirect_stderr != "false")) {
		std::cerr << "ERROR : file in line " << nbLine << " 'redirect_stderr' must be 'true' or 'false'" << std::endl;
	} else {
		this->_redirect_stderr = (redirect_stderr == "true") ? (true) : (false);
	}
}

void								ProgramFeature::setStdoutLogfile(std::string const & stdoutlogfile, int nbLine) {
	if (stdoutlogfile.size() <= 0)
		std::cerr << "ERROR : file in line " << nbLine << " 'stdoutlogfile' is empty" << std::endl;
	else
		this->_stdoutlogfile = stdoutlogfile;
}

void								ProgramFeature::setStderrLogfile(std::string const & stderrlogfile, int nbLine) {
	if (stderrlogfile.size() <= 0)
		std::cerr << "ERROR : file in line " << nbLine << " 'stderrlogfile' is empty" << std::endl;
	else
		this->_stderrlogfile = stderrlogfile;
}

void								ProgramFeature::setEnv(std::string const & env, int nbLine) {
	m_str							res;
	std::string						tmp = env;

	int 			a = tmp.find(",");
	while (a > 0) {
		std::string 				tt = tmp.substr(0, a);
		if (tt.find("=") <= 0) {
			std::cerr << "ERROR : file in line " << nbLine << " 'env' bad syntax" << std::endl;
		} else {
			res[tt.substr(0, tt.find("="))] = tt.substr(tt.find("="), tt.size());
		}
		tmp = tmp.substr(a, tmp.size());
		a = tmp.find(",");
	}
	if (tmp.find("=") <= 0) {
		std::cerr << "ERROR : file in line " << nbLine << " 'env' bad syntax" << std::endl;
	} else {
		res[tmp.substr(0, tmp.find("="))] = tmp.substr(tmp.find("="), tmp.size());
	}
	if (res.empty()) {
		std::cerr << "ERROR : file in line " << nbLine << " 'env' is empty" << std::endl;
	} else {
		this->_env = res;
	}
}

void								ProgramFeature::setFeature(std::string const & line, int nbLine) {

	if (line.find('=') <= 0 && line.find('=') >= line.size())
		std::cerr << "ERROR file : in line " << nbLine << " bad syntax" << std::endl;
	else {
		std::string 				key = line.substr(0, line.find('='));
		std::string 				value = line.substr(line.find('=') + 1, line.size());
		if (this->_mapSet.find(key) != this->_mapSet.end()) {
			setFunc 		func = this->_mapSet[key];
			(this->*func)(value, nbLine);
		}
		else
			std::cerr << "ERROR file : in line " << nbLine << " key is unknow" << std::endl;
	}
}

std::string const				ProgramFeature::getProgramName(void) const {
	return this->_programName;
}

std::string							ProgramFeature::getCommand(void) const {
	return this->_command;
}

std::string							ProgramFeature::getProcessName(void) const {
	return this->_processName;
}

std::string							ProgramFeature::getDirectory(void) const {
	return this->_directory;
}

v_int								ProgramFeature::getUmask(void) const {
	return this->_umask;
}

int									ProgramFeature::getNumProcs(void) const {
	return this->_numprocs;
}

bool								ProgramFeature::getAutoStart(void) const {
	return this->_autostart;
}

eRestart							ProgramFeature::getAutoRestart(void) const {
	return this->_autorestart;
}

v_int								ProgramFeature::getExitcodes(void) const {
	return this->_exitcodes;
}

int									ProgramFeature::getStopSignal(void) const {
	return this->_stopsignal;
}

int									ProgramFeature::getStopWaitSec(void) const {
	return this->_stopwaitsecs;
}

int									ProgramFeature::getStartRetries(void) const {
	return this->_startRetries;
}

int									ProgramFeature::getStartSuccessTime(void) const {
	return this->_startsuccesstime;
}


bool								ProgramFeature::getRedirectStderr(void) const {
	return this->_redirect_stderr;
}

std::string							ProgramFeature::getStdoutLogfile(void) const {
	return this->_stdoutlogfile;
}

std::string							ProgramFeature::getStderrLogfile(void) const {
	return this->_stderrlogfile;
}

m_str								ProgramFeature::getEnv(void) const {
	return this->_env;
}

void								ProgramFeature::replace(std::string & str, std::string s1, std::string s2) {
	int a = 0;

	while ((a = str.find(s1)) >= 0) {
		str.replace(a, a, s2);
	}
}

bool								ProgramFeature::isGood(void) const {
	if (this->_programName.size() == 0 || this->_processName.size() == 0 || this->_command.size() == 0)
		return false;
	return true;
}