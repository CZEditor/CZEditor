#include "CzeWindow.hpp"
#include <QtGui/QWindow>
#include <QtWidgets/QLayout>
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

class CzeAbstractTitlebarButton : public QAbstractButton
{
public:
	CzeAbstractTitlebarButton(QWidget* parent) : QAbstractButton(parent)
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
		paintButton(qp);
		qp.setPen(QPen(QColor(255, 255, 255), 2));
		paintButton(qp);

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
		
		buttonAction();
	}

	virtual void paintButton(QPainter& qp) = 0;
	virtual void buttonAction() = 0;

	bool hovered = false;
	bool held = false;
};

class CzeCloseButton : public CzeAbstractTitlebarButton
{
public:
	CzeCloseButton(QWidget* parent) : CzeAbstractTitlebarButton(parent) { }

	void paintButton(QPainter& qp)
	{
		qp.drawLine(7, 6, width() - 7, height() - 6);
		qp.drawLine(width() - 7, 6, 7, height() - 6);
	}


	void buttonAction()
	{
		parentWidget()->close();
	}
};

class CzeMaximizeButton : public CzeAbstractTitlebarButton
{
public:
	CzeMaximizeButton(QWidget* parent) : CzeAbstractTitlebarButton(parent) { }

	void paintButton(QPainter& qp)
	{
		if (parentWidget()->windowState() & Qt::WindowMaximized) {
			qp.drawRect(QRectF(width() / 2 - 3.5 + 1.5, height() / 2 - 3.5 + 1.5, 7, 7));
			qp.drawRect(QRectF(width() / 2 - 3.5 - 1.5, height() / 2 - 3.5 - 1.5, 7, 7));
		} else {
			qp.drawRect(6, 6, width() - 6 * 2, height() - 6 * 2);
			qp.drawRect(6, 8, width() - 6 * 2, height() - 6 * 2 - 2);
		}
	}


	void buttonAction()
	{
		if (parentWidget()->windowState() & Qt::WindowMaximized) {
			parentWidget()->showNormal();
		} else {
			parentWidget()->showMaximized();
		}
	}
};

CzeWindow::CzeWindow(QWidget* parent, const char* title) : QWidget(nullptr)
{
	init(title, new QWidget(this));
}

CzeWindow::CzeWindow(QWidget* parent, QWidget* innerIn) : QWidget(nullptr)
{
	init("", innerIn);
}

void CzeWindow::init(const char* title, QWidget* innerIn)
{
	setWindowFlag(Qt::FramelessWindowHint);
	setAttribute(Qt::WA_NoSystemBackground, true);
	setAttribute(Qt::WA_TranslucentBackground, true);
	show();
	resizehelper = new QWidget(this);
	titlebar = new QWidget(this);
	titlebar->setGeometry(0, 0, 128, 24);
	titlebar->setMinimumSize(128, 24);
	inner = innerIn;
	(corners[0][0] = new QWidget(this))->setCursor(QCursor(Qt::SizeFDiagCursor));
	(corners[1][0] = new QWidget(this))->setCursor(QCursor(Qt::SizeVerCursor));
	(corners[2][0] = new QWidget(this))->setCursor(QCursor(Qt::SizeBDiagCursor));
	(corners[0][1] = new QWidget(this))->setCursor(QCursor(Qt::SizeHorCursor));
	(corners[2][1] = new QWidget(this))->setCursor(QCursor(Qt::SizeHorCursor));
	(corners[0][2] = new QWidget(this))->setCursor(QCursor(Qt::SizeBDiagCursor));
	(corners[1][2] = new QWidget(this))->setCursor(QCursor(Qt::SizeVerCursor));
	(corners[2][2] = new QWidget(this))->setCursor(QCursor(Qt::SizeFDiagCursor));
	closebutton = new CzeCloseButton(this);
	maximizebutton = new CzeMaximizeButton(this);
	SetTitle(title);
	setMinimumSize(128, 32);
	done = true;
	resize(200, 200);
}
