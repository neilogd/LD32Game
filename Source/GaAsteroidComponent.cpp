#include "GaAsteroidComponent.h"

#include "System/Scene/Rendering/ScnDebugRenderComponent.h"

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
	
	
	
	Super::update( Tick );
}

//////////////////////////////////////////////////////////////////////////
// onAttach
void GaAsteroidComponent::onAttach( ScnEntityWeakRef Parent )
{
	// Set size.
	Size_ = BcRandom::Global.randRealRange( MinSize_, MaxSize_ );
}
