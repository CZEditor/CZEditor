#pragma once

#include <QtWidgets/QMainWindow>
#include <QtWidgets/QSplitter>
#include "CzeViewport.hpp"
#include "CzeWindow.hpp"
#include "Property.hpp"
#include "ICZEditor.hpp"
#include "CzeProperties.hpp"

class CZEditor :  public CzeWindow, public ICZEditor
{
    Q_OBJECT

public:
    CZEditor(QWidget *parent = nullptr);
    void SelectParams(Params* params);
    ~CZEditor();
private:
    QSplitter* hozsplitter;
    QSplitter* vertsplitter;
    CzeViewport* viewport;
    CzeProperties* propertyWidget;
};
