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
    void SelectKeyframe(Keyframe* keyframe);
    ~CZEditor();
    void timerEvent(QTimerEvent* event);
    void keyPressEvent(QKeyEvent* event);
private:
    QSplitter* hozsplitter;
    QSplitter* vertsplitter;
    CzeViewport* viewport;
    CzeProperties* propertyWidget;
};
