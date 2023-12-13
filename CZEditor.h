#pragma once

#include <QtWidgets/QMainWindow>
#include <QtWidgets/QSplitter>
#include "CzeViewport.h"
#include "CzeWindow.h"

class CZEditor : public CzeWindow
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
