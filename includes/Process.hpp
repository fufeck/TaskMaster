// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   Process.hpp                                        :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: ftaffore <ftaffore@student.42.fr>          +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2015/04/13 12:51:30 by ftaffore          #+#    #+#             //
//   Updated: 2015/04/13 12:51:31 by ftaffore         ###   ########.fr       //
//                                                                            //
// ************************************************************************** //

#ifndef PROCESS_HPP
# define PROCESS_HPP

#include <fstream>
#include <signal.h>
#include <thread>
#include <mutex>
#include <iostream>
#include <string>
#include <vector>
#include <map>

enum eState {STOPPED, RUNNING, ERROR};
enum eRestart {ALREADY, UNEXPECTED, NEVER};
enum eCmp {NOTHING, LITTLE, BIG};

class Process
{
public:
	typedef					void (Process::*setFunc)(std::string const &cmd);
	//typedef				void (Process::*transFunc)(std::string const &cmd) const;

private:
	const std::string 					_name;
	std::ofstream   					_fileLog;
	eState								_state;
	pid_t								_pidChild;
	clock_t 							_lastTime;

	// feature
	std::string							_command;
	std::string 						_directory;
	std::string 						_umask;
	int									_numprocs;
	bool 								_autostart;
	eRestart							_autorestart;
	std::vector<int>					_exitcodes;
	int									_stopsignal;
	int									_stopwaitsecs;
	bool								_redirect_stderr;
	std::string							_stdoutlogfile;
	std::string							_stderrlogfile;
	std::map<std::string, std::string>	_varEnv;


	std::mutex							_mutex;

	// map
	std::map<std::string, setFunc>		_mapSet;
	std::map<std::string, eState>		_mapState;
	std::map<std::string, eRestart>		_mapRestart;
	std::map<std::string, int>			_mapExit;
	std::map<std::string, bool>			_mapTruth;
	std::vector< std::vector<int> >		_mapUmask;

	// func private
	int									_transformToInt(std::string const & command) const ;
	bool								_transformToBool(std::string const & command) const ;
	eRestart							_transformToRestart(std::string const & command) const ;
	std::vector<int>					_transformToExitCodes(std::string const & command) const ;
	int									_transformToStopSignal(std::string const & command) const ;
	std::map<std::string, std::string>	_transformToVarEnv(std::string const & command) const ;


	Process();
	Process(Process const &);
	//Process const &						operator=(Process const &);

public:
	Process(std::string const &name, std::ofstream logFile, StrStr const &infoProcess);
	eCmp								operator==(Process const &) const ;
	~Process(void);

	void								autostart(void);
	void								start(void);
	void								restart(void);
	void								stop(void);
	void								status(void);
	void								reload(StrStr const &newProcess);
	void								exit(void);
	
	/***GETTEUR***/

	eState								getState(void) const;
	std::string							getCommand(void) const;
	std::string							getName(void) const;
	std::string							getDirectory(void) const;
	std::string							getUmask(void) const;
	int									getNumProcs(void) const;
	bool								getAutoStart(void) const;
	eRestart							getAutoRestart(void) const;
	std::vector<int>					getExitcodes(void) const;
	int									getStopSignal(void) const;
	int									getStopWaitSec(void) const;
	bool								getRedirectStderr(void) const;
	std::string							getStdoutLogfile(void) const;
	std::string							getStderrLogfile(void) const;
	std::map<std::string, std::string>	getVarEnv(void) const;

	/***SETTEUR***/

	void								setCommand(std::string const &);
	//void								setName(std::string const &);
	void								setDirectory(std::string const &);
	void								setUmask(std::string const &);
	void								setNumProcs(std::string const &);
	void								setAutoStart(std::string const &);
	void								setAutoRestart(std::string const &);
	void								setExitcodes(std::string const &);
	void								setStopSignal(std::string const &);
	void								setStopWaitSec(std::string const &);
	void								setRedirectStderr(std::string const &);
	void								setStdoutLogfile(std::string const &);
	void								setStderrLogfile(std::string const &);
	void								setVarEnv(std::string const &);
};

#endif