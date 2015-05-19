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

ParseYaml::ParseYaml(int ac, char **av) : _start(true) {
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

void					ParseYaml::_empturStr(std::string & line) {
	int 				c = -1;
	if ((c = line.find(";")) >= 0)
		line = line.substr(0, c);

	unsigned int a = 0;
	while ((a < line.size() && line[a] ==  ' ') || (a < line.size() && line[a] == '\t'))
		a++;
	if (line.size() > 0) {

		int b = line.size() - 1;
		while ((b > 0 && line[b] ==  ' ') || (b > 0 && line[b] == '\t'))
			b--;
		line = line.substr(a, b + 1);
	}

}

bool					ParseYaml::getStart(void) {
	bool	 			tmp = this->_start;
	this->_start = false;
	return tmp;
}

void					ParseYaml::dd(void) {
	for (m_feature::const_iterator it = this->_programFeature.begin(); it != this->_programFeature.end(); it++) {
		it->second.display();
	}
}

void					ParseYaml::reloadFile(void) {
	std::ifstream   	file;
	int					nbLine = 1;
	std::string			currentProgram = "";

	this->_programFeature.clear();
	file.open(this->_confFileName);
	if (!file.is_open()) {
		std::cerr << "ERROR file : " << this->_confFileName << " cant be opened." << std::endl;
		throw std::exception();
	}
	while (file.tellg() >= 0) {

		std::string			line = "";
		std::getline(file, line);
		this->_empturStr(line);
		if (line.size() > 0 && line.find(PROGRAM_DE) == 0) {
			if (line.find("]") != line.size() - 1) {
				std::cerr << "ERROR file : in line " << nbLine << " bad syntax." << std::endl;
				currentProgram = "";
			}
			else {
				currentProgram = line.substr(9, line.size() - 10);
				if (this->_programFeature.find(currentProgram) != this->_programFeature.end())
					std::cerr << "ERROR file : in line " << nbLine << " twice program name." << std::endl;
				else {
					ProgramFeature 		newProgram(currentProgram);
					this->_programFeature[currentProgram] = newProgram;
				}
				
			}
		} else if (line.size() > 0 && line == TASKMASTER_DE) {
			currentProgram = "Taskmaster";
		} else if (line.size() > 0) {
			if (currentProgram == "Taskmaster") {
				if (line.substr(0, 8) != "logfile=")
					std::cerr << "ERROR file : in line " << nbLine << " bad syntax." << std::endl;
				else
					this->_logFileName = line.substr(8, line.size());
			} else {
				if (this->_programFeature.find(currentProgram) == this->_programFeature.end()) {
					std::cerr << "No program declare to params in line " << nbLine << "." << std::endl;
				} else {
					this->_programFeature[currentProgram].setFeature(line, nbLine);
				}
			}
		}
		nbLine++;
	}
	for (m_feature::iterator it = this->_programFeature.begin(); it != this->_programFeature.end(); it++) {
		if (!it->second.isGood()) {
			std::cerr << "ERROR : [" << it->first << "] too much error to create process." << std::endl;
			this->_programFeature.erase(it);
		}
	}
	file.close();
	this->dd();
}
