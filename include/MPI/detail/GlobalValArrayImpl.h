#ifndef GLOBALVALARRAYIMPL_H
#define GLOBALVALARRAYIMPL_H

#include "../GlobalValArray.h"
#include "../Communicator.h"

namespace _MYNAMESPACE_
{
    namespace MPI
    {
        template < class Policy >
        GlobalValArray< Policy >::GlobalValArray( typename GlobalValArray< Policy >::CommPtr comm,
                               const std::size_t size,
                               const std::size_t localhalosize,
                               const std::string &windowobjectname ) :
            Policy( comm, size, localhalosize, windowobjectname )
        {

        }

        template < class Policy >
        GlobalValArray<Policy>::GlobalValArray( const GlobalValArray &rhs ) :
            Policy( static_cast< const Policy >(rhs) )
        {

        }

        template < class Policy >
        GlobalValArray<Policy>& GlobalValArray<Policy>::operator =(const GlobalValArray<Policy> &rhs)
        {
            return static_cast< GlobalValArray<Policy>& >( Policy::operator =( rhs ) );
        }

        template < class Policy >
        GlobalValArray<Policy>& GlobalValArray<Policy>::operator+=( const GlobalValArray &rhs )
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
        GlobalValArray<Policy>& GlobalValArray<Policy>::operator-=( const GlobalValArray &rhs )
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
        GlobalValArray<Policy>& GlobalValArray<Policy>::operator*=( const GlobalValArray &rhs )
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
        GlobalValArray<Policy>& GlobalValArray<Policy>::operator/=( const GlobalValArray &rhs )
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
        GlobalValArray<Policy>& GlobalValArray<Policy>::operator=( const expression& rhs )
        {
            auto lhsptr = this->GetBasePtr();
            for( auto ii = 0UL; ii < this->GetLocalSize(); ii++ )
            {
                lhsptr[ii] = rhs[ii];
            }

            return *this;
        }


        template < class Policy >
        const typename GlobalValArray<Policy>::value_type GlobalValArray<Policy>::sum() const
        {
            using result_type = typename Policy::value_type;
            result_type val = 0.0;
            for( auto ii = 0UL; ii < this->GetLocalSize(); ii++ )
            {
                val += this->GetBasePtr()[ii];
            }
            CommPtr Comm = this->GetCommPtr();
            Comm->Barrier();
            typename Policy::value_type out = 0.0;
            Comm->Allreduce<typename Policy::value_type>( val,
                                                          out,
                                                          MPIOpType<typename Policy::value_type, SUM >(),
                                                          1 );
            return static_cast< const result_type >(out);
        }

        template < class Policy >
        const typename GlobalValArray<Policy>::value_type GlobalValArray<Policy>::inner_product( const GlobalValArray& rhs ) const
        {
            using result_type = typename Policy::value_type;
            result_type val = ZEROVALUE<result_type>();
            const auto lhsptr = this->GetBasePtr();
            const auto rhsptr = rhs.GetBasePtr();
            for( auto ii = 0UL; ii < this->GetLocalSize(); ii++ )
            {
                val += lhsptr[ii] * rhsptr[ii];
            }
            CommPtr Comm = this->GetCommPtr();
            Comm->Barrier();
            typename Policy::value_type out = 0.0;
            Comm->Allreduce<typename Policy::value_type>( val,
                                                          out,
                                                          MPIOpType<typename Policy::value_type, SUM >(),
                                                          1 );
            return static_cast< const result_type >(out);
        }

        template < class Policy >
        const typename GlobalValArray<Policy>::value_type GlobalValArray<Policy>::norm() const
        {
            using result_type = typename GlobalValArray<Policy>::value_type;
            result_type   val = ZeroType<result_type>();

            for( auto ii = 0UL; ii < this->GetLocalSize(); ii++ )
            {
                val += std::abs( this->GetBasePtr()[ii] );
            }
            CommPtr Comm = this->GetCommPtr();
            Comm->Barrier();
            result_type out = 0.0;
            Comm->Allreduce< result_type >( val,
                                            out,
                                            MPIOpType< result_type, SUM >(),
                                            1 );
            return static_cast< const result_type >(out);
        }

        template < class Policy >
        const typename GlobalValArray<Policy>::value_type GlobalValArray<Policy>::norm2() const
        {
            using result_type = typename GlobalValArray<Policy>::value_type;
            result_type   val = ZeroType<result_type>();

            for( auto ii = 0UL; ii < this->GetLocalSize(); ii++ )
            {
                val += ( this->GetBasePtr()[ii] * this->GetBasePtr()[ii] );
            }

            CommPtr Comm = this->GetCommPtr();
            Comm->Barrier();
            result_type out = 0.0;
            Comm->Allreduce<result_type>( val,
                                          out,
                                          MPIOpType<result_type, SUM >(),
                                          1 );
            return static_cast< const result_type >(out);
        }
    }
}

#endif // GLOBALVALARRAYIMPL_H
