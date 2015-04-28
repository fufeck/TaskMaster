// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   Program.hpp                                        :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: ftaffore <ftaffore@student.42.fr>          +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2015/04/13 12:51:30 by ftaffore          #+#    #+#             //
//   Updated: 2015/04/13 12:51:31 by ftaffore         ###   ########.fr       //
//                                                                            //
// ************************************************************************** //

#ifndef PROGRAM_HPP
# define PROGRAM_HPP

#include <string.h>
#include <errno.h>
#include <functional>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fstream>
#include <signal.h>
#include <thread>
#include <mutex>
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include "LogOutPut.hpp"
#include "ProgramFeature.hpp"

enum eState {STOPPED, RUNNING, ERROR};

struct Process
{
	pid_t	pid;
	bool	isRunning;
	int	 	returnCode;

	~Process() {}
	Process(pid_t id) : pid(id), isRunning(true), returnCode(0) {}
};

class Program
{
private:
	eState								_state;
	clock_t 							_lastTime;
	std::vector<Process>				_process;
	ProgramFeature						_feature;

	LogOutPut							*_logOutPut;
	std::ofstream						_stdoutLogfile;
	std::ofstream						_stderrLogfile;

	std::map<std::string, eState>		_mapState;
	std::vector< std::vector<int> >		_mapUmask;

	void 								_setUmask(void);
	void 								_setDirectory(void);
	void 								_redirectLogfile(std::string const &fileName, int start);
	void 								_executeProgram(void);
	void								_runProgram(void);

	Program const &						operator=(Program const &);
	Program(Program const &);
	Program();
public:
	Program(ProgramFeature, LogOutPut *);
	~Program(void);

	eState								getState(void) const;
	ProgramFeature						getFeature(void) const;

	void								timerCheck(void);

	void 								checkProcess(void);
	void								autostart(void);
	void								start(void);
	void								restart(void);
	void								stop(void);
	void								status(void);
	void								reload(ProgramFeature const &);

};

#endif