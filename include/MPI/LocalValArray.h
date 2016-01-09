#ifndef LOCALVALARRAY_H
#define LOCALVALARRAY_H

#include <sstream>
#include <vector>
#include "../MPL/meta_function.h"
#include "ExpressionTemplate.h"
#include "../Utility/ForwardAccessIterator.h"


namespace _MYNAMESPACE_
{
    namespace MPI
    {
        using namespace Utility;
        template < class T >
        class LocalValArray
        {
            friend class ForwardAccessIterator<T>;
        public:
            using value_type = typename std::enable_if< MPL::is_pod_with_complex< T >::value, T >::type;
            using pointer    = value_type*;
            using const_pointer    = const pointer;
            using reference  = value_type&;
            using const_reference = const value_type&;
            using size_t     = std::size_t;
            using iterator   =  ForwardAccessIterator<T>;

            iterator            begin();
            iterator            end();

            explicit LocalValArray( pointer baseptr, const size_t localSize, const size_t HaloSize = 0 );
            LocalValArray( const LocalValArray& rhs );
            LocalValArray( LocalValArray&& rhs );

            ~LocalValArray();

            const_pointer       data() const;
            pointer             data();

            LocalValArray&      operator =( const LocalValArray& rhs )
            {
                if( m_LocalSize == rhs.m_LocalSize )
                {
                    memcpy( m_BasePtr, rhs.data(), sizeof( value_type ) * m_LocalSize );
                }

                return *this;
            }

            LocalValArray&      operator =( LocalValArray&& rhs )
            {
                if( this != rhs )
                {
                    m_BasePtr   = rhs.m_BasePtr;
                    m_LocalSize = rhs.size();
                    m_HaloSize  = rhs.halo_size();

                    rhs.m_BasePtr   = nullptr;
                    rhs.m_LocalSize = 0;
                    rhs.m_HaloSize  = 0;
                }

                return *this;
            }

            template< class expression >
            LocalValArray&      operator=( const expression& rhs );

            reference           operator []( const size_t index );
            const_reference     operator []( const size_t index ) const;
            const value_type    at( const size_t index ) const;

            size_t              size() const;
            size_t              halo_size() const;

            const value_type    sum() const;
            const value_type    innter_product( const LocalValArray& rhs ) const;

            LocalValArray&      operator +=( const LocalValArray& rhs );
            LocalValArray&      operator -=( const LocalValArray& rhs );
            LocalValArray&      operator *=( const LocalValArray& rhs );
            LocalValArray&      operator /=( const LocalValArray& rhs );

            REGIST_VAL_EXPRESSION_BINARY2( VAL_EXPRESSION, BinaryOp2, Pow )
            REGIST_VAL_EXPRESSION_BINARY2( VAL_EXPRESSION, BinaryOp2, ATan2 )

            REGIST_VAL_EXPRESSION_BINARY( VAL_EXPRESSION, BinaryOp, Add, + )
            REGIST_VAL_EXPRESSION_BINARY( VAL_EXPRESSION, BinaryOp, Sub, - )
            REGIST_VAL_EXPRESSION_BINARY( VAL_EXPRESSION, BinaryOp, Mul, * )
            REGIST_VAL_EXPRESSION_BINARY( VAL_EXPRESSION, BinaryOp, Div, / )

            REGIST_VAL_EXPRESSION_UNARY( VAL_EXPRESSION, UnaryOp, Abs )
            REGIST_VAL_EXPRESSION_UNARY( VAL_EXPRESSION, UnaryOp, Sin )
            REGIST_VAL_EXPRESSION_UNARY( VAL_EXPRESSION, UnaryOp, Cos )
            REGIST_VAL_EXPRESSION_UNARY( VAL_EXPRESSION, UnaryOp, Tan )
            REGIST_VAL_EXPRESSION_UNARY( VAL_EXPRESSION, UnaryOp, Sinh )
            REGIST_VAL_EXPRESSION_UNARY( VAL_EXPRESSION, UnaryOp, Cosh )
            REGIST_VAL_EXPRESSION_UNARY( VAL_EXPRESSION, UnaryOp, Tanh )
            REGIST_VAL_EXPRESSION_UNARY( VAL_EXPRESSION, UnaryOp, ASin )
            REGIST_VAL_EXPRESSION_UNARY( VAL_EXPRESSION, UnaryOp, ACos )
            REGIST_VAL_EXPRESSION_UNARY( VAL_EXPRESSION, UnaryOp, ATan )
            REGIST_VAL_EXPRESSION_UNARY( VAL_EXPRESSION, UnaryOp, Log )
            REGIST_VAL_EXPRESSION_UNARY( VAL_EXPRESSION, UnaryOp, Log2 )
            REGIST_VAL_EXPRESSION_UNARY( VAL_EXPRESSION, UnaryOp, Log10 )
            REGIST_VAL_EXPRESSION_UNARY( VAL_EXPRESSION, UnaryOp, Exp )
            REGIST_VAL_EXPRESSION_UNARY( VAL_EXPRESSION, UnaryOp, Sqrt )

        private:
            pointer         m_BasePtr;
            size_t          m_LocalSize;
            size_t          m_HaloSize;
        };

        template < class T >
        const typename LocalValArray<T>::value_type LocalValArray<T>::innter_product( const LocalValArray<T>& rhs ) const
        {
            using type = typename LocalValArray<T>::value_type;
            type val = MPL::ZeroType< type >();

            const auto lhsptr = this->data();
            const auto rhsptr = rhs.data();

            for( auto ii = 0UL; ii < m_LocalSize; ii++ )
            {
                val += lhsptr[ii] * rhsptr[ii];
            }
            return static_cast< const type >( val );
        }

        template < class T >
        typename LocalValArray<T>::const_pointer LocalValArray<T>::data() const
        {
            return static_cast< LocalValArray<T>::const_pointer >(m_BasePtr);
        }

    }
}

#endif // LOCALVALARRAY_H

#include "detail/LocalValArrayImpl.h"
