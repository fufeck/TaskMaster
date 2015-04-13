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

enum eState {STOP, RUNNING, ERROR};
enum eRestart {ALREADY, UNEXPECTED, NEVER};
enum eExit {QUIT};

class Process
{
public:
	typedef				void (Process::*setFunc)(std::string const &cmd);

private:
	eState								_state = STOP;
	pid_t								_pidChild;
	clock_t 							_startTime;
	std::ifstream   					_fileLog;

	// feature
	std::string							_command = "";
	std::string 						_process_name = "";
	std::string 						_directory = ".";
	std::string 						_umask = "022";
	int									_numprocs = 1;
	bool 								_autostart = true;
	eRestart							_autorestart = NEVER;
	std::vector<int>					_exitcodes = {1};
	eExit								_stopsignal = QUIT;
	int									_stopwaitsecs = 10;
	bool								_redirect_stderr = false;
	std::string							_stdoutlogfile = "tmp/stdinLog";
	std::string							_stderrlogfile = "tmp/stderrLog";
	std::map<std::string, std::string>	_varEnv = {};

	std::map<std::string, setFunc>		_mapSet;
	std::map<std::string, eState>		_mapState;
	std::map<std::string, eRestart>		_mapRestart;
	std::map<std::string, eExit>		_mapExit;
	std::map<std::string, bool>			_mapTruth;
	Process();
	Process(Process const &);
public:
	Process(std::map<std::string, std::string> const &infoProcess);
	Process const &						operator=(Process const &);
	~Process(void);

	void								autostart(void);
	void								start(void);
	void								restart(void);
	void								stop(void);
	void								status(void);
	void								reload(void);
	void								exit(void);
	
	/***GETTEUR***/

	std::string							getCommand(void) const;
	std::string							getProcessName(void) const;
	int									getNumProcs(void) const;
	bool								getAutoStart(void) const;
	bool								getAutoRestart(void) const;
	std::vector<int>					getExitcodes(void) const;
	int									getStopSignal(void) const;
	bool								getRedirectStderr(void) const;
	std::string							getStdoutLogfile(void) const;
	std::string							getStderrLogfile(void) const;

	/***SETTEUR***/

	void								setCommand(std::string const &);
	void								setProcessName(std::string const &);
	void								setNumProcs(std::string const &);
	void								setAutoStart(std::string const &);
	void								setAutoRestart(std::string const &);
	void								setExitcodes(std::string const &);
	void								setStopSignal(std::string const &);
	void								setRedirectStderr(std::string const &);
	void								setStdoutLogfile(std::string const &);
	void								setStderrLogfile(std::string const &);
};

#endif