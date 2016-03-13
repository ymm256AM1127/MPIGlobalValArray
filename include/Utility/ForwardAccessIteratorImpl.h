#ifndef FORWARDACCESSITERATORIMPL_H
#define FORWARDACCESSITERATORIMPL_H

#include "ForwardAccessIterator.h"
#include <climits>

namespace _MYNAMESPACE_
{
    namespace Utility
    {
        template < typename T >
        ForwardAccessIterator<T>::ForwardAccessIterator(const ForwardAccessIterator &rhsiterator)
        {
            m_dataPtr  = rhsiterator.m_dataPtr;
            m_index    = rhsiterator.m_index;
            m_maxIndex = rhsiterator.m_maxIndex;
        }

        template < typename T >
        ForwardAccessIterator<T>::ForwardAccessIterator()
        {
            m_dataPtr = nullptr;
            m_index   = ULONG_MAX;
        }

        template < typename T >
        ForwardAccessIterator<T>::ForwardAccessIterator( typename ForwardAccessIterator<T>::pointer ptr, const std::size_t index, std::size_t maxindex)
        {
            m_dataPtr   = ptr;
            m_index     = index;
            m_maxIndex  = maxindex;
        }

        template < typename T >
        ForwardAccessIterator<T> &ForwardAccessIterator<T>::operator++()
        {
            m_index++;

            if( m_index == m_maxIndex )
            {
                m_index = ULONG_MAX;
                m_dataPtr = nullptr;
            }
            return *this;
        }

        template < typename T >
        ForwardAccessIterator<T> ForwardAccessIterator<T>::operator++(int)
        {
            ForwardAccessIterator result = *this;

            m_index++;

            if( m_index == m_maxIndex )
            {
                m_index = ULONG_MAX;
                m_dataPtr = nullptr;
            }

            return result;
        }

        template < typename T >
        T &ForwardAccessIterator<T>::operator*()
        {
            static T dummy;

            return (m_index != m_maxIndex ? m_dataPtr[m_index] : dummy);
        }

        template < typename T >
        bool ForwardAccessIterator<T>::operator!=(const ForwardAccessIterator &iterator)
        {
            return this->m_dataPtr != iterator.m_dataPtr || this->m_index != iterator.m_index;
        }

        template < typename T >
        bool ForwardAccessIterator<T>::operator==(const ForwardAccessIterator &iterator)
        {
            return !(*this != iterator );
        }
    }
}

#endif // FORWARDACCESSITERATORIMPL_H
