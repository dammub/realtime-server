// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Pawn.h"
#include "MemoryBitStream.h"
#include "ActionMath.h"
#include <memory>
#include "ActionEntity.generated.h"






UCLASS()
class AActionEntity : public APawn
{
	GENERATED_BODY()

public:
	// 'GOBJ' = 1196376650;
	virtual uint32_t GetClassId() const /*{ return 'GOBJ'; }*/ ;
	//virtual uint32_t GetClassId() const { return 1196376650; }

	enum ECatReplicationState
	{
		ECRS_Pose = 1 << 0,
		ECRS_Color = 1 << 1,
		ECRS_PlayerId = 1 << 2,
		ECRS_Health = 1 << 3,

		ECRS_AllState = ECRS_Pose | ECRS_Color | ECRS_PlayerId | ECRS_Health
	};

public:
	virtual void	Update() {}

	virtual uint32_t	Write( OutputMemoryBitStream& inOutputStream, uint32_t inDirtyState ) const { return 0; }
	virtual void		Read( InputMemoryBitStream& inInputStream ) {}


	void		SetPlayerId( uint32_t inPlayerId ) { mPlayerId = inPlayerId; }
	uint32_t	GetPlayerId()						const { return mPlayerId; }

	void			SetActionEntityVelocity( const FVector& inVelocity ) { Velocity = inVelocity; }
	const FVector&	GetActionEntityVelocity()						const { return Velocity; }


	//const FVector&		GetLocation()				const { return mLocation; }
	//void		SetLocation( const FVector& inLocation ) { mLocation = inLocation; }

	//void	SetRotation( float inRotation ) { mRotation = inRotation; };
	//float	GetRotation()					const { return mRotation; }

	//void		SetColor( const FVector& inColor ) { mColor = inColor; }
	//const FVector&		GetColor()					const { return mColor; }

	int			GetNetworkId()				const { return mNetworkId; }
	void		SetNetworkId( int inNetworkId ) { mNetworkId = inNetworkId;  }

protected:
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = ActionPawnMovement )
		FVector Velocity;

private:


	//FVector				mVelocity;
	FVector											mLocation;
	FVector											mColor;

	float											mRotation;
	float											mScale;
	int												mIndexInWorld;

	bool											mDoesWantToDie;

	int												mNetworkId;

	//float				mMaxLinearSpeed;
	//float				mMaxRotationSpeed;

	////bounce fraction when hitting various things
	//float				mWallRestitution;
	//float				mCatRestitution;


	uint32_t			mPlayerId;
protected:

	///move down here for padding reasons...

	float				mLastMoveTimestamp;

	//float				mThrustDir;
	int					mHealth;

	bool				mIsShooting;
};


typedef std::shared_ptr< AActionEntity >	GameObjectPtr;