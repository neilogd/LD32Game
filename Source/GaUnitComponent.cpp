#include "GaUnitComponent.h"

#include "System/Scene/Rendering/ScnDebugRenderComponent.h"

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
	auto GroundPosition = MaVec3d( Position.x(), -2.0f, Position.z() );

	ScnDebugRenderComponent::pImpl()->drawLine( Position, GroundPosition, RsColour( 0.0f, 1.0f, 0.0f, 0.5f ), 0 );
	ScnDebugRenderComponent::pImpl()->drawCircle( GroundPosition, MaVec3d( 0.5f, 0.5f, 0.5f ), RsColour( 0.0f, 1.0f, 0.0f, 0.5f ), 0 );
}