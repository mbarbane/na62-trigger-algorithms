/*
 * RichData.cpp
 *
 *  Created on: 29/gen/2015
 *      Author: Valeria Fascianelli
 *      Email: vxf441@hep.ph.bham.ac.uk
 */

#include "RichData.h"
#include "../rich_algorithm/tdcb_buffer.h"

#include <eventBuilding/Event.h>
#include <l0/MEPFragment.h>
#include <stdlib.h>
#include <cstdint>

namespace na62 {

RichData::RichData() {

	nHits = 0;
	nHits_tot = 0;
    nWords = 0;
    rich_TrbHeader = 0;
    rich_FPGAHeader = new FPGADataHeader*[maxNFPGA];
    rich_FrameHeader = new FrameDataHeader**[maxNFPGA];
    tdc_data = new TrbData*[maxNhit];

    for (int i = 0; i < maxNFPGA; i++){
    	rich_FrameHeader[i] = new FrameDataHeader*[maxNFrame];

    }
}

RichData::~RichData() {}

void RichData::setHits(l0::MEPFragment* trbDataFragment) {

	char* payload = trbDataFragment->getPayload();
	rich_TrbHeader = (TrbDataHeader*) payload;

	for (int iFPGA = 0; iFPGA < maxNFPGA; iFPGA++) {

		rich_FPGAHeader[iFPGA] = (FPGADataHeader*) payload + 1 + iFPGA + nWords;

		for (int iFrame = 0; iFrame < maxNFrame; iFrame++) {

			rich_FrameHeader[iFPGA][iFrame] = (FrameDataHeader*) payload + 2 + iFPGA + nWords;
			nWords += (uint) rich_FrameHeader[iFPGA][iFrame]->nWordsPerFrame;
			nHits = ((uint) rich_FrameHeader[iFPGA][iFrame]->nWordsPerFrame) - 1;

			if (nHits) {

				for (int ihit = 0; ihit < nHits; ihit++){

					tdc_data[ihit + nHits_tot] = (TrbData*) payload + 2 + iFPGA + nWords - nHits + ihit;

					if (ihit == (nHits - 1)) nHits_tot += nHits;

				}


			}

		}
	}

}

}

