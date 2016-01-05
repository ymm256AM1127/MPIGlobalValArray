#ifndef VALARRAY_H
#define VALARRAY_H

#include "WindowObject.h"
#include "ExpressionTemplate.h"

namespace _MYNAMESPACE_
{
    namespace MPI
    {
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
            template< class expression >
            ValArray&           operator=( const expression& rhs );

            ~ValArray(){}

            ValArray&           operator+=( const ValArray& rhs );
            ValArray&           operator-=( const ValArray& rhs );
            ValArray&           operator*=( const ValArray& rhs );
            ValArray&           operator/=( const ValArray& rhs );

            T                   sum() ;

            template < class L, class R >
            friend val_expression< BinaryOp2< PowExpression< T >, L, R > > Pow ( const L& l, const R& r )
            {
                using Op = BinaryOp2< PowExpression< T >, L, R >;
                return val_expression< Op >( Op( PowExpression< T >() , l, r ) );
            }

            template < class L, class R >
            friend val_expression< BinaryOp2< ATan2Expression< T >, L, R > > ATan2 ( const L& l, const R& r )
            {
                using Op = BinaryOp2< ATan2Expression< T >, L, R >;
                return val_expression< Op >( Op(ATan2Expression< T >() , l, r ) );
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

            #define REGIST_VAL_EXPRESSION_UNARY( type ) template < class R > \
            friend val_expression< UnaryOp< type##Expression< T >, R > > type ( const R& r ) \
            { \
                using Op = UnaryOp< type##Expression< T >, R >; \
                return val_expression< Op >( Op( type##Expression< T >() , r ) ); \
            } \

            REGIST_VAL_EXPRESSION_UNARY( Abs )
            REGIST_VAL_EXPRESSION_UNARY( Sin )
            REGIST_VAL_EXPRESSION_UNARY( Cos )
            REGIST_VAL_EXPRESSION_UNARY( Tan )
            REGIST_VAL_EXPRESSION_UNARY( Sinh )
            REGIST_VAL_EXPRESSION_UNARY( Cosh )
            REGIST_VAL_EXPRESSION_UNARY( Tanh )
            REGIST_VAL_EXPRESSION_UNARY( ASin )
            REGIST_VAL_EXPRESSION_UNARY( ACos )
            REGIST_VAL_EXPRESSION_UNARY( ATan )
            REGIST_VAL_EXPRESSION_UNARY( Log )
            REGIST_VAL_EXPRESSION_UNARY( Log2 )
            REGIST_VAL_EXPRESSION_UNARY( Log10 )
            REGIST_VAL_EXPRESSION_UNARY( Exp )
            REGIST_VAL_EXPRESSION_UNARY( Sqrt )

            private:
        };
    }
}

#endif // MPIVALARRAY_H

#include "detail/ValArrayImpl.h"


