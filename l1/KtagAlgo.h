/*
 * KtagAlgo.h
 *
 *  Created on: Jan 25, 2015
 *      Author: angela romano
 *      Email: axr@hep.ph.bham.ac.uk
 */

#pragma once
#ifndef KTAGALGORITHM_H_
#define KTAGALGORITHM_H_

#include <sys/types.h>
#include <cstdint>

#include "../common/decoding/DecoderHandler.h"
#include "L1InfoToStorage.h"

namespace na62 {

class Event;

class KtagAlgo {
public:
	/**
	 * @param event Event* This is a pointer to the built Event containing all subevents (except those from the LKr)
	 *
	 * The event pointer may not be deleted by this method as it is deleted by the EventBuilder
	 *
	 * @return uint_fast8_t <0> if the event is rejected, the L1 trigger type word in other cases.
	 */
	static uint_fast8_t processKtagTrigger(DecoderHandler& decoder,L1InfoToStorage* l1Info);
	/**
	 * Calculates the sectorID based on the Tel62 and FPGA IDs. Possible results are between 0 and 7
	 */
	static uint calculateSector(uint tel62ID, uint fpgaID) {
		return ((tel62ID << 2) | fpgaID) / 3;
	}

private:
	static double averageCHODHitTime;
	static uint l1RefTimeDetId;
	static uint l0tpSourceId;
	static uint chodSourceId;
	static bool isCHODEmpty;

};

} /* namespace na62 */
#endif /* KTAGALGORITHM_H_ */
