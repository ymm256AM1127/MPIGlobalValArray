#ifndef VALARRAYIMPL_H
#define VALARRAYIMPL_H

#include "../ValArray.h"

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
    }
}

#endif // VALARRAYIMPL_H
