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

#include "RawImagesWindow.h"
#include "ui_RawImagesWindow.h"
#include <QMessageBox>
#include "GuiController.h"
#include <time.h>
#include <QLabel>
#include <QStringList>
#include <QTimer>
#include <QString>
#include <QProcess>
#include <ntk/camera/rgbd_frame_recorder.h>
#include <ntk/camera/rgbd_processor.h>
#include <ntk/utils/opencv_utils.h>
#include <fstream>
#ifdef USE_FREENECT
# include <ntk/camera/kinect_grabber.h>
#endif

#include <QCloseEvent>

using namespace ntk;
int ik=0;
RawImagesWindow::RawImagesWindow(GuiController& controller, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::RawImagesWindow),
    m_controller(controller)
{

    timer=new QTimer(this);
    procPreprocess = new QProcess(this);
    procPredict = new QProcess(this);
    ui->setupUi(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(slotButtonCapture()));
    connect( procPreprocess, SIGNAL( finished(int) ), this, SLOT(slotPreprocessingComplete()) );
    connect( procPredict, SIGNAL( finished(int) ), this, SLOT(slotPredictComplete()) );
    //connect(ui->depthView, SIGNAL(mouseMoved(int,int)),
    //      &m_controller, SLOT(on_depth_mouse_moved(int,int)));

}

RawImagesWindow::~RawImagesWindow()
{
    delete ui;
}

void RawImagesWindow :: update(const ntk::RGBDImage& image)
{
    if (ui->colorView->isVisible())
        ui->colorView->setImage(image.rgb());
    if (ui->depthView->isVisible())
    {

        double min_dist = m_controller.rgbdProcessor().minDepth();
        double max_dist = m_controller.rgbdProcessor().maxDepth();
        cv::Mat1f masked_distance; image.depth().copyTo(masked_distance);
        apply_mask(masked_distance, image.depthMask());
        cv::Mat3b depth_as_color;
        compute_color_encoded_depth(masked_distance, depth_as_color, &min_dist, &max_dist);
        ui->depthView->setImage(depth_as_color);
    }

}

void RawImagesWindow::on_action_Quit_triggered()
{
    m_controller.quit();
}

void RawImagesWindow::on_action_GrabFrames_toggled()
{
    m_controller.setGrabFrames(true);
}

void RawImagesWindow::on_action_GrabOneFrame_triggered()
{

    m_controller.frameRecorder()->saveCurrentFrame(m_controller.lastImage());
}

void RawImagesWindow::on_action_GrabFrames_toggled_stop()
{
    m_controller.setGrabFrames(false);
}


void RawImagesWindow::closeEvent(QCloseEvent *event)
{
    ui->action_Quit->trigger();
    event->accept();
}

void RawImagesWindow::slotButtonCapture()
{
m_controller.resetCapturedFrames();

    ui->label_2->setText("Capturing");

    timer->setInterval(1000);
    timer->start();
    if(ik==3)
    {
        timer->stop();
        m_controller.setGrabFrames(true);
        ik=0;
    }
    else
    {
        ui->label_2->setText("Capturing starts in "+QString::number(3-ik)+"s");
        ik++;
    }


}
void RawImagesWindow::slotButtonStop()
{
    QMessageBox msgBox;
    m_controller.setGrabFrames(false);

    ui->label_2->setText("Extracting Features");

    procPreprocess->start("./main_testing");

    //system("./testing");

}

void RawImagesWindow::slotPreprocessingComplete(){
    if ( procPreprocess->exitStatus() == 0 ){
        QStringList args;
        args<<"test.txt"<<"Train_model.model"<<"out";
        ui->label_2->setText("Predicting");
        procPredict->start("./svm-predict", args);
    }
    else{
        ui->label_2->setText("Error! No person detected.");
    }
}


void RawImagesWindow::slotPredictComplete(){
    int Arr[8]={0};
    if ( procPredict->exitStatus() == 0 ){

        QFile file("out");
        if(!file.open( QIODevice::ReadOnly | QIODevice::Text ))
        {
            ui->label_2->setText("File not created");
        }
        ui->label_2->setText("");
        while(!file.atEnd())
        {
            QString line=file.readLine();
            int num=line.toInt();
            if(num==1)
            {
                ui->label_2->setText(ui->label_2->text()+ "Body Swing ");

            }
            if(num==2)
            {
               ui->label_2->setText(ui->label_2->text()+ " Looking Around ");

            }
            if(num==3)
            {
                ui->label_2->setText(ui->label_2->text()+ "Palm Action ");

            }
            if(num==4)
            {
                ui->label_2->setText(ui->label_2->text()+ "Rising Hand ");

            }
            if(num==5)
            {
                ui->label_2->setText(ui->label_2->text()+ "Hand Sweep ");

            }
            if(num==6)
            {
                ui->label_2->setText(ui->label_2->text()+ "Stretching ");

            }
            if(num==7)
            {
               ui->label_2->setText(ui->label_2->text()+ " Move Right Leg ");

            }
            if(num==8)
            {
               ui->label_2->setText(ui->label_2->text()+ " Waving ");

            }
            if(num<9 && num>0){
                Arr[num-1]++;
            }
        }
        int max=Arr[0];
        int index=1;
        for(int i=1;i<8;i++)
        {
            if(Arr[i]>max)
            {
                max=Arr[i];
                index=i+1;
            }
        }
        //ui->label_2->setText("");
        //QString temp= ui->label_2->text();
        /*
            if(Arr[0]!=0)
            {
                ui->label_2->setText(ui->label_2->text() +"Body Swing ");
            }
            if(Arr[1]!=0)
            {
                ui->label_2->setText(ui->label_2->text() +" Looking Around ");
            }
            if(Arr[2]!=0)
            {
                ui->label_2->setText(ui->label_2->text() +" Palm Action ");
            }
            if(Arr[3]!=0)
            {
                ui->label_2->setText(ui->label_2->text() +" Rising Hand ");
            }
            if(Arr[4]!=0)
            {
                ui->label_2->setText(ui->label_2->text() +" Hand Sweep ");
            }
            if(Arr[5]!=0)
            {
                ui->label_2->setText(ui->label_2->text() +" Stretching ");
            }
            if(Arr[6]!=0)
            {
                ui->label_2->setText(ui->label_2->text() +" Move Right Leg ");
            }
            if(Arr[7]!=0)
            {
                ui->label_2->setText(ui->label_2->text() +" Waving ");
            }
           //ui->label_2->setText(temp);*/
            /*
        switch(index)
        {
        case 1: ui->label_2->setText("Body Swing");break;
        case 2: ui->label_2->setText("Looking Around");break;
        case 3: ui->label_2->setText("Palm Action");break;
        case 4: ui->label_2->setText("Rising Hand");break;
        case 5: ui->label_2->setText("Hand Sweep");break;
        case 6: ui->label_2->setText("Stretching");break;
        case 7: ui->label_2->setText("Move Right Leg");break;
        case 8: ui->label_2->setText("Waving");break;

        }*/






       }
    else{
        ui->label_2->setText("Error in predecting...");
    }
}
