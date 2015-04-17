#include "GaSceneComponent.h"

#include "System/Scene/Rendering/ScnDebugRenderComponent.h"

//////////////////////////////////////////////////////////////////////////
// Define resource internals.
REFLECTION_DEFINE_DERIVED( GaSceneComponent );

void GaSceneComponent::StaticRegisterClass()
{
	ReRegisterClass< GaSceneComponent, Super >()
		.addAttribute( new ScnComponentAttribute( 0 ) );
}

//////////////////////////////////////////////////////////////////////////
// Ctor
GaSceneComponent::GaSceneComponent()
{

}

//////////////////////////////////////////////////////////////////////////
// Dtor
//virtual
GaSceneComponent::~GaSceneComponent()
{

}

//////////////////////////////////////////////////////////////////////////
// update
//virtual
void GaSceneComponent::update( BcF32 Tick )
{
	ScnDebugRenderComponent::pImpl()->drawGrid( 
		MaVec3d( 0.0f, 0.0f, 0.0f ),
		MaVec3d( 500.0f, 0.0f, 500.0f ),
		1.0f,
		10.0f,
		0 );
	Super::update( Tick );
}
