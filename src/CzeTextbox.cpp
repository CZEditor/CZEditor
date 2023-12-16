#include "CzeTextbox.hpp"
#include "global.hpp"

void CzeTextbox::paintEvent(QPaintEvent* event)
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
	qp.drawRect(QRectF(r.x() + 1.5, r.y() + 1.5, r.width() - 3, r.height() - 3));
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
	qp.setPen(QColor(0, 0, 0));
	qp.setBrush(Qt::NoBrush);
	qp.drawRect(QRectF(r.x() + 0.5, r.y() + 0.5, r.width() - 1, r.height() - 1));


}