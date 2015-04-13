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

#include <map>
#include <string>


typedef std::map<std::string, std::string>		StrStr;
typedef std::map<std::string, infoProcess>		mStrStr;

class ParseYaml
{
	int						_index;
	std::string				_fileName;

	ParseYaml();
	ParseYaml(ParseYaml const &);
	ParseYaml const &		operator=(ParseYaml const &);
public:
	ParseYaml(std::string const &fileName);
	~ParseYaml();

	std::string				getLogFile(void);
	infoProcess				getAllProcess(void);
	infoProcess				getProcess(std::string const &name);

	void					reloadFile(void);
};

#endif