#ifndef LINCONGAUSSR_NESTING_H
#define LINCONGAUSSR_NESTING_H

#include "linear_constraints.h"
#include "elliptical_slice_sampling.h"

using namespace Rcpp;
using namespace arma;
using namespace std;

namespace linconGaussR{

class shiftind {
    public:
        arma::vec shift;
        arma::uvec idx;
        shiftind() = default;
        shiftind(arma::vec shift, arma::uvec idx): shift(shift), idx(idx){}
};

class Nesting{
    public: 
        Nesting() = default;
};


/* """
        One nested domain for HDR
        :param linear_constraints: instance of linear constraints
        :param shift: shift defining the nesting
        """ */
class HDRNesting : public Nesting{
    public:
        ShiftedLinearConstraints shifted_lincon;
        arma::vec shift;
        int dim;
        double log_conditional_probability;

        HDRNesting() = default;
        HDRNesting(arma::mat A, arma::vec b, arma::vec shift) :shifted_lincon(A,b,shift) ,shift(shift){
            dim = shifted_lincon.D;
        }

        inline arma::mat sample_from_nesting(int n_samples, arma::vec x_init, int n_skip);
        inline void compute_log_nesting_factor(arma::mat X){// X stores COLUMN vectors!!
            log_conditional_probability = log(accu(shifted_lincon.integration_domain(X))) - log((double) X.n_rows );
        }
};

inline arma::mat HDRNesting::sample_from_nesting(int n_samples, arma::vec x_init, int n_skip){
    EllipticalSliceSampler sampler(n_samples, shifted_lincon, n_skip, x_init);
    sampler.run();
    return sampler.loop_state.samples;
}

/* 
        """
        Constructs a nesting given linear constraints and a fraction of samples that should lie inside the new nesting.
        Takes the samples as input since the subset is constructed directly from samples. Note the difference to
        HDRNesting, which is pre-constructed given shift values.
        :param fraction: Fraction of samples that should lie in the new domain
        :param linear_constraints: instance of LinearConstraints
        :param n_save: number of samples to save from inside the domain
        """ */


class SubsetNesting: public Nesting{
    public:
        double fraction;
        LinearConstraints lincon;
        int n_save;
        int n_inside; 
        double log_conditional_probability;
        arma::vec shift;
        arma::vec x_in;// not sure if it is indeed vec, seems in the py implementation they vectorized things
        ShiftedLinearConstraints shifted_lincon;

        SubsetNesting() = default;
        SubsetNesting(LinearConstraints lin_con, double fraction, int n_save = 1): lincon(lin_con), fraction(fraction), n_save(n_save){}

        inline void compute_log_nesting_factor(arma::mat X);
        inline arma::mat sample_from_nesting(int n, arma::vec x_init, int n_skip);
        inline void update_properties_from_samples(arma::mat X);

    private:
        inline shiftind _update_find_shift(arma::vec shiftvals); // nor sure yet if shiftvals should be vec 
        inline arma::uvec _update_fix_shift(arma::vec shift, arma::vec shiftvals); // like above

};







}





#endif