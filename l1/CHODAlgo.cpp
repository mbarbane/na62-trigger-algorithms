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
uint CHODAlgo::l1RefTimeDetId;
uint CHODAlgo::l0tpSourceId = 0;
uint CHODAlgo::chodSourceId = 1;
bool CHODAlgo::isCHODEmpty = false;

CHODAlgo::CHODAlgo() {
}

CHODAlgo::~CHODAlgo() {
// TODO Auto-generated destructor stub
}

uint_fast8_t CHODAlgo::processCHODTrigger(DecoderHandler& decoder, L1InfoToStorage* l1Info) {

//	LOG_INFO<< "Initial Time " << time[0].tv_sec << " " << time[0].tv_usec << ENDL;

	using namespace l0;

	nMaxSlabs = 6;
	nHits_H = 0;
	nHits_V = 0;

//	LOG_INFO<< "Event number = " << decoder.getDecodedEvent()->getEventNumber() << ENDL;
//	LOG_INFO<< "Timestamp = " << std::hex << decoder.getDecodedEvent()->getTimestamp() << std::dec << ENDL;
//	LOG_INFO << "Average Hit Time (initial value) " << averageHitTime << ENDL;

//	LOG_INFO<< "l1ReferenceTimeSource " << l1Info->getL1ReferenceTimeSource() << ENDL;
//	LOG_INFO<< "l0tpSourceId " << l0tpSourceId << ENDL;
//	LOG_INFO<< "chodSourceId " << chodSourceId << ENDL;

	l1RefTimeDetId = l1Info->getL1ReferenceTimeSource();
//	LOG_INFO<< "l1RefTimeDetId " << l1RefTimeDetId << ENDL;
//	LOG_INFO<< "isCHODEmpty (beginning) " << isCHODEmpty << ENDL;

	TrbFragmentDecoder& chodPacket =
			(TrbFragmentDecoder&) decoder.getDecodedCHODFragment(0);
//	LOG_INFO<< "First time check (inside iterator) " << time[1].tv_sec << " " << time[1].tv_usec << ENDL;
	/**
	 * Get Arrays with hit Info
	 */
	const uint64_t* const edge_times = chodPacket.getTimes();
	const uint_fast8_t* const edge_chIDs = chodPacket.getChIDs();
	const bool* const edge_IDs = chodPacket.getIsLeadings();
	const uint_fast8_t* const edge_tdcIDs = chodPacket.getTdcIDs();
	double finetime, edgetime;

	uint numberOfEdgesOfCurrentBoard = chodPacket.getNumberOfEdgesStored();
//	LOG_INFO<< "CHOD: Tel62 ID " << chodPacket.getFragmentNumber() << " - Number of Edges found " << numberOfEdgesOfCurrentBoard << ENDL;
//	LOG_INFO<< "Reference detector fine time " << decoder.getDecodedEvent()->getFinetime() << ENDL;

	if(!numberOfEdgesOfCurrentBoard) isCHODEmpty = true;
//	LOG_INFO<< "isCHODEmpty " << isCHODEmpty << ENDL;
	l1Info->setCHODEmptyFlag(isCHODEmpty);

	for (uint iEdge = 0; iEdge != numberOfEdgesOfCurrentBoard; iEdge++) {

//		finetime = decoder.getDecodedEvent()->getFinetime() * 0.097464731802;
//		edgetime = (edge_times[iEdge] - decoder.getDecodedEvent()->getTimestamp() * 256.) * 0.097464731802;
//		LOG_INFO<< "finetime (in ns) " << finetime << ENDL;
//		LOG_INFO<< "edgetime (in ns) " << edgetime << ENDL;
		/**
		 * Process leading edges only
		 *
		 */
		if (edge_IDs[iEdge]) {
			const int roChID = (edge_tdcIDs[iEdge] * 32) + edge_chIDs[iEdge];
//			LOG_INFO<< "Readout Channel ID " << roChID << ENDL;
//			LOG_INFO<< "Geom Slab ID " << slabGeo[roChID] << ENDL;

			/**
			 * Process only first 128 readout channels, corresponding to low-threshold LAV FEE
			 *
			 */
			if (slabGeo[roChID] < 128) {
				finetime = decoder.getDecodedEvent()->getFinetime() * 0.097464731802;
				edgetime = (edge_times[iEdge] - decoder.getDecodedEvent()->getTimestamp() * 256.) * 0.097464731802;
//				LOG_INFO<< "finetime (in ns) " << finetime << " edgetime (in ns) " << edgetime << ENDL;

				if (((l1RefTimeDetId==l0tpSourceId) && (fabs(edgetime - finetime) <= 20.))
						|| ((l1RefTimeDetId == chodSourceId) && (fabs(edgetime - finetime) <= 30.))) {
//				if(fabs(edgetime - finetime) <= 30.) { //if ref detector is LKr
//				if(fabs(edgetime - finetime) <= 20.) { //otherwise

//  				LOG_INFO<< "Edge " << iEdge << " ID " << edge_IDs[iEdge] << ENDL;
//	   				LOG_INFO<< "Edge " << iEdge << " chID " << (uint) edge_chIDs[iEdge] << ENDL;
//					LOG_INFO<< "Edge " << iEdge << " tdcID " << (uint) edge_tdcIDs[iEdge] << ENDL;
//					LOG_INFO<< "Edge " << iEdge << " time " << std::hex << edge_times[iEdge] << std::dec << ENDL;

					slabID = slabGeo[roChID];
//					quadrantID = slabID / 16.;
					planeID = slabID / 64.;

//					LOG_INFO<< "CHOD slab ID " << slabID << ENDL;
//					LOG_INFO<< "CHOD quadrant ID " << quadrantID << ENDL;
//					LOG_INFO<< "CHOD plane ID " << planeID << ENDL;

					if (planeID)
						nHits_V++;
					else
						nHits_H++;

					if(l1RefTimeDetId==chodSourceId) averageHitTime += edgetime;

				}
			}
		}
	}
//	LOG_INFO<< "time check " << time[2].tv_sec << " " << time[2].tv_usec << ENDL;
//	}
//	LOG_INFO<<" CHODAlgo.cpp: Analysed Event " << decoder.getDecodedEvent()->getEventNumber() << " - nHits(H) " << nHits_H << " nHits(V) " << nHits_V << ENDL;

//	LOG_INFO<< "time check (final)" << time[3].tv_sec << " " << time[3].tv_usec << ENDL;

//	for (int i = 0; i < 3; i++) {
//		if (i)
//			LOG_INFO<<((time[i+1].tv_sec - time[i].tv_sec)*1e6 + time[i+1].tv_usec) - time[i].tv_usec << " ";
//		}
//	LOG_INFO<< ((time[3].tv_sec - time[0].tv_sec)*1e6 + time[3].tv_usec) - time[0].tv_usec << ENDL;

	if ((l1RefTimeDetId==chodSourceId) && (nHits_V + nHits_H)) averageHitTime = averageHitTime/(nHits_V + nHits_H);
	else averageHitTime = 0.;

	l1Info->setCHODAverageTime(averageHitTime);
	l1Info->setL1CHODProcessed();

//	LOG_INFO<< "PAT=============== number of Hits " << nHits_V + nHits_H << ENDL;
//	LOG_INFO<< "PAT=============== average HitTime " << averageHitTime << ENDL;
//	LOG_INFO<< "PAT=============== L1CHODProcessed Flag " << (uint)l1Info->isL1CHODProcessed() << ENDL;

	averageHitTime = 0;
//	LOG_INFO<< "PAT=============== reset average HitTime " << averageHitTime << ENDL;
	isCHODEmpty = false;
//	LOG_INFO<< "PAT=============== reset is CHODEmpty Flag " << isCHODEmpty << ENDL;

	return (((nHits_V + nHits_H) > 0) && ((nHits_V + nHits_H) < nMaxSlabs));
//	return (((nHits_V == 1) && (nHits_H == 1))
//			|| ((nHits_V == 2) && (nHits_H == 1))
//			|| ((nHits_V == 1) && (nHits_H == 2)));

}

}
/* namespace na62 */

