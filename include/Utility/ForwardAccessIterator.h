#ifndef FORWARDACCESSITERATOR_H
#define FORWARDACCESSITERATOR_H

#include <iterator>
#include "../SharedLibraryDefine.h"

namespace _MYNAMESPACE_
{
    namespace Utility
    {
        /*!
         * \brief The ArrayAccessIterator class
         */
        template < typename T >
        class ForwardAccessIterator : public std::iterator< std::forward_iterator_tag, T >
        {
        public:
            using value_type = T;
            using pointer    = value_type*;
            using size_t     = std::size_t;
            using reference  = value_type&;

            ForwardAccessIterator( const ForwardAccessIterator& rhsiterator );

            ForwardAccessIterator();
            ForwardAccessIterator( pointer ptr, const std::size_t index, std::size_t maxindex );

            ForwardAccessIterator&     operator++();
            //! std::iteratorでは引数がint型で定義されている。
            ForwardAccessIterator      operator++( int );
            reference                  operator*();

            bool operator!=(const ForwardAccessIterator& iterator);
            bool operator==(const ForwardAccessIterator& iterator);

        private:
            pointer             m_dataPtr;
            size_t              m_index;
            size_t              m_maxIndex;

            //! コンストラクタはprivateで定義

        };
    }
}

#endif // FORWARDACCESSITERATOR_H

#include "ForwardAccessIteratorImpl.h"
