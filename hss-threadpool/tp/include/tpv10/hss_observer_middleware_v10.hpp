#ifndef OBSERVER_MIDDLEWARE_V10_H
#define OBSERVER_MIDDLEWARE_V10_H

#include "hss_taskmanager_publisher_v10.hpp"
#include "hss_publisher_v10.hpp"
#include "hss_subscriber_v10.hpp"
//#include <hss_lock_free_queue.hpp>



namespace hss
{

template <typename R, typename... Args>
struct TmObserverMiddleware : public TaskManagerPublisher<R, Args...>
{

    TmObserverMiddleware() : TaskManagerPublisher<R, Args...> ()
    {
        this->start();
    }

    TmObserverMiddleware(int num_threads) : TaskManagerPublisher<R, Args...> (num_threads)
    {
        this->start();
    }

    ~TmObserverMiddleware()
    {
        this->stop();
    }

    Publisher<R, Args...> *getPublisher() { return this; } //return _taskManagerPublisher.get(); }
    
    // Realiza una nueva subscrición y devuelve el puntero al nuevo subscriptor.
    // La subscripción la realiza con los parametros faciltiados. Para el ejemplo
    // del User, se le pasará el User a subscribir en la instanacia SubscriberUser
    // y devolverá un puntero a esta instancia recien creada.
    Subscriber<R, Args...> *getSubscriber(const std::string & event_name)
    {
        try {
            Subscriber<R, Args...> *_subscriber = new Subscriber<R, Args...>(event_name);
            return dynamic_cast<Subscriber<R, Args...> *>
                    (this->Subscribe(event_name, _subscriber));

            // Devolvemos el puntero a memoria dinamica. Pero no debemos preocuparnos por la
            // liberación de memoria por parte del usuario en el main ya que se almacena en
            // la estructura de subscribers como unique_ptr. Eso implica que será liberada
            // cuando se destruya esa estructura y por tanto, la del usuario en el main.
            // Eso deberá tenerlo en cuenta desde el main pero debería ser el ciclo de uso normal.
            //
            // 30.05.22
            // Si queremos gestionar el puntero dinamico con smart pointers y poder devolverlo al mainç
            // no podemos utilizar unique_ptr. Al hacer el move() en la f(x) Subscribe() del Publisher()
            // implica que el puntero que tenemos en esta f(x) deja de estar operativo y no podemos
            // devolverlo. Debemos utilizar un shared_ptr para que el puntero se diga manteniendo en la
            // estructura multimap _subscribers y pueda ser gestionado también por el main.
            // De hecho, esta f(x) no puede devolver el puntero del *subscriber sino que tiene que hacer
            // el .get() del shared_ptr de la estructura.

            //return _subscriber;

        } catch (...) {
            throw;
        }
    }

    void stopAndFinish(){
        this->stop();
    }
}; // end class ObserverMiddleware
} // end namespace hss
#endif
