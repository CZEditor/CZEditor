#include "CzeProperties.hpp"
#include <qformlayout.h>

CzeProperties::CzeProperties(QWidget* parent) : CzeWindow(parent, "Properties")
{
	QFormLayout* l = new QFormLayout(this);
	setLayout(l);
	source = new CzeParamView(this, nullptr);
	actions = new CzeParamViewList(this, nullptr);
	effects = new CzeParamViewList(this, nullptr);
	l->addRow(source);
	l->addRow(actions);
	l->addRow(effects);
	l->setFieldGrowthPolicy(QFormLayout::FieldsStayAtSizeHint);
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