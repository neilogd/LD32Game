#include "GaUnitComponent.h"

#include "System/Content/CsCore.h"

#include "System/Scene/Rendering/ScnDebugRenderComponent.h"
#include "System/Scene/Rendering/ScnParticleSystemComponent.h"
#include "System/Scene/Sound/ScnSoundEmitter.h"

//////////////////////////////////////////////////////////////////////////
// Constants.
const BcF32 GaUnitComponent::RADAR_GROUND_Y = -2.0f;
const RsColour GaUnitComponent::RADAR_COLOUR( 0.0f, 1.0f, 0.0f, 0.5f );
const RsColour GaUnitComponent::RADAR_COLOUR_ATTACK( 1.0f, 0.0f, 0.0f, 0.5f );

//////////////////////////////////////////////////////////////////////////
// Define resource internals.
REFLECTION_DEFINE_BASIC( GaUnitAction );

void GaUnitAction::StaticRegisterClass()
{
	ReField* Fields[] =
	{
		new ReField( "Name_", &GaUnitAction::Name_ ),
		new ReField( "Description_", &GaUnitAction::Description_ ),
		new ReField( "Icon_", &GaUnitAction::Icon_ ),
		new ReField( "MouseButton_", &GaUnitAction::MouseButton_ ),
		new ReField( "ActionID_", &GaUnitAction::ActionID_ ),
		new ReField( "TargetClasses_", &GaUnitAction::TargetClasses_ ),
		new ReField( "Cost_", &GaUnitAction::Cost_ ),
	};
	
	ReRegisterClass< GaUnitAction >( Fields );
}

//////////////////////////////////////////////////////////////////////////
// Ctor
GaUnitAction::GaUnitAction():
	Name_(),
	Description_(),
	Icon_( 0 ),
	MouseButton_(),
	ActionID_( BcErrorCode ),
	Cost_( 0 ),
	TargetClasses_()
{

}

//////////////////////////////////////////////////////////////////////////
// Define resource internals.
REFLECTION_DEFINE_DERIVED( GaUnitComponent );

void GaUnitComponent::StaticRegisterClass()
{
	ReField* Fields[] = 
	{
		new ReField( "Team_", &GaUnitComponent::Team_, bcRFF_IMPORTER ),
		new ReField( "Actions_", &GaUnitComponent::Actions_, bcRFF_IMPORTER ),
		new ReField( "ShadowSize_", &GaUnitComponent::ShadowSize_, bcRFF_IMPORTER ),
	};
	
	ReRegisterClass< GaUnitComponent, Super >( Fields )
		.addAttribute( new ScnComponentAttribute( 0 ) );
}

//////////////////////////////////////////////////////////////////////////
// Ctor
GaUnitComponent::GaUnitComponent():
	Team_( 0 ),
	Actions_(),
	ShadowSize_( 1.0f ),
	ParticlesAdd_( nullptr ),
	ParticlesSub_( nullptr ),
	ShadowParticle_( nullptr )
{
	SoundEmitters_.fill( nullptr );
}

//////////////////////////////////////////////////////////////////////////
// Dtor
//virtual
GaUnitComponent::~GaUnitComponent()
{

}

//////////////////////////////////////////////////////////////////////////
// update
void GaUnitComponent::update( BcF32 Tick )
{
	auto Position = getParentEntity()->getWorldPosition();
	auto GroundPosition = MaVec3d( Position.x(), RADAR_GROUND_Y, Position.z() );

	RsColour TeamColour = getTeamColour();
	TeamColour.a( 0.5f );

	ScnDebugRenderComponent::pImpl()->drawLine( Position, GroundPosition, TeamColour, 0 );
	ScnDebugRenderComponent::pImpl()->drawCircle( GroundPosition, MaVec3d( 0.5f, 0.5f, 0.5f ), TeamColour, 0 );

	if( ShadowParticle_ )
	{
		ShadowParticle_->Position_ = getParentEntity()->getWorldPosition() * 2.0f - MaVec3d( 0.0f, ShadowSize_ / 16.0f, 0.0f );
		ShadowParticle_->CurrentTime_ = 0.0f;
		ShadowParticle_->MaxTime_ = 0.5f;
		ShadowParticle_->Alive_ = BcTrue;
	}
}

//////////////////////////////////////////////////////////////////////////
// onAttach
void GaUnitComponent::onAttach( ScnEntityWeakRef Parent )
{
	for( BcU32 Idx = 0; Idx < SoundEmitters_.size(); ++Idx )
	{
		SoundEmitters_[ Idx ] = getComponentAnyParentByType< ScnSoundEmitterComponent >( Idx );
	}

	ParticlesAdd_ = getComponentAnyParentByType< ScnParticleSystemComponent >( 0 );
	BcAssert( ParticlesAdd_ );
	ParticlesSub_ = getComponentAnyParentByType< ScnParticleSystemComponent >( 1 );
	BcAssert( ParticlesSub_ );

	setupShadow();

	Super::onAttach( Parent );
}

//////////////////////////////////////////////////////////////////////////
// setupShadow
void GaUnitComponent::setupShadow()
{
	auto Unit = getComponentByType< GaUnitComponent >();
	BcAssert( Unit );
	BcAssert( ParticlesAdd_ );
	if( ParticlesAdd_->allocParticle( ShadowParticle_ ) )
	{
		RsColour TeamColour = Unit->getTeamColour();
		TeamColour.a( 0.75f );

		ShadowParticle_->Position_ = getParentEntity()->getWorldPosition() * 2.0f - MaVec3d( 0.0f, ShadowSize_ / 16.0f, 0.0f );
		ShadowParticle_->Velocity_ = MaVec3d( 0.0f, 0.0f, 0.0f );
		ShadowParticle_->Acceleration_ = MaVec3d( 0.0f, 0.0f, 0.0f );
		ShadowParticle_->Scale_ = MaVec2d( ShadowSize_, ShadowSize_ );
		ShadowParticle_->MinScale_ = MaVec2d( ShadowSize_, ShadowSize_ );
		ShadowParticle_->MaxScale_ = MaVec2d( ShadowSize_, ShadowSize_ );
		ShadowParticle_->Rotation_ = 0.0f;
		ShadowParticle_->RotationMultiplier_ = 0.0f;
		ShadowParticle_->Colour_ = TeamColour;
		ShadowParticle_->MinColour_ = TeamColour;
		ShadowParticle_->MaxColour_ = RsColour::BLACK;
		ShadowParticle_->TextureIndex_ = 2;
		ShadowParticle_->CurrentTime_ = 0.0f;
		ShadowParticle_->MaxTime_ = 0.5f;
		ShadowParticle_->Alive_ = BcTrue;
	}
}

//////////////////////////////////////////////////////////////////////////
// getTeamColour
RsColour GaUnitComponent::getTeamColour() const
{
	switch( Team_ )
	{
	default:
		return RsColour::YELLOW;
	case 1:
		return RsColour::RED;
	case 2:
		return RsColour::BLUE;

	}
}

//////////////////////////////////////////////////////////////////////////
// playLoop
void GaUnitComponent::playSound( BcU32 Channel, const std::string Name )
{
	ScnSoundRef Sound;
	if( CsCore::pImpl()->requestResource( "sounds", Name, Sound ) )
	{
		if( SoundEmitters_[ Channel ] )
		{
			SoundEmitters_[ Channel ]->play( Sound );
		}
	}	
}

//////////////////////////////////////////////////////////////////////////
// stopLoop
void GaUnitComponent::stopSound( BcU32 Channel )
{
	if( SoundEmitters_[ Channel ] )
	{
		SoundEmitters_[ Channel ]->stopAll();
	}
}