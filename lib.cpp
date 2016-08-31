#include <iostream>

#include <gazebo/transport/transport.hh>
//#include <gazebo/msgs/contacts.pb.h>
#include <gazebo/msgs/msgs.hh>
#include <gazebo/gazebo_client.hh>

#include <gazebo/math/gzmath.hh>

class Callback
{

  typedef void (*fct_ptr)();

public:
  Callback() : _p(NULL){};
  virtual ~Callback() = default;

  void set_callback(fct_ptr _ptr)
  {
    _p = _ptr;
  }

  /////////////////////////////////////////////////
  // Function is called everytime a message is received.
  void cb(ConstWorldStatisticsPtr &_msg)
  {
    if (_p)
    {
      _p();
    }
  }

private:
  fct_ptr _p;
};

#ifdef __cplusplus
extern "C" {
#endif

typedef void (*contact_cb_t)(int x, int y, int z);
contact_cb_t contact_cb = NULL;

/////////////////////////////////////////////////
// Function is called everytime a message is received.
void cb(ConstWorldStatisticsPtr &_msg)
{
  // Dump the message contents to stdout.
  std::cout << _msg->DebugString();
}

void pub_pos()
{
  // Load gazebo
  gazebo::client::setup();

  // Create our node for communication
  gazebo::transport::NodePtr node(new gazebo::transport::Node());
  node->Init();

  // Publish to a Gazebo topic
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

void set_pos(char *name, int x, int y, int z)
{
  // Load gazebo
  gazebo::client::setup();

  // Create our node for communication
  gazebo::transport::NodePtr node(new gazebo::transport::Node());
  node->Init();

  // Publish to a Gazebo topic
  gazebo::transport::PublisherPtr pub = node->Advertise<gazebo::msgs::Model>("~/model/modify");
  gazebo::msgs::Model msg;
  msg.set_name(std::string(name));

  // Wait for a subscriber to connect
  pub->WaitForConnection();

  // Publisher loop...replace with your own code.
  while (true)
  {
    // Throttle Publication
    gazebo::common::Time::MSleep(100);

    // Generate a pose
    ignition::math::Pose3d pose(x, y, z, 0, 0, 0);

    // Convert to a pose message
    gazebo::msgs::Set(msg.mutable_pose(), pose);

    pub->Publish(msg);
  }

  // Make sure to shut everything down.
  gazebo::client::shutdown();
}

void cb_contact(ConstContactsPtr &_msg)
{
  // Dump the message contents to stdout.
  for (unsigned int i = 0; i < _msg->contact_size(); ++i)
  {
    std::cout << "Collision between[" << _msg->contact(i).collision1()
              << "] and [" << _msg->contact(i).collision2() << "]\n";

    for (unsigned int j = 0; j < _msg->contact(i).position_size(); ++j)
    {
      std::cout << j << "  Position:"
                << _msg->contact(i).position(j).x() << " "
                << _msg->contact(i).position(j).y() << " "
                << _msg->contact(i).position(j).z() << "\n";
      std::cout << "   Normal:"
                << _msg->contact(i).normal(j).x() << " "
                << _msg->contact(i).normal(j).y() << " "
                << _msg->contact(i).normal(j).z() << "\n";
      std::cout << "   Depth:" << _msg->contact(i).depth(j) << "\n";

      if ( contact_cb ) {
        contact_cb(_msg->contact(i).position(j).x(),_msg->contact(i).position(j).y(),_msg->contact(i).position(j).z());
      }
    }
  }
}

void subscribe(contact_cb_t _cb, char *topic)
{
  // Load gazebo
  gazebo::client::setup();

  // Create our node for communication
  gazebo::transport::NodePtr node(new gazebo::transport::Node());
  node->Init();

  contact_cb = _cb;

  // Listen to Gazebo world_stats topic
  gazebo::transport::SubscriberPtr sub = node->Subscribe(topic, cb_contact);

  // Busy wait loop...replace with your own code as needed.
  while (true)
    gazebo::common::Time::MSleep(10);

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
