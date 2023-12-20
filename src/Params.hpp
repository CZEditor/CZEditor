#pragma once
#include <unordered_map>
#include "Property.hpp"
#include <xstring>

#define INIT_PARAMS(cls) cls() { params = getDefaultParams(); }

class Params
{
public:
	std::unordered_map<std::string, Property*> elements;
};