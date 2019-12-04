#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    virtual ~MainWindow() override;

    virtual void resizeEvent(QResizeEvent * event) override;
    virtual void moveEvent(QMoveEvent * event) override;
protected:
    virtual void closeEvent(QCloseEvent * event) override;

	virtual void setupMainMenu();
	virtual void setupCameraGui();
	virtual void setupMainDocumentArea();
	virtual void setupMeasurementArea();
	virtual void setupSystemTray();
};
#endif // MAINWINDOW_H
