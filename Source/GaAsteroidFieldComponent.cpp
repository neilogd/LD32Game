#include "GaAsteroidFieldComponent.h"

#include "System/Scene/Rendering/ScnDebugRenderComponent.h"

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
	};
		
	ReRegisterClass< GaAsteroidFieldComponent, Super >( Fields )
		.addAttribute( new ScnComponentAttribute( 0 ) );
}

//////////////////////////////////////////////////////////////////////////
// Ctor
GaAsteroidFieldComponent::GaAsteroidFieldComponent()
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

		auto Entity = ScnCore::pImpl()->spawnEntity( 
			ScnEntitySpawnParams( 
				Template->getName().getUnique(),
				Template, 
				MaMat4d(),
				getParentEntity() )	);
		BcAssert( Entity != nullptr );
	}
	Super::onAttach( Parent );
}
