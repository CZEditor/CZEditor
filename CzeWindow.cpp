#include "CzeWindow.hpp"
#include <QWindow>
#include <QLayout>
#include <QtWidgets/QToolButton>

class CzeWindowLayout : public QLayout
{
public:
	CzeWindowLayout(QWidget* parent) : QLayout(parent)
	{
		inner = new QWidget();
		addWidget(inner);
	}

	QWidget* inner;
};

class CzeCloseButton : public QAbstractButton
{
public:
	CzeCloseButton(QWidget* parent) : QAbstractButton(parent)
	{
		setFixedSize(18, 18);
		
		setAttribute(Qt::WA_NoMousePropagation);
	}

	void paintEvent(QPaintEvent* event)
	{
		QPainter qp(this);
		QRadialGradient grad(width()/2,height()*2,height()*3);
		qp.setRenderHint(QPainter::Antialiasing);
		qp.setRenderHint(QPainter::TextAntialiasing);
		if (held)
		{
			grad.setColorAt(0.3, GetAccentColor(192, 192));
			grad.setColorAt(0.45, GetAccentColor(255, 64));
			grad.setColorAt(0.5, GetAccentColor(255, 32));
			grad.setColorAt(0.52, GetAccentColor(50, 64));
			grad.setColorAt(0.66, GetAccentColor(50, 128));
			qp.setPen(QColor(255, 255, 255));
			qp.setBrush(grad);
		}
		else if (hovered)
		{
			grad.setColorAt(0.3, GetAccentColor(192, 255));
			grad.setColorAt(0.45, GetAccentColor(255, 128));
			grad.setColorAt(0.5, GetAccentColor(255, 92));
			grad.setColorAt(0.52, GetAccentColor(50, 128));
			grad.setColorAt(0.66, GetAccentColor(50, 255));
			qp.setPen(QColor(255, 255, 255));
			qp.setBrush(grad);
		}
		else
		{
			//grad.setColorAt(0.3, GetAccentColor(192, 255));
			grad.setColorAt(0.45, GetAccentColor(32, 32));
			grad.setColorAt(0.5, GetAccentColor(32, 16));
			grad.setColorAt(0.52, GetAccentColor(12, 92));
			grad.setColorAt(0.66, GetAccentColor(12, 224));
			qp.setPen(QColor(255, 255, 255));
			qp.setBrush(grad);
			
		}

		qp.drawRoundedRect(QRectF(1.5, 1.5, ((float)width()) - 3.0, ((float)height()) - 3.0), 4, 4);
		qp.setBrush(Qt::NoBrush);
		qp.setPen(QColor(0, 0, 0));
		qp.drawRoundedRect(QRectF(0.5, 0.5, ((float)width()) - 1.0, ((float)height()) - 1.0), 4, 4);
		qp.setPen(QPen(QColor(0, 0, 0),4));
		qp.drawLine(7, 6, width() - 7, height() - 6);
		qp.drawLine(width() - 7, 6, 7, height() - 6);
		qp.setPen(QPen(QColor(255, 255, 255), 2));
		qp.drawLine(7, 6, width() - 7, height() - 6);
		qp.drawLine(width() - 7, 6, 7, height() - 6);
		
		
		event->accept();
	}

	void enterEvent(QEnterEvent* event)
	{
		hovered = true;
		repaint();
		QWidget::enterEvent(event);
	}
	
	void leaveEvent(QEvent* event)
	{
		hovered = false;
		repaint();
		QWidget::leaveEvent(event);
	}

	void mousePressEvent(QMouseEvent* event)
	{
		held = true;
		repaint();
		QWidget::mousePressEvent(event);
	}

	void mouseReleaseEvent(QMouseEvent* event)
	{
		held = false;
		repaint();
		QWidget::mouseReleaseEvent(event);
		parentWidget()->close();
	}

	

	bool hovered = false;
	bool held = false;
};

CzeWindow::CzeWindow(QWidget* parent, const char* title) : QWidget(nullptr)
{
	
	//setStyleSheet("#CzeWindow { border-image: url(:/CZEditor/Rounded Frame.png) 6; border-width: 6px;}");
	
	setWindowFlag(Qt::FramelessWindowHint);
	setAttribute(Qt::WA_NoSystemBackground, true);
	setAttribute(Qt::WA_TranslucentBackground, true);
	show();
	resizehelper = new QWidget(this);
	resizehelper->setObjectName("CzeWindow");
	titlebar = new QWidget(this);
	titlebar->setObjectName("CzeWindowTitlebar");
	titlebar->setGeometry(0, 0, 128, 24);
	//titlebar->setStyleSheet("#CzeWindowTitlebar { border-image: url(:/CZEditor/Rounded Frame.png) 6; border-width: 6px; }");
	titlebar->setMinimumSize(128, 24);
	inner = new QWidget(this);
	(corners[0][0] = new QWidget(this))->setCursor(QCursor(Qt::SizeFDiagCursor));
	(corners[1][0] = new QWidget(this))->setCursor(QCursor(Qt::SizeVerCursor));
	(corners[2][0] = new QWidget(this))->setCursor(QCursor(Qt::SizeBDiagCursor));
	(corners[0][1] = new QWidget(this))->setCursor(QCursor(Qt::SizeHorCursor));
	(corners[2][1] = new QWidget(this))->setCursor(QCursor(Qt::SizeHorCursor));
	(corners[0][2] = new QWidget(this))->setCursor(QCursor(Qt::SizeBDiagCursor));
	(corners[1][2] = new QWidget(this))->setCursor(QCursor(Qt::SizeVerCursor));
	(corners[2][2] = new QWidget(this))->setCursor(QCursor(Qt::SizeFDiagCursor));
	closebutton = new CzeCloseButton(this);
	SetTitle(title);
	setMinimumSize(128, 32);
	done = true;
	resize(200, 200);
	
}
