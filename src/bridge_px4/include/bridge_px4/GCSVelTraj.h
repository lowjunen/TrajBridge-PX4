
/**************************************************************************
  File Name    : GCSVelTraj.h
  Author       : Keiko Nagami
                 Multi-Robot Systems Lab (MSL), Stanford University
  Contact      : knagami@stanford.edu
  Create Time  : Apr 28, 2020.
  Description  : Based on GCS. Publish velocity setpoint commands based on
                 position + yaw errors from a desired trajectory (loaded
                 through csv file) via PID control. Written for a single
                 drone, to be run on a laptop communicating with the drone.
**************************************************************************/

#ifndef __GCSVelTraj_H__
#define __GCSVelTraj_H__

#include "ros/ros.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include <geometry_msgs/PoseStamped.h>
#include <geometry_msgs/TwistStamped.h>

#include <Eigen/Dense>

using namespace Eigen;
using namespace std;

class GCSVelTraj
{
public:
  // Constructor
  GCSVelTraj();
  virtual ~GCSVelTraj();

  // Setpoint Function(s)
  void update_setpoint();

protected:
  ros::NodeHandle nh;

private:
  // ROS variables
  ros::Publisher     vel_sp_pub;
  ros::Subscriber    pose_curr_sub;

  // Trajectory Variables
  int                 n_fr;
  string              traj_id;
  vector<double>           t_traj;
  vector< vector<double> >   st_traj;

  // Gains
  double kp;
  double ki;
  double kd;

  double integral_x;
  double integral_y;
  double integral_z;
  double integral_yaw;

  double max_integral;
  string drone_id;
  string target_id;

  double err_x;
  double err_y;
  double err_z;
  double err_yaw;

  double err_x_prev;
  double err_y_prev;
  double err_z_prev;
  double err_yaw_prev;

  // Time Variables
  double          t_end;      // end of single trajectory
  double          t_final;    // end of total trajectory
  ros::Time       t_start;    // Start time using world clock
  ros::Duration   t_prev;     // Previous time

  geometry_msgs::TwistStamped  vel_sp;
  geometry_msgs::Vector3       vel_angular;
  geometry_msgs::PoseStamped   pose_t_curr;

  // Counters and Time Variables
  int k_main;
  int k_traj;
  int k_loop;

  // Functions
  void load_trajectory(const string &input);
  void pose_curr_cb(const geometry_msgs::PoseStamped::ConstPtr& msg);
  void compute_integral(double &integral_term, double prev_val, double curr_val, double dt_secs);
};

#endif