#ifndef GLOBALVALARRAY_H
#define GLOBALVALARRAY_H

#include "WindowObject.h"
#include "LocalValArray.h"
#include "ExpressionTemplateForGlobalValArray.h"

namespace _MYNAMESPACE_
{
    namespace MPI
    {
        template < class Policy >
        class GlobalValArray : public Policy
        {
        public:
            using value_type     = typename std::enable_if< MPL::is_pod_with_complex< typename Policy::value_type >::value, typename Policy::value_type >::type;
            using pointer        = value_type*;
            using CommPtr        = Environment::CommPtr;

            using Policy::operator [];

            explicit GlobalValArray( CommPtr comm,
                               const std::size_t size,
                               const std::size_t localhalosize = 0,
                               const std::string&  windowobjectname = std::string() );
            GlobalValArray( const GlobalValArray& rhs );

            GlobalValArray&             operator =( const GlobalValArray& rhs );
            template< class expression >
            GlobalValArray&             operator=( const expression& rhs );

            ~GlobalValArray(){}

            GlobalValArray&             operator+=( const GlobalValArray& rhs );
            GlobalValArray&             operator-=( const GlobalValArray& rhs );
            GlobalValArray&             operator*=( const GlobalValArray& rhs );
            GlobalValArray&             operator/=( const GlobalValArray& rhs );

            const value_type            sum() const;
            const value_type            norm() const;
            const value_type            norm2() const;
            const value_type            max() const;
            const value_type            min() const;
            const value_type            inner_product( const GlobalValArray& rhs ) const ;

            LocalValArray< value_type > GetLocalValArray()
            {
                return std::move( LocalValArray< value_type >( this->GetBasePtr(), this->GetLocalSize(), this->GetLocalHaloSize() ) );
            }

            REGIST_VAL_EXPRESSION_BINARY2( VAL_EXPRESSIONGlobalValArray, BinaryOp2ForGlobalValArray, Pow )
            REGIST_VAL_EXPRESSION_BINARY2( VAL_EXPRESSIONGlobalValArray, BinaryOp2ForGlobalValArray, ATan2 )

            REGIST_VAL_EXPRESSION_BINARY( VAL_EXPRESSIONGlobalValArray, BinaryOpForGlobalValArray, Add, + )
            REGIST_VAL_EXPRESSION_BINARY( VAL_EXPRESSIONGlobalValArray, BinaryOpForGlobalValArray, Sub, - )
            REGIST_VAL_EXPRESSION_BINARY( VAL_EXPRESSIONGlobalValArray, BinaryOpForGlobalValArray, Mul, * )
            REGIST_VAL_EXPRESSION_BINARY( VAL_EXPRESSIONGlobalValArray, BinaryOpForGlobalValArray, Div, / )

            REGIST_VAL_EXPRESSION_UNARY( VAL_EXPRESSIONGlobalValArray, UnaryOpForGlobalValArray, Abs )
            REGIST_VAL_EXPRESSION_UNARY( VAL_EXPRESSIONGlobalValArray, UnaryOpForGlobalValArray, Sin )
            REGIST_VAL_EXPRESSION_UNARY( VAL_EXPRESSIONGlobalValArray, UnaryOpForGlobalValArray, Cos )
            REGIST_VAL_EXPRESSION_UNARY( VAL_EXPRESSIONGlobalValArray, UnaryOpForGlobalValArray, Tan )
            REGIST_VAL_EXPRESSION_UNARY( VAL_EXPRESSIONGlobalValArray, UnaryOpForGlobalValArray, Sinh )
            REGIST_VAL_EXPRESSION_UNARY( VAL_EXPRESSIONGlobalValArray, UnaryOpForGlobalValArray, Cosh )
            REGIST_VAL_EXPRESSION_UNARY( VAL_EXPRESSIONGlobalValArray, UnaryOpForGlobalValArray, Tanh )
            REGIST_VAL_EXPRESSION_UNARY( VAL_EXPRESSIONGlobalValArray, UnaryOpForGlobalValArray, ASin )
            REGIST_VAL_EXPRESSION_UNARY( VAL_EXPRESSIONGlobalValArray, UnaryOpForGlobalValArray, ACos )
            REGIST_VAL_EXPRESSION_UNARY( VAL_EXPRESSIONGlobalValArray, UnaryOpForGlobalValArray, ATan )
            REGIST_VAL_EXPRESSION_UNARY( VAL_EXPRESSIONGlobalValArray, UnaryOpForGlobalValArray, Log )
            REGIST_VAL_EXPRESSION_UNARY( VAL_EXPRESSIONGlobalValArray, UnaryOpForGlobalValArray, Log2 )
            REGIST_VAL_EXPRESSION_UNARY( VAL_EXPRESSIONGlobalValArray, UnaryOpForGlobalValArray, Log10 )
            REGIST_VAL_EXPRESSION_UNARY( VAL_EXPRESSIONGlobalValArray, UnaryOpForGlobalValArray, Exp )
            REGIST_VAL_EXPRESSION_UNARY( VAL_EXPRESSIONGlobalValArray, UnaryOpForGlobalValArray, Sqrt )

        private:

        };
    }
}

#endif // GLOBALVALARRAY_H

#include "detail/GlobalValArrayImpl.h"
