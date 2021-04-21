#ifndef __GCSVel_H__
#define __GCSVel_H__

#include "ros/ros.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include <geometry_msgs/PoseStamped.h>
#include <geometry_msgs/Twist.h>

#include <Eigen/Dense>

using namespace Eigen;
using namespace std;

class GCSVel
{
public:
  // Constructor
  GCSVel();
  virtual ~GCSVel();

  // Setpoint Function(s)
  void update_setpoint();

protected:
  ros::NodeHandle nh;

private:
  // ROS variables
  ros::Publisher     velocity_sp_pub;
  ros::Subscriber    pose_curr_sub;

  // Gains
  double kp;
  double ki;
  double kd;

  double integral_x;
  double integral_y;
  double integral_z;

  double max_integral;
  string drone_id;

  double err_x;
  double err_y;
  double err_z;

  double err_x_prev;
  double err_y_prev;
  double err_z_prev;

  // Desired Hover Position
  double hover_x;
  double hover_y;
  double hover_z;

  // Time Variables
  double          t_final;    //  end of total trajectory
  ros::Time       t_start;    // Start time using world clock
  ros::Duration   t_prev;     // Previous time

  geometry_msgs::Twist       vel_sp;
  geometry_msgs::Vector3     vel_angular;
  geometry_msgs::PoseStamped pose_t_curr;

  // Counters and Time Variables
  int k_main;

  // Functions
  void pose_curr_cb(const geometry_msgs::PoseStamped::ConstPtr& msg);
  void compute_integral(double &integral_term, double prev_val, double curr_val, double dt_secs);
};

#endif