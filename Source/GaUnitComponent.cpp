#include "GaUnitComponent.h"

#include "System/Scene/Rendering/ScnDebugRenderComponent.h"

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
	};
	
	ReRegisterClass< GaUnitComponent, Super >( Fields )
		.addAttribute( new ScnComponentAttribute( 0 ) );
}

//////////////////////////////////////////////////////////////////////////
// Ctor
GaUnitComponent::GaUnitComponent():
	Team_( 0 ),
	Actions_()
{

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

	ScnDebugRenderComponent::pImpl()->drawLine( Position, GroundPosition, RADAR_COLOUR, 0 );
	ScnDebugRenderComponent::pImpl()->drawCircle( GroundPosition, MaVec3d( 0.5f, 0.5f, 0.5f ), RADAR_COLOUR, 0 );
}