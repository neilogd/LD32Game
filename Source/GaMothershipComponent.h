#pragma once
#include "Psybrus.h"
#include "Math/MaQuat.h"

//////////////////////////////////////////////////////////////////////////
// GaMothershipComponent
class GaMothershipComponent:
	public ScnComponent,
	public ReIObjectNotify
{
public:
	REFLECTION_DECLARE_DERIVED( GaMothershipComponent, ScnComponent );

	GaMothershipComponent();
	virtual ~GaMothershipComponent();

	void update( BcF32 Tick ) override;

	void onAttach( ScnEntityWeakRef Parent ) override;

	void onObjectDeleted( class ReObject* Object ) override;

	void addResources( BcF32 Resources );
	BcBool subResources( BcF32 Resources );
	BcF32 getResources() const { return TotalResources_; }

private:
	class ScnEntity* MinerEntity_;
	BcF32 TotalResources_;

	MaVec3d TargetPosition_;	
	MaQuat TargetRotation_;

	std::vector< class GaAsteroidComponent* > Asteroids_;
	std::vector< class GaMinerComponent* > Miners_;

};
