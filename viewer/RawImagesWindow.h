/**
 * This file is part of the rgbdemo project.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Author: Nicolas Burrus <nicolas@burrus.name>, (C) 2010, 2011
 */

#ifndef RAWIMAGESWINDOW_H
#define RAWIMAGESWINDOW_H

#include <ntk/camera/calibration.h>

#include <QMainWindow>
#include <QProcess>

namespace Ui {
    class RawImagesWindow;
}

class GuiController;

class RawImagesWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit RawImagesWindow(GuiController& controller, QWidget *parent = 0);
    ~RawImagesWindow();

public:
    void update(const ntk::RGBDImage& image);
    QTimer* timer;
    QProcess* procPreprocess;
    QProcess* procPredict;

private:
    Ui::RawImagesWindow *ui;
    GuiController& m_controller;


  public slots:
    void closeEvent(QCloseEvent *event);
    void slotButtonCapture();
    void slotButtonStop();
    void slotPreprocessingComplete();
    void slotPredictComplete();

private slots:
    void on_action_GrabFrames_toggled();
    void on_action_GrabFrames_toggled_stop();
    void on_action_GrabOneFrame_triggered();
    void on_action_Quit_triggered();

    friend class GuiController;
};

#endif // RAWIMAGESWINDOW_H
