#ifndef INTEGRATION_LOOP_H
#define INTEGRATION_LOOP_H

#include "loop.h"

class IntegrationLoop: public Loop {
    public:
        int n_samples;
        int dim;
        IntegrationLoop() = default;
        IntegrationLoop(LinearConstraints linear_con, int nsamples, int nskip): Loop(linear_con, nskip){
            n_samples = nsamples;
            dim = linear_con.D;
        }
        void run();
};

#endif