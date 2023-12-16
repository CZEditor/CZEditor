#pragma once

#include <QtWidgets/QLineEdit>
#include <QtGui/QPainter>

class CzeTextbox : public QLineEdit
{
public:
	CzeTextbox(QWidget* parent) : QLineEdit(parent)
	{
		setFont(QFont("Segoe UI", 9));
		setFixedHeight(24);
	}
	void paintEvent(QPaintEvent* event);
};