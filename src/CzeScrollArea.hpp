#pragma once
#include <QWidget>
#include <qevent.h>
#include <qobject.h>

class CzeScrollBar : public QWidget
{
	Q_OBJECT
public:
	CzeScrollBar(QWidget* parent);
	void paintEvent(QPaintEvent* event);
	void mousePressEvent(QMouseEvent* event);
	void mouseMoveEvent(QMouseEvent* event);

	int maximum;
	int value;
	QPoint draggingpos;
	bool dragging = false;

signals:
	void valueChanged(int value);
};

class CzeScrollArea : public QWidget
{
	Q_OBJECT
public:

	CzeScrollArea(QWidget* parent);
	void paintEvent(QPaintEvent* event);
	void childEvent(QChildEvent* event)
	{
		//qWarning("%i", event->type());
		if (done && event->type() == QEvent::Type::ChildAdded)
		{
			if (event->child()->isWidgetType())
			{
				((QWidget*)event->child())->setParent(inner);
				return;
			}
		}
		QWidget::childEvent(event);
	}
	void setLayout(QLayout* layout)
	{
		inner->setLayout(layout);
	}
	
	void resizeEvent(QResizeEvent* event)
	{
		verticalScrollBar->setGeometry(event->size().width() - verticalScrollBar->width(), 0, verticalScrollBar->width(), event->size().height());
		verticalScrollBar->maximum = inner->height();
		QWidget::resizeEvent(event);
	}
	void onScrollChanged(int value);

	
	CzeScrollBar* verticalScrollBar;

	QWidget* inner;
	bool done = false;
};

