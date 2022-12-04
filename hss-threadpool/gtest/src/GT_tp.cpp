#include <gtest/gtest.h>
#include <hss_thread_pool_v1.hpp>
// #include <future>
// #include <iostream>

// void task1(){
//   std::cout << "sleep  task1 5segs threadId: " << std::this_thread::get_id() << std::endl;
//   std::this_thread::sleep_for(std::chrono::seconds(5));
//   std::cout << "wakeup task1 5segs threadId: " << std::this_thread::get_id() << std::endl;
// }


// void task2(){
//   std::cout << "sleep  task2 2segs threadId: " << std::this_thread::get_id() << std::endl;
//   std::this_thread::sleep_for(std::chrono::seconds(2));
//   std::cout << "wakeup task2 2segs threadId: " << std::this_thread::get_id() << std::endl;
// }

// void task3(){
//   std::cout << "sleep  task3 1segs threadId: " << std::this_thread::get_id() << std::endl;
//   std::this_thread::sleep_for(std::chrono::seconds(1));
//   std::cout << "wakeup task3 1segs threadId: " << std::this_thread::get_id() << std::endl;
// }

// void task4(){
//   std::cout << "sleep  task4 0.5segs threadId: " << std::this_thread::get_id() << std::endl;
//   std::this_thread::sleep_for(std::chrono::milliseconds(500));
//   std::cout << "wakeup task4 0.5segs threadId: " << std::this_thread::get_id() << std::endl;
// }
int task1(){
  std::cout << "sleep  task1 5segs threadId: " << std::this_thread::get_id() << std::endl;
  std::this_thread::sleep_for(std::chrono::seconds(5));
  std::cout << "wakeup task1 5segs threadId: " << std::this_thread::get_id() << std::endl;
  return 1;
}


int task2(){
  std::cout << "sleep  task2 2segs threadId: " << std::this_thread::get_id() << std::endl;
  std::this_thread::sleep_for(std::chrono::seconds(2));
  std::cout << "wakeup task2 2segs threadId: " << std::this_thread::get_id() << std::endl;
  return 2;
}

int task3(){
  std::cout << "sleep  task3 1segs threadId: " << std::this_thread::get_id() << std::endl;
  std::this_thread::sleep_for(std::chrono::seconds(1));
  std::cout << "wakeup task3 1segs threadId: " << std::this_thread::get_id() << std::endl;
  return 3;
}

int task4(){
  std::cout << "sleep  task4 0.5segs threadId: " << std::this_thread::get_id() << std::endl;
  std::this_thread::sleep_for(std::chrono::milliseconds(500));
  std::cout << "wakeup task4 0.5segs threadId: " << std::this_thread::get_id() << std::endl;
  return 4;
}

TEST(TS_hssThreadPool, GT_tp_GC)
{
  try
  {
    // 1st version. Default constructor starts TaskManager() thread. 
    //hss::ThreadPool<void()> hssThreadPool;
    hss::ThreadPool<int()> hssThreadPool;
    auto f1a = hssThreadPool.createTask(task1);

    // Para que el valor de retorno no dependa de la inicialización del thread pool, utilizamos auto
    // en vez de std::future<void> o std::future<int> etc.
    //std::future<void> f1b = hssThreadPool.createTask(task1);    
    auto f1b = hssThreadPool.createTask(task1);   
    auto f1c = hssThreadPool.createTask(task1);    
    auto f1d = hssThreadPool.createTask(task1);    
       
    auto f2 = hssThreadPool.createTask(task2);
    auto f3 = hssThreadPool.createTask(task4);
    auto f4 = hssThreadPool.createTask(task1);
    auto f5 = hssThreadPool.createTask(task4);
    auto f6 = hssThreadPool.createTask(task4);
    auto f7 = hssThreadPool.createTask(task3);

    auto f8 = hssThreadPool.createTask(task2);

    auto f9 = hssThreadPool.createTask(task4);
    auto f10 = hssThreadPool.createTask(task2);

    std::cout << "start main .get() calls" << std::endl;
    int res1a = f1a.get();
    std::cout << "res1a: " << res1a << std::endl;
    f1b.get();
    f1c.get();
    f1d.get();
    f2.get();
    f3.get();
    f4.get();
    f5.get();
    f6.get();
    f7.get();
    f8.get();
    f9.get();
    f10.get();
    EXPECT_EQ(true, true);
  }
  catch (...)
  {
    std::cerr << "GT_tp_GC: " << std::endl;
    ADD_FAILURE_AT("GT_tp.cpp", 31);
  }
}
