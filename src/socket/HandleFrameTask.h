/*
 * HandleFrameTask.h
 *
 *  Created on: Jun 27, 2014
 *      Author: root
 */

#ifndef HANDLEFRAMETASK_H_
#define HANDLEFRAMETASK_H_

#include <tbb/task.h>

#include <socket/EthernetUtils.h>
#include <boost/timer/timer.hpp>

namespace na62 {

class HandleFrameTask: public tbb::task {
private:
	DataContainer container;

	void processARPRequest(struct ARP_HDR* arp);

	/**
	 * @return <true> If no checksum errors have been found
	 */
	bool checkFrame(struct UDP_HDR* hdr, uint16_t length);

	static uint16_t L0_Port;
	static uint16_t CREAM_Port;
	static uint32_t MyIP;

	/*
	 * Store the current Burst ID and the next one separately. As soon as an EOB event is
	 * received the nextBurstID_ will be set. Then the currentBurstID will be updated later
	 * to make sure currently enqueued frames in other threads are not processed with
	 * the new burstID
	 */
	static uint32_t currentBurstID_;
	static uint32_t nextBurstID_;
	static boost::timer::cpu_timer eobFrameReceivedTime_;
public:
	HandleFrameTask(DataContainer&& _container);
	virtual ~HandleFrameTask();

	tbb::task* execute();

	static void Initialize();

	static uint32_t getCurrentBurstId() {
		return currentBurstID_;
	}

	static uint32_t getNextBurstId() {
		return nextBurstID_;
	}

	static void setNextBurstId(uint32_t burstID) {
		nextBurstID_ = burstID;
		eobFrameReceivedTime_.start();
	}
};

} /* namespace na62 */

#endif /* HANDLEFRAMETASK_H_ */
