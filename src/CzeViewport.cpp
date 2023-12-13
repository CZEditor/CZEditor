#include "CzeViewport.hpp"
#include "CzeButton.hpp"

CzeViewport::CzeViewport(QWidget* parent) : CzeWindow(parent)
{
	SetTitle("Viewport");
	test = new CzeButton(this, "YEAH", [&]() {return; }, nullptr);
	test->move(16,16);
	
}