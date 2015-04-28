// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   ParseYaml.hpp                                      :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: ftaffore <ftaffore@student.42.fr>          +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2015/04/13 12:48:49 by ftaffore          #+#    #+#             //
//   Updated: 2015/04/13 12:48:52 by ftaffore         ###   ########.fr       //
//                                                                            //
// ************************************************************************** //

#ifndef PARSE_YAML_HPP
# define PARSE_YAML_HPP

#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <string>
#include "ProgramFeature.hpp"

#define		TASKMASTER_DE	"[Taskmaster]"
#define		PROGRAM_DE		"[program:"

typedef	std::map<std::string, ProgramFeature> m_feature;

class ParseYaml
{
public:
	
private:
	bool					_start;
	std::string				_confFileName;
	std::string   			_logFileName;
	m_feature				_programFeature;

	void					_empturStr(std::string & line);
	ParseYaml();
	ParseYaml(ParseYaml const &);
	ParseYaml const &		operator=(ParseYaml const &);
public:
	ParseYaml(int ac, char **av);
	~ParseYaml();

	void					dd(void);
	std::string				getLogFileName(void);
	m_feature				getAllProgramFeature(void) const;
	ProgramFeature			getProgramFeature(std::string const &name) const;

	bool					getStart(void);
	void					reloadFile(void);
};

#endif