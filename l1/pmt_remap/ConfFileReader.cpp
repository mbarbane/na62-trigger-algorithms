/*
 * ConfFileReader.cpp
 *
 *  Created on: 30/gen/2015
 *      Author: Valeria Fascianelli
 *      Email: vxf441@hep.ph.bham.ac.uk
 */

#include "ConfFileReader.h"

ConfFileReader::ConfFileReader(const char* filename) :
		file(filename), line(), failed(false) {
}

ConfFileReader::ConfFileReader(const std::string& filename) :
		file(filename.c_str()), line(), failed(false) {
}

bool ConfFileReader::isValid() const {
	if (!file)
		return false;
	else
		return true;
}

bool ConfFileReader::nextLine() {
	getline(file, line);
	if (file.eof())
		return false;
	else
		return true;

}

void ConfFileReader::skip_fields(std::istringstream& ist, const int n) {
	if (n < 1)
		return;
	std::string tmp;
	for (int i = 1; i <= n; ++i) {
		ist >> tmp;
	}
}

bool ConfFileReader::inputFailed() const {
	return failed;
}

