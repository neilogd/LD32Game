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
		new ReField( "ActionID_", &GaUnitAction::ActionID_ ),
		new ReField( "TargetClasses_", &GaUnitAction::TargetClasses_ ),
	};
	
	ReRegisterClass< GaUnitAction >( Fields );
}

//////////////////////////////////////////////////////////////////////////
// Define resource internals.
REFLECTION_DEFINE_DERIVED( GaUnitComponent );

void GaUnitComponent::StaticRegisterClass()
{
	ReField* Fields[] = 
	{
		new ReField( "Team_", &GaUnitComponent::Team_ )
	};
	
	ReRegisterClass< GaUnitComponent, Super >( Fields )
		.addAttribute( new ScnComponentAttribute( 0 ) );
}

//////////////////////////////////////////////////////////////////////////
// Ctor
GaUnitComponent::GaUnitComponent():
	Team_( 0 )
{

}

//////////////////////////////////////////////////////////////////////////
// Dtor
//virtual
GaUnitComponent::~GaUnitComponent()
{

}
