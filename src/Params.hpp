#pragma once
#include <unordered_map>
#include "Property.hpp"

class Params
{
public:
	std::unordered_map<std::string, Property*> elements;
};