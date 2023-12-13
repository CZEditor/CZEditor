#pragma once
#include <QtWidgets/QToolButton.h>
#include <functional>

class CzeButton : public QToolButton
{
	Q_OBJECT

public:
	CzeButton(QWidget* parent, const char* text, std::function<void()> callbackIn, void* argsIn);
	void pressedEvent()
	{
		callback();
		printf("clicked\n");
	}
	std::function<void()> callback;
	void* args;
};

