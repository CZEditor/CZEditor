#include "CzeWindow.h"
#include <QWindow>

CzeWindow::CzeWindow(QWidget* parent) : QWidget(nullptr)
{
	
	setStyleSheet("#CzeWindow { border-image: url(:/CZEditor/Square Frame.png) 2; border-width: 2px;}");
	
	setWindowFlag(Qt::FramelessWindowHint);

	resizehelper = new QWidget(this);
	resizehelper->setObjectName("CzeWindow");
	titlebar = new QWidget(this);
	titlebar->setObjectName("CzeWindowTitlebar");
	titlebar->setGeometry(0, 0, 20, 20);
	titlebar->setStyleSheet("#CzeWindowTitlebar { border-image: url(:/CZEditor/Square Frame.png) 2; border-width: 2px; }");
	titlebar->setMinimumSize(128, 20);
	(corners[0][0] = new QWidget(this))->setCursor(QCursor(Qt::SizeFDiagCursor));
	(corners[1][0] = new QWidget(this))->setCursor(QCursor(Qt::SizeVerCursor));
	(corners[2][0] = new QWidget(this))->setCursor(QCursor(Qt::SizeBDiagCursor));
	(corners[0][1] = new QWidget(this))->setCursor(QCursor(Qt::SizeHorCursor));
	(corners[2][1] = new QWidget(this))->setCursor(QCursor(Qt::SizeHorCursor));
	(corners[0][2] = new QWidget(this))->setCursor(QCursor(Qt::SizeBDiagCursor));
	(corners[1][2] = new QWidget(this))->setCursor(QCursor(Qt::SizeVerCursor));
	(corners[2][2] = new QWidget(this))->setCursor(QCursor(Qt::SizeFDiagCursor));
	setMinimumSize(128, 32);
	

	show();
}
