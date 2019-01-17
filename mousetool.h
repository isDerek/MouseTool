#ifndef MOUSETOOL_H
#define MOUSETOOL_H

#include <QMainWindow>
#include "mousedebug.h"

class QFile;
namespace Ui {
class MouseTool;
}

class MouseTool : public QMainWindow
{
    Q_OBJECT

public:
    explicit MouseTool(QWidget *parent = nullptr);
    ~MouseTool();
    void initGUI();
    MouseDebug *mousedebug = new MouseDebug;

private slots:
    void on_usbDebugBtn_clicked();

private:
    QFile *qssFile;
    Ui::MouseTool *ui;
};

#endif // MOUSETOOL_H
