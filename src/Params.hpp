#pragma once
#include <unordered_map>
#include "Property.hpp"

#define INIT_PARAMS(cls) cls() { params = getDefaultParams(); }

class Params
{
public:
	std::unordered_map<std::string, Property*> elements;
};

class KeyframeParam
{
public:
	virtual Params* getDefaultParams() = 0;
	Params* params;
};

typedef KeyframeParam*(*constructorPtr)();

typedef std::unordered_map<std::string, constructorPtr> KeyframeConstructorDict;

class KeyframeParamRegisterator // they call me doof
{
public:
	                                        //  +--normal function pointer which just returns a KeyframeParam object pointer
									        //  |
	                                        //  V
	KeyframeParamRegisterator(std::string name, constructorPtr constructor, KeyframeConstructorDict &globallist)
	{
		globallist[name] = constructor;
	}
};


//      +--Will make a static class which, when the program starts up, get constructed and add a function to a global list. That function just constructs a new instance of the provided class.
//      |
//      V
#define RegisterKeyframeParam(name, className, globalList) \
KeyframeParam* className##Constructor()\
{\
	return (KeyframeParam*)(new className##());\
}\
static KeyframeParamRegisterator className##Registerator(name,&className##Constructor,globalList);