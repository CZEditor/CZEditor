#include "CzeViewport.hpp"
#include "CzeButton.hpp"

CzeViewport::CzeViewport(QWidget* parent) : CzeWindow(parent)
{
	SetTitle("Viewport");
	(new CzeButton(this, "LOOONG        BUTTTONNNN", [&]() {return; }, nullptr))->move(16, 20);
	(new CzeButton(this, "LONG        BUTTTON", [&]() {return; }, nullptr))->move(16, 50);
	(new CzeButton(this, "LONG      BUTTON", [&]() {return; }, nullptr))->move(16, 80);
	(new CzeButton(this, "LONG BUTTON", [&]() {return; }, nullptr))->move(16, 110);
	(new CzeButton(this, "                      \n                       ", [&]() {return; }, nullptr))->setGeometry(110, 110,64,64);

}