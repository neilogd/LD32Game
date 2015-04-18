#include "GaAsteroidFieldComponent.h"
#include "GaAsteroidComponent.h"

#include "System/Scene/Rendering/ScnDebugRenderComponent.h"
#include "System/Scene/Physics/ScnPhysicsWorldComponent.h"
#include "System/Scene/Physics/ScnPhysicsRigidBodyComponent.h"
#include "System/Scene/Physics/ScnPhysicsSphereCollisionComponent.h"
#include "System/Scene/ScnCore.h"

#include "Base/BcRandom.h"

//////////////////////////////////////////////////////////////////////////
// Define resource internals.
REFLECTION_DEFINE_DERIVED( GaAsteroidFieldComponent );

void GaAsteroidFieldComponent::StaticRegisterClass()
{
	ReField* Fields[] = 
	{
		new ReField( "AsteroidTemplates_", &GaAsteroidFieldComponent::AsteroidTemplates_, bcRFF_IMPORTER ),
		new ReField( "NoofAsteroids_", &GaAsteroidFieldComponent::NoofAsteroids_, bcRFF_IMPORTER ),
		new ReField( "MinSize_", &GaAsteroidFieldComponent::MinSize_, bcRFF_IMPORTER ),
		new ReField( "MaxSize_", &GaAsteroidFieldComponent::MaxSize_, bcRFF_IMPORTER ),
		new ReField( "MinVelocity_", &GaAsteroidFieldComponent::MinVelocity_, bcRFF_IMPORTER ),
		new ReField( "MaxVelocity_", &GaAsteroidFieldComponent::MaxVelocity_, bcRFF_IMPORTER ),
		new ReField( "Width_", &GaAsteroidFieldComponent::Width_, bcRFF_IMPORTER ),
		new ReField( "Height_", &GaAsteroidFieldComponent::Height_, bcRFF_IMPORTER ),
		new ReField( "Depth_", &GaAsteroidFieldComponent::Depth_, bcRFF_IMPORTER ),
		new ReField( "Margin_", &GaAsteroidFieldComponent::Margin_, bcRFF_IMPORTER ),

		new ReField( "Asteroids_", &GaAsteroidFieldComponent::Asteroids_, bcRFF_TRANSIENT ),
	};
		
	ReRegisterClass< GaAsteroidFieldComponent, Super >( Fields )
		.addAttribute( new ScnComponentAttribute( 0 ) );
}

//////////////////////////////////////////////////////////////////////////
// Ctor
GaAsteroidFieldComponent::GaAsteroidFieldComponent():
	NoofAsteroids_( 0 ),
	MinSize_( 1.0f ),
	MaxSize_( 1.0f ),
	MinVelocity_( 1.0f ),
	MaxVelocity_( 1.0f ),
	Width_( 32.0f ),
	Height_( 32.0f ),
	Depth_( 1.0f ),
	Margin_( 1.0f )
{

}

//////////////////////////////////////////////////////////////////////////
// Dtor
//virtual
GaAsteroidFieldComponent::~GaAsteroidFieldComponent()
{

}

//////////////////////////////////////////////////////////////////////////
// update
void GaAsteroidFieldComponent::update( BcF32 Tick )
{
	ScnDebugRenderComponent::pImpl()->drawLine( 
		MaVec3d( -Width_, 0.0f, -Height_ ),
		MaVec3d(  Width_, 0.0f, -Height_ ),
		RsColour::GREEN,
		0 );
	ScnDebugRenderComponent::pImpl()->drawLine( 
		MaVec3d( -Width_, 0.0f,  Height_ ),
		MaVec3d(  Width_, 0.0f,  Height_ ),
		RsColour::GREEN,
		0 );
	ScnDebugRenderComponent::pImpl()->drawLine( 
		MaVec3d( -Width_, 0.0f, -Height_ ),
		MaVec3d( -Width_, 0.0f,  Height_ ),
		RsColour::GREEN,
		0 );
	ScnDebugRenderComponent::pImpl()->drawLine( 
		MaVec3d(  Width_, 0.0f, -Height_ ),
		MaVec3d(  Width_, 0.0f,  Height_ ),
		RsColour::GREEN,
		0 );

	ScnDebugRenderComponent::pImpl()->drawLine( 
		MaVec3d( -Width_, 0.0f, -( Height_ + Margin_ ) ),
		MaVec3d(  Width_, 0.0f, -( Height_ + Margin_ ) ),
		RsColour::RED,
		0 );
	ScnDebugRenderComponent::pImpl()->drawLine( 
		MaVec3d( -Width_, 0.0f,  ( Height_ + Margin_ ) ),
		MaVec3d(  Width_, 0.0f,  ( Height_ + Margin_ ) ),
		RsColour::RED,
		0 );

	// Check if we need to move any to other top/bottom side,
	// or constrain them to the width.
	for( auto Asteroid : Asteroids_ )
	{
		auto AsteroidComponent = Asteroid->getComponentByType< GaAsteroidComponent >();
		auto RigidBody = Asteroid->getComponentByType< ScnPhysicsRigidBodyComponent >();
		auto Position = RigidBody->getPosition();

		if( Position.z() < -( Height_ + Margin_ ) )
		{
			recycle( AsteroidComponent );
		}
		else if( Position.z() > ( Height_ + Margin_ ) )
		{
			recycle( AsteroidComponent );
		}
		else if( AsteroidComponent->getSize() < 0.1f )
		{
			recycle( AsteroidComponent );
		}
	}

	Super::update( Tick );
}

//////////////////////////////////////////////////////////////////////////
// onAttach
void GaAsteroidFieldComponent::onAttach( ScnEntityWeakRef Parent )
{
	// Spawn some.
	BcAssert( AsteroidTemplates_.size() > 0 );

	for( BcU32 Idx = 0; Idx < NoofAsteroids_; ++Idx )
	{
		size_t TemplateIdx = static_cast< size_t >( BcRandom::Global.rand() ) % AsteroidTemplates_.size();
		auto Template = AsteroidTemplates_[ TemplateIdx ];

		// Random position.
		MaMat4d Transform;

		Transform.translation(
			MaVec3d( 
				BcRandom::Global.randRealRange( -Width_, Width_ ),
				0.0f,
				BcRandom::Global.randRealRange( -Height_, Height_ )
				) );

		// Spawn new asteroid.
		auto Entity = ScnCore::pImpl()->spawnEntity( 
			ScnEntitySpawnParams( 
				Template->getName().getUnique(),
				Template, 
				Transform,
				getParentEntity() )	);
		BcAssert( Entity != nullptr );
		Asteroids_.push_back( Entity );

		// Register for an event to setup at the end of the update tick.
		ScnCore::pImpl()->subscribe( sysEVT_SYSTEM_POST_UPDATE, this,
			[ this, Entity ]( EvtID, const EvtBaseEvent& )->eEvtReturn
			{
				recycle( Entity->getComponentByType< GaAsteroidComponent >(), BcFalse );
				return evtRET_REMOVE;
			} );
	}

	Super::onAttach( Parent );
}

//////////////////////////////////////////////////////////////////////////
// onDetach
void GaAsteroidFieldComponent::onDetach( ScnEntityWeakRef Parent )
{
	Super::onDetach( Parent );
}

//////////////////////////////////////////////////////////////////////////
// recycle
void GaAsteroidFieldComponent::recycle( GaAsteroidComponent* Asteroid, BcBool SetPosition )
{
	auto RigidBody = Asteroid->getComponentByType< ScnPhysicsRigidBodyComponent >();
	auto Position = RigidBody->getPosition();
	auto Size = BcRandom::Global.randRealRange( MinSize_, MaxSize_ );
	Asteroid->setSize( Size );

	if( SetPosition )
	{
		MaVec3d NewPosition = MaVec3d(
			BcRandom::Global.randRealRange( -Width_, Width_ ),
			0.0f,
			-Height_ + 0.05f );
		RigidBody->translate( -Position + NewPosition );
	}

	const BcF32 AngularRange = 1.0f;
	RigidBody->setAngularVelocity( MaVec3d( 
		BcRandom::Global.randRealRange( -AngularRange, AngularRange ),
		BcRandom::Global.randRealRange( -AngularRange, AngularRange ),
		BcRandom::Global.randRealRange( -AngularRange, AngularRange ) ) );


	auto Velocity = BcRandom::Global.randRealRange( MinVelocity_, MaxVelocity_ );
	RigidBody->setLinearVelocity( MaVec3d( 0.0f, 0.0f, Velocity ) );
}
