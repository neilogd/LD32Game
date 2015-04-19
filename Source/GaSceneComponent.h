#pragma once
#include "Psybrus.h"

//////////////////////////////////////////////////////////////////////////
// GaSceneComponent
class GaSceneComponent:
	public ScnComponent
{
public:
	REFLECTION_DECLARE_DERIVED( GaSceneComponent, ScnComponent );

	GaSceneComponent();
	virtual ~GaSceneComponent(); 

	void update( BcF32 Tick ) override;

	void onAttach( ScnEntityWeakRef Parent ) override;


private:
	class ScnCanvasComponent* Canvas_;
	class ScnFontComponent* Font_;

};
