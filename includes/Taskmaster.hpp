// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   Taskmaster.hpp                                     :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: ftaffore <ftaffore@student.42.fr>          +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2015/04/15 14:27:57 by ftaffore          #+#    #+#             //
//   Updated: 2015/04/15 14:27:58 by ftaffore         ###   ########.fr       //
//                                                                            //
// ************************************************************************** //

#ifndef TASKMASTER_HPP
# define TASKMASTER_HPP

#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <sstream>
#include <fstream>
#include <unistd.h>
#include <map>
#include <list>
#include <string>
#include <thread>
#include <mutex>
#include "LogOutPut.hpp"
#include "ParseYaml.hpp"
#include "Program.hpp"

#define PRONPT		"$Taskmaster>"
#define	START 		"start"
#define	RESTART 	"restart"
#define	STOP 		"stop"
#define	STATUS 		"status"
#define	RELOAD 		"reload"
#define	EXIT 		"exit"

typedef				std::map<std::string, Program *> PList;

class Taskmaster
{
public:
	typedef							void (Taskmaster::*CmdFunc)(std::string const &programName);

private:
	ParseYaml						_parse;
	PList							_programs;
	LogOutPut						*_logOutPut;

	std::map<std::string, CmdFunc>	_cmds;


	void							_start(std::string const &programName = "");
	void							_restart(std::string const &programName = "");
	void							_stop(std::string const &programName = "");
	void							_status(std::string const &programName = "");
	void							_reload(std::string const &programName = "");

	void							_delProgram(std::string const &name = "");
	void							_addProgram(std::string const &nameProgram, ProgramFeature const &newProgramFeature);

	Taskmaster();
	Taskmaster(Taskmaster const &);
	Taskmaster const &				operator=(Taskmaster const &);
public:
	Taskmaster(int ac, char **av);
	~Taskmaster();

	void							exit(void);
	void							runCheck(void);
	void							runCmd(void);

	void							run(void);
};

void								myPuts(int fd, std::string str);

#endif