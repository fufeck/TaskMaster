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

#include <fstream>
#include <map>
#include <string>


typedef std::map<std::string, std::string>		StrStr;
typedef std::map<std::string, StrStr>			mStrStr;

class ParseYaml
{
	mStrStr					_tmp;
	std::string				_fileName;
	std::ofstream   		_logFile;

	ParseYaml();
	ParseYaml(ParseYaml const &);
	ParseYaml const &		operator=(ParseYaml const &);
public:
	ParseYaml(int ac, char **);
	~ParseYaml();

	std::ofstream			getLogFile(void);
	mStrStr					getAllProcess(void) const;
	StrStr					getProcess(std::string const &name) const;

	void					reloadFile(void);
};

#endif