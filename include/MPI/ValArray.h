#ifndef VALARRAY_H
#define VALARRAY_H

#include "WindowObject.h"
#include <math.h>

namespace _MYNAMESPACE_
{
    namespace MPI
    {
        template < class Arg, class Result >
        struct UnaryFunction
        {
            typedef Arg    argument_type;
            typedef Result result_type;
        };

        template <class Arg1, class Arg2, class Result>
        struct BinaryFunction
        {
            typedef Arg1   first_argument_type;
            typedef Arg2   second_argument_type;
            typedef Result result_type;
        };

        template <class Op, class Arg0 >
        struct UnaryOp
        {
            using result_type = typename Op::result_type;
            using value_type  = typename Arg0::value_type;
            using size_t      = typename Arg0::size_t;

            const Op    Operation;
            const Arg0&  Argument0;

            UnaryOp(const Op& op, const Arg0& a0)
                : Operation( op ), Argument0( a0 ) {}

            result_type LocalAt( const size_t index ) const
            {
                return Operation( Argument0.LocalAt( index ) );
            }

            result_type operator[]( const size_t index) const
            {
                return Operation ( Argument0.LocalAt( index ) );
            }

        };

        template <class Op, class Arg0, class Arg1>
        struct BinaryOp
        {
            using result_type = typename Op::result_type;
            using value_type  = typename Arg0::value_type;
            using size_t      = std::size_t;

            const Op      Operation;
            const Arg0&    Argument0;
            const Arg1&    Argument1;

            BinaryOp( const Op& op, const Arg0& a0, const Arg1& a1 )
                : Operation( op ), Argument0( a0 ), Argument1( a1 ) {}

            result_type LocalAt( const size_t index ) const
            {
                return Operation( Argument0.LocalAt( index ) , Argument1.LocalAt( index ) );
            }

            value_type operator[]( const size_t index ) const
            {
                return Operation( Argument0.LocalAt( index ) , Argument1.LocalAt( index ) );
            }

        };

        template< class Op >
        class val_expression
        {
            typedef typename std::remove_reference< Op >::type  RmExpr;

            Op expr;
        public:
            using value_type  = typename RmExpr::value_type;
            using result_type = typename RmExpr::result_type;

            explicit val_expression( const RmExpr& e ) : expr( e ) {}

            result_type LocalAt( const size_t index ) const
            {
                return expr.LocalAt( index );
            }

            result_type operator[]( const size_t index ) const
            {
                return expr.LocalAt( index );
            }
        };


        template < class  T >
        struct AddExpression : BinaryFunction< T, T, T >
        {
            T operator()( const T& lhs, const T& rhs) const
            {
                return lhs + rhs;
            }
        };

        template < class  T >
        struct SubExpression : BinaryFunction< T, T, T >
        {
            T operator()( const T& lhs, const T& rhs) const
            {
                return lhs - rhs;
            }
        };

        template < class  T >
        struct MulExpression : BinaryFunction< T, T, T >
        {
            T operator()( const T& lhs, const T& rhs) const
            {
                return lhs * rhs;
            }
        };

        template < class  T >
        struct DivExpression : BinaryFunction< T, T, T >
        {
            T operator()( const T& lhs, const T& rhs) const
            {
                return lhs / rhs;
            }
        };


        template < class T >
        struct AbsExpression : UnaryFunction< T , T >
        {
            T operator()( const T& x ) const
            {
                return std::abs( x );
            }
        };

        template < class T >
        struct SinExpression : UnaryFunction< T , T >
        {
            T operator()( const T& x ) const
            {
                return std::sin( x );
            }
        };

        template < class T >
        struct CosExpression : UnaryFunction< T , T >
        {
            T operator()( const T& x ) const
            {
                return std::cos( x );
            }
        };

        template < class T >
        struct TanExpression : UnaryFunction< T , T >
        {
            T operator()( const T& x ) const
            {
                return std::tan( x );
            }
        };



        template < class T >
        class ValArray : public WindowObject<T>
        {
        public:
            using value_type     = typename std::enable_if< MPL::is_pod< T >::value, T >::type;
            using pointer        = value_type*;
            using CommPtr        = Environment::CommPtr;

            using WindowObject<T>::operator [];

            explicit ValArray( CommPtr comm,
                               const std::size_t size,
                               const std::size_t localhalosize = 0,
                               const std::string&  windowobjectname = std::string() );
            ValArray( const ValArray& rhs );

            ValArray&           operator =( const ValArray& rhs );

            ~ValArray(){}

            ValArray&           operator+=( const ValArray& rhs );
            ValArray&           operator-=( const ValArray& rhs );
            ValArray&           operator*=( const ValArray& rhs );
            ValArray&           operator/=( const ValArray& rhs );

            template< class expression >
            ValArray&           operator=( const expression& rhs )
            {
                auto lhsptr = this->GetBasePtr();
                for( auto ii = 0UL; ii < this->GetLocalSize(); ii++ )
                {
                    lhsptr[ii] = rhs[ii];
                }

                return *this;
            }

            template < class L, class R >
            friend val_expression< BinaryOp< AddExpression< T >, L, R > > operator +( const L& l, const R& r )
            {
                using Op = BinaryOp< AddExpression< T >, L, R >;
                return val_expression< Op >( Op( AddExpression< T >() , l, r ) );
            }

            template < class L, class R >
            friend val_expression< BinaryOp< SubExpression< T >, L, R > > operator -( const L& l, const R& r )
            {
                using Op = BinaryOp< SubExpression< T >, L, R >;
                return val_expression< Op >( Op( SubExpression< T >() , l, r ) );
            }

            template < class L, class R >
            friend val_expression< BinaryOp< MulExpression< T >, L, R > > operator *( const L& l, const R& r )
            {
                using Op = BinaryOp< MulExpression< T >, L, R >;
                return val_expression< Op >( Op( MulExpression< T >() , l, r ) );
            }

            template < class L, class R >
            friend val_expression< BinaryOp< DivExpression< T >, L, R > > operator /( const L& l, const R& r )
            {
                using Op = BinaryOp< DivExpression< T >, L, R >;
                return val_expression< Op >( Op( DivExpression< T >() , l, r ) );
            }

            template < class R >
            friend val_expression< UnaryOp< AbsExpression< T >, R > > Abs ( const R& r )
            {
                using Op = UnaryOp< AbsExpression< T >, R >;
                return val_expression< Op >( Op( AbsExpression< T >() , r ) );
            }

            template < class R >
            friend val_expression< UnaryOp< SinExpression< T >, R > > Sin ( const R& r )
            {
                using Op = UnaryOp< SinExpression< T >, R >;
                return val_expression< Op >( Op( SinExpression< T >() , r ) );
            }

            template < class R >
            friend val_expression< UnaryOp< CosExpression< T >, R > > Cos ( const R& r )
            {
                using Op = UnaryOp< CosExpression< T >, R >;
                return val_expression< Op >( Op( CosExpression< T >() , r ) );
            }

            template < class R >
            friend val_expression< UnaryOp< CosExpression< T >, R > > Tan ( const R& r )
            {
                using Op = UnaryOp< TanExpression< T >, R >;
                return val_expression< Op >( Op( TanExpression< T >() , r ) );
            }

        private:
        };

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



#endif // MPIVALARRAY_H
