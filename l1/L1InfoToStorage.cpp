/*
 * L1InfoToStorage.cpp
 *
 *  Created on: 2 Nov 2015
 *      Author: Jonas Kunze (kunze.jonas@gmail.com)
 */

#include "L1InfoToStorage.h"

#include <iostream>
#include <vector>
#include <options/Logging.h>
#include <string.h>

#include <sys/time.h>

L1InfoToStorage* L1InfoToStorage::theInstance = nullptr;

L1InfoToStorage::L1InfoToStorage() {
	LOG_INFO<< "********************In L1 InfoToStorage" << ENDL;
	chodTime = 0.;
	richTime = 0.;
	L1CHODProcessed_ = false;
	L1RICHProcessed_ = false;
	l1ReferenceTimeSource_ = 0;
	isCHODEmptyFlag_ = false;
}

L1InfoToStorage::~L1InfoToStorage() {
}

L1InfoToStorage* L1InfoToStorage::GetInstance() {

	if (theInstance == nullptr) {
		theInstance = new L1InfoToStorage();
	}
	return theInstance;

}

double L1InfoToStorage::getCHODAverageTime() {
	return chodTime;
}

void L1InfoToStorage::setCHODAverageTime(double time) {
	chodTime = time;
}

double L1InfoToStorage::getRICHAverageTime() {
	return richTime;
}

void L1InfoToStorage::setRICHAverageTime(double time) {
	richTime = time;
}

uint L1InfoToStorage::getL1ReferenceTimeSource(){
	return l1ReferenceTimeSource_;
}

void L1InfoToStorage::setL1ReferenceTimeSource(uint l0SourceId) {
	l1ReferenceTimeSource_ = l0SourceId;
}

bool L1InfoToStorage::getCHODEmptyFlag(){
	return isCHODEmptyFlag_;
}

void L1InfoToStorage::setCHODEmptyFlag(bool chodEmptyFlag){
	isCHODEmptyFlag_ = chodEmptyFlag;
}
