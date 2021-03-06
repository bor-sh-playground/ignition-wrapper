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
  std::cout << "Start ignition" << std::endl;

  ignition::transport::Node node;
  std::string topic = "/foo";

  std::cout << "Subscribing " << std::endl;

  // Subscribe to a topic by registering a callback.
  if (!node.Subscribe(topic, cb))
  {
    std::cerr << "Error subscribing to topic [" << topic << "]" << std::endl;
    return;
  }

  ignition::transport::waitForShutdown();
  return;
}

#ifdef __cplusplus  
} // extern "C"  
#endif
