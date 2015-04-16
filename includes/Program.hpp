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
#include "ProgramFeature.hpp"

enum eState {STOPPED, RUNNING, ERROR};

class Program
{
private:
	eState								_state;
	pid_t								_pidChild;
	clock_t 							_lastTime;
	ProgramFeature						_feature;
	std::mutex							_mutex;

	// map
	std::map<std::string, eState>		_mapState;
	std::vector< std::vector<int> >		_mapUmask;

	void 								_setUmask(void);
	void 								_setDirectory(void);
	void 								_executeProgram(void);
	void 								_redirectLogfile(std::string const &fileName, int start);

	Program const &						operator=(Program const &);
	Program(Program const &);
	Program();
public:
	Program(ProgramFeature);
	~Program(void);
	void								runProgram(void);

	void								autostart(void);
	void								start(void);
	void								restart(void);
	void								stop(void);
	void								status(void) const;
	void								reload(ProgramFeature const &);
	
	/***GETTEUR***/
	eState								getState(void) const;
	ProgramFeature						getFeature(void) const;
};

#endif