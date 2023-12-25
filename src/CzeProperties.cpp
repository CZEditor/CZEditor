#include "CzeProperties.hpp"
#include <QLayout>
#include <qformlayout.h>
#include "CzeScrollArea.hpp"
#include <qsizepolicy.h>
#include "Sources.hpp"
#include "Actions.hpp"
#include "Effects.hpp"
#include <qvariant.h>

CzeProperties::CzeProperties(QWidget* parent) : CzeWindow(parent, "Properties")
{
	QVBoxLayout* filllayout = new QVBoxLayout(this);
	setLayout(filllayout);
	CzeScrollArea* scroll = new CzeScrollArea(this);
	filllayout->addWidget(scroll);
	QVBoxLayout* l = new QVBoxLayout(scroll);
	scroll->setLayout(l);
	source = new CzeParamView(scroll, nullptr, &SourcesDict);
	actions = new CzeParamViewList(scroll, nullptr, &ActionsDict);
	effects = new CzeParamViewList(scroll, nullptr, &EffectsDict);
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
	source->params = (KeyframeParam**)&keyframe->source;
	source->UpdateParams();
	actions->paramsList = (std::list<KeyframeParam*>*)(&(keyframe->actions));
	actions->UpdateParams();
	effects->paramsList = (std::list<KeyframeParam*>*)(&(keyframe->effects));
	effects->UpdateParams();
}

CzeParamViewList::CzeParamViewList(QWidget* parent, std::list<KeyframeParam*>* paramsListIn, KeyframeConstructorDict* constructorsIn) : QWidget(parent)
{
	constructors = constructorsIn;
	QVBoxLayout* vbox = new QVBoxLayout(this);
	setLayout(vbox);
	inner = new QWidget(this);
	QFormLayout* l = new QFormLayout(inner);
	inner->setLayout(l);
	vbox->addWidget(inner);
	CzeButton* plusbutton = new CzeButton(this, "+");
	list = new QComboBox(this);
	for (auto& i : *constructors)
	{
		list->addItem(i.first.c_str(), i.first.c_str());
	}
	vbox->addWidget(list);
	vbox->addWidget(plusbutton);
	connect(plusbutton, &CzeButton::pressed, this, &CzeParamViewList::AddParams);
	l->setFieldGrowthPolicy(QFormLayout::FieldsStayAtSizeHint);
	paramsList = paramsListIn;
}

void CzeParamViewList::UpdateParams()
{
	while (inner->layout()->count())
	{
		inner->layout()->removeItem(inner->layout()->itemAt(0));
	}
	for (auto& param : *paramsList)
	{
		inner->layout()->addWidget(new CzeParamView(inner, &param, constructors));
	}
}

void CzeParamViewList::AddParams()
{
	if (paramsList)
	{
		paramsList->push_back(constructors->operator[](list->currentText().toStdString())());
		UpdateParams();
	}
}

CzeParamView::CzeParamView(QWidget* parent, KeyframeParam** paramsIn, KeyframeConstructorDict* constructorsIn) : QWidget(parent)
{
	constructors = constructorsIn;
	inner = new QWidget(this);
	QFormLayout* l = new QFormLayout(inner);
	inner->setLayout(l);
	l->setFieldGrowthPolicy(QFormLayout::FieldsStayAtSizeHint);
	QVBoxLayout* vbox = new QVBoxLayout(this);
	setLayout(vbox);
	list = new QComboBox(this);
	for (auto& i : *constructors)
	{
		list->addItem(i.first.c_str(), i.first.c_str());
	}
	connect(list, &QComboBox::currentIndexChanged, this, &CzeParamView::ChangeParams);
	vbox->addWidget(list);
	vbox->addWidget(inner);

	params = paramsIn;
	if (params)
	{
		UpdateParams();
	}
}

void CzeParamView::UpdateParams()
{
	if (!params || !*params)
		return;
	QFormLayout* l = (QFormLayout*)inner->layout();
	int c = l->rowCount();
	for (int i = 0; i < c; i++)
	{
		l->removeRow(0);
	}
	for (auto& it : (*params)->params->elements)
	{
		QWidget* w = it.second->Widget(inner);
		l->addRow(new CzeLabel(this, it.first.c_str()), w);
	}
	setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
}

void CzeParamView::ChangeParams(int index)
{
	KeyframeConstructor constr = constructors->operator[](list->itemText(index).toStdString());
	delete *params;
	*params = constr();
	UpdateParams();
}

/*

	*/