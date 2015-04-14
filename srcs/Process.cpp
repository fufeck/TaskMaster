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

Process::Process(std::string const &name, std::ofstream logFile, StrStr const &infoProcess) _name(name), _logFile(logFile) {

	//state map
	this->_state = STOPPED;
	this->__command = "";
	this->__name = "";
	this->__directory = ".";
	this->__umask = "022";
	this->__numprocs = 1;
	this->__autostart = true;
	this->__autorestart = NEVER;
	this->__exitcodes = {1};
	this->__stopsignal = SIGKILL;
	this->__stopwaitsecs = 10;
	this->__redirect_stderr = false;
	this->__stdoutlogfile = "tmp/stdoutLog";
	this->__stderrlogfile = "tmp/stderrLog";
	this->__varEnv = {};

	this->_mapState["STOPPED"] = STOPPED;
	this->_mapState["RUNNING"] = RUNNING;
	this->_mapState["ERROR"] = ERROR;
	//restart map
	this->_mapRestart["already"] = ALREADY;
	this->_mapRestart["unexpected"] = UNEXPECTED;
	this->_mapRestart["never"] = NEVER;
	//exit map
	this->_mapExit["exit"] = SIGKILL;
	//truth map
	this->_mapTruth["true"] = true;
	this->_mapTruth["false"] = false;
	//transform map
	/*this->_mapTransform[COMMAND] = &Process::_transformToString;
	this->_mapTransform[NAME] = &Process::_transformToString;
	this->_mapTransform[DIRECTORY] = &Process::_transformToString;
	this->_mapTransform[UMASK] = &Process::_transformToString;
	this->_mapTransform[NUM_PROCS] = &Process::_transformToInt;
	this->_mapTransform[AUTOSTART] = &Process::_transformToBool;
	this->_mapTransform[AUTORESTART] = &Process::_transformToRestart;
	this->_mapTransform[EXIT_CODES] = &Process::_transformToExitCodes;
	this->_mapTransform[STOPSIGNAL] = &Process::_transformToStopSignal;
	this->_mapTransform[STOP_WAIT_SECS] = &Process::_transformToInt;
	this->_mapTransform[REDIRECT_SDTERR] = &Process::_transformToBool;
	this->_mapTransform[STDOUT_LOGFILE] = &Process::_transformToString;
	this->_mapTransform[STDERR_LOGFILE] = &Process::_transformToString;
	this->_mapTransform[VAR_ENV] = &Process::_transformToVarEnv;
*/
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


	this->_mapSet[COMMAND] = &Process::setCommand;
	this->_mapSet[NAME] = &Process::setName;
	this->_mapSet[DIRECTORY] = &Process::setDirectory;
	this->_mapSet[UMASK] = &Process::setUmask;
	this->_mapSet[NUM_PROCS] = &Process::setNumProcs;
	this->_mapSet[AUTOSTART] = &Process::setAutostart;
	this->_mapSet[AUTORESTART] = &Process::setAutorestart;
	this->_mapSet[EXIT_CODES] = &Process::setExitCodes;
	this->_mapSet[STOPSIGNAL] = &Process::setStopSignal;
	this->_mapSet[STOP_WAIT_SECS] = &Process::setStopWaitSec;
	this->_mapSet[REDIRECT_SDTERR] = &Process::setRedirectStderr;
	this->_mapSet[STDOUT_LOGFILE] = &Process::setStdoutLogfile;
	this->_mapSet[STDERR_LOGFILE] = &Process::setStderrtLogfile;
	this->_mapSet[VAR_ENV] = &Process::setVarEnv;

	this->_initProcess(infoProcess);
}


eCmp								Process::operator==(Process const & other) const {
	if (this->_command != other.getCommand() ||
		this->_varEnv != other.getVarEnv() ||
		this->_directory != other.getDirectory() ||
		this->_numprocs != other.getNumProcs() ||
		this->_umask != other.getUmask()) {
		return BIG;
	} else if (this->_autostart != other.getAutoStart() ||
				this->_autorestart != other.getAutoRestart() ||
				this->_exitcodes != other.getExitCodes() ||
				this->_stopsignal != other.getStopSignal() ||
				this->_stopwaitsecs != other.getStopWaitSec() ||
				this->_redirect_stderr != other.getRedirectStderr() ||
				this->_stdoutlogfile != other.getStdoutLogfile() ||
				this->_stderrlogfile != other.getStderrLogfile()) {
		return LITTLE;
	}
	return NOTHING;
}

void									Process::_initProcess(StrStr const &infoProcess) {

	for (StrStr::iterator it = infoProcess.begin(); it != infoProcess.end(); it++) {
		if (this->_mapSet.find(it->first) != this->_mapSet.end()) {
			this->_mapSet[it->first](it->second);
		} else {
			this->_logFile << "ERROR : command " << it->first << " is unknow." << std::endl;
		}
	}
}

/***************************/
/*********TRANSFORM*********/
/***************************/

bool								Process::_transformToBool(std::string const & command) const {
	bool 							ret = false;

	if (this->_mapTruth.find(it->second) != this->_mapTruth.end()) {
			ret = this->_mapTruth[it->second];
	} else {
		this->_logFile << "ERROR : boolean is not valid" << std::endl;
	}
	return ret;
}

eRestart							Process::_transformToRestart(std::string const & command) const {
	eRestart						ret = NEVER;

	if (this->_mapRestart.find(command) != this->_mapRestart.end()) {
		ret = this->_mapRestart[it->second];
	} else {
		this->_logFile << "ERROR : autoRestart is not valid" << std::endl;
	}
	return ret;
}

std::vector<int>						Process::_transformToExitCodes(std::string const & command) const {
	std::vector<int>					ret;
	std::string 						str = replace(command.begin(), command.end(), ",", " ");
	std::stringstream 					ss;
	ss.str(str);

	int 				tmp;
	while (ss >> tmp) {
		ret.push_back(tmp);
	}
}

int										Process::_transformToStopSignal(std::string const & command) const {
	eExit								ret = NEVER;

	if (this->_mapExit.find(command) != this->_mapExit.end()) {
		ret = this->_mapExit[it->second];
	} else {
		this->_logFile << "ERROR : exit signal is not valid" << std::endl;
	}
	return ret;
}

std::map<std::string, std::string>		Process::_transformToVarEnv(std::string const & command) const {

}

/***************************/
/**********SETTEUR**********/
/***************************/

void								setCommand(std::string const & command) {
	if (command != "")
		this->_command = command;
	else {
		this->_logFile << "ERROR : command is empty" << std::endl;
		this->_state = ERROR;
	}
}

void								setName(std::string const & name) {
	if (name != "")
		this->_name = name;
	else {
		this->_logFile << "ERROR : process have no name" << std::endl;
		this->_state = ERROR;
	}
}

void								setDirectory(std::string const & directory) {
	if (directory != "")
		this->_directory = directory;
	else
		this->_logFile << "ERROR : directory is not valid" << std::endl;
}

void								setUmask(std::string const & umask) {
	if (umask != "" && umask.size() != 3)
		this->_umask = umask;
	else
		this->_logFile << "ERROR : umask is not valid" << std::endl;
}

void								setNumProcs(std::string const & numProcs) {
	int 							res = atoi(numProcs);

	if (res > 0)
		this->_numprocs = res;
	else
		this->_logFile << "ERROR : num process is not valid" << std::endl;
}

void								setAutoStart(std::string const & autostart) {
	bool 							res = this->_transformToBool(autostart);

	this->_autostart = res;
}

void								setAutoRestart(std::string const & autorestart) {
	eRestart 						res = this->_transformToRestart(autorestart);

	this->_autostart = res;
}

void								setExitcodes(std::string const & exitCodes) {
	std::vector<int> 					res = this->_transformToExitCodes(exitCodes);

	this->_exitcodes = res;
}

void								setStopSignal(std::string const & stopSignal) {
	eStop 							res = this->_transformToStopSignal(stopSignal);

	this->_stopsignal = res;
}

void								setStopWaitSec(std::string const & wait) {
	int 							res = atoi(wait);

	if (res >= 0)
		this->_stopwaitsecs = res;
	else
		this->_logFile << "ERROR : stop wait secs is not valid" << std::endl;
}

void								setRedirectStderr(std::string const & redirect_stderr) {
	bool 							res = this->_transformToBool(redirect_stderr);

	this->_redirect_stderr = res;
}

void								setStdoutLogfile(std::string const & stdoutlogfile) {
	if (stdoutlogfile != "")
		this->_stdoutlogfile = stdoutlogfile;
	else
		this->_logFile << "ERROR : stdoutlogfile is not valid" << std::endl;
}

void								setStderrLogfile(std::string const & stderrlogfile) {
	if (stderrlogfile != "")
		this->_stderrlogfile = stderrlogfile;
	else
		this->_logFile << "ERROR : stderrlogfile is not valid" << std::endl;
}

void								setStderrVarEnv(std::string const & env) {
	std::map<std::string, std::string>	res = this->_transformToVarEnv(env);

	this->_varEnv = res;
}


/***************************/
/**********GETTEUR**********/
/***************************/

std::string							getCommand(void) const {
	return this->_command;
}

std::string							getName(void) const {
	return this->_name;
}

std::string							getDirectory(void) const {
	return this->_directory;
}

std::string							getUmask(void) const {
	return this->_umask;
}

int									getNumProcs(void) const {
	return this->_numprocs;
}

bool								getAutoStart(void) const {
	return this->_autostart;
}

eRestart							getAutoRestart(void) const {
	return this->_autorestart;
}

std::vector<int>					getExitcodes(void) const {
	return this->_exitcodes;
}

int									getStopSignal(void) const {
	return this->_stopsignal;
}

int									getStopWaitSec(void) const {
	return this->_stopwaitsecs;
}

bool								getRedirectStderr(void) const {
	return this->_redirect_stderr;
}

std::string							getStdoutLogfile(void) const {
	return this->_stdoutlogfile;
}

std::string							getStderrLogfile(void) const {
	return this->_stderrlogfile;
}

std::map<std::string, std::string>	getVarEnv(void) const {
	return this->_varEnv;
}

/***************************/
/**********COMMAND**********/
/***************************/

void 								Process::_redirectLogfile(void) {
	int 							pOut[2];
	int 							pErr[2];

	if (pipe(pOut) < 0) {
		this->_logFile << this->_name << " : pipe fail." << std::endl;
		std::cerr << this->_name << " : pipe fail." << std::endl;
		//throw std::exception();
	}
	if ((pOut[0] = open(this->_stdoutlogfile, O_CREATE)) < 0) {
		this->_logFile << this->_name << " : open stdout fail." << std::endl;
		std::cerr << this->_name << " : open stdout fail." << std::endl;
		//throw std::exception();
	}
	dup2(pOut[1], 1);
	if (this->_redirect_stderr) {
		if (pipe(pErr) < 0) {
			this->_logFile << this->_name << " : pipe fail." << std::endl;
			std::cerr << this->_name << " : pipe fail." << std::endl;
			//throw std::exception();
		}
		if ((pErr[0] = open(this->_stderrlogfile, O_CREATE)) < 0) {
			this->_logFile << this->_name << " : open stderr fail." << std::endl;
			std::cerr << this->_name << " : open stderr fail." << std::endl;
			//throw std::exception();
		}
		dup2(pErr[1], 2);
	}
}

void 								Process::_setUmask(void) {
	mode_t							mask = 0;

	mask = (this->_mapUmask[0][this->_umask[0]] | this->_mapUmask[1][this->_umask[1]] | this->_mapUmask[2][this->_umask[2]]);
	umask(mask);
}

void 								Process::_setDirectory(void) {
	if (chdir(this->_directory) < 0) {
		this->_logFile << this->_name << " => ERROR : directory is incorrect." << std::endl;
		std::cerr << this->_name << " => ERROR : directory is incorrect." << std::endl;
	}
}

void 								Process::_executeProcess(void) {
	//execl(const char * path, const char * arg, ...);
	this->_logFile << this->_name << " => ERROR : execute command." << std::endl;
	//std::cerr << this->_name << " => ERROR : execute command." << std::endl;

}

void								threadFunc(void) {
	t_pid							pid;
	int 							ret;	

	if ((pid = fork()) < 0) {
		this->_logFile << this->_name << " : fork fail." << std::endl;
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
		this->_logFile << this->_name << " RUNNING with pid " << pid << std::endl;
		this->_mutes.unlock();
		wait(&ret);
		this->_mutes.lock();
		this->_pidChild = 0;
		this->_state = STOPPED;
		this->_logFile << this->_name << " STOPPED" << pid << std::endl;
		this->_mutes.unlock();
	}
	exit(1);
}

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
		this->_logFile << this->_name << " is already running." << std::endl;
	} else if (this->_state == ERROR) {
		this->_logFile << this->_name << " dont be running." << std::endl;
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
			this->_logFile << this->_name << " can not be stopped." << std::endl;
		}
	} else if (this->_state == STOPPED) {
		this->_logFile << this->_name << " is already stopped." << std::endl;
	} else if (this->_state == ERROR) {
		this->_logFile << this->_name << " process have an error." << std::endl;
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


