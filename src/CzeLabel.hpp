#pragma once

#include <QLabel>
#include "global.hpp"

class CzeLabel : public QLabel
{
public:
	CzeLabel(QWidget* parent, const QString &text) : QLabel(text, parent)
	{
		setFont(defaultfont);
		setStyleSheet("color: white;");
	}
};