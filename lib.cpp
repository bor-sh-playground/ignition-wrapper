#include <iostream>

#include <gazebo/transport/transport.hh>
#include <gazebo/msgs/msgs.hh>
#include <gazebo/gazebo_client.hh>

#include <gazebo/math/gzmath.hh>

#ifdef __cplusplus
extern "C" {  
#endif  

/////////////////////////////////////////////////
// Function is called everytime a message is received.
void cb(ConstWorldStatisticsPtr &_msg)
{
  // Dump the message contents to stdout.
  std::cout << _msg->DebugString();
}

void pub_pos() {
  // Load gazebo
  gazebo::client::setup();

  // Create our node for communication
  gazebo::transport::NodePtr node(new gazebo::transport::Node());
  node->Init();

  // Publish to a Gazebo topic
//  gazebo::transport::PublisherPtr pub =
//    node->Advertise<gazebo::msgs::Pose>("~/pose/modify");
  gazebo::transport::PublisherPtr pub = node->Advertise<gazebo::msgs::Model>("~/model/modify");
  gazebo::msgs::Model msg;
  msg.set_name("box");

  // Wait for a subscriber to connect
  pub->WaitForConnection();

  // Publisher loop...replace with your own code.
  while (true)
  {
    // Throttle Publication
    gazebo::common::Time::MSleep(100);

    // Generate a pose
    ignition::math::Pose3d pose(4, 0, 0, 0, 0, 0);

    // Convert to a pose message
    gazebo::msgs::Set(msg.mutable_pose(), pose);

    pub->Publish(msg);
  }

  // Make sure to shut everything down.
  gazebo::client::shutdown();
}

/////////////////////////////////////////////////
void run()
{
  // Load gazebo
  gazebo::client::setup();

  // Create our node for communication
  gazebo::transport::NodePtr node(new gazebo::transport::Node());
  node->Init();

  // Listen to Gazebo world_stats topic
  gazebo::transport::SubscriberPtr sub = node->Subscribe("~/world_stats", cb);

  // Busy wait loop...replace with your own code as needed.
  while (true)
    gazebo::common::Time::MSleep(10);

  // Make sure to shut everything down.
  gazebo::client::shutdown();
}

#ifdef __cplusplus  
} // extern "C"  
#endif
