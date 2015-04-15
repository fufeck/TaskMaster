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
#include "ProcessFeature.hpp"

enum eState {STOPPED, RUNNING, ERROR};

class Process
{
private:
	eState								_state;
	pid_t								_pidChild;
	clock_t 							_lastTime;
	ProcessFeature						_feature;
	std::mutex							_mutex;

	// map
	std::map<std::string, eState>		_mapState;
	std::vector< std::vector<int> >		_mapUmask;

	void 								_setUmask(void);
	void 								_setDirectory(void);
	void 								_executeProcess(void);
	void								_threadFunc(void);

	Process();
	Process(Process const &);
	Process const &						operator=(Process const &);

public:
	Process(ProcessFeature);
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
};

#endif