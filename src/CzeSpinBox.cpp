#include "CzeSpinBox.hpp"
#include "global.hpp"
#include <QPainter>
#include <qstyle.h>
#include <QLineEdit>

class CzeSpinTextbox : public QLineEdit
{
public:
	CzeSpinTextbox(QWidget* parent) : QLineEdit(parent)
	{
		setFont(QFont("Segoe UI", 9));
		//setFixedHeight(24);
	}
	void paintEvent(QPaintEvent* event);
};

void CzeSpinTextbox::paintEvent(QPaintEvent* event)
{
	QPainter qp(this);
	qp.setRenderHint(QPainter::Antialiasing);
	QFont fn("Segoe UI", 9);
	QFontMetrics fm(fn);
	QSize tsize = fm.size(0, text());
	QLinearGradient grad(0, 0, 0, height());

	grad.setColorAt(0.0, GetAccentColor(255, 128));
	grad.setColorAt(0.25, GetAccentColor(255, 32));
	grad.setColorAt(0.3, GetAccentColor(255, 10));
	grad.setColorAt(0.4, GetAccentColor(255, 0));
	grad.setColorAt(0.6, GetAccentColor(32, 32));
	grad.setColorAt(1, GetAccentColor(0, 64));
	qp.setFont(fn);
	qp.setPen(Qt::NoPen);
	qp.setBrush(grad);
	QRectF r = rect();
	qp.drawRect(QRectF(r.x() + 1.5, r.y() + 1.5, r.width() - 4, r.height() - 4));
	qp.setPen(QColor(255, 255, 255));
	qp.drawText(2, tsize.height(), text());

	if (selectionLength() > 0)
	{
		QSize beginpos = fm.size(0, text().left(selectionStart()));
		QSize endpos = fm.size(0, text().left(selectionEnd()));
		qp.setBrush(GetAccentColor(255, 255));
		qp.setPen(Qt::NoPen);
		qp.drawRect(QRect(beginpos.width() + 2, 2, endpos.width() + 2 - beginpos.width(), endpos.height() + 2));
		qp.setPen(QColor(0, 0, 0));
		qp.drawText(beginpos.width() + 2, tsize.height(), text().mid(selectionStart(), selectionLength()));

		qp.setPen(QColor(255, 255, 255));
	}
	if (hasFocus())
	{
		QSize curpos = fm.size(0, text().left(cursorPosition()));
		qp.drawLine(QLineF(curpos.width() + 2.5f, 2.5f, curpos.width() + 2.5f, curpos.height() + 4.5f));
	}
}


CzeSpinBox::CzeSpinBox(QWidget* parent) : QSpinBox(parent)
{
	setMaximum(1000);
	setMinimum(-1000);
	setLineEdit(new CzeSpinTextbox(this));
	setStyleSheet("QSpinBox::down-button{ height: 12px; width:13px; }\n"
		"QSpinBox::up-button{ height: 12px; width:13px; }\n");
	setFixedHeight(24);
}


void CzeSpinBox::paintEvent(QPaintEvent* event)
{
	QPainter qp(this);
	qp.setRenderHint(QPainter::Antialiasing);
	QFont fn("Segoe UI", 9);
	QFontMetrics fm(fn);
	QSize tsize = fm.size(0, text());
	QLinearGradient grad(0, 0, 0, height());

	grad.setColorAt(0.0, GetAccentColor(255, 128));
	grad.setColorAt(0.25, GetAccentColor(255, 32));
	grad.setColorAt(0.3, GetAccentColor(255, 10));
	grad.setColorAt(0.4, GetAccentColor(255, 0));
	grad.setColorAt(0.6, GetAccentColor(32, 32));
	grad.setColorAt(1, GetAccentColor(0, 64));
	qp.setFont(fn);
	qp.setPen(QColor(255, 255, 255));
	qp.setBrush(grad);
	QRectF r = rect();
	qp.drawRect(QRectF(0.5, 0.5, r.width() - 12, r.height() - 2));
	qp.drawPie(QRectF(r.width() - 16 + 0.5, 0.5 , 11, 22), 0, 180 * 16);
	qp.drawPie(QRectF(r.width() - 16 + 0.5, 0.5, 11, 22), 0, -180 * 16);
    static QPointF downarrow[3] = { {r.width() - 12 - 0.5,12 + 2.5},{r.width() - 12 + 4.5,12 + 2.5},{r.width() - 12 + 2,12 + 6.5} };
	qp.drawPolygon(downarrow, 3);
	static QPointF uparrow[3] = { {r.width() - 12 - 0.5,12 - 3.5},{r.width() - 12 + 4.5,12 - 3.5},{r.width() - 12 + 2,12 - 7.5} };
	qp.drawPolygon(uparrow, 3);
}
