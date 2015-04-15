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
#include "ProcessFeature.hpp"

#define		TASKMASTER_DE	"[Taskmaster]"
#define		PROGRAM_DE		"[program:"

typedef	std::map<std::string, ProcessFeature> m_str_feature

class ParseYaml
{
public:
	static void				replace(std::string & str, std::string s1, std::string s2);
	
private:
	std::string				_confFileName;
	std::string   			_logFileName;
	m_feature				_processFeature;

	ParseYaml();
	ParseYaml(ParseYaml const &);
	ParseYaml const &			operator=(ParseYaml const &);
public:
	ParseYaml(int ac, char **av);
	~ParseYaml();

	std::string				getLogFileName(void);
	m_feature				getAllProcessFeature(void) const;
	ProcessFeature			getProcessFeature(std::string const &name) const;

	void					reloadFile(void);
};

#endif