#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>

#include "rheedCore.h"
#include "./QCommandWidget.h"

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    MainWindow(rheedCore *core, QWidget *parent = nullptr);
    virtual ~MainWindow() override;

    virtual void resizeEvent(QResizeEvent * event) override;
    virtual void moveEvent(QMoveEvent * event) override;
protected:
    virtual void closeEvent(QCloseEvent * event) override;

	virtual void setupMainMenu();
	virtual void setupDeviceGui();
	virtual void setupMainDocumentArea();
	virtual void setupMeasurementArea();
	virtual void setupSystemTray();

    static void redrawGuiRequet(S_command* pwhat, void* pThis, I_CommonBase *pSource);
    virtual QWidget* createCameraGUI(RCI::I_Camera * pCamera);
private:
    rheedCore* m_pCore = nullptr;
    std::map<I_CommonBase*, QDeviceWidget*> mapDeviceWidgets;
};
#endif // MAINWINDOW_H
