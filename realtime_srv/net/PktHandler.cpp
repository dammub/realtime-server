#ifdef __linux__

#include <realtime_srv/net/PktHandler.h>

using namespace realtime_srv;
using namespace muduo;


PktHandler::PktHandler( ReceivedPacketBlockQueue* const inRecvPktBQ,
	PktProcessCallback pktProcessCallback )
	:
	recvedPktBQ_( inRecvPktBQ ),
	pktHandleThread_(
		std::bind( &PktHandler::ProcessPkt, this, pktProcessCallback ),
		"rs_pkt_handler" )
{}

void PktHandler::ProcessPkt( PktProcessCallback pktProcessCb )
{
	const int wakeupIntervalSec = 6;
	const int MicroSecsPerSec = 1000 * 1000;
	const int64_t waitTimeOut = static_cast< int64_t >(
		wakeupIntervalSec * MicroSecsPerSec );

	///// inefficient mode 
	//ReceivedPacket recvedPacket;
	//while ( true )
	//{
	//	if ( recvedPktBQ_->wait_dequeue_timed( recvedPacket, waitTimeOut ) )
	//	{
	//		pktProcessCallback( recvedPacket );
	//	}
	//	else
	//	{
	//		recvedPacket = ReceivedPacket(); // release the last one, but keep the memory
	//	}
	//	DoPendingFuncs();
	//}


	///// efficient mode
	size_t cnt = 0;
	// correct way below
	std::vector< ReceivedPacket > recvedPackets( kMaxPacketsCountPerRound );
	// wrong way below : concurrentQueue will not release the last group
	//std::vector< ReceivedPacket > recvedPackets;
	//recvedPackets.reserve( kMaxPacketsCountPerRound );

	while ( true )
	{
		cnt = recvedPktBQ_->wait_dequeue_bulk_timed(
			recvedPackets.begin(), kMaxPacketsCountPerRound, waitTimeOut );

		DoPendingFuncs();

		if ( cnt == 0 ) // timeout condition, release the last group, but keep the memory
		{ for ( auto& rp : recvedPackets ) rp = ReceivedPacket(); } 
		else
		{ for ( size_t i = 0; i != cnt; ++i ) pktProcessCb( recvedPackets[i] ); }
	}

}

void PktHandler::DoPendingFuncs()
{
	std::vector<PendingFunc> tempPendingFuncs;
	{
		MutexLockGuard lock( mutex_ );
		tempPendingFuncs.swap( pendingFuncs_ );
	}
	for ( size_t i = 0; i < tempPendingFuncs.size(); ++i )
	{
		tempPendingFuncs[i]();
	}
}

void realtime_srv::PktHandler::AppendToPendingFuncs( PendingFunc func )
{
	MutexLockGuard lock( mutex_ );
	pendingFuncs_.push_back( std::move( func ) );
}

#endif // __linux__