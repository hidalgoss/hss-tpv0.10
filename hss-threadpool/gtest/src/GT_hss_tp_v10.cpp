#include <gtest/gtest.h>
#include <iostream>
#include <hss_observer_middleware_v10.hpp>
#include "hss_sensor_v10.hpp"
#include <hss_bmrk_utils.h>


TEST(TS_hssObserverPattern, GT_hss_observer_middleware_v10_GC1)
{
    try
    {
        hss::TmObserverMiddleware<void, hss::Sensor> communicator(4);

        hss::Subscriber<void, hss::Sensor> *subscriber1 =
                communicator.getSubscriber("event 1");

        hss::Sensor sensor_result1;
        int id_sensor1 = sensor_result1.id_sensor;

        auto fx1 = [&sensor_result1](const hss::Sensor & sensor)
        {
            DEBUG_MSG("Update() sensor rec id: *" << sensor.id_sensor
                      << "* threadID: *" << std::this_thread::get_id() << "*");
            sensor_result1 = sensor;
        };

        subscriber1->registerUpdate( fx1 );

        // TODO - 20.05.22
        // El problema que me aparece con este diseño es que el getPublisher() y getSubscriber()
        // muestran metodos al usuario de este main que no deberían ser accesibles como Update()
        // Subscribe(), etc. Hay que ver como ocultar estos metodos. Puede que haya que introducir
        // alguna clase intermedia que herede y oculte esta implementación .... a ver...
        //communicator.getSubscriber()->

        hss::Sensor sensor_result2;
        int id_sensor2 = sensor_result2.id_sensor;
        auto fx2 = [&sensor_result2](const hss::Sensor & sensor)
        {
            DEBUG_MSG("Update() sensor rec id: *" << sensor.id_sensor
                      << "* threadID: *" << std::this_thread::get_id() << "*");
            sensor_result2 = sensor;
        };
        hss::Subscriber<void, hss::Sensor> *subscriber2 =
                communicator.getSubscriber("event 1");
        subscriber2->registerUpdate( fx2 );

        hss::Sensor sensor_result3;
        int id_sensor3 = sensor_result3.id_sensor;
        auto fx3 = [&sensor_result3](const hss::Sensor & sensor)
        {
            DEBUG_MSG("Update() sensor rec id: *" << sensor.id_sensor
                      << "* threadID: *" << std::this_thread::get_id() << "*");
            sensor_result3 = sensor;
        };
        hss::Subscriber<void, hss::Sensor> *subscriber3 =
                communicator.getSubscriber("event 2");
        subscriber3->registerUpdate( fx3 );

        hss::Sensor sensor1(0.1, 0.2, 0.3, 0.0);
        hss::Sensor sensor2(2.1, 2.2, 2.3, 0.0);
        hss::Sensor sensor3(3.1, 3.2, 3.3, 0.0);

        // Notified expected over Subs 1 & 2, not 3.
        std::vector<std::future<void>> futures_s1 =
                communicator.getPublisher()->Notify("event 1", std::forward<hss::Sensor>(sensor1));
        for(auto &f : futures_s1){
            f.get();
        }

        EXPECT_EQ(sensor_result1._xt, sensor1._xt);
        EXPECT_EQ(sensor_result1._yt, sensor1._yt);
        EXPECT_EQ(sensor_result1._zt, sensor1._zt);
        EXPECT_EQ(sensor_result1.id_sensor, sensor1.id_sensor);

        EXPECT_EQ(sensor_result2._xt, sensor1._xt);
        EXPECT_EQ(sensor_result2._yt, sensor1._yt);
        EXPECT_EQ(sensor_result2._zt, sensor1._zt);
        EXPECT_EQ(sensor_result2.id_sensor, sensor1.id_sensor);

        EXPECT_NE(sensor_result3._xt, sensor1._xt);
        EXPECT_NE(sensor_result3._yt, sensor1._yt);
        EXPECT_NE(sensor_result3._zt, sensor1._zt);
        EXPECT_EQ(sensor_result3.id_sensor, id_sensor3);

        // Set initial conditions in sensor_result id's.
        sensor_result1.id_sensor = id_sensor1;
        sensor_result2.id_sensor = id_sensor2;

        // Notified expected over Subs 1 & 2, not 3.
        std::vector<std::future<void>> futures_s2 =
                communicator.getPublisher()->Notify("event 1", std::forward<hss::Sensor>(sensor2));
        for(auto &f : futures_s2){
            f.get();
        }

        EXPECT_EQ(sensor_result1._xt, sensor2._xt);
        EXPECT_EQ(sensor_result1._yt, sensor2._yt);
        EXPECT_EQ(sensor_result1._zt, sensor2._zt);
        EXPECT_EQ(sensor_result1.id_sensor, sensor2.id_sensor);

        EXPECT_EQ(sensor_result2._xt, sensor2._xt);
        EXPECT_EQ(sensor_result2._yt, sensor2._yt);
        EXPECT_EQ(sensor_result2._zt, sensor2._zt);
        EXPECT_EQ(sensor_result2.id_sensor, sensor2.id_sensor);

        EXPECT_NE(sensor_result3._xt, sensor2._xt);
        EXPECT_NE(sensor_result3._yt, sensor2._yt);
        EXPECT_NE(sensor_result3._zt, sensor2._zt);
        EXPECT_EQ(sensor_result3.id_sensor, id_sensor3);

        // Set initial conditions in sensor_result id's.
        sensor_result1.id_sensor = id_sensor1;
        sensor_result2.id_sensor = id_sensor2;

        std::vector<std::future<void>> futures_s3 =
                communicator.getPublisher()->Notify("event 2", std::forward<hss::Sensor>(sensor3));
        for(auto &f : futures_s3){
            f.get();
        }

        EXPECT_EQ(sensor_result1.id_sensor, id_sensor1);
        EXPECT_EQ(sensor_result2.id_sensor, id_sensor2);

        EXPECT_EQ(sensor_result3._xt, sensor3._xt);
        EXPECT_EQ(sensor_result3._yt, sensor3._yt);
        EXPECT_EQ(sensor_result3._zt, sensor3._zt);
        EXPECT_EQ(sensor_result3.id_sensor, sensor3.id_sensor);

        //std::this_thread::sleep_for(std::chrono::milliseconds(500));
        communicator.stopAndFinish();
        std::cout << "sale " << std::endl;
    }
    catch (...)
    {
        std::cerr << "GT_hss_observer_middleware_v7_GC: " << std::endl;
        ADD_FAILURE_AT("GT_hss_tp_v7.cpp", 189);
    }
}



TEST(TS_hssObserverPattern, GT_hss_observer_middleware_v10_GC2)
{
    try
    {
        hss::TmObserverMiddleware<int, hss::Sensor> communicator(4);

        hss::Subscriber<int, hss::Sensor> *subscriber1 =
                communicator.getSubscriber("event 1");

        hss::Sensor sensor_result1;
        int id_sensor1 = sensor_result1.id_sensor;

        auto fx1 = [&sensor_result1](const hss::Sensor & sensor) -> int
        {
            DEBUG_MSG("Update() sensor rec id: *" << sensor.id_sensor
                      << "* threadID: *" << std::this_thread::get_id() << "*");
            sensor_result1 = sensor;
            return sensor.id_sensor;
        };

        subscriber1->registerUpdate( fx1 );

        // TODO - 20.05.22
        // El problema que me aparece con este diseño es que el getPublisher() y getSubscriber()
        // muestran metodos al usuario de este main que no deberían ser accesibles como Update()
        // Subscribe(), etc. Hay que ver como ocultar estos metodos. Puede que haya que introducir
        // alguna clase intermedia que herede y oculte esta implementación .... a ver...
        //communicator.getSubscriber()->

        hss::Sensor sensor_result2;
        int id_sensor2 = sensor_result2.id_sensor;
        auto fx2 = [&sensor_result2](const hss::Sensor & sensor) -> int
        {
            DEBUG_MSG("Update() sensor rec id: *" << sensor.id_sensor
                      << "* threadID: *" << std::this_thread::get_id() << "*");
            sensor_result2 = sensor;
            return sensor.id_sensor;
        };
        hss::Subscriber<int, hss::Sensor> *subscriber2 =
                communicator.getSubscriber("event 1");
        subscriber2->registerUpdate( fx2 );

        hss::Sensor sensor_result3;
        int id_sensor3 = sensor_result3.id_sensor;
        auto fx3 = [&sensor_result3](const hss::Sensor & sensor) -> int
        {
            DEBUG_MSG("Update() sensor rec id: *" << sensor.id_sensor
                      << "* threadID: *" << std::this_thread::get_id() << "*");
            sensor_result3 = sensor;
            return sensor.id_sensor;
        };
        hss::Subscriber<int, hss::Sensor> *subscriber3 =
                communicator.getSubscriber("event 2");
        subscriber3->registerUpdate( fx3 );

        hss::Sensor sensor1(0.1, 0.2, 0.3, 0.0);
        hss::Sensor sensor2(2.1, 2.2, 2.3, 0.0);
        hss::Sensor sensor3(3.1, 3.2, 3.3, 0.0);

        // Notified expected over Subs 1 & 2, not 3.
        std::vector<std::future<int>> futures_s1 =
                communicator.getPublisher()->Notify("event 1", std::forward<hss::Sensor>(sensor1));
        for(auto &f : futures_s1){
            int val = f.get();
        }

        EXPECT_EQ(sensor_result1._xt, sensor1._xt);
        EXPECT_EQ(sensor_result1._yt, sensor1._yt);
        EXPECT_EQ(sensor_result1._zt, sensor1._zt);
        EXPECT_EQ(sensor_result1.id_sensor, sensor1.id_sensor);

        EXPECT_EQ(sensor_result2._xt, sensor1._xt);
        EXPECT_EQ(sensor_result2._yt, sensor1._yt);
        EXPECT_EQ(sensor_result2._zt, sensor1._zt);
        EXPECT_EQ(sensor_result2.id_sensor, sensor1.id_sensor);

        EXPECT_NE(sensor_result3._xt, sensor1._xt);
        EXPECT_NE(sensor_result3._yt, sensor1._yt);
        EXPECT_NE(sensor_result3._zt, sensor1._zt);
        EXPECT_EQ(sensor_result3.id_sensor, id_sensor3);

        // Set initial conditions in sensor_result id's.
        sensor_result1.id_sensor = id_sensor1;
        sensor_result2.id_sensor = id_sensor2;

        // Notified expected over Subs 1 & 2, not 3.
        std::vector<std::future<int>> futures_s2 =
                communicator.getPublisher()->Notify("event 1", std::forward<hss::Sensor>(sensor2));
        for(auto &f : futures_s2){
            int val = f.get();
        }

        EXPECT_EQ(sensor_result1._xt, sensor2._xt);
        EXPECT_EQ(sensor_result1._yt, sensor2._yt);
        EXPECT_EQ(sensor_result1._zt, sensor2._zt);
        EXPECT_EQ(sensor_result1.id_sensor, sensor2.id_sensor);

        EXPECT_EQ(sensor_result2._xt, sensor2._xt);
        EXPECT_EQ(sensor_result2._yt, sensor2._yt);
        EXPECT_EQ(sensor_result2._zt, sensor2._zt);
        EXPECT_EQ(sensor_result2.id_sensor, sensor2.id_sensor);

        EXPECT_NE(sensor_result3._xt, sensor2._xt);
        EXPECT_NE(sensor_result3._yt, sensor2._yt);
        EXPECT_NE(sensor_result3._zt, sensor2._zt);
        EXPECT_EQ(sensor_result3.id_sensor, id_sensor3);

        // Set initial conditions in sensor_result id's.
        sensor_result1.id_sensor = id_sensor1;
        sensor_result2.id_sensor = id_sensor2;

        std::vector<std::future<int>> futures_s3 =
                communicator.getPublisher()->Notify("event 2", std::forward<hss::Sensor>(sensor3));
        for(auto &f : futures_s3){
            int val = f.get();
        }

        EXPECT_EQ(sensor_result1.id_sensor, id_sensor1);
        EXPECT_EQ(sensor_result2.id_sensor, id_sensor2);

        EXPECT_EQ(sensor_result3._xt, sensor3._xt);
        EXPECT_EQ(sensor_result3._yt, sensor3._yt);
        EXPECT_EQ(sensor_result3._zt, sensor3._zt);
        EXPECT_EQ(sensor_result3.id_sensor, sensor3.id_sensor);

        //std::this_thread::sleep_for(std::chrono::milliseconds(500));
        communicator.stopAndFinish();
        std::cout << "sale " << std::endl;
    }
    catch (...)
    {
        std::cerr << "GT_hss_observer_middleware_v7_GC: " << std::endl;
        ADD_FAILURE_AT("GT_hss_tp_v7.cpp", 189);
    }
}

