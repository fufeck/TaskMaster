// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   Taskmaster.hpp                                     :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: ftaffore <ftaffore@student.42.fr>          +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2015/04/13 12:49:24 by ftaffore          #+#    #+#             //
//   Updated: 2015/04/13 12:49:24 by ftaffore         ###   ########.fr       //
//                                                                            //
// ************************************************************************** //

#ifndef TASKMASTER_HPP
# define TASKMASTER_HPP

#include <sstream>
#include <fstream>
#include <unistd.h>
#include <map>
#include <list>
#include <string>
#include "ParseYaml.hpp"
#include "Process.hpp"

#define	START 		"start"
#define	RESTART 	"restart"
#define	STOP 		"stop"
#define	STATUS 		"status"
#define	RELOAD 		"reload"
#define	EXIT 		"exit"

typedef				std::map< std::string, Process *> PList;

class Taskmaster
{
public:
	typedef				void (Taskmaster::*Cmd)(std::string const &cmd);

private:
	ParseYaml					_parse;

	std::map<std::string, Cmd>	_cmds;
	PList						_process;

	// feature
	std::ofstream   			_logFile;

	void						_autoStart(std::string const &processName = "");
	void						_deleteProcess(std::string const &name = "");
	void						_createProcess(std::string const &name, StrStr const &infoNewProcess);

	void						_start(std::string const &processName = "");
	void						_restart(std::string const &processName = "");
	void						_stop(std::string const &processName = "");
	void						_status(std::string const &processName = "");
	void						_reload(std::string const &processName = "");
	void						_exit(void);

	Taskmaster();
	Taskmaster(Taskmaster const &);
	Taskmaster const &			operator=(Taskmaster const &);
public:
	Taskmaster(int ac, char **av);
	~Taskmaster();

	void						run(void);
};

#endif