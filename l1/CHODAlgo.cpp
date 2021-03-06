/*
 * CHODAlgo.cpp
 *
 *  Created on: May 11, 2015
 *      Author: angela romano
 *      Email: axr@hep.ph.bham.ac.uk 
 */

#include "CHODAlgo.h"

#include <eventBuilding/Event.h>
#include <eventBuilding/SourceIDManager.h>
#include <l0/MEPFragment.h>
#include <l0/Subevent.h>
#include <string.h>
#include <math.h>
#include <sys/time.h>

#include <options/Logging.h>

#include "../common/decoding/TrbFragmentDecoder.h"
#include "chod_algorithm/ParsConfFile.h"

namespace na62 {

double CHODAlgo::averageHitTime = 0.;
CHODParsConfFile* CHODAlgo::infoCHOD_ = CHODParsConfFile::GetInstance();
int * CHODAlgo::slabGeo = infoCHOD_->getGeoSlabMap();
uint CHODAlgo::nHits_V;
uint CHODAlgo::nHits_H;
uint CHODAlgo::nMaxSlabs;
int CHODAlgo::slabID;
//int CHODAlgo::quadrantID;
int CHODAlgo::planeID;

CHODAlgo::CHODAlgo() {
}

CHODAlgo::~CHODAlgo() {
// TODO Auto-generated destructor stub
}

uint_fast8_t CHODAlgo::processCHODTrigger(DecoderHandler& decoder,
		L1InfoToStorage* l1Info) {

//	LOG_INFO("Initial Time " << time[0].tv_sec << " " << time[0].tv_usec);

	using namespace l0;

	nMaxSlabs = 6;
	nHits_H = 0;
	nHits_V = 0;

//	LOG_INFO("Event number = " << decoder.getDecodedEvent()->getEventNumber());
//	LOG_INFO("CHODAlgo: event timestamp = " << std::hex << decoder.getDecodedEvent()->getTimestamp() << std::dec);
//	LOG_INFO("Average Hit Time (initial value) " << averageHitTime);

	TrbFragmentDecoder& chodPacket = (TrbFragmentDecoder&) decoder.getDecodedCHODFragment(0);
	if (!chodPacket.isReady() || chodPacket.isBadFragment()) {
		LOG_ERROR("CHODAlgo: This looks like a Bad Packet!!!! ");
		return 0;
	}
//	LOG_INFO("First time check (inside iterator) " << time[1].tv_sec << " " << time[1].tv_usec);
	/**
	 * Get Arrays with hit Info
	 */
	const uint64_t* const edge_times = chodPacket.getTimes();
	const uint_fast8_t* const edge_chIDs = chodPacket.getChIDs();
	const bool* const edge_IDs = chodPacket.getIsLeadings();
	const uint_fast8_t* const edge_tdcIDs = chodPacket.getTdcIDs();
	double finetime, edgetime;

	uint numberOfEdgesOfCurrentBoard = chodPacket.getNumberOfEdgesStored();
//	LOG_INFO("CHOD: Tel62 ID " << chodPacket.getFragmentNumber() << " - Number of Edges found " << numberOfEdgesOfCurrentBoard);
//	LOG_INFO("Reference detector fine time " << decoder.getDecodedEvent()->getFinetime());

	for (uint iEdge = 0; iEdge != numberOfEdgesOfCurrentBoard; iEdge++) {
		/**
		 * Process leading edges only
		 *
		 */
		if (edge_IDs[iEdge]) {
			const int roChID = (edge_tdcIDs[iEdge] * 32) + edge_chIDs[iEdge];
//			LOG_INFO("Readout Channel ID " << roChID);
//			LOG_INFO("Geom Slab ID " << slabGeo[roChID]);

			/**
			 * Process only first 128 readout channels, corresponding to low-threshold LAV FEE
			 *
			 */
			if (slabGeo[roChID] < 128) {
				finetime = decoder.getDecodedEvent()->getFinetime()
						* 0.097464731802;
				edgetime = (edge_times[iEdge]
						- decoder.getDecodedEvent()->getTimestamp() * 256.)
						* 0.097464731802;
//				LOG_INFO("finetime (in ns) " << finetime << " edgetime (in ns) " << edgetime);

//				if (fabs(edgetime - finetime) <= 30.) { //if ref detector is LKr
				if (fabs(edgetime - finetime) <= 20.) { //otherwise
					averageHitTime += edgetime;

//  				LOG_INFO("Edge " << iEdge << " ID " << edge_IDs[iEdge]);
//	   				LOG_INFO("Edge " << iEdge << " chID " << (uint) edge_chIDs[iEdge]);
//					LOG_INFO("Edge " << iEdge << " tdcID " << (uint) edge_tdcIDs[iEdge]);
//					LOG_INFO("Edge " << iEdge << " time " << std::hex << edge_times[iEdge] << std::dec);

					slabID = slabGeo[roChID];
//					quadrantID = slabID / 16.;
					planeID = slabID / 64.;

//					LOG_INFO("CHOD slab ID " << slabID);
//					LOG_INFO("CHOD quadrant ID " << quadrantID);
//					LOG_INFO("CHOD plane ID " << planeID);

					if (planeID)
						nHits_V++;
					else
						nHits_H++;
				}
			}
		}
	}
//	LOG_INFO("time check " << time[2].tv_sec << " " << time[2].tv_usec);
//	}
//	LOG_INFO("CHODAlgo.cpp: Analysed Event " << decoder.getDecodedEvent()->getEventNumber() << " - nHits(H) " << nHits_H << " nHits(V) " << nHits_V);

//	LOG_INFO("time check (final)" << time[3].tv_sec << " " << time[3].tv_usec);

//	for (int i = 0; i < 3; i++) {
//		if (i)
//			LOG_INFO(((time[i+1].tv_sec - time[i].tv_sec)*1e6 + time[i+1].tv_usec) - time[i].tv_usec << " ");
//		}
//	LOG_INFO(((time[3].tv_sec - time[0].tv_sec)*1e6 + time[3].tv_usec) - time[0].tv_usec);

	if (nHits_V + nHits_H)
		averageHitTime = averageHitTime / (nHits_V + nHits_H);
	else
		averageHitTime = -1.0e+28;

	l1Info->setCHODAverageTime(averageHitTime);
	l1Info->setL1CHODProcessed();

//	LOG_INFO("CHODAlgo=============== number of Hits " << nHits_V + nHits_H);
//	LOG_INFO("CHODAlgo=============== average HitTime " << averageHitTime);
//	LOG_INFO("CHODAlgo=============== L1CHODProcessed Flag " << (uint)l1Info->isL1CHODProcessed());

	averageHitTime = 0;
//	LOG_INFO("CHODAlgo=============== reset average HitTime " << averageHitTime);

	return (((nHits_V + nHits_H) > 0) && ((nHits_V + nHits_H) < nMaxSlabs));
//	return (((nHits_V == 1) && (nHits_H == 1))
//			|| ((nHits_V == 2) && (nHits_H == 1))
//			|| ((nHits_V == 1) && (nHits_H == 2)));

}

}
/* namespace na62 */

