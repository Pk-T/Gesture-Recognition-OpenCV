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

#include "new.h"
#include "ui_new.h"

#include "GuiController.h"

#include <ntk/camera/rgbd_frame_recorder.h>
#include <ntk/camera/rgbd_processor.h>
#include <ntk/utils/opencv_utils.h>
#ifdef USE_FREENECT
# include <ntk/camera/kinect_grabber.h>
#endif

#include <QCloseEvent>

using namespace ntk;

new::new(GuiController& controller, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::new),
    m_controller(controller)
{
  ui->setupUi(this);

  connect(ui->depthView, SIGNAL(mouseMoved(int,int)),
          &m_controller, SLOT(on_depth_mouse_moved(int,int)));
  ui->depthView->setMouseTracking(true);

  ui->action_Show_Object_Detector->setDisabled(true);
  ui->action_3D_View->setDisabled(true);

  if (m_controller.grabber().isSynchronous())
    ui->syncMode->setChecked(true);
}

new::~new()
{
  delete ui;
}

void new :: update(const ntk::RGBDImage& image)
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
  // ui->depthView->setImageAsColor(image.depth(), &min_dist, &max_dist);
  // ui->depthView->setImage(image.depth(), &min_dist, &max_dist);
  if (image.amplitude().data && ui->amplitudeView->isVisible())
    ui->amplitudeView->setImage(image.amplitude());
  if (image.intensity().data && ui->intensityView->isVisible())
    ui->intensityView->setImage(image.intensity());  

  int x,y;
  ui->depthView->getLastMousePos(x,y);
  m_controller.on_depth_mouse_moved(x,y);
}


void new::on_action_Quit_triggered()
{
  m_controller.quit();
}

void new::on_action_GrabFrames_toggled(bool active)
{
  m_controller.setGrabFrames(active);
}
