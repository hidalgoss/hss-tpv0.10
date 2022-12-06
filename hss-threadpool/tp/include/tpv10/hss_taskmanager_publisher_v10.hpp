#ifndef TASK_MANAGER_V10_H
#define TASK_MANAGER_V10_H


#include <map>
#include <mutex>
#include <future>
#include <thread>
#include <iostream>
//#include <hss_ts_queue.hpp>
#include "hss_publisher_v10.hpp"
#include <hss_iqueue_v10.hpp>

#ifdef DEBUG
#define DEBUG_MSG(str) do { std::cout << str << std::endl; } while( false )
#else
#define DEBUG_MSG(str) do { } while ( false )
#endif

namespace hss
{
// R: Return type, void as instante.
// ...Args: f(x) input parameter list signature, 'const myClass & obj' as instance.
template <typename R, typename... Args>
class TaskManagerPublisher : public Publisher<R, Args...>
{
    std::mutex sharedQueueMutex;
    std::condition_variable condTaskQueue;

    std::map<std::thread::id, std::unique_ptr<std::thread>> thread_map;
    //hss::thread_safe_queue<std::function<void()>> _task_fifo;
    hss::IQueue<std::function<void()>> _task_fifo;

    bool isTaskLoopRunning = false;
    int _num_threads = 0;

    std::mutex activeTasksMutex;
    int _num_active_tasks = 0;

protected:
    void TaskLoop()
    {
        try{
            isTaskLoopRunning = true;
            while(isTaskLoopRunning)
            {
                std::mutex localMutex;
                bool result_pop = false;
                std::function<void()> task;

                {
                    std::unique_lock<std::mutex> localUniqueLock(localMutex);
                    condTaskQueue.wait(localUniqueLock, [this]
                    {
                        DEBUG_MSG("TaskLoop. Notification received. threadID: *"
                                  << std::this_thread::get_id() << "*");
                        return (!_task_fifo.empty() || !isTaskLoopRunning);
                    });// end condTaskQueue

                    // if TaskLoop should finish running, exit inmediately.
                    if(!isTaskLoopRunning)
                        return;

                    // 8.05.22 - He modificado la f(x) de la queue para soportar Task que
                    // sean unique_ptr... Falta probarla.
                    result_pop = _task_fifo.try_pop( task );

                    if(result_pop){
                        DEBUG_MSG("TaskLoop. poped task threadID: *" << std::this_thread::get_id() << "*");
                        {
                            std::lock_guard<std::mutex> lk(activeTasksMutex);
                            _num_active_tasks++;
                        }
                        task();
                        std::lock_guard<std::mutex> lk(activeTasksMutex);
                        _num_active_tasks--;
                    }
                }

            }
        }
        catch (...) {
            DEBUG_MSG("TaskManager() exception. TaskLoop()");
            throw;
        }
    }



    // Create and spawn _num_threads running the TaskLoop f(x) in each one, sharing
    // queue of Task's.
    void start()
    {
        try {
            if(isTaskLoopRunning == true)
                throw std::runtime_error("Task pool is already running");

            for(int i=0; i<_num_threads; i++) {
                //std::thread TaskLoopThread;
                //TaskLoopThread = std::move(
                std::thread TaskLoopThread = std::move(
                            std::thread(&hss::TaskManagerPublisher<R, Args ...>::TaskLoop,
                            this)
                            );

                auto t_id = TaskLoopThread.get_id();

                // IMPORTANTE.
                // Hay que hacer el std::move(TaskLoopThread). Si no hacemos el move nos
                // muestra el error de compilación de que usamos una not used deleted function o algo asi.
                thread_map.insert(
                            std::make_pair(t_id,
                                           std::make_unique<std::thread>(std::move(TaskLoopThread))));

                DEBUG_MSG("TaskLoop() started for thread_id: *" << t_id << "*");
            }
        } catch (...) {
            DEBUG_MSG("TaskManager() exception. Finishing all running threads.");
            isTaskLoopRunning = false;
            // Avisamos a TODOS los threads para su finalización, al haber encontrado error.
            condTaskQueue.notify_all();
        }
    }


    void stop()
    {
        try {
            isTaskLoopRunning = false;
            condTaskQueue.notify_all();
            for(auto &t : thread_map){
                if(t.second->joinable())
                    t.second->join();
            }
            DEBUG_MSG("DEBUG_MSG TaskManager() stop(). All running threads stoped.");
            std::cout << "TaskManager() stop(). All running threads stoped." << std::endl;
        } catch (...) {
            DEBUG_MSG("TaskManager() exception. stop().");
            throw;
        }
    }


    // Lo que hacemos no es publicar un Object si no un conjunto de argumentos
    // que seran los de entrada en la signature del listener.
    template<class F>
    //void push_task(std::function<R(Args...)> cb, Args && ... args){
    std::future<typename std::result_of<F(Args...)>::type> push_task(F&& cb, Args && ... args){
        try {
            using return_type = typename std::result_of<F(Args...)>::type;


            // 29.05.22
            // El motivo de la necesidad de utilizar el shared_ptr en la packaged_task
            // y poder agregarla a la queue parece estar explicado en la definición del
            // std::bind NOTES.
            // https://stackoverflow.com/questions/37816559/using-stdpackaged-task-to-queue-casyncsocket-detach-socket-tasks-causes-compil
            auto task = std::make_shared< std::packaged_task<return_type()> >
                    (std::bind(std::forward<F>(cb), std::forward<Args>(args)...));

            std::future<return_type> local_future = task->get_future();

            DEBUG_MSG("push_task !!! tid *" << std::this_thread::get_id() << "*" );
            _task_fifo.push( [task]() -> void { (*task)(); } );
            condTaskQueue.notify_one();
            return local_future;

        } catch (...) {
            DEBUG_MSG("TaskManager() exception. push_task().");
            throw;
        }
    }

//    template<typename F,typename A>
//    std::future<typename std::result_of<F(A&&)>::type>
//    spawn_task(F&& f,A&& a)
//    {
//        typedef typename std::result_of<F(A&&)>::type result_type;
//        std::packaged_task<result_type(A&&)>
//                task(std::move(f));
//        std::future<result_type> res(task.get_future());
//        std::thread t(std::move(task),std::move(a));
//        t.detach();
//        return res;
//    }

    TaskManagerPublisher(IQueue<std::function<void()>> & task_fifo)
    {
        try {
            this->_task_fifo = task_fifo;
            // by default we will use hw_conc -1 threads for task pool.
            int num_threads = std::thread::hardware_concurrency() - 1;
            if(num_threads <= 0){
                _num_threads = 1;
            }else{
                _num_threads = num_threads;
            }

        } catch (...) {
            throw;
        }
    }

    // If num_threads > max_thread std::bad_alloc is thrown.
    // Is not possible to create more threads than supported by hw.
    TaskManagerPublisher(IQueue<std::function<void()>> & task_fifo, int num_threads)
    {
        try {
            this->_task_fifo = task_fifo;
            // by default we will use hw_conc -1 threads for task pool.
            _num_threads = std::thread::hardware_concurrency() - 1;
            if(_num_threads <= 0){
                _num_threads = 1;
            }else if(num_threads > _num_threads){
                throw std::bad_alloc();
            }else{
                _num_threads = num_threads;
            }

        } catch (...) {
            throw;
        }
    }



    ~TaskManagerPublisher()
    {
        DEBUG_MSG("~TaskManager() - Starting Destructor.");
        this->stop();
        DEBUG_MSG("~TaskManager() Dest executed. TaskLoop() finalized & ThreadPoolv3 destroyed.");
    }




    // Mandatory to be executed before task pool has been started 'start()'.
    void set_task_pool_num_threads(int num_threads){
        try {
            if(isTaskLoopRunning == true)
                throw std::runtime_error("Task pool is already running");

            int max_threads = std::thread::hardware_concurrency() - 1;
            if(_num_threads > max_threads)
                throw std::bad_alloc();

            _num_threads = num_threads;

        } catch (...) {
            throw;
        }
    }

    bool taskPending(){
        return (_num_active_tasks>0 || !_task_fifo.empty());
    }


    // Funcion bloqueante que espera a la finalización de todas las Task
    // que hay en cola.
//    void waitFutures(){
//        try {
//            for(auto &f : futures){
//                if(f.valid()){
//                    //bool status = f.get();
//                    //f.get();
//                }else{
//                    std::cout << "future not valid" << std::endl;
//                }

//            }
//            futures.clear();
//        } catch (...) {
//            throw;
//        }
//    }


    // Creo el valor de retorno de los future todavia no es del todo correcto ya que
    // es  dinamico y no lo estamos haciendo...
    // Ademas, cuando se hace un publish de un event1 podemos tener subscriptores con
    // funciones que decuelvan void y otros que devuelvan int por ejemplo por lo que
    // para un mismo Notify, devolvería dos tipos de future distintos...
    std::vector<std::future<R>> Notify(const std::string & event_name, Args && ... args) override
    {
        try {
            std::vector<std::future<R>> local_futures;

            auto range = this->_subscribers.equal_range(event_name);
            for(typename hss::IPublisher<R, Args...>::subscribers_iterator_t it = range.first;
                it != range.second;
                it++ )
            {
//                auto local_future = push_task(std::forward<std::function<R(Args...)>>(it->second->_update),
//                                              std::forward<Args>(args)...);
                local_futures.push_back(
                            push_task(std::forward<std::function<R(Args...)>>(it->second->_update),
                                      std::forward<Args>(args)...)
                            );// end push_back.
            }// end for()
            return local_futures;

        } catch (...) {
            DEBUG_MSG("TaskManager() exception. Notify().");
            throw;
        }
    }


}; // end class TaskManager

} // end namespace hss
#endif

