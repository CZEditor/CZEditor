#pragma once
#include <unordered_map>
#include "Property.hpp"
#include <xstring>

class Params
{
public:
	std::unordered_map<std::string, Property*> elements;
};