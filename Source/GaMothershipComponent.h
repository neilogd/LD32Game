#pragma once
#include "Psybrus.h"
#include "Math/MaQuat.h"

//////////////////////////////////////////////////////////////////////////
// GaMothershipComponent
class GaMothershipComponent:
	public ScnComponent
{
public:
	REFLECTION_DECLARE_DERIVED( GaMothershipComponent, ScnComponent );

	GaMothershipComponent();
	virtual ~GaMothershipComponent();

	void update( BcF32 Tick ) override;

	void onAttach( ScnEntityWeakRef Parent ) override;

	void addResources( BcF32 Resources );
	BcBool subResources( BcF32 Resources );
	

private:
	class ScnEntity* MinerEntity_;
	BcF32 TotalResources_;

	MaVec3d TargetPosition_;	
	MaQuat TargetRotation_;

};
