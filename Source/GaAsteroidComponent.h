#pragma once
#include "Psybrus.h"

//////////////////////////////////////////////////////////////////////////
// GaAsteroidComponent
class GaAsteroidComponent:
	public ScnComponent
{
public:
	REFLECTION_DECLARE_DERIVED( GaAsteroidComponent, ScnComponent );

	GaAsteroidComponent();
	virtual ~GaAsteroidComponent();

	void update( BcF32 Tick ) override;

	void onAttach( ScnEntityWeakRef Parent ) override;

	void setSize( BcF32 Size ) { Size_ = Size; }
	BcF32 getSize() const { return Size_; }

private:
	BcF32 MassSizeRatio_;
	BcF32 Size_;
	BcF32 OldSize_;
};
