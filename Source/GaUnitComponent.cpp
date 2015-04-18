#include "GaUnitComponent.h"

//////////////////////////////////////////////////////////////////////////
// Define resource internals.
REFLECTION_DEFINE_BASIC( GaUnitAction );

void GaUnitAction::StaticRegisterClass()
{
	ReField* Fields[] =
	{
		new ReField( "Name_", &GaUnitAction::Name_ ),
		new ReField( "Description_", &GaUnitAction::Description_ ),
		new ReField( "Shortcut_", &GaUnitAction::Shortcut_ ),
		new ReField( "Cost_", &GaUnitAction::Cost_ ),
		new ReField( "ActionID_", &GaUnitAction::ActionID_ ),
		new ReField( "TargetClasses_", &GaUnitAction::TargetClasses_ ),
	};
	
	ReRegisterClass< GaUnitAction >( Fields );
}

//////////////////////////////////////////////////////////////////////////
// Ctor
GaUnitAction::GaUnitAction():
	Name_(),
	Description_(),
	Shortcut_(),
	Cost_( 0 ),
	ActionID_( BcErrorCode ),
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
