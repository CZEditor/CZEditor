#pragma once

#include "Params.hpp"
#include "CzeWindow.hpp"
#include "CzeButton.hpp"
#include <qformlayout.h>
#include "Property.hpp"
#include "CzeLabel.hpp"

class CzeProperties : public CzeWindow
{
public:
	CzeProperties(QWidget* parent = nullptr) : CzeWindow(parent)
	{
		layout = new QFormLayout(this);
		setLayout(layout);
	}

	void UpdateParams()
	{
		if (!params)
			return;
		int c = layout->rowCount();
		for (int i = 0; i < c; i++)
		{
			layout->removeRow(0);
		}
		for (auto& it : params->elements)
		{
			QWidget* w = it.second->Widget(this);
			layout->addRow(new CzeLabel(this,it.first.c_str()), w);
		}
	}


	QFormLayout* layout;
	Params* params = 0;
};