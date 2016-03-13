#ifndef MATHHELPER_H
#define MATHHELPER_H

#include <cmath>
#include <complex>

namespace _MYNAMESPACE_
{
    namespace  MPI
    {
        inline bool operator < ( std::complex<float> &lhs, std::complex<float>& rhs)
        {
            return std::norm( lhs ) < std::norm( rhs );
        }

        inline bool operator > ( std::complex<float> &lhs, std::complex<float>& rhs)
        {
            return std::norm( lhs ) > std::norm( rhs );
        }

        inline bool operator >= ( std::complex<float> &lhs, std::complex<float>& rhs)
        {
            return !( lhs  <  rhs );
        }

        inline bool operator <= ( std::complex<float> &lhs, std::complex<float>& rhs)
        {
            return !( lhs  >  rhs );
        }

        inline bool operator < ( std::complex<double> &lhs, std::complex<double>& rhs)
        {
            return std::norm( lhs ) < std::norm( rhs );
        }

        inline bool operator > ( std::complex<double> &lhs, std::complex<double>& rhs)
        {
            return std::norm( lhs ) > std::norm( rhs );
        }

        inline bool operator >= ( std::complex<double> &lhs, std::complex<double>& rhs)
        {
            return !( lhs  <  rhs );
        }

        inline bool operator <= ( std::complex<double> &lhs, std::complex<double>& rhs)
        {
            return !( lhs  >  rhs );
        }
    }
}

#endif // MATHHELPER_H
