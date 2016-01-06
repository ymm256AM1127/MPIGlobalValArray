#ifndef VALARRAYIMPL_H
#define VALARRAYIMPL_H

#include "../ValArray.h"
#include "../Communicator.h"

namespace _MYNAMESPACE_
{
    namespace MPI
    {
        template < class T >
        ValArray<T>::ValArray( CommPtr comm,
                               const std::size_t size,
                               const std::size_t localhalosize,
                               const std::string &windowobjectname ) :
            WindowObject<T>( comm, size, localhalosize, windowobjectname )
        {

        }

        template < class T >
        ValArray<T>::ValArray( const ValArray &rhs ) :
            WindowObject<T>( static_cast< const WindowObject<T> >(rhs) )
        {

        }

        template < class T >
        ValArray<T>& ValArray<T>::operator =(const ValArray<T> &rhs)
        {
            return static_cast< ValArray<T>& >( WindowObject<T>::operator =( rhs ) );
        }

        template < class T >
        ValArray<T>& ValArray<T>::operator+=( const ValArray &rhs )
        {
            auto lhsptr = this->GetBasePtr();
            auto rhsptr = rhs.GetBasePtr();
            for( auto ii = 0UL; ii < this->GetLocalSize(); ii++ )
            {
                lhsptr[ii] += rhsptr[ii];
            }

            return *this;
        }

        template < class T >
        ValArray<T>& ValArray<T>::operator-=( const ValArray &rhs )
        {
            auto lhsptr = this->GetBasePtr();
            auto rhsptr = rhs.GetBasePtr();
            for( auto ii = 0UL; ii < this->GetLocalSize(); ii++ )
            {
                lhsptr[ii] -= rhsptr[ii];
            }

            return *this;
        }

        template < class T >
        ValArray<T>& ValArray<T>::operator*=( const ValArray &rhs )
        {
            auto lhsptr = this->GetBasePtr();
            auto rhsptr = rhs.GetBasePtr();
            for( auto ii = 0UL; ii < this->GetLocalSize(); ii++ )
            {
                lhsptr[ii] *= rhsptr[ii];
            }

            return *this;
        }

        template < class T >
        ValArray<T>& ValArray<T>::operator/=( const ValArray &rhs )
        {
            auto lhsptr = this->GetBasePtr();
            auto rhsptr = rhs.GetBasePtr();
            for( auto ii = 0UL; ii < this->GetLocalSize(); ii++ )
            {
                lhsptr[ii] /= rhsptr[ii];
            }

            return *this;
        }

        template < class T >
        template< class expression >
        ValArray<T>& ValArray<T>::operator=( const expression& rhs )
        {
            auto lhsptr = this->GetBasePtr();
            for( auto ii = 0UL; ii < this->GetLocalSize(); ii++ )
            {
                lhsptr[ii] = rhs[ii];
            }

            return *this;
        }


        template < class T >
        T ValArray<T>::sum()
        {
            T val = 0.0;
            for( auto ii = 0UL; ii < this->GetLocalSize(); ii++ )
            {
                val += this->GetBasePtr()[ii];
            }
            CommPtr Comm = this->GetCommPtr();
            Comm->Barrier();
            T out = 0.0;
            Comm->Allreduce<T>( val, out, MPI_SUM, 1 );
            return out;
        }
    }
}

#endif // VALARRAYIMPL_H
