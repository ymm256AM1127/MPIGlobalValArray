#ifndef EXPRESSIONTEMPLATEFORGLOBALVALARRAY_H
#define EXPRESSIONTEMPLATEFORGLOBALVALARRAY_H

#include "ExpressionTemplate.h"

namespace _MYNAMESPACE_
{
    namespace  MPI
    {
        template <class Op, class Arg0 >
        struct UnaryOpForGlobalValArray
        {
            using result_type = typename Op::result_type;
            using value_type  = typename Arg0::value_type;
            using size_t      = std::size_t;

            const Op&    Operation;
            const Arg0&  Argument0;

            UnaryOpForGlobalValArray(const Op& op, const Arg0& a0)
                : Operation( op ), Argument0( a0 ) {}

            result_type operator[]( const size_t index) const
            {
                return Operation ( Argument0.LocalAt( index ) );
            }

            result_type LocalAt( const size_t index) const
            {
                return Operation ( Argument0.LocalAt( index ) );
            }
        };

        template <class Op, class Arg0, class Arg1>
        struct BinaryOpForGlobalValArray
        {
            using result_type = typename Op::result_type;
            using value_type  = typename Arg0::value_type;
            using size_t      = std::size_t;

            const Op&      Operation;
            const Arg0&    Argument0;
            const Arg1&    Argument1;

            BinaryOpForGlobalValArray( const Op& op, const Arg0& a0, const Arg1& a1 )
                : Operation( op ), Argument0( a0 ), Argument1( a1 ) {}

            value_type operator[]( const size_t index ) const
            {
                return Operation( Argument0.LocalAt( index ) , Argument1.LocalAt( index ) );
            }

            result_type LocalAt( const size_t index) const
            {
                return Operation( Argument0.LocalAt( index ) , Argument1.LocalAt( index ) );
            }
        };

        template <class Op, class Arg0, class Arg1>
        /*!
         * \brief The BinaryOp2 struct
         * powなど2パラメータを必要とする関数用。
         */
        struct BinaryOp2ForGlobalValArray
        {
            using result_type = typename Op::result_type;
            using value_type  = typename Arg0::value_type;
            using size_t      = std::size_t;

            const Op&      Operation;
            const Arg0&    Argument0;
            const Arg1&    Argument1;

            BinaryOp2ForGlobalValArray( const Op& op, const Arg0& a0, const Arg1& a1 )
                : Operation( op ), Argument0( a0 ), Argument1( a1 ) {}

            value_type operator[]( const size_t index ) const
            {
                return Operation( Argument0.LocalAt( index ) , Argument1 );
            }

            value_type LocalAt( const size_t index ) const
            {
                return Operation( Argument0.LocalAt( index ) , Argument1 );
            }
        };

        template< class Op >
        class VAL_EXPRESSIONGlobalValArray
        {
            typedef typename std::remove_reference< Op >::type  RmExpr;

            Op expr;
        public:
            using value_type  = typename RmExpr::value_type;
            using result_type = typename RmExpr::result_type;
            using size_t      = size_t;

            explicit VAL_EXPRESSIONGlobalValArray( const RmExpr& e ) : expr( e ) {}

            result_type LocalAt( const size_t index ) const
            {
                return expr.LocalAt( index ) ;
            }

            result_type operator[]( const size_t index ) const
            {
                return expr.LocalAt( index ) ;
            }
        };
    }
}

#endif // EXPRESSIONTEMPLATEFORGLOBALVALARRAY_H
