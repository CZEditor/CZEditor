#include "CzeButton.hpp"


CzeButton::CzeButton(QWidget* parent, const char* text, std::function<void()> callbackIn, void* argsIn) : QToolButton(parent)
{
	callback = callbackIn;
	args = argsIn;
	connect(this, &CzeButton::pressed, this, &CzeButton::pressedEvent);
}