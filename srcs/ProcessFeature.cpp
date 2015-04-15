// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   ProcessFeature.cpp                                 :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: ftaffore <ftaffore@student.42.fr>          +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2015/04/15 14:46:08 by ftaffore          #+#    #+#             //
//   Updated: 2015/04/15 14:46:09 by ftaffore         ###   ########.fr       //
//                                                                            //
// ************************************************************************** //

ProcessFeature::ProcessFeature(std::string const &programName) : _programName(programName) {
	this->_nameProcess = "";
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
	this->_redirect_stderr = false;
	this->_stdoutlogfile = "tmp/stdoutLog";
	this->_stderrlogfile = "tmp/stderrLog";

	this->_mapSet[COMMAND] = &ProcessFeature::setCommand;
	this->_mapSet[PROCESS_NAME] = &ProcessFeature::setName;
	this->_mapSet[DIRECTORY] = &ProcessFeature::setDirectory;
	this->_mapSet[UMASK] = &ProcessFeature::setUmask;
	this->_mapSet[NUM_PROCS] = &ProcessFeature::setNumProcs;
	this->_mapSet[AUTOSTART] = &ProcessFeature::setAutostart;
	this->_mapSet[AUTORESTART] = &ProcessFeature::setAutorestart;
	this->_mapSet[EXIT_CODES] = &ProcessFeature::setExitCodes;
	this->_mapSet[STOPSIGNAL] = &ProcessFeature::setStopSignal;
	this->_mapSet[STOP_WAIT_SECS] = &ProcessFeature::setStopWaitSec;
	this->_mapSet[REDIRECT_SDTERR] = &ProcessFeature::setRedirectStderr;
	this->_mapSet[STDOUT_LOGFILE] = &ProcessFeature::setStdoutLogfile;
	this->_mapSet[STDERR_LOGFILE] = &ProcessFeature::setStderrLogfile;
	this->_mapSet[ENV] = &ProcessFeature::setEnv;
}

ProcessFeature::~ProcessFeature(void) {}

cmpFeature							ProcessFeature::operator==(ProcessFeature const & other) const {
	if (this->_command != other.getCommand() ||
		this->_env != other.getEnv() ||
		this->_directory != other.getDirectory() ||
		this->_numprocs != other.getNumProcs() ||
		this->_umask != other.getUmask()) {
		return MUST_RESTART;
	} else if (this->_autostart != other.getAutoStart() ||
				this->_autorestart != other.getAutoRestart() ||
				this->_exitcodes != other.getExitCodes() ||
				this->_stopsignal != other.getStopSignal() ||
				this->_stopwaitsecs != other.getStopWaitSec() ||
				this->_redirect_stderr != other.getRedirectStderr() ||
				this->_stdoutlogfile != other.getStdoutLogfile() ||
				this->_stderrlogfile != other.getStderrLogfile()) {
		return NO_RESTART;
	}
	return NOTHING;
}

void								ProcessFeature::setCommand(std::string const & command, int nbLine) {
	if (command.size() <= 0) {
		std::cerr << "ERROR : file in line " << line << " 'command' is empty" << std::endl;
	}
	this->_command = command;
}

void								ProcessFeature::setName(std::string const & processName, int nbLine) {
	if (processName.size() <= 0) {
		std::cerr << "ERROR : file in line " << line << " 'process_name' is empty" << std::endl;
	} else if (processName = "(program_name)") {
		this->_processName = programName;
	} else {
		this->_processName = processName;
	}
}

void								ProcessFeature::setDirectory(std::string const & processName, int nbLine) {
	if (directory.size() <= 0 && directory != ".") {
		std::cerr << "ERROR : file in line " << line << " 'directory' is empty" << std::endl;
	} else {
		this->_directory = directory;
	}
}

void								ProcessFeature::setUmask(std::string const & umask, int nbLine) {
	if (umask.size() != 3) {
		std::cerr << "ERROR : file in line " << line << " 'umask' bad syntax" << std::endl;
	} else if (umask[0] < '0' || umask[0] > '7' || umask[1] < '0' || umask[1] > '7' || umask[2] < '0' || umask[2] > '7') {
		std::cerr << "ERROR : file in line " << line << " 'umask' bad syntax" << std::endl;
	} else {
		v_int			tmp;

		this->_umask.push_back(static_cast<int>(umask[0]));
		this->_umask.push_back(static_cast<int>(umask[1]));
		this->_umask.push_back(static_cast<int>(umask[2]));
		this->_umask = tmp;
	}
}

void								ProcessFeature::setNumProcs(std::string const & numprocs, int nbLine) {
	if (numprocs.size() <= 0) {
		std::cerr << "ERROR : file in line " << line << " 'numprocs' is empty" << std::endl;
	}
	int 			nb = atoi(numprocs);
	if (nb <= 0 && nb > 10) {
		std::cerr << "ERROR : file in line " << line << " 'numprocs' must be between 1 and 10" << std::endl;
	} else {
		this->_numprocs = nb;
	}
}

void								ProcessFeature::setAutoStart(std::string const & autostart, int nbLine) {
	if (autostart.size() =< 0 || autostart != "true" || autostart != "false") {
		std::cerr << "ERROR : file in line " << line << " 'autostart' must be 'true' or 'false'" << std::endl;
	} else {
		this->_autostart = (autostart == "true") ? (true) : (false);
	}
}

void								ProcessFeature::setAutoRestart(std::string const & autorestart, int nbLine) {
	if (autostart.size() =< 0 || autorestart != "allthetime" || autostart != "never" || autostart != "unexpected") {
		std::cerr << "ERROR : file in line " << line << " 'autorestart' must be 'allthetime' or 'never' or 'unexpected'" << std::endl;
	} else {
		this->_autorestart = (autorestart == "allthetime") ? (ALL_THE_TIME) : ((autorestart == "never") ? (NEVER) : (UNEXPECTED));
	}
}

void								ProcessFeature::setExitcodes(std::string const & exitCodes, int nbLine) {
	v_int							res;
	std::string						tmp = exitCodes;
	ParseYaml::replace(tmp, ",", " ");
	std::stringstream 					ss;
	ss.str(tmp);
	int 		nb = 0;
	while (ss >> nb) {
		res.push_back(nb);
	}
	if (res.empty()) {
		std::cerr << "ERROR : file in line " << line << " 'exitcodes' is empty" << std::endl;
	} else {
		this->_exitcodes = res;
	}
}

void								ProcessFeature::setStopSignal(std::string const & stopSignal, int nbLine) {
	eStop 							res = this->_transformToStopSignal(stopSignal);

	this->_stopsignal = res;
}

void								ProcessFeature::setStopWaitSec(std::string const & stopwaitsecs, int nbLine) {
	if (stopwaitsecs.size() <= 0) {
		std::cerr << "ERROR : file in line " << line << " 'stopwaitsecs' is empty" << std::endl;
	}
	int 			nb = atoi(stopwaitsecs);
	if (nb <= 0 && nb > 10) {
		std::cerr << "ERROR : file in line " << line << " 'stopwaitsecs' must be between 1 and 10" << std::endl;
	} else {
		this->_stopwaitsecs = nb;
	}
}

void								ProcessFeature::setRedirectStderr(std::string const & redirect_stderr, int nbLine) {
	if (redirect_stderr.size() =< 0 || redirect_stderr != "true" || redirect_stderr != "false") {
		std::cerr << "ERROR : file in line " << line << " 'redirect_stderr' must be 'true' or 'false'" << std::endl;
	} else {
		this->_redirect_stderr = (redirect_stderr == "true") ? (true) : (false);
	}
}

void								ProcessFeature::setStdoutLogfile(std::string const & stdoutlogfile, int nbLine) {
	if (stdoutlogfile.size() <= 0)
		std::cerr << "ERROR : file in line " << line << " 'stdoutlogfile' is empty" << std::endl;
	else
		this->_stdoutlogfile = stdoutlogfile
}

void								ProcessFeature::setStderrLogfile(std::string const & stderrlogfile, int nbLine) {
	if (stderrlogfile.size() <= 0)
		std::cerr << "ERROR : file in line " << line << " 'stderrlogfile' is empty" << std::endl;
	else
		this->_stderrlogfile = stderrlogfile
}

void								ProcessFeature::setEnv(std::string const & env, int nbLine) {
	m_str							res;
	std::string						tmp = exitCodes;

	int 			a = tmp.find(",");
	while (a > 0) {
		tt = tmp.substr(0, a);
		if (tt.find("=") <= 0) {
			std::cerr << "ERROR : file in line " << line << " 'env' bad syntax" << std::endl;
		} else {
			res[tt.substr(0, tt.find("="))] = tt.substr(tt.find("="), tt.size());
		}
		tmp = tmp.substr(a, tmp.size());
		a = tmp.find(",");
	}
	if (tt.find("=") <= 0) {
		std::cerr << "ERROR : file in line " << line << " 'env' bad syntax" << std::endl;
	} else {
		res[tt.substr(0, tt.find("="))] = tt.substr(tt.find("="), tt.size());
	}
	if (res.empty()) {
		std::cerr << "ERROR : file in line " << line << " 'env' is empty" << std::endl;
	} else {
		this->_env = res;
	}
}

void								ProcessFeature::setParams(std::string const & line, int nbLine) {

	if (line.find('=') <= 0 && line.find('=') >= line.size())
		std::cerr << "ERROR file : in line " << nbLine << " bad syntax" << std::endl;
	else {
		std::string 				key = line.substr(0, line.find('='));
		std::string 				value = line.substr(line.find('='), line.size());
		if (this->_mapSet.find(key) != this->_mapSet.end())
			this->_mapSet[key](value, nbLine);
		else
			std::cerr << "ERROR file : in line " << nbLine << " key is unknow" << std::endl;
	}
}

std::string							ProcessFeature::getCommand(void) const {
	return this->_command;
}

std::string							ProcessFeature::getName(void) const {
	return this->_name;
}

std::string							ProcessFeature::getDirectory(void) const {
	return this->_directory;
}

v_int								ProcessFeature::getUmask(void) const {
	return this->_umask;
}

int									ProcessFeature::getNumProcs(void) const {
	return this->_numprocs;
}

bool								ProcessFeature::getAutoStart(void) const {
	return this->_autostart;
}

eRestart							ProcessFeature::getAutoRestart(void) const {
	return this->_autorestart;
}

v_int								ProcessFeature::getExitcodes(void) const {
	return this->_exitcodes;
}

int									ProcessFeature::getStopSignal(void) const {
	return this->_stopsignal;
}

int									ProcessFeature::getStopWaitSec(void) const {
	return this->_stopwaitsecs;
}

bool								ProcessFeature::getRedirectStderr(void) const {
	return this->_redirect_stderr;
}

std::string							ProcessFeature::getStdoutLogfile(void) const {
	return this->_stdoutlogfile;
}

std::string							ProcessFeature::getStderrLogfile(void) const {
	return this->_stderrlogfile;
}

m_str								ProcessFeature::getVarEnv(void) const {
	return this->_varEnv;
}

bool								isGood(void) const {
	if (this->_nameProcess.size() <= 0 || this->_command.size() <= 0)
		return false;
	return true;
}