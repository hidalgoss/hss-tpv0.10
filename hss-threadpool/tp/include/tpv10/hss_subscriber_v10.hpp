#ifndef SUBSCRIBER_V10_H
#define SUBSCRIBER_V10_H

#include <iostream>
#include "hss_isubscriber_v10.hpp"

//#define DEBUG
#ifdef DEBUG
#define DEBUG_MSG(str) do { std::cout << str << std::endl; } while( false )
#else
#define DEBUG_MSG(str) do { } while ( false )
#endif

namespace hss
{

template <typename R, typename... Args>
class Subscriber : public ISubscriber<R, Args...>
{

public:
//    Subscriber() : ISubscriber<Args...>() {}

    Subscriber(const std::string & event_name) :
        ISubscriber<R, Args...>(event_name) {}

    ~Subscriber() {}

}; // end class Subscriber

} // end namespace hss
#endif
