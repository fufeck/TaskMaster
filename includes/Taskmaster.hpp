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

#include <unistd.h>
#include <map>
#include <string>
#include "ParseYaml.hpp"
#include "Process.hpp"

#define	START 		"start"
#define	RESTART 	"restart"
#define	STOP 		"stop"
#define	STATUS 		"status"
#define	RELOAD 		"reload"
#define	EXIT 		"exit"

class Taskmaster
{
public:
	typedef				void (Taskmaster::*Cmd)(std::string const &cmd);
	typedef				std::list<std::string, Process> PList

private:
	ParseYaml					_parse;

	std::map<std::string, Cmd>	_cmds;
	ProcessList					_process;

	// feature
	std::string					_logFile;
	std::ifstream				_file;

	void						start(std::string const &cmd);
	void						restart(std::string const &cmd);
	void						stop(std::string const &cmd);
	void						status(std::string const &cmd);
	void						reload(std::string const &cmd);
	void						exit(std::string const &cmd);

	Taskmaster();
	Taskmaster(Taskmaster const &);
	Taskmaster const &			operator=(Taskmaster const &);

public:
	Taskmaster(int ac, char **av);
	~Taskmaster();

	void						run(void);
};

#endif