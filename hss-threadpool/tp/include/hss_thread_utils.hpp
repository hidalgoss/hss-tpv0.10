#ifndef HSSTHREADUTILS_H
#define HSSTHREADUTILS_H

#include <iostream>
#include <thread>
#include <numeric> // std::accumulate
#include <vector>
#include <algorithm> // for_each
#include <functional> // std::mem_fn


namespace hss {
namespace Utils {
  
  // ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- 
  // Definimos esta clase como callable object 'operator()' de manera que la podemos
  // invocar desde la creación del thread.
  // ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- 
  template<typename Iterator, typename T>
  class ParallelAccumulate
  {
    Iterator _first;
    Iterator _last;
    T _init;
    T & _result;

    // ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- 
    struct accumulate_block
    {
      void operator()(Iterator first, Iterator last, T& result){
        //std::cout << "accBlock threadId: " << std::this_thread::get_id() << std::endl;
        result = std::accumulate(first, last, result);
      }
    };

    // ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- 
    static T parallel_accumulate(Iterator first, Iterator last, T init)
    {
      unsigned long const length = std::distance(first, last);
      if (!length)
        return init;

      unsigned long const min_per_thread = 25;
      unsigned long const max_threads = (length + min_per_thread - 1) / min_per_thread;
      unsigned long const hardware_threads = std::thread::hardware_concurrency();
      unsigned long const num_threads = std::min(hardware_threads != 0 ? hardware_threads : 2, max_threads);
      unsigned long const block_size = length / num_threads;

      std::vector<T> results(num_threads);
      std::vector<std::thread> threads(num_threads - 1);

      Iterator block_start = first;
      for (unsigned long i = 0; i < (num_threads - 1); ++i)
      {
        Iterator block_end = block_start;
        std::advance(block_end, block_size);
        threads[i] = std::thread(
          //accumulate_block<Iterator, T>(),
          accumulate_block(),
            block_start, block_end, std::ref(results[i]));
        block_start = block_end;
      }

      //accumulate_block<Iterator, T>()(
      accumulate_block()(
        block_start, last, results[num_threads - 1]);

      std::for_each(threads.begin(), threads.end(),
        std::mem_fn(&std::thread::join));
      return std::accumulate(results.begin(), results.end(), init);
    }

  public:
    ParallelAccumulate() = delete;

    explicit ParallelAccumulate(Iterator first, Iterator last, T init, T & result) : 
      _first{first} ,
      _last(last) ,
      _init(init) ,
      _result(result) {};

    // ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- 
    void operator()() const
    {
      std::cout << "operator() threadId: " << std::this_thread::get_id() << std::endl;
      _result = parallel_accumulate(_first, _last, _init);
    }

    // TODO - 15.03.22.
    // La idea de este segundo operator() era la de hacer posible al llamada de este 
    // objeto desde la creación del std::thread myThread(<este operator>); 
    // pero de momento no ha funcionado. 
    // ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- 
    void operator()(Iterator first, Iterator last, T init, T& result) 
    {
      std::cout << "operator(args) threadId: " << std::this_thread::get_id() << std::endl;
      _result = result = parallel_accumulate(first, last, init);
    }
    

    // ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- 
    float getResult(){
      return _result;
    }
  };// end class ParallelAccumulate.

  
  
    
  // ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- 
  // Podemos utilizar RAII para asegurarnos que la ejecución del thread que crea
  // al thread que se recibe como parametro, no termina antes de que este thread
  // haya terminado. 
  // En caso de que el thread principal termine antes, se destruyen en orden inverso
  // al de creación por lo que antes de destruirse el thread principal, se hace el 
  // join en el destructor del thread gestionado aqui ya que se destruye antes este.
  // ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- 
  class ThreadGuard
  {
    std::thread & _threadRefToGuard;

  public:
    explicit ThreadGuard(std::thread & threadRefReceived) : _threadRefToGuard(threadRefReceived) {}
    
    ThreadGuard(ThreadGuard const&) = delete;
    ThreadGuard& operator=(ThreadGuard const&) = delete;

    ~ThreadGuard()
    {
      if (_threadRefToGuard.joinable())
      {
        _threadRefToGuard.join();
      }
    }
  };// end class ThreadGuard.


    
  // class ThreadUtils {
  // public:
  // };// end class ThreadUtils.


}// end namespace Utils
}// end namespace hss

#endif //HSSTHREADUTILS_H