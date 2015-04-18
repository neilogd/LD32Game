#include "GaAsteroidComponent.h"

#include "System/Scene/Rendering/ScnDebugRenderComponent.h"
#include "System/Scene/Physics/ScnPhysicsRigidBodyComponent.h"

#include "Base/BcRandom.h"

//////////////////////////////////////////////////////////////////////////
// Define resource internals.
REFLECTION_DEFINE_DERIVED( GaAsteroidComponent );

void GaAsteroidComponent::StaticRegisterClass()
{
	/*
	ReField* Fields[] = 
	{
		new ReField( "Size_", &GaAsteroidComponent::Size_ ),
	};
	*/
	
	ReRegisterClass< GaAsteroidComponent, Super >()
		.addAttribute( new ScnComponentAttribute( 0 ) );
}

//////////////////////////////////////////////////////////////////////////
// Ctor
GaAsteroidComponent::GaAsteroidComponent()
{

}

//////////////////////////////////////////////////////////////////////////
// Dtor
//virtual
GaAsteroidComponent::~GaAsteroidComponent()
{

}

//////////////////////////////////////////////////////////////////////////
// update
void GaAsteroidComponent::update( BcF32 Tick )
{
	auto Transform = getParentEntity()->getWorldMatrix();

#if 0
	ScnDebugRenderComponent::pImpl()->drawEllipsoid( 
		Transform.translation(),
		MaVec3d( Size_, Size_, Size_ ),
		RsColour::WHITE,
		0 );
#endif

	Super::update( Tick );
}

//////////////////////////////////////////////////////////////////////////
// onAttach
void GaAsteroidComponent::onAttach( ScnEntityWeakRef Parent )
{
	Super::onAttach( Parent );
}
