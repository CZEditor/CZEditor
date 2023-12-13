#include "CzeViewport.h"
#include "CzeButton.h"

CzeViewport::CzeViewport(QWidget* parent) : CzeWindow(parent)
{
	SetTitle("Viewport");
	test = new CzeButton(this, "YEAH", [&]() {return; }, nullptr);
	test->move(16,16);
	
}