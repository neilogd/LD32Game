#pragma once
#include "Psybrus.h"

//////////////////////////////////////////////////////////////////////////
// GaMinerComponent
class GaMinerComponent:
	public ScnComponent
{
public:
	REFLECTION_DECLARE_DERIVED( GaMinerComponent, ScnComponent );

	GaMinerComponent();
	virtual ~GaMinerComponent();

	void update( BcF32 Tick ) override;

	void onAttach( ScnEntityWeakRef Parent ) override;
	void onDetach( ScnEntityWeakRef Parent ) override;

private:
	BcF32 MaxVelocity_;
	BcF32 MaxForce_;
	BcF32 MiningDistance_;
	BcF32 MiningRate_;
	BcF32 MiningSizeThreshold_;

	enum class State
	{
		IDLE,
		MINING,
		ACCIDENTING,
		RETURNING,
	};

	State State_;

	BcF32 CirclingTimer_;
	MaVec3d TargetPosition_;
	class GaUnitComponent* Target_;
};
