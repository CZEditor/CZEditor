#pragma once

#include "CzeWindow.h"

class CzeViewport : public CzeWindow
{
	Q_OBJECT

public:
	CzeViewport(QWidget* parent);
	QWidget* test;
};

