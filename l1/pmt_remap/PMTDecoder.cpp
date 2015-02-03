/*
 * PMTDecoder.cpp
 *
 *  Created on: 30/gen/2015
 *      Author: Valeria Fascianelli
 *      Email: vxf441@hep.ph.bham.ac.uk
 */

#include "ConfFileReader.hpp"
#include "PMTDecoder.h"
#include <iostream>
#include <vector>

using namespace std;

PMTDecoder::PMTDecoder() {

	ConfFileReader file("../config/RICH.conf");

	if (file.isValid()) {

		cout << "File open" << endl;

		while (file.nextLine()) {

			if (file.getField<string>(1) == "#") {
				continue;
			}
			if (file.getField<string>(1) == "NROChannels=") {
				nChannels = file.getField<int>(2);
			}

			if (file.getField<string>(1).find("ChRemap_") != string::npos) {
				for (int iCh = 0; iCh < nChannels / 16; iCh++) {
					char name[100];
					sprintf(name, "ChRemap_%d=", iCh);
					string remap = (string) name;
					cout << remap << endl;
					cout << file.getField<string>(1) << endl;
					;
					if (file.getField<string>(1) == remap) {
						for (int jCh = 0; jCh < 16; jCh++) {
							channelRemap.push_back(file.getField<int>(jCh + 2));
						}
					}
				}
			}
			if (file.getField<string>(1) == "SCPosition_SC_60=") {
				cout << "End file" << endl;
			}
		}
	}
}

PMTDecoder::~PMTDecoder() {
}

int PMTDecoder::getChannelRemap(int ROID) {

	int geoID = channelRemap.at(ROID);
	return geoID;

}

vector<int> PMTDecoder::getChannelRemap() {

	return channelRemap;

}

