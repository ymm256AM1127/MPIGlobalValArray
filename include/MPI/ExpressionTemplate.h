#ifndef EXPRESSIONTEMPLATE_H
#define EXPRESSIONTEMPLATE_H

#include <cmath>
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

        };

        template <class Arg1, class Arg2, class Result>
        struct BinaryFunction
        {
            using first_argument_type  = Arg1;
            using second_argument_type = Arg2;
            using result_type   = Result;
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

            const Op&      Operation;
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

            result_type LocalAt( const size_t index ) const
            {
                return Operation( Argument0.LocalAt( index ) , Argument1 );
            }

            value_type operator[]( const size_t index ) const
            {
                return Operation( Argument0.LocalAt( index ) , Argument1 );
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

        #define REGIST_BINARY_OP_OPERATOR( type, OpMark ) template< class T > \
        struct type##Expression : BinaryFunction< T, T, T > \
        { \
            using value_type  = T; \
            using result_type = T; \
            T operator()( const T& lhs, const T& rhs) const \
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
        struct type##Expression : BinaryFunction< T, T, T > \
        { \
            using value_type  = T; \
            using result_type = T; \
            T operator()( const T& lhs, const T& rhs) const \
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
        struct type##Expression : UnaryFunction< T , T > \
        { \
            using value_type  = T; \
            using result_type = T; \
            T operator()( const T& x ) const \
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
