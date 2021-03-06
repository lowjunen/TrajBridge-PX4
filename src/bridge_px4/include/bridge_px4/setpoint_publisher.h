
/**************************************************************************
  File Name    : setpoint_publisher.h
  Author       : JunEn Low
                 Multi-Robot Systems Lab (MSL), Stanford University
  Contact      : jelow@stanford.edu
  Create Time  : Mar 16, 2021.
  Description  : Publisher to Handle Setpoint by Error Over Mavlink
**************************************************************************/

#ifndef __SETPOINT_PUBLISHER_NODE_H__
#define __SETPOINT_PUBLISHER_NODE_H__

#include "ros/ros.h"

#include <geometry_msgs/Pose.h>
#include <geometry_msgs/PoseStamped.h>
#include <geometry_msgs/Quaternion.h>

#include <mavros_msgs/State.h>
#include <mavros_msgs/CommandTOL.h>
#include <mavros_msgs/CommandCode.h>

#include <Eigen/Dense>

using namespace Eigen;
using namespace std;

class SetpointPublisher {
public:
   // Constructor
   SetpointPublisher();
   virtual ~SetpointPublisher();
protected:
  ros::NodeHandle nh;
private:
   // State Machines
   enum sp_pub_state_machine {
      STARTUP,
      LINKED,
      HOVER,
      ACTIVE,
      COMPLETE,
   } sp_pub_state;

   enum sp_stream_state_machine {
      SP_STREAM_ON,
      SP_STREAM_OFF,
   } sp_stream_state;

   enum mc_stream_state_machine {
      MC_STREAM_ON,
      MC_STREAM_OFF,
   } mc_stream_state;

   // Input Params
   string drone_id;
   float sp_out_hz;
   float checkup_hz;
   float sp_gcs_hz_min;
   float checkup_hz_min;

   // ROS Publishers and Subscribers
   ros::Publisher     pose_sp_pub;
   ros::Subscriber    pose_sp_sub;
   ros::Subscriber    pose_curr_sub;
   ros::Subscriber    mav_state_sub;

   // ROS Timers
   ros::Timer setpointLoop;   // setpoint update timer
   ros::Timer checkupLoop;      // savepoint update timer
   
   // ROS Services
   ros::ServiceClient land_client;
   
   // Quad State/Parameter Variables
   geometry_msgs::PoseStamped pose_t_sp_gcs;      // Setpoint Commanded by GCS
   geometry_msgs::PoseStamped pose_t_sp_out;      // Setpoint Sent to Drone
   geometry_msgs::PoseStamped pose_t_curr;        // Current Pose
   geometry_msgs::Pose pose_st;                 // Starting Pose
   geometry_msgs::Pose pose_sa;                 // Savepoint Pose (for failsafes and active hover)
   mavros_msgs::State  mode_cr;                 // Current Mavros Mode

   // Counters and Time Variables
   int count_main;
   ros::Duration setpoint_dt_max;
   ros::Duration checkup_dt_max;

   // Constants
   geometry_msgs::Quaternion quat_forward;

   // Telemetry Function(s)
   void pose_sp_cb(const geometry_msgs::PoseStamped::ConstPtr& msg);
   void pose_curr_cb(const geometry_msgs::PoseStamped::ConstPtr& msg);
   void mav_state_cb(const mavros_msgs::State::ConstPtr& msg);

   void setpoint_cb(const ros::TimerEvent& event);
   void checkup_cb(const ros::TimerEvent& event);

   // Functions
   void land();
   void pub_check();
};

#endif
