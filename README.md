# `linconGaussR`: partial R implementation of `LinConGauss`
###  _samples of Gaussians under linear domain constraints_

A header-only R package implementing `LinConGauss` in C++ and R. 

The original Python implementation can be found [here](https://github.com/alpiges/LinConGauss). The reference of `LinConGauss` is this [paper](https://arxiv.org/abs/1910.09328). It's very likely this repo will only implement the sampling methods in the near future.

It is not on CRAN and probably won't, to install, use

```r
devtools::install_github("YunyiShen/linconGaussR")
```

An example would be:

```r
my_sample <- linconGauss(100, diag(2),c(0,0),diag(2),c(0,0))
MASS_sample <- MASS::mvrnorm(1000,c(0,0),diag(2))
plot(MASS_sample)
points(my_sample,col = "red")
abline(h=0)
abline(v=0)
```

For developers: This package was written as header-only, with all the sampling method/classes used in the `inst/include` directory. If you just wish to use the C++ API of this implementation, consider install this package, and add `linconGaussR` (for sure `RcppArmadillo` and `Rcpp`) to your `LinkingTo` field of the description. The main sampling function is `linconGaussR::linconGauss_cpp`. Below is a sample implementation calling this method only:

```cpp
// [[Rcpp::depends(RcppArmadillo)]]
#include <linconGaussR.h>// we call RcppArmadillo

using namespace Rcpp;
using namespace arma;
using namespace std;
using namespace linconGaussR;


//[[Rcpp::export]]
arma::mat linconGauss_cpp(int n, // sample size
                            arma::mat A, // linear constraint
                            arma::vec b, // offset of the linear constraint, so that Ax+b>0
                            arma::mat Sigma, // covariance
                            arma::vec mu, // mean
                            arma::vec x_init, // an initial value, necessary
                            bool intersection=true, // whether to sample from the intersection (otherwise from the joint)
                            int nskp=5){ // number of sample to skip during iterations
    return linconGaussR::linconGauss_cpp(n,A,b,Sigma,mu,x_init,intersection,nskp);
}

```
