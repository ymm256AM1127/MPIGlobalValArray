#ifndef VALARRAY_H
#define VALARRAY_H

#include "WindowObject.h"
#include "ExpressionTemplate.h"
#include "LocalValArray.h"

namespace _MYNAMESPACE_
{
    namespace MPI
    {
        template < class Policy >
        class ValArray : public Policy
        {
        public:
            using value_type     = typename std::enable_if< MPL::is_pod_with_complex< typename Policy::value_type >::value, typename Policy::value_type >::type;
            using pointer        = value_type*;
            using CommPtr        = Environment::CommPtr;

            using Policy::operator [];

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

            value_type          sum() ;

            LocalValArray< value_type > GetLocalValArray()
            {
                return std::move( LocalValArray< value_type >( this->GetBasePtr(), this->GetLocalSize(), this->GetLocalHaloSize() ) );
            }

            template < class L, class R >
            friend val_expression< BinaryOp2< PowExpression< typename Policy::value_type >, L, R > > Pow ( const L& l, const R& r )
            {
                using Op = BinaryOp2< PowExpression< typename Policy::value_type >, L, R >;
                return val_expression< Op >( Op( PowExpression< typename Policy::value_type >() , l, r ) );
            }

            template < class L, class R >
            friend val_expression< BinaryOp2< ATan2Expression< typename Policy::value_type >, L, R > > ATan2 ( const L& l, const R& r )
            {
                using Op = BinaryOp2< ATan2Expression< typename Policy::value_type >, L, R >;
                return val_expression< Op >( Op(ATan2Expression< typename Policy::value_type >() , l, r ) );
            }

            template < class L, class R >
            friend val_expression< BinaryOp< AddExpression< typename Policy::value_type >, L, R > > operator +( const L& l, const R& r )
            {
                using Op = BinaryOp< AddExpression< typename Policy::value_type >, L, R >;
                return val_expression< Op >( Op( AddExpression< typename Policy::value_type >() , l, r ) );
            }

            template < class L, class R >
            friend val_expression< BinaryOp< SubExpression< typename Policy::value_type >, L, R > > operator -( const L& l, const R& r )
            {
                using Op = BinaryOp< SubExpression< typename Policy::value_type >, L, R >;
                return val_expression< Op >( Op( SubExpression< typename Policy::value_type >() , l, r ) );
            }

            template < class L, class R >
            friend val_expression< BinaryOp< MulExpression< typename Policy::value_type >, L, R > > operator *( const L& l, const R& r )
            {
                using Op = BinaryOp< MulExpression< typename Policy::value_type >, L, R >;
                return val_expression< Op >( Op( MulExpression< typename Policy::value_type >() , l, r ) );
            }

            template < class L, class R >
            friend val_expression< BinaryOp< DivExpression< typename Policy::value_type >, L, R > > operator /( const L& l, const R& r )
            {
                using Op = BinaryOp< DivExpression< typename Policy::value_type >, L, R >;
                return val_expression< Op >( Op( DivExpression< typename Policy::value_type >() , l, r ) );
            }

            #define REGIST_VAL_EXPRESSION_UNARY( type ) template < class R > \
            friend val_expression< UnaryOp< type##Expression< typename Policy::value_type >, R > > type ( const R& r ) \
            { \
                using Op = UnaryOp< type##Expression< typename Policy::value_type >, R >; \
                return val_expression< Op >( Op( type##Expression< typename Policy::value_type >() , r ) ); \
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
