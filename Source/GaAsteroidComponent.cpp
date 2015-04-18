#include "GaAsteroidComponent.h"

#include "System/Scene/Rendering/ScnDebugRenderComponent.h"
#include "System/Scene/Physics/ScnPhysicsRigidBodyComponent.h"

#include "Base/BcRandom.h"

//////////////////////////////////////////////////////////////////////////
// Define resource internals.
REFLECTION_DEFINE_DERIVED( GaAsteroidComponent );

void GaAsteroidComponent::StaticRegisterClass()
{
	ReField* Fields[] = 
	{
		new ReField( "MinSize_", &GaAsteroidComponent::MinSize_, bcRFF_IMPORTER ),
		new ReField( "MaxSize_", &GaAsteroidComponent::MaxSize_, bcRFF_IMPORTER ),
		new ReField( "MinVelocity_", &GaAsteroidComponent::MinVelocity_, bcRFF_IMPORTER ),
		new ReField( "MaxVelocity_", &GaAsteroidComponent::MaxVelocity_, bcRFF_IMPORTER ),
		new ReField( "WrapDistance_", &GaAsteroidComponent::WrapDistance_, bcRFF_IMPORTER ),

		new ReField( "Size_", &GaAsteroidComponent::Size_ ),
	};
	
	ReRegisterClass< GaAsteroidComponent, Super >( Fields )
		.addAttribute( new ScnComponentAttribute( 0 ) );
}

//////////////////////////////////////////////////////////////////////////
// Ctor
GaAsteroidComponent::GaAsteroidComponent():
	MinSize_( 1.0f ),
	MaxSize_( 1.0f ),
	Size_( 1.0 )
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

	ScnDebugRenderComponent::pImpl()->drawEllipsoid( 
		Transform.translation(),
		MaVec3d( Size_, Size_, Size_ ),
		RsColour::WHITE,
		0 );

	ScnDebugRenderComponent::pImpl()->drawLine( 
		MaVec3d( -1000.0f, 0.0f, -WrapDistance_ ),
		MaVec3d(  1000.0f, 0.0f, -WrapDistance_ ),
		RsColour::GREEN,
		0 );
	ScnDebugRenderComponent::pImpl()->drawLine( 
		MaVec3d( -1000.0f, 0.0f, WrapDistance_ ),
		MaVec3d(  1000.0f, 0.0f, WrapDistance_ ),
		RsColour::GREEN,
		0 );


	Super::update( Tick );
}

//////////////////////////////////////////////////////////////////////////
// onAttach
void GaAsteroidComponent::onAttach( ScnEntityWeakRef Parent )
{
	// Set size.
	Size_ = BcRandom::Global.randRealRange( MinSize_, MaxSize_ );

	auto Velocity = BcRandom::Global.randRealRange( MinVelocity_, MaxVelocity_ );
	auto RigidBody = Parent->getComponentByType< ScnPhysicsRigidBodyComponent >();
	RigidBody->setLinearVelocity( MaVec3d( 0.0f, 0.0f, Velocity ) );

	Super::onAttach( Parent );
}
