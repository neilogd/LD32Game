#pragma once
#include "Psybrus.h"
#include "System/Renderer/RsTypes.h"

//////////////////////////////////////////////////////////////////////////
// GaUnitAction
struct GaUnitAction
{
	REFLECTION_DECLARE_BASIC( GaUnitAction );

	GaUnitAction();

	std::string Name_;
	std::string Description_;
	BcU32 Icon_;
	BcU32 MouseButton_;
	EvtID ActionID_;
	std::vector< BcName > TargetClasses_;
	BcU32 Cost_;
};

//////////////////////////////////////////////////////////////////////////
// GaUnitActionEvent
struct GaUnitActionEvent : public EvtEvent< GaUnitActionEvent >
{
	BcU32 Cost_;
	class GaUnitComponent* SourceUnit_;
	class GaUnitComponent* TargetUnit_;
};

//////////////////////////////////////////////////////////////////////////
// GaUnitComponent
class GaUnitComponent:
	public ScnComponent
{
public:
	static const BcF32 RADAR_GROUND_Y;
	static const RsColour RADAR_COLOUR;
	static const RsColour RADAR_COLOUR_ATTACK;

public:
	REFLECTION_DECLARE_DERIVED( GaUnitComponent, ScnComponent );

	GaUnitComponent();
	virtual ~GaUnitComponent();

	void update( BcF32 Tick ) override;

	void setTeam( BcU32 Team ) { Team_ = Team; }
	BcU32 getTeam() const { return Team_; }
	const std::vector< GaUnitAction >& getActions() const { return Actions_; }

private:
	BcU32 Team_;
	std::vector< GaUnitAction > Actions_;
};
