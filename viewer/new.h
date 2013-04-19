#ifndef new_H
#define new_H

#include <nestk/ntk/camera/calibration.h>

#include <QMainWindow>

namespace Ui {
    class new;
}

class GuiController;

class new : public QMainWindow
{
    Q_OBJECT

public:
    explicit new(GuiController& controller, QWidget *parent = 0);
    ~new();

public:
    void update(const ntk::RGBDImage& image);

private:
    Ui::new *ui;
    GuiController& m_controller;

  public slots:
    void closeEvent(QCloseEvent *event);

private slots:
    void on_action_GrabFrames_toggled(bool );
    void on_action_Quit_triggered();
    void on_action_Stop();
    friend class GuiController;
};

#endif // new_H
