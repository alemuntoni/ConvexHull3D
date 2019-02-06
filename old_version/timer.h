#ifndef TIMER_H
#define TIMER_H

#include <QString>

#include <time.h>
#include <iostream>

using namespace std;

class Timer
{

        public:

                Timer (QString _caption) : caption(_caption)
                {
                        secs = 0.f;

                        start();
                }


                inline void start()
                {
                        _start = clock();
                }


                inline void stop_and_print()
                {
                        _stop = clock();

                        float cycles = _stop - _start;

                        secs = ((float)cycles) / ((float)CLOCKS_PER_SEC);

                        cout << caption.toStdString() << " [" << secs << " secs]" << endl;
                }


                inline float delay()
                {
                        return secs;
                }





        private:

                QString caption;

                clock_t _start, _stop;

                float secs;
};

#endif // TIMER_H
