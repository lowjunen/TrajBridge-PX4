/**************************************************************************
  File Name    : hr_control.h
  Author       : JunEn Low
                 Multi-Robot Systems Lab (MSL), Stanford University
  Contact      : jelow@stanford.edu
  Create Time  : Jan 27, 2020.
  Description  : Publisher to Handle Setpoint by Error Over Mavlink
**************************************************************************/

#ifndef __HR_CONTROL_H__
#define __HR_CONTROL_H__

#include "ros/ros.h"
#include "bridge_px4/TrajTransfer.h"

#include <std_msgs/Float32MultiArray.h>
#include <geometry_msgs/PoseStamped.h>
#include <geometry_msgs/TwistStamped.h>
#include <mavros_msgs/AttitudeTarget.h>
#include <Eigen/Dense>
#include <numeric>

using namespace Eigen;
using namespace std;
using namespace std_msgs;

using OneD = vector<double>;
using TwoD = vector<vector<double>>;
using ThreeD = vector<vector<vector<double>>>;


class HR_Control
{
public:
  // Constructor
  HR_Control();
  virtual ~HR_Control();

  bool transfer(bridge_px4::TrajTransfer::Request& req, bridge_px4::TrajTransfer::Response& res);
  void trajectory_execute();

protected:
  ros::NodeHandle nh;

private:
  // ROS variables
  ros::ServiceServer traj_server;
  ros::Subscriber    pose_curr_sub;
  ros::Subscriber    vel_curr_sub;
  ros::Publisher     att_sp_pub;

  // Trajectory Variables
  int N;
  
  vector<float> x_arr;
  vector<float> l_arr;
  vector<float> L_arr;

  Matrix<float,4,1>  l_curr;
  Matrix<float,4,10> L_curr;
  Matrix<float,10,1> x_bar;
  Matrix<float,10,1> x_curr;  // Current State
  Matrix<float,4,1>  u_br;  // Current State

  // Time Variables
  double      t_dt;
  ros::Time   t_next;      // end of  single trajectory
  ros::Time   t_start;    // Start time using world clock

  // Counters and Time Variables
  int k_main;
  
  // Quad Setpoints
  mavros_msgs::AttitudeTarget att_sp_out;      // Setpoint Attitude (body rate, orientation, thrust) Out

  // Function(s)
  void pose_curr_cb(const geometry_msgs::PoseStamped::ConstPtr& msg);
  void vel_curr_cb(const geometry_msgs::TwistStamped::ConstPtr& msg);

};

#endif