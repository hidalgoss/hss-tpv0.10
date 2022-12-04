#ifndef SENSOR_V10_H
#define SENSOR_V10_H

#include <iostream>


#ifdef DEBUG
#define DEBUG_MSG(str) do { std::cout << str << std::endl; } while( false )
#else
#define DEBUG_MSG(str) do { } while ( false )
#endif

namespace hss
{

// Sensor para la ecuación de trayectoria general en R3, en función del tiempo:
// |r(t)| = x(t)|i| + y(t)|j| + z(t)|k|;
// Es responsabilidad del usuario establecer la base R3 sobre la que trabaja el origen
// de coordenadas, así como la dirección, sentido y magnitud de los vectores para cada
// una de las coordenadas.
struct Sensor {
    static int last_id_sensor;
    int id_sensor;
    double _xt;
    double _yt;
    double _zt;
    double _t;

    Sensor()
    {
        this->id_sensor = ++last_id_sensor;
        this->_xt = 0;
        this->_yt = 0;
        this->_zt = 0;
        this->_t = 0;
    }


    Sensor(double xt, double yt, double zt, double t)
    {
        this->id_sensor = ++last_id_sensor;
        this->_xt = xt;
        this->_yt = yt;
        this->_zt = zt;
        this->_t = t;
    }

    Sensor(Sensor & sensor)
    {
        this->id_sensor = sensor.id_sensor;
        this->_xt = sensor._xt;
        this->_yt = sensor._yt;
        this->_zt = sensor._zt;
        this->_t =  sensor._t;
    }

    Sensor(Sensor && sensor)
    {
        this->id_sensor = sensor.id_sensor;
        this->_xt = sensor._xt;
        this->_yt = sensor._yt;
        this->_zt = sensor._zt;
        this->_t =  sensor._t;

//        sensor.id_sensor = -1;
//        sensor._xt = 0;
//        sensor._yt = 0;
//        sensor._zt = 0;
//        sensor._t  = 0;
    }

    Sensor &operator=(const Sensor & sensor)
    {
        this->id_sensor = sensor.id_sensor;
        this->_xt = sensor._xt;
        this->_yt = sensor._yt;
        this->_zt = sensor._zt;
        this->_t =  sensor._t;
        return *this;
    }

    int idSensor(){
        return this->id_sensor;
    }

    void setIdSensor(int id_sensor){
        this->id_sensor = id_sensor;
    }
};//end class Sensor.

int Sensor::last_id_sensor = 0;

} // end namespace hss
#endif

