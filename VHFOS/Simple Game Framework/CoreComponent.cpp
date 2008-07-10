#include "CoreComponent.h"

sgfCoreComponent::sgfCoreComponent(sgfCore* core)
	:core(core)
{
}

sgfCoreComponent::~sgfCoreComponent()
{
}

sgfCore* sgfCoreComponent::getCore() const
{
	return core;
}