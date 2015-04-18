#include "GaTeamMemberComponent.h"

//////////////////////////////////////////////////////////////////////////
// Define resource internals.
REFLECTION_DEFINE_DERIVED( GaTeamMemberComponent );

void GaTeamMemberComponent::StaticRegisterClass()
{
	ReField* Fields[] = 
	{
		new ReField( "Team_", &GaTeamMemberComponent::Team_ ),
	};
	
	ReRegisterClass< GaTeamMemberComponent, Super >( Fields )
		.addAttribute( new ScnComponentAttribute( 0 ) );
}

//////////////////////////////////////////////////////////////////////////
// Ctor
GaTeamMemberComponent::GaTeamMemberComponent():
	Team_( BcErrorCode )
{

}

//////////////////////////////////////////////////////////////////////////
// Dtor
//virtual
GaTeamMemberComponent::~GaTeamMemberComponent()
{

}
