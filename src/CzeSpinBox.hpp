#pragma once

#include <QSpinBox>

class CzeSpinBox : public QSpinBox
{
public:
	CzeSpinBox(QWidget* parent = nullptr);
	void paintEvent(QPaintEvent* event);
};