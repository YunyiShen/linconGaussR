#ifndef ELLIPTICAL_SLICE_SAMPLING_H
#define ELLIPTICAL_SLICE_SAMPLING_H

#include "active_intersections.h"
#include "angle_sampler.h"
#include "ellipse.h"
#include "linear_constraints.h"
#include "loop.h"

#include <math.h>
#include <RcppArmadillo.h>

using namespace Rcpp;
using namespace arma;
using namespace std;

class EllipticalSliceSampler : public SamplingLoop
{
public:
    int dim;
    SamplerState loop_state;
    EllipticalSliceSampler() = default;
    EllipticalSliceSampler(int n_iter,
                           LinearConstraints linear_con,
                           int nskip,
                           arma::vec x_init) : loop_state(x_init),
                                               SamplingLoop(n_iter, linear_con, nskip)
    {
        dim = linear_con.D; // python version it is called N_dim
    };
    bool is_converged();
    arma::vec compute_next_point(arma::vec x0);
    void run();
};

bool EllipticalSliceSampler::is_converged()
{
    return loop_state.iteration >= n_iterations;
}

arma::vec EllipticalSliceSampler::compute_next_point(arma::vec x0)
{
    /* """
        Computes the next sample from the linearly constrained unit Gaussian
        :param x0: current state
        :return: new state
        """ */
    arma::vec x1(dim, arma::fill::randn);
    Ellipse ellipse(x0, x1);
    ActiveIntersections active_intersections(ellipse, lincon);
    AngleSampler slice_sampler(active_intersections);
    if (!active_intersections.ellipse_in_domain)
    {
        Rcpp::stop("ellipse is outside of integration domain, reconstruct a new ellipse (should not happen at all!)");
    }
    double t_new = slice_sampler.draw_angle();
    return ellipse.x(t_new);
}

/* while not self.is_converged():
            x = self.loop_state.samples[-1]
            for i in range(self.n_skip + 1):
                x = self.compute_next_point(x)
                while not self.lincon.integration_domain(x):
                    print('Point outside domain, resample')
                    x = self.compute_next_point(self.loop_state.samples[-1])

            self.loop_state.update(x) */

void EllipticalSliceSampler::run()
{
    /* """
        Sample from a linearly constrained unit Gaussian until stopping criterion is reached.
        :return: None
        """ */
    arma::vec x;
    while (!this->is_converged())
    {
        int n_sample = loop_state.samples.n_rows;
        x = trans(loop_state.samples.row(n_sample - 1));
        for (int i = 0; i <= n_skip; i++)
        {
            x = this->compute_next_point(x);
            while (arma::as_scalar(lincon.integration_domain(x)) != 1)
            {
                x = this->compute_next_point(x);
            }
        }
        loop_state.update(x);
    }
}

#endif