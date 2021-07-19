#include "Individual.h"

Individual::Individual(int m_in) :  v(0.5),
                                    m(m_in) {
    static rnd::rng coordRng;
    coordinates.x = rnd::randomInt(0, m)(coordRng);
    coordinates.y = rnd::randomInt(0, m)(coordRng);
                                
}

Individual::Individual(int m_in, double v_in) : m(m_in),
                                                v(v_in) {
    static rnd::rng coordRng;
    coordinates.x = rnd::randomInt(0, m)(coordRng);
    coordinates.y = rnd::randomInt(0, m)(coordRng);
}


/*Individual::~Individual() // in case I need a destructor, might come in handy for new generation?
{
}*/