#ifndef MY_TASK_V10_H
#define MY_TASK_V10_H

#include <map>
#include <deque>
#include <mutex>
#include <future>
#include <thread>
#include <utility>
#include <iostream>
#include <functional>




#ifdef DEBUG
#define DEBUG_MSG(str) do { std::cout << str << std::endl; } while( false )
#else
#define DEBUG_MSG(str) do { } while ( false )
#endif

namespace hss
{

template <typename R, typename ...Args>
class my_task
{
    // Para que pueda invocar a la fn() ha sido necesario que el compilador
    // me dejara y no quejara al poner los parentesis. Esto lo he conseguido
    // mediante el valor de retorno del std::bind. El error lo tenia en que
    // hacia el result_of de std::function<void(const Args && ...)> pero
    // hay que expandir los argumentos y es lo que me faltaba. Añadir
    // el (Args...):
    // std::function<void(const Args && ...)>(Args...)::type
    using function_type = std::function<R(Args ...)>;

    //using Ret = std::invoke_result_t<function_type, Args...>;
    using Ret = typename std::result_of<function_type(Args ...)>::type;

    //std::function<Ret()> fn;
    std::unique_ptr<std::function<void()>> fn;
    //std::function<void()> fn;

    //std::function<R(Args && ...)> fn;

    //std::promise<bool> *_taskComplete;             // the promise of the result
    std::unique_ptr<std::promise<bool>> _taskComplete;

public:

     my_task(function_type && func,
                     Args && ... args,
                     std::unique_ptr<std::promise<bool>> && taskComplete = nullptr) {
                     //std::promise<bool> *taskComplete = nullptr) {


     using return_type = typename std::result_of<function_type(Args...)>::type;

//     fn.reset(std::make_unique< std::packaged_task<return_type()>>(
//                std::bind(std::forward<function_type>(func),
//                          std::forward<Args>(args)...)
//            ));

//     fn = std::make_unique<
//             std::packaged_task<return_type()>>(
//                                                    std::bind(std::forward<function_type>(return_type),
//                                                              std::forward<Args>(args)...)
//                                                   );

        //fn = std::bind(std::forward<function_type>(func), std::forward<Args>(args)...);
        //fn = std::bind(&func, (args)...);
        _taskComplete = std::move(taskComplete);
    }

    void run(Args && ... args)
    {
        fn(std::forward<Args>(args)...);
    }

    auto Call()
    {
        return (*fn)();
    }

    std::future<bool> getFuture(void)
    {
        return _taskComplete->get_future();
    };

    void setPromiseValue(bool value)
    {
        _taskComplete->set_value(value);
    };

    void setPromise(std::unique_ptr<std::promise<bool>> && taskComplete)
    {
        _taskComplete = std::move(taskComplete);
    };

//    void setPromise(std::promise<bool> *prom)
//    {
//        _taskComplete = prom;
//    };

    void operator()(Args && ... args)
    {
        fn(std::forward<Args>(args)...);
    }

    auto operator()()
    {
        return (*fn)();
    }
}; // end class my_task.

} // end namespace hss
#endif

