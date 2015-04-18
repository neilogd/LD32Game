#include "GaAsteroidComponent.h"

#include "System/Scene/Rendering/ScnDebugRenderComponent.h"
#include "System/Scene/Physics/ScnPhysicsRigidBodyComponent.h"
#include "System/Scene/Physics/ScnPhysicsCollisionComponent.h"

#include "Base/BcRandom.h"

//////////////////////////////////////////////////////////////////////////
// Define resource internals.
REFLECTION_DEFINE_DERIVED( GaAsteroidComponent );

void GaAsteroidComponent::StaticRegisterClass()
{
	ReField* Fields[] = 
	{
		new ReField( "MassSizeRatio_", &GaAsteroidComponent::MassSizeRatio_, bcRFF_IMPORTER ),
		new ReField( "Size_", &GaAsteroidComponent::Size_, bcRFF_IMPORTER ),
	};
	
	ReRegisterClass< GaAsteroidComponent, Super >( Fields )
		.addAttribute( new ScnComponentAttribute( 0 ) );
}

//////////////////////////////////////////////////////////////////////////
// Ctor
GaAsteroidComponent::GaAsteroidComponent():
	MassSizeRatio_( 1.0f ),
	Size_( 1.0f ),
	OldSize_( 0.0f )
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
	if( Size_ > 0.0f )
	{
		if( Size_ != OldSize_ )
		{
			auto CollisionComponent = getComponentByType< ScnPhysicsCollisionComponent >();
			BcAssert( CollisionComponent );
			CollisionComponent->setLocalScaling( MaVec3d( Size_, Size_, Size_ ) );
			OldSize_ = Size_;
		}
	}
}

//////////////////////////////////////////////////////////////////////////
// onAttach
void GaAsteroidComponent::onAttach( ScnEntityWeakRef Parent )
{
	Super::onAttach( Parent );
}
