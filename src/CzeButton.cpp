#include "CzeButton.hpp"


CzeButton::CzeButton(QWidget* parent, const char* text, void* dataIn) : QAbstractButton(parent)
{
	data = dataIn;
	//connect(this, &CzeButton::pressed, this, &CzeButton::pressedEvent);
	QFont fn("Segoe UI", 9);
	QFontMetrics fm(fn);
	setText(text);
	setFixedSize(fm.size(0, text)+QSize(8,6));
}