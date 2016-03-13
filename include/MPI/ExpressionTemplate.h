#ifndef EXPRESSIONTEMPLATE_H
#define EXPRESSIONTEMPLATE_H

#include "../MPL/has_member_function.h"

HAS_MEMBER_FUNCTION( LocalAt );

#include <cmath>
#include <type_traits>
/*!
 * 式テンプレート
 * UnraryとBinaryの演算用評価クラスを定義することで、演算子や算術関数の遅延実行を実現する。
 */
namespace _MYNAMESPACE_
{
    namespace MPI
    {
        template < class Arg, class Result >
        struct UnaryFunction
        {
            using argument_type = Arg;
            using result_type   = Result;
        };

        template <class Arg1, class Arg2, class Result>
        struct BinaryFunction
        {
            using first_argument_type  = Arg1;
            using second_argument_type = Arg2;
            using result_type   = Result;
        };

        struct valarray_global_tag{ static const int id = 0; };
        struct valarray_local_tag { static const int id = 1; };

        template< class T>
        struct val_array_trais
        {
            typename std::conditional< has_member_function_LocalAt< T >::value,
                                      valarray_global_tag,
                                      valarray_local_tag >::type tag;
        };

        template <class Op, class Arg0 >
        struct UnaryOp
        {
            using result_type = typename Op::result_type;
            using value_type  = typename Arg0::value_type;
            using size_t      = std::size_t;

            const Op&    Operation;
            const Arg0&  Argument0;

            UnaryOp(const Op& op, const Arg0& a0)
                : Operation( op ), Argument0( a0 ) {}

            result_type operator[]( const size_t index) const
            {
                return Operation ( Argument0[ index ] );
            }
        };

        template <class Op, class Arg0, class Arg1>
        struct BinaryOp
        {
            using result_type = typename Op::result_type;
            using value_type  = typename Arg0::value_type;
            using size_t      = std::size_t;

            const Op&      Operation;
            const Arg0&    Argument0;
            const Arg1&    Argument1;

            BinaryOp( const Op& op, const Arg0& a0, const Arg1& a1 )
                : Operation( op ), Argument0( a0 ), Argument1( a1 ) {}

            value_type operator[]( const size_t index ) const
            {
                return Operation( Argument0[ index ] , Argument1[ index ] );
            }
        };

        template <class Op, class Arg0, class Arg1>
        /*!
         * \brief The BinaryOp2 struct
         * powなど2パラメータを必要とする関数用。
         */
        struct BinaryOp2
        {
            using result_type = typename Op::result_type;
            using value_type  = typename Arg0::value_type;
            using size_t      = std::size_t;

            const Op&      Operation;
            const Arg0&    Argument0;
            const Arg1&    Argument1;

            BinaryOp2( const Op& op, const Arg0& a0, const Arg1& a1 )
                : Operation( op ), Argument0( a0 ), Argument1( a1 ) {}

            value_type operator[]( const size_t index ) const
            {
                return Operation( Argument0[ index ] , Argument1 );
            }
        };

        #define REGIST_VAL_EXPRESSION_UNARY( expression, funcBinder, type ) template < class R > \
        friend expression< funcBinder< type##Expression< value_type >, R > > type ( const R& r ) \
        { \
            using Op = funcBinder< type##Expression< value_type >, R >; \
            return expression< Op >( Op( type##Expression< value_type >() , r ) ); \
        } \

        #define  REGIST_VAL_EXPRESSION_BINARY( expression, funcBinder, operation, mark ) template < class L, class R > \
        friend expression< funcBinder < operation##Expression< value_type >, L, R > > operator mark ( const L& l, const R& r ) \
        {\
            using Op = funcBinder < operation##Expression < value_type >, L, R >;\
            return expression< Op >( Op( operation##Expression < value_type >() , l, r ) );\
        }\

        #define  REGIST_VAL_EXPRESSION_BINARY2( expression, funcBinder, operation ) template < class L, class R > \
        friend expression < funcBinder < operation##Expression< value_type >, L, R > > operation ( const L& l, const R& r ) \
        {\
            using Op = funcBinder < operation##Expression < value_type >, L, R >;\
            return expression < Op >( Op( operation##Expression < value_type >() , l, r ) );\
        }\


        template< class Op >
        class VAL_EXPRESSION
        {
            typedef typename std::remove_reference< Op >::type  RmExpr;

            Op expr;
        public:
            using value_type  = typename RmExpr::value_type;
            using result_type = typename RmExpr::result_type;
            using size_t      = size_t;

            explicit VAL_EXPRESSION( const RmExpr& e ) : expr( e ) {}

            result_type operator[]( const size_t index ) const
            {
                return expr[ index ];
            }
        };

        #define REGIST_BINARY_OP_OPERATOR( type, OpMark ) template< class T > \
        struct type##Expression : public BinaryFunction< T, T, T > \
        { \
            typename BinaryFunction< T, T, T >::result_type operator()( const typename BinaryFunction< T, T, T >::first_argument_type& lhs, const typename BinaryFunction< T, T, T >::second_argument_type& rhs) const \
            { \
                return lhs OpMark rhs; \
            } \
        } \

        /*!
         * \brief REGIST_BINARY_OP_TYPE \
         * ここに引数が2個のファンクタを登録する
         */
        REGIST_BINARY_OP_OPERATOR( Add, + );
        REGIST_BINARY_OP_OPERATOR( Sub, - );
        REGIST_BINARY_OP_OPERATOR( Mul, * );
        REGIST_BINARY_OP_OPERATOR( Div, / );

        #define REGIST_BINARY_OP_TYPE( type, OpMark ) template< class T > \
        struct type##Expression : public BinaryFunction< T, T, T > \
        { \
            typename BinaryFunction< T, T, T >::result_type operator()( const typename BinaryFunction< T, T, T >::first_argument_type& lhs, const typename BinaryFunction< T, T, T >::second_argument_type& rhs) const \
            { \
                return OpMark ( lhs, rhs ); \
            } \
        } \

        /*!
         * \brief REGIST_BINARY_OP_TYPE \
         * ここにpowなど引数を2個必要とする数値演算用のファンクタを登録する
         */
        REGIST_BINARY_OP_TYPE( Pow, std::pow );
        REGIST_BINARY_OP_TYPE( ATan2, std::atan2 );


        /*!
         * \brief REGIST_UNARY_OP_TYPE \
         * ここに引数が1個のファンクタを登録する
         */
        #define REGIST_UNARY_OP_TYPE( type, OpMark ) template < class T > \
        struct type##Expression : public UnaryFunction< T , T > \
        { \
            typename UnaryFunction< T , T >::result_type operator()( const typename UnaryFunction< T , T >::argument_type& x ) const \
            { \
                return OpMark( x ); \
            } \
        } \

        REGIST_UNARY_OP_TYPE( Abs, std::abs );
        REGIST_UNARY_OP_TYPE( Sin, std::sin );
        REGIST_UNARY_OP_TYPE( Cos, std::cos );
        REGIST_UNARY_OP_TYPE( Tan, std::tan );
        REGIST_UNARY_OP_TYPE( Sinh, std::sinh );
        REGIST_UNARY_OP_TYPE( Cosh, std::cosh );
        REGIST_UNARY_OP_TYPE( Tanh, std::tanh );
        REGIST_UNARY_OP_TYPE( ASin, std::asin );
        REGIST_UNARY_OP_TYPE( ACos, std::acos );
        REGIST_UNARY_OP_TYPE( ATan, std::atan );
        REGIST_UNARY_OP_TYPE( Log, std::log );
        REGIST_UNARY_OP_TYPE( Log10, std::log10 );
        REGIST_UNARY_OP_TYPE( Log2, std::log2 );
        REGIST_UNARY_OP_TYPE( Exp, std::exp );
        REGIST_UNARY_OP_TYPE( Sqrt, std::sqrt );
    }
}

#endif // EXPRESSIONTEMPLATE_H
