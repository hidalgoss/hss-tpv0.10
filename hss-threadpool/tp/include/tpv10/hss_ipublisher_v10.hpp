#ifndef IPUBLISHER_V10_H
#define IPUBLISHER_V10_H

#include <map>
#include <memory>
#include <future>
#include <iostream>
#include "hss_isubscriber_v10.hpp"

namespace hss
{

// En esta version vamos a tratar de hacer una aproximación Pub/Sub
// mas cercana a a la de la doc C++ API Desing.
template <typename R, typename... Args>
class IPublisher
{

protected:
    int last_id_subscriber;
    std::multimap<std::string, std::shared_ptr<ISubscriber<R, Args...>>> _subscribers;

public:
    typedef std::multimap<std::string, std::shared_ptr<ISubscriber<R, Args...>>> subscribers_map_t;
    typedef typename subscribers_map_t::iterator subscribers_iterator_t;
    //using iterator_subscribers_t = typename std::multimap<std::string, std::shared_ptr<ISubscriber<Args...>>>::iterator;

    IPublisher() { last_id_subscriber = 0; }
    virtual ~IPublisher() {}

    virtual ISubscriber<R, Args...> * Subscribe(const std::string & event_name,
                           ISubscriber<R, Args...> * subscriber) = 0;

    // Consideramos que un subscriptor puede estar subscrito a varios eventos.
    // Para dejar de registrarlo en el evento, necesitamos su id_subscriptor.
    // Pero solo hay que dejar de registrarlo para esos eventos.
    virtual void Unsubscribe(const std::string & event_name,
                             int id_subscriber) = 0;


    // Notifica a todos los subscriptores de un event_name, los Args... indicados.
    virtual std::vector<std::future<R>> Notify(const std::string & event_name, Args && ...args) = 0;

    int getLastIdSubscriber(){
        return last_id_subscriber;
    }

    void incrementLastIdSubscriber(){
        last_id_subscriber++;
    }
}; // end class Publisher

} // end namespace hss
#endif
