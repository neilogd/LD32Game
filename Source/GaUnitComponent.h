#pragma once
#include "Psybrus.h"

//////////////////////////////////////////////////////////////////////////
// GaUnitAction
struct GaUnitAction
{
	REFLECTION_DECLARE_BASIC( GaUnitAction );

	GaUnitAction(){}

	std::string Name_;
	std::string Description_;
	std::string Shortcut_;
	BcU32 ActionID_;
	std::vector< BcName > TargetClasses_;
};

//////////////////////////////////////////////////////////////////////////
// GaUnitComponent
class GaUnitComponent:
	public ScnComponent
{
public:
	REFLECTION_DECLARE_DERIVED( GaUnitComponent, ScnComponent );

	GaUnitComponent();
	virtual ~GaUnitComponent();

	BcU32 getTeam() const { return Team_; }

private:
	BcU32 Team_;
	BcBool IsSelectable_;

	std::vector< GaUnitAction > Actions_;
};
