// base class for loop

#ifndef LOOP_H
#define LOOP_H

#include "linear_constraints.h"

class Loop{
    public:
        LinearConstraints lincon;
        int n_skip;
    Loop(){
        LinearConstraints linear_con;
        lincon = linear_con;
        n_skip = 0;
    }
    Loop(LinearConstraints linear_con, int nskip){
        lincon = linear_con;
        n_skip = nskip;
    }
    void run(){
        return;
    }
};


class SamplingLoop: public Loop{
    public: 
        int n_iterations;
        SamplingLoop(int n_iter, LinearConstraints linear_con, int nskip){
            n_iterations = n_iter;
            lincon = linear_con;
            n_skip = nskip;
        }
        arma::mat compute_next_point(arma::mat x0){
            return x0;
        }
};


#endif