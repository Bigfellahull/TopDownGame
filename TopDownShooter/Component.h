#pragma once

#include <set>

typedef unsigned int ComponentType;

static const ComponentType InvalidComponentType = 0;

struct Component 
{
	static const ComponentType Type = InvalidComponentType;
};