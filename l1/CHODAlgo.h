/*
 * CHODAlgo.h
 *
 *  Created on: May 11, 2015
 *      Author: angela romano
 *      Email: axr@hep.ph.bham.ac.uk
 */

#pragma once
#ifndef CHODALGORITHM_H_
#define CHODALGORITHM_H_

#include <sys/types.h>
#include <cstdint>

#include "../common/decoding/DecoderHandler.h"
#include "chod_algorithm/ParsConfFile.h"
#include "L1InfoToStorage.h"

#define maxNhits 500

namespace na62 {


class CHODAlgo {
public:
	/**
	 * @param event Event* This is a pointer to the built Event containing all subevents (except those from the LKr)
	 *
	 * The event pointer may not be deleted by this method as it is deleted by the EventBuilder
	 *
	 * @return uint_fast8_t <0> if the event is rejected, the L1 trigger type word in other cases.
	 */

	CHODAlgo();
	~CHODAlgo();
	static uint_fast8_t processCHODTrigger(DecoderHandler& decoder, L1InfoToStorage* l1Info);

private:

	static CHODParsConfFile* infoCHOD_;
	static int* slabGeo;
	static uint nHits_V, nHits_H;
	static uint nMaxSlabs;
	static int slabID;
//	static int quadrantID;
	static int planeID;
	static double averageHitTime;
	static uint l1RefTimeDetId;
	static uint l0tpSourceId;
	static uint chodSourceId;
	static bool isCHODEmpty;

};

} /* namespace na62 */
#endif /* CHODALGORITHM_H_ */
