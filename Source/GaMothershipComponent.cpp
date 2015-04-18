#include "GaMothershipComponent.h"

#include "System/Scene/Rendering/ScnDebugRenderComponent.h"

//////////////////////////////////////////////////////////////////////////
// Define resource internals.
REFLECTION_DEFINE_DERIVED( GaMothershipComponent );

void GaMothershipComponent::StaticRegisterClass()
{
	ReRegisterClass< GaMothershipComponent, Super >()
		.addAttribute( new ScnComponentAttribute( 0 ) );
}

//////////////////////////////////////////////////////////////////////////
// Ctor
GaMothershipComponent::GaMothershipComponent()
{

}

//////////////////////////////////////////////////////////////////////////
// Dtor
//virtual
GaMothershipComponent::~GaMothershipComponent()
{

}

//////////////////////////////////////////////////////////////////////////
// update
//virtual
void GaMothershipComponent::update( BcF32 Tick )
{
}
