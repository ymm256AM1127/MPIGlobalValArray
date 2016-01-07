#ifndef VALARRAYIMPL_H
#define VALARRAYIMPL_H

#include "../ValArray.h"
#include "../Communicator.h"

namespace _MYNAMESPACE_
{
    namespace MPI
    {
        template < class Policy >
        ValArray<Policy>::ValArray( CommPtr comm,
                               const std::size_t size,
                               const std::size_t localhalosize,
                               const std::string &windowobjectname ) :
            Policy( comm, size, localhalosize, windowobjectname )
        {

        }

        template < class Policy >
        ValArray<Policy>::ValArray( const ValArray &rhs ) :
            Policy( static_cast< const Policy >(rhs) )
        {

        }

        template < class Policy >
        ValArray<Policy>& ValArray<Policy>::operator =(const ValArray<Policy> &rhs)
        {
            return static_cast< ValArray<Policy>& >( Policy::operator =( rhs ) );
        }

        template < class Policy >
        ValArray<Policy>& ValArray<Policy>::operator+=( const ValArray &rhs )
        {
            auto lhsptr = this->GetBasePtr();
            auto rhsptr = rhs.GetBasePtr();
            for( auto ii = 0UL; ii < this->GetLocalSize(); ii++ )
            {
                lhsptr[ii] += rhsptr[ii];
            }

            return *this;
        }

        template < class Policy >
        ValArray<Policy>& ValArray<Policy>::operator-=( const ValArray &rhs )
        {
            auto lhsptr = this->GetBasePtr();
            auto rhsptr = rhs.GetBasePtr();
            for( auto ii = 0UL; ii < this->GetLocalSize(); ii++ )
            {
                lhsptr[ii] -= rhsptr[ii];
            }

            return *this;
        }

        template < class Policy >
        ValArray<Policy>& ValArray<Policy>::operator*=( const ValArray &rhs )
        {
            auto lhsptr = this->GetBasePtr();
            auto rhsptr = rhs.GetBasePtr();
            for( auto ii = 0UL; ii < this->GetLocalSize(); ii++ )
            {
                lhsptr[ii] *= rhsptr[ii];
            }

            return *this;
        }

        template < class Policy >
        ValArray<Policy>& ValArray<Policy>::operator/=( const ValArray &rhs )
        {
            auto lhsptr = this->GetBasePtr();
            auto rhsptr = rhs.GetBasePtr();
            for( auto ii = 0UL; ii < this->GetLocalSize(); ii++ )
            {
                lhsptr[ii] /= rhsptr[ii];
            }

            return *this;
        }

        template < class Policy >
        template< class expression >
        ValArray<Policy>& ValArray<Policy>::operator=( const expression& rhs )
        {
            auto lhsptr = this->GetBasePtr();
            for( auto ii = 0UL; ii < this->GetLocalSize(); ii++ )
            {
                lhsptr[ii] = rhs[ii];
            }

            return *this;
        }


        template < class Policy >
        typename ValArray<Policy>::value_type ValArray<Policy>::sum()
        {
            typename Policy::value_type val = 0.0;
            for( auto ii = 0UL; ii < this->GetLocalSize(); ii++ )
            {
                val += this->GetBasePtr()[ii];
            }
            CommPtr Comm = this->GetCommPtr();
            Comm->Barrier();
            typename Policy::value_type out = 0.0;
            Comm->Allreduce<typename Policy::value_type>( val, out, MPI_SUM, 1 );
            return out;
        }
    }
}

#endif // VALARRAYIMPL_H
