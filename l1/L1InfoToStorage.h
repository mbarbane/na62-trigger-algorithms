/*
 * L1InfoToStorage.h
 *
 *  Created on: 2 Nov 2015
 *      Author: Jonas Kunze (kunze.jonas@gmail.com)
 */

#pragma once
#ifndef L1_INFO_TO_STORAGE_H_
#define L1_INFO_TO_STORAGE_H_

#include <iostream>
#include <vector>
#include <string>

using namespace std;

class L1InfoToStorage {
public:
	L1InfoToStorage();
	~L1InfoToStorage();

	static L1InfoToStorage* GetInstance();

	double getCHODAverageTime();
	void setCHODAverageTime(double time);

	double getRICHAverageTime();
	void setRICHAverageTime(double time);

	uint getL1ReferenceTimeSource();
	void setL1ReferenceTimeSource(uint l0SourceId);

	bool getCHODEmptyFlag();
	void setCHODEmptyFlag(bool chodEmptyFlag);
	void resetCHODEmptyFlag() {
		isCHODEmptyFlag_ = false;
	}

	void setL1CHODProcessed() {
		L1CHODProcessed_ = true;
	}

	bool isL1CHODProcessed() const {
		return L1CHODProcessed_;
	}

	void resetL1CHODProcessed() {
		L1CHODProcessed_ = false;
	}

	void setL1RICHProcessed() {
		L1RICHProcessed_ = true;
	}

	bool isL1RICHProcessed() const {
		return L1RICHProcessed_;
	}

	void resetL1RICHProcessed() {
		L1RICHProcessed_ = false;
	}


private:

	static L1InfoToStorage* theInstance;  // singleton instance

	double chodTime;
	bool L1CHODProcessed_;
	double richTime;
	bool L1RICHProcessed_;
	uint l1ReferenceTimeSource_;
	bool isCHODEmptyFlag_;

};

#endif /* L1_INFO_TO_STORAGE_H_ */

