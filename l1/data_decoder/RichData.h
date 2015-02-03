 /* RichData.h
 *
 *  Created on: 29/gen/2015
 *      Author: Valeria Fascianelli
 *      Email : vxf441@hep.ph.bham.ac.uk
 */


#ifndef RICHALGORITHM_H_
#define RICHALGORITHM_H_

#include <sys/types.h>
#include <cstdint>
#include <l0/MEPFragment.h>

namespace na62 {

struct TrbDataHeader {       // Tel62 readout board header

	uint8_t flags       :8;
	uint8_t triggerType :8;
	uint8_t sourceSubID :8;  // Tel62 readout board ID
	uint8_t format      :8;

}__attribute__ ((__packed__));

struct FPGADataHeader {     // FPGA header

	uint8_t noFrame         :8;
	uint8_t noNonEmptyFrame :8;
	uint8_t FPGAID          :8;
	uint8_t errFlags        :8;

}__attribute__ ((__packed__));

struct FrameDataHeader {    // Frame header

	uint16_t coarseFrameTime :16;
	uint16_t nWordsPerFrame  :16;

}__attribute__ ((__packed__));

struct TrbData {

	uint32_t time  :19;  // hit time measurement (100ps LSB)
	uint8_t  chID  :5;   // TDC channel ID
	uint8_t  tdcID :4;   // TDC chip ID
	uint8_t  ID    :4;   // 0x4 (leading time), 0x5 (trailing time)
}__attribute__ ((__packed__));


class RichData {
public:
	RichData();
	virtual ~RichData();
	void setHits(l0::MEPFragment*);

	uint nHits;
	uint nHits_tot;
	uint nWords;
	TrbDataHeader*     rich_TrbHeader;    // array maxNTEL size
	FPGADataHeader**   rich_FPGAHeader;   // array maxNFPGA size
	FrameDataHeader*** rich_FrameHeader;  // array maxNFrame size
	TrbData**          tdc_data;

private:

};

}

#endif /* RICHALGORITHM_H_ */
