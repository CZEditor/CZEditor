#pragma once

#include <QLabel>

class CzeLabel : public QLabel
{
public:
	CzeLabel(QWidget* parent, const QString &text) : QLabel(text, parent)
	{
		setFont(QFont("Segoe UI", 9));
		setStyleSheet("color: white;");
	}
};