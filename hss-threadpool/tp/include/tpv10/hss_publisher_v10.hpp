#ifndef PUBLISHER_V10_H
#define PUBLISHER_V10_H

#include <map>
#include <iostream>
#include "hss_ipublisher_v10.hpp"
#include "hss_subscriber_v10.hpp"

namespace hss
{

// Hacemos un registerListener con una signature de parametros de entrada
// como por ejemplo void(const myClass & onj, int num, double realvalue).
// Para publicar datos a esos callbacks necesitamos esa signature.
// Mediante el uso de packaged arguments expandimos los argumentos de
// manera generica en función de cada caso.
template <typename R, typename... Args>
class Publisher : public IPublisher<R, Args...>
{

public:
    Publisher()  : IPublisher<R, Args...>() {}
     ~Publisher(){}

    // Esto se invoca creando un puntero a la Interfaz y se le asigna la
    // dirección de un objeto creado de una clase derivada.
    // ISubscriber<Args...> *subs = new SubscriberUser(...);
    // ...Subscribe("event nmae 1", subs);
    // ó
    // SubscriberUser user(...);
    // ...Subscribe("event nmae 1", &user);
    ISubscriber<R, Args...> *Subscribe(const std::string & event_name, ISubscriber<R, Args...> * subscriber) override
    {
        try {
            subscriber->setSubscriberId(++this->last_id_subscriber);
            // Necesitamos copiar el shared_ptr para incrementar el counter.
            // Si no lo hacemos, al hacer el make_pair ptr se queda a null.
            std::shared_ptr<ISubscriber<R, Args...>> ptr(subscriber);
            std::shared_ptr<ISubscriber<R, Args...>> internalPointer = ptr;
            this->_subscribers.insert(
                        std::make_pair(event_name, internalPointer // moving shared_ptr avoids increasing counter but leaves ptr to null.
                                       ));// end insert.
            return ptr.get();
        } catch (...) {
            DEBUG_MSG("Publisher() exception. Subscribe().");
            throw;
        }
    }

    void Unsubscribe(const std::string & event_name, int id_subscriber) override
    {
        try {
            // Como debemos buscar por event_name, no debemos hacer un find_if.
            // Como debemos buscar por la key std::string, utilizamos .equal_range().
//            auto subscriptor_to_unregister =
//                    std::find_if(this->_subscribers.begin(),
//                                 this->_subscribers.end(),
//                                 [id_subscriber](const std::pair<std::string, ISubscriber<Args...>> subscriber)
//            {
//                return (id_subscriber == subscriber.second->id_subscriber &&
//                        event_name==subscriber.first );
//            });// end find_if.

//            if(subscriptor_to_unregister != this->_subscribers.end()){
//                this->_subscribers.erase(subscriptor_to_unregister);
//            }

            // Como el map esta ordenado, los iterators .first y .second indican
            // en rango de elementos que buscamos.
            auto range = this->_subscribers.equal_range(event_name);
            for(auto it = range.first; it != range.second; it++ ){
                this->_subscribers.erase(it);
            }

        } catch (...) {
            throw;
        }
    }

    //void Notify(std::string & event_name, Args && ...args) override {}


}; // end class Publisher

} // end namespace hss
#endif
