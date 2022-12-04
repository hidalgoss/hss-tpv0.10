#ifndef ISUBSCRIBER_V10_H
#define ISUBSCRIBER_V10_H

#include <iostream>
#include <functional>


#ifdef DEBUG
#define DEBUG_MSG(str) do { std::cout << str << std::endl; } while( false )
#else
#define DEBUG_MSG(str) do { } while ( false )
#endif

namespace hss
{

template <typename R, typename... Args>
class ISubscriber
{

protected:
    int _id_subscriber;
    std::string _event_name;
    //std::function<void(Args...)> _update;

public:
    // Seems std::function requires user arguments as const lvalue references.
    std::function<R(const Args &...)> _update;

    ISubscriber(const std::string & event_name) :
        _event_name(event_name) {}

    virtual ~ISubscriber() {}

    void registerUpdate(const std::function<R(const Args & ...)> &update)
    {
        try {
            _update = update;
        } catch (...) {
            DEBUG_MSG("ISubscriber() exception. registerUpdate()");
            throw;
        }
    }

    void setSubscriberId(int id_subscriber) { _id_subscriber = id_subscriber; }

    int getSubscriberId() { return _id_subscriber; }

}; // end class Subscriber

} // end namespace hss
#endif
