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

ParseYaml::ParseYaml(int ac, char **) {
	if (ac != 2)
		//throw expection
	this->_fileName = av[1];
	this->_reloadFile();
	if (!this->_logFile.is_open()) {
		//throw
	}
}

ParseYaml::~ParseYaml() {
	this->_logFile.close();
}


std::ofstream			ParseYaml::getLogFile(void) {
	return this->_logFile;
}

mStrStr					ParseYaml::getAllProcess(void) const {
	return this->_tmp;
}

StrStr					ParseYaml::getProcess(std::string const &name) const {
	for (mStrStr::iterator it = this->_tmp.begin(); it != this->_tmp.end(); it++) {
		if (it->first == name) {
			return it->second;
		}
	}
}

void					ParseYaml::reloadFile(void) {
	std::ifstream   	file;
	int					nbLine = 0;
	std::string			currentProcess = "";

	this->_file.open(av[1]);
	if (!file.is_open()) {
		//throw
	}
	while (file.tellg() > 0) {
		std::string			line = "";
		std::getline(file, line);

		line = line.replace(" ", "");
		if ((int a = line.find(";")) != line.end())
			line = line.substr(0, a);
		if (line.size() > 0 && line.find("[program:") == 0) {
			if (line.find("]") != line.size())
				//throw

			currentProcess = line.substr(9, line.size() - 1);
			if (currentProcess)
				//throw
//			this->_tmp[currentProcess] = std::map<std::string, StrStr> );
		} else if (line.size() > 0 && line == "[Taskmaster]") {
			currentProcess = "Taskmaster";
		} else if (line.size() > 0) {
			if (currentProcess == "")
				//throw
			if (currentProcess == "Taskmaster") {
				if (line != "logfile=")
					//throw
				this->_logFile.open(line.substr(8, line.size()));
				if (!this->_logFile.is_open()) {
					//throw
				}
			} else {
				if (line.find('=') == line.end() && line.find('=') <= 0 && line.find('=') >= line.size())
					//throw
				this->_tmp[currentProcess][line.substr(0, "=")] = line.substr(line.find('='), line.size());
			}
		}
		nbLine++;
	}
}