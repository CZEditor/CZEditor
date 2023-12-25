#include "CzeProperties.hpp"
#include <QLayout>
#include <qformlayout.h>
#include "CzeScrollArea.hpp"
#include <qsizepolicy.h>

CzeProperties::CzeProperties(QWidget* parent) : CzeWindow(parent, "Properties")
{
	QVBoxLayout* filllayout = new QVBoxLayout(this);
	setLayout(filllayout);
	CzeScrollArea* scroll = new CzeScrollArea(this);
	filllayout->addWidget(scroll);
	QVBoxLayout* l = new QVBoxLayout(scroll);
	scroll->setLayout(l);
	source = new CzeParamView(scroll, nullptr);
	actions = new CzeParamViewList(scroll, nullptr);
	effects = new CzeParamViewList(scroll, nullptr);
	l->setSizeConstraint(QLayout::SetMinimumSize);
	l->addWidget(new CzeLabel(scroll,"Source"));
	l->addWidget(source);
	l->addWidget(new CzeLabel(scroll,"Actions"));
	l->addWidget(actions);
	l->addWidget(new CzeLabel(scroll,"Effects"));
	l->addWidget(effects);
}

void CzeProperties::UpdateParams()
{
	if (!keyframe)
		return;
	source->params = keyframe->source;
	source->UpdateParams();
	actions->paramsList = (std::list<KeyframeParam*>*)(&(keyframe->actions));
	actions->UpdateParams();
	effects->paramsList = (std::list<KeyframeParam*>*)(&(keyframe->effects));
	effects->UpdateParams();
}

CzeParamViewList::CzeParamViewList(QWidget* parent, std::list<KeyframeParam*>* paramsListIn) : QWidget(parent)
{
	QFormLayout* l = new QFormLayout(this);
	setLayout(l);
	l->setFieldGrowthPolicy(QFormLayout::FieldsStayAtSizeHint);
	paramsList = paramsListIn;
}

void CzeParamViewList::UpdateParams()
{
	while (layout()->count())
	{
		layout()->removeItem(layout()->itemAt(0));
	}
	for (auto& param : *paramsList)
	{
		layout()->addWidget(new CzeParamView(this, param));
	}
}

CzeParamView::CzeParamView(QWidget* parent, KeyframeParam* paramsIn) : QWidget(parent)
{
	QFormLayout* l = new QFormLayout(this);
	setLayout(l);
	l->setFieldGrowthPolicy(QFormLayout::FieldsStayAtSizeHint);
	params = paramsIn;
	if (params)
	{
		UpdateParams();
	}
}

void CzeParamView::UpdateParams()
{
	if (!params)
		return;
	QFormLayout* l = (QFormLayout*)layout();
	int c = l->rowCount();
	for (int i = 0; i < c; i++)
	{
		l->removeRow(0);
	}
	for (auto& it : params->params->elements)
	{
		QWidget* w = it.second->Widget(this);
		l->addRow(new CzeLabel(this, it.first.c_str()), w);
	}
	setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
}


/*

	*/