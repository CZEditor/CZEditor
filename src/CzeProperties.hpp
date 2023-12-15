#pragma once

#include "Params.hpp"
#include "CzeWindow.hpp"
#include "CzeButton.hpp"


class CzeProperties : public CzeWindow
{
public:
	CzeProperties(QWidget* parent = nullptr) : CzeWindow(parent)
	{
		(new CzeButton(this, "updateparam", [&]() {UpdateParams(); }))->move(16, 16);
		params.elements["test"] = new IntProperty(new IntData(123));
		params.elements["test2"] = new IntProperty(new IntData(14234));
		params.elements["test3"] = new IntProperty(new IntData(132375));
	}

	void UpdateParams()
	{
		qWarning("huh");
		for (auto& it : params.elements)
		{
			qWarning("%s: %ls",it.first,it.second->Serialize().data());
		}
	}

	Params params;
};