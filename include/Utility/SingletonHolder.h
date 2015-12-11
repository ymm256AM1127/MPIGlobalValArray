#ifndef SINGLETONHOLDER_H
#define SINGLETONHOLDER_H

#include <stdio.h>

template < class T > class SingletonHolder
{
public:
    typedef T InstanceType;
    static InstanceType* GetInstance()
    {
        if( m_pInstance == NULL )
        {
            m_pInstance = new InstanceType();
        }
        return m_pInstance;
    }

    static void Destory()
    {
        if( m_pInstance != NULL )
        {
            delete m_pInstance;
            m_pInstance = NULL;
        }
    }

    static bool IsCreated()
    {
        return m_pInstance != NULL;
    }

    static bool IsDestroyed()
    {
        return m_pInstance == NULL;
    }

private:
    static T *m_pInstance;
    SingletonHolder(){};
    SingletonHolder(const SingletonHolder &rhs);
    SingletonHolder& operator=(const SingletonHolder &rhs);
    virtual ~SingletonHolder()
    {
        if( m_pInstance != NULL )
        {
            delete m_pInstance;
        }
    };
};

template <class T>
typename SingletonHolder<T>::InstanceType* SingletonHolder<T>::m_pInstance = nullptr;

#endif // SINGLETONHOLDER_H
