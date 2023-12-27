#include "CzeProperties.hpp"
#include <QLayout>
#include <qformlayout.h>
#include "CzeScrollArea.hpp"
#include <qsizepolicy.h>
#include "Sources.hpp"
#include "Actions.hpp"
#include "Effects.hpp"
#include <qvariant.h>
#include <qstring.h>

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
	source->keyframe = keyframe;
	actions->paramsList = (std::list<KeyframeParam*>*)(&(keyframe->actions));
	actions->UpdateParams();
	actions->keyframe = keyframe;
	effects->paramsList = (std::list<KeyframeParam*>*)(&(keyframe->effects));
	effects->UpdateParams();
	effects->keyframe = keyframe;
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
		inner->layout()->takeAt(0)->widget()->deleteLater();
	}
	for (auto& param : *paramsList)
	{
		QWidget* holder = new QWidget(inner);
		QHBoxLayout* hbox = new QHBoxLayout(holder);
		holder->setLayout(hbox);
		CzeParamView* paramview = new CzeParamView(holder, &param, constructors);
		paramview->keyframe = keyframe;
		hbox->addWidget(paramview);
		QVBoxLayout* vbox = new QVBoxLayout(holder);
		hbox->addLayout(vbox);
		CzeButton* minusbutton = new CzeButton(holder, "-", &param);
		connect(minusbutton, &CzeButton::pressed, this, &CzeParamViewList::RemoveParams);
		inner->layout()->addWidget(holder);
	}
}

void CzeParamViewList::AddParams(void* data)
{
	if (paramsList)
	{
		paramsList->push_back(constructors->operator[](list->currentText().toStdString())(keyframe));
		UpdateParams();
	}
}

void CzeParamViewList::RemoveParams(void* data)
{
	if (!paramsList)
		return;
	KeyframeParam** params = (KeyframeParam**)data;
	paramsList->remove(*params);
	UpdateParams();
}


CzeParamView::CzeParamView(QWidget* parent, KeyframeParam** paramsIn, KeyframeConstructorDict* constructorsIn) : QWidget(parent)
{
	constructors = constructorsIn;
	inner = new QWidget(this);
	QVBoxLayout* l = new QVBoxLayout(inner);
	inner->setLayout(l);
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
	QVBoxLayout* l = (QVBoxLayout*)inner->layout();
	int c = l->count();
	for (int i = 0; i < c; i++)
	{
		l->takeAt(0)->widget()->deleteLater();
	}
	for (auto& it : (*params)->params->elements)
	{
		QWidget* w = it.second->Widget(inner);
		l->addWidget(new CzeLabel(this, it.first.c_str()));
		l->addWidget(w);
	}
	setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
}

void CzeParamView::ChangeParams(int index)
{
	KeyframeConstructor constr = constructors->operator[](list->itemText(index).toStdString());
	delete *params;
	*params = constr(keyframe);
	UpdateParams();
}

/*

	*/