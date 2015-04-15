// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   ParseYaml.cpp                                      :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: ftaffore <ftaffore@student.42.fr>          +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2015/04/13 12:48:36 by ftaffore          #+#    #+#             //
//   Updated: 2015/04/13 12:48:37 by ftaffore         ###   ########.fr       //
//                                                                            //
// ************************************************************************** //

#include "ParseYaml.hpp"

ParseYaml::ParseYaml(int ac, char **av) {
	if (ac != 2) {
		std::cerr << "Usage : ./Taskmaster file.conf" << std::endl;
		throw std::exception;
	}
	this->_fileNameConf = av[1];
	this->_logFileName = "";
	this->reloadFile();
}

ParseYaml::~ParseYaml() {

}

std::string				ParseYaml::getLogFileName(void) {
	return this->_logFileName;
}

m_feature				ParseYaml::getAllProcessFeature(void) const {
	return this->_processFeature;
}

ProcessFeature			ParseYaml::getProcessFeature(std::string const &name) const {
	ProcessFeature 		res;

	for (m_str_feature::iterator it = this->_processFeature.begin(); it != this->_processFeature.end(); it++) {
		if (it->first == name) {
			res = it->second;
		}
	}
	return res;
}

void					ParseYaml::replace(std::string & str, std::string s1, std::string s2) {
	int a = 0;

	while ((a = str.find(s1)) >= 0) {
		str.replace(a, a, s2);
	}
}

void					ParseYaml::reloadFile(void) {
	std::ifstream   	file;
	int					nbLine = 0;
	std::string			currentProcess = "";

	file.open(this->_fileName);
	if (!file.is_open()) {
		std::cerr << "ERROR file : " << this->_fileName << " cant be opened." << std::endl;
		throw std::exception;
	}
	while (file.tellg() > 0) {
		std::string			line = "";
		std::getline(file, line);
		ParseYaml::replace(line, std::string(" "), std::string(""));
		int a = -1;
		if ((a = line.find(";")) >= 0)
			line = line.substr(0, a);
		if (line.size() > 0 && line.find(PROGRAM_DE) == 0) {
			if (line.find("]") != line.size())
				std::cerr << "ERROR file : in line " << nbLine << " bad syntax." << std::endl;

			currentProcess = line.substr(9, line.size() - 1);
			if (this->_processFeature.find(currentProcess) != this->_processFeature.end())
				std::cerr << "ERROR file : in line " << nbLine << " twice program name." << std::endl;
			this->_processFeature(currentProcess, ProcessFeature(currentProcess));
		} else if (line.size() > 0 && line == TASKMASTER_DE) {
			currentProcess = "Taskmaster";
		} else if (line.size() > 0) {
			if (currentProcess == "")
				std::cerr << "ERROR file : in line " << nbLine << " no program has been said before params." << std::endl;
			if (currentProcess == "Taskmaster") {
				if (line != "logfile=")
					std::cerr << "ERROR file : in line " << nbLine << " bad syntax" << std::endl;
				this->_logFileName = line.substr(8, line.size());
			} else {
				this->_processFeature[currentProcess].setFeature(line, nbLine);
			}
		}
		nbLine++;
	}
	for (m_str_feature::iterator it = this->_processFeature.begin(); it != this->_processFeature.end(); it++) {
		if (!it->second.isGood()) {
			std::cerr << "ERROR : " << it->first << " too much error to create process." << std::endl;
			this->_processFeature(it);
		}
	}
	file.close();
}