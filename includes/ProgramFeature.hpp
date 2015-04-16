// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   ProgramFeature.hpp                                 :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: ftaffore <ftaffore@student.42.fr>          +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2015/04/15 14:30:30 by ftaffore          #+#    #+#             //
//   Updated: 2015/04/15 14:30:31 by ftaffore         ###   ########.fr       //
//                                                                            //
// ************************************************************************** //

#ifndef PROGRAM_FEATURE_HPP
# define PROGRAM_FEATURE_HPP

#include <signal.h>
#include <sstream>
#include <stdlib.h>
#include <iostream>
#include <map>
#include <vector>
#include <string>


#define		COMMAND 		"command"
#define		PROCESS_NAME 	"process_name"
#define		NUM_PROCS 		"numprocs"
#define		DIRECTORY 		"directory"
#define		UMASK 			"umask"
#define		AUTOSTART 		"autostart"
#define		AUTORESTART 	"autorestart"
#define		EXIT_CODES 		"exitcodes"
#define		STOPSIGNAL 		"stopsignal"
#define		STOP_WAIT_SECS 	"stopwaitsecs"
#define		REDIRECT_SDTERR "redirect_stderr"
#define		STDOUT_LOGFILE 	"stdout_logfile"
#define		STDERR_LOGFILE 	"stderr_logfile"
#define		ENV 			"environment"

typedef std::map<std::string, std::string>	m_str;
typedef std::vector<int> 					v_int;

enum cmpFeature {MUST_RESTART, NO_RESTART, NOTHING};
enum eRestart {ALL_THE_TIME, UNEXPECTED, NEVER};

class ProgramFeature
{
public:
	typedef							void (ProgramFeature::*setFunc)(std::string const & command, int nbLine);

	std::string							_programName;

	std::string 						_processName;
	std::string							_command;
	std::string 						_directory;
	v_int 								_umask;
	int									_numprocs;
	bool 								_autostart;
	eRestart							_autorestart;
	v_int								_exitcodes;
	int									_stopsignal;
	int									_stopwaitsecs;
	bool								_redirect_stderr;
	std::string							_stdoutlogfile;
	std::string							_stderrlogfile;
	m_str								_env;


	std::map<std::string, setFunc>		_mapSet;
	
	ProgramFeature();
	ProgramFeature(std::string const &programName);
	ProgramFeature(ProgramFeature const &);
	~ProgramFeature();
	ProgramFeature&						operator=(ProgramFeature const & other);
	cmpFeature							operator==(ProgramFeature const & other) const;
	void								replace(std::string & str, std::string s1, std::string s2);

	void								setCommand(std::string const & command, int nbLine);
	void								setProcessName(std::string const & processName, int nbLine);
	void								setDirectory(std::string const & directory, int nbLine);
	void								setUmask(std::string const & umask, int nbLine);
	void								setNumProcs(std::string const & numProcs, int nbLine);
	void								setAutoStart(std::string const & autostart, int nbLine);
	void								setAutoRestart(std::string const & autorestart, int nbLine);
	void								setExitcodes(std::string const & exitCodes, int nbLine);
	void								setStopSignal(std::string const & stopSignal, int nbLine);
	void								setStopWaitSec(std::string const & wait, int nbLine);
	void								setRedirectStderr(std::string const & redirect_stderr, int nbLine);
	void								setStdoutLogfile(std::string const & stdoutlogfile, int nbLine);
	void								setStderrLogfile(std::string const & stderrlogfile, int nbLine);
	void								setEnv(std::string const & env, int nbLine);
	void								setFeature(std::string const & line, int nbLine);
	bool								isGood(void) const;

	std::string	const					getProgramName(void) const;
	std::string							getCommand(void) const;
	std::string							getProcessName(void) const;
	std::string							getDirectory(void) const;
	v_int								getUmask(void) const;
	int									getNumProcs(void) const;
	bool								getAutoStart(void) const;
	eRestart							getAutoRestart(void) const;
	v_int								getExitcodes(void) const;
	int									getStopSignal(void) const;
	int									getStopWaitSec(void) const;
	bool								getRedirectStderr(void) const;
	std::string							getStdoutLogfile(void) const;
	std::string							getStderrLogfile(void) const;
	m_str								getEnv(void) const;

};

#endif