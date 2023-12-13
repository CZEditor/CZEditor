#pragma once

#include <QtWidgets/QMainWindow>
#include <QtWidgets/QSplitter>
#include "CzeViewport.h"

class CZEditor : public QMainWindow
{
    Q_OBJECT

public:
    CZEditor(QWidget *parent = nullptr);
    ~CZEditor();
private:
    QSplitter* hozsplitter;
    QSplitter* vertsplitter;
    CzeViewport* viewport;
};
