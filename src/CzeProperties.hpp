#pragma once

#include "Params.hpp"
#include "CzeWindow.hpp"
#include "CzeButton.hpp"
#include <QLayout>
#include "Property.hpp"
#include "CzeLabel.hpp"
#include <QComboBox>

class CzeParamView : public QWidget
{
public:
	CzeParamView(QWidget* parent, KeyframeParam* paramsIn, KeyframeConstructorDict* constructorsIn);
	void UpdateParams();

	KeyframeParam* params;
	QWidget* inner;
	QComboBox* list;
	KeyframeConstructorDict* constructors;
};

class CzeParamViewList : public QWidget
{
public:
	CzeParamViewList(QWidget* parent, std::list<KeyframeParam*>* paramsListIn, KeyframeConstructorDict* constructorsIn);
	void UpdateParams();

	std::list<KeyframeParam*>* paramsList;
	KeyframeConstructorDict* constructors;
};

class CzeProperties : public CzeWindow
{
public:
	CzeProperties(QWidget* parent = nullptr);

	void UpdateParams();

	Keyframe* keyframe = 0;
	CzeParamView* source;
	CzeParamViewList* actions;
	CzeParamViewList* effects;
};