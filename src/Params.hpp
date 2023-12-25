#pragma once
#include <unordered_map>
#include "Property.hpp"

#define INIT_PARAMS(cls,paramname) cls() { params = getDefaultParams(); } \
virtual const char* name() \
{\
return paramname;\
}

class Params
{
public:
	std::unordered_map<std::string, Property*> elements;
};

class KeyframeParam
{
public:
	virtual Params* getDefaultParams() = 0;
	virtual const char* name() = 0;
	Params* params;
};