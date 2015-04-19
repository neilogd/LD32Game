#pragma once
#include "Psybrus.h"

//////////////////////////////////////////////////////////////////////////
// GaIntroComponent
class GaIntroComponent:
	public ScnComponent
{
public:
	REFLECTION_DECLARE_DERIVED( GaIntroComponent, ScnComponent );

	GaIntroComponent();
	virtual ~GaIntroComponent();

	void update( BcF32 Tick ) override;

	void onAttach( ScnEntityWeakRef Parent ) override;
	void onDetach( ScnEntityWeakRef Parent ) override;

private:
	class ScnCanvasComponent* Canvas_;
	class ScnFontComponent* Font_;

	BcF32 Timer_;

};
