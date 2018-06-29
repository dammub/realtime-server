#include "realtime_srv/common/RealtimeSrvShared.h"

using namespace realtime_srv;

bool Action::Write( OutputBitStream& inOutputStream ) const
{
	mInputState.Write( inOutputStream );
	inOutputStream.Write( mTimestamp );

	return true;
}

bool Action::Read( InputBitStream& inInputStream )
{
	mInputState.Read( inInputStream );
	inInputStream.Read( mTimestamp );

	return true;
}


