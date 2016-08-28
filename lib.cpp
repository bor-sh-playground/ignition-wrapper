#include <iostream>
#include <string>
#include <ignition/msgs.hh>
#include <ignition/transport.hh>

#ifdef __cplusplus
extern "C" {  
#endif  

void print_my_stuff() {
  std::cout << "Start ignition";    
}

//////////////////////////////////////////////////
/// \brief Function called each time a topic update is received.
void cb(const ignition::msgs::StringMsg &_msg)
{
  std::cout << "Msg: " << _msg.data() << std::endl << std::endl;
}

void contact(const ignition::msgs::Contacts &_msg)
{
  std::cout << "Got msg " << std::endl;
}

void waitForShutdown(){
  ignition::transport::waitForShutdown();
}

//////////////////////////////////////////////////
void run()
{
  std::cout << "Start ignition";

  ignition::transport::Node node;
  std::string topic = "/gazebo/default/box/link/my_contact";

  // Subscribe to a topic by registering a callback.
  if (!node.Subscribe(topic, contact))
  {
    std::cerr << "Error subscribing to topic [" << topic << "]" << std::endl;
    return;
  }

  return;
}

#ifdef __cplusplus  
} // extern "C"  
#endif
