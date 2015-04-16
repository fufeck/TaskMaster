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
		throw std::exception();
	}
	this->_confFileName = av[1];
	this->_logFileName = "";
	this->reloadFile();
	if (this->_logFileName == "") {
		std::cerr << "ERROR : logfile is not define." << std::endl;
		throw std::exception();
	}
	this->_start = true;
}

ParseYaml::~ParseYaml() {

}

std::string				ParseYaml::getLogFileName(void) {
	return this->_logFileName;
}

m_feature				ParseYaml::getAllProgramFeature(void) const {
	return this->_programFeature;
}

ProgramFeature			ParseYaml::getProgramFeature(std::string const &programName) const {
	ProgramFeature 		res(programName);

	for (m_feature::const_iterator it = this->_programFeature.begin(); it != this->_programFeature.end(); it++) {
		if (it->first == programName) {
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

bool					ParseYaml::getStart(void) const {
	return this->_start;
}

void					ParseYaml::reloadFile(void) {
	std::ifstream   	file;
	int					nbLine = 0;
	std::string			currentProgram = "";

	file.open(this->_confFileName);
	if (!file.is_open()) {
		std::cerr << "ERROR file : " << this->_confFileName << " cant be opened." << std::endl;
		throw std::exception();
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

			currentProgram = line.substr(9, line.size() - 1);
			if (this->_programFeature.find(currentProgram) != this->_programFeature.end())
				std::cerr << "ERROR file : in line " << nbLine << " twice program name." << std::endl;
			ProgramFeature 		newProgram(currentProgram);
			this->_programFeature[currentProgram] = newProgram;
		} else if (line.size() > 0 && line == TASKMASTER_DE) {
			currentProgram = "Taskmaster";
		} else if (line.size() > 0) {
			if (currentProgram == "")
				std::cerr << "ERROR file : in line " << nbLine << " no program has been said before params." << std::endl;
			if (currentProgram == "Taskmaster") {
				if (line != "logfile=")
					std::cerr << "ERROR file : in line " << nbLine << " bad syntax." << std::endl;
				this->_logFileName = line.substr(8, line.size());
			} else {
				this->_programFeature[currentProgram].setFeature(line, nbLine);
			}
		}
		nbLine++;
	}
	for (m_feature::iterator it = this->_programFeature.begin(); it != this->_programFeature.end(); it++) {
		if (!it->second.isGood()) {
			std::cerr << "ERROR : " << it->first << " too much error to create process." << std::endl;
			this->_programFeature.erase(it);
		}
	}
	file.close();
	this->_start = false;
}
