#pragma once

#include <QSpinBox>

class CzeSpinBox : public QSpinBox
{
public:
	CzeSpinBox(QWidget* parent = nullptr);
	void paintEvent(QPaintEvent* event);
};

class CzeDoubleSpinBox : public QDoubleSpinBox
{
public:
	CzeDoubleSpinBox(QWidget* parent = nullptr);
	void paintEvent(QPaintEvent* event);
};