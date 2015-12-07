#ifndef MPISHAREDALLOCATOR_H
#define MPISHAREDALLOCATOR_H

#include <unordered_map>
#include <exception>
#include "MPIWapperUtils.h"
#include <mpi.h>
#include <iostream>

//static std::unordered_map< void*, MPI_Win > MPIWinAllocMap;

template <class T>
struct MPISharedAllocator
{
    using value_type = T;

    MPISharedAllocator() {}
    virtual ~MPISharedAllocator() {}

    template <class U>
    MPISharedAllocator( const MPISharedAllocator<U>&) {}

    // �������m��
    T* allocate(std::size_t n)
    {
        int MyRank  = MPIRank();
        int MPIsize = MPISize();

        //! �V�����R�~���j�P�[�^�̍쐬
        MPI_Comm    shmcomm;
        MPI_Comm_split_type( MPI_COMM_WORLD, MPI_COMM_TYPE_SHARED, 0, MPI_INFO_NULL, &shmcomm );

        //! �������T�C�Y�̌v�Z
        //! ���͂�n�̎g�p�̓O���[�o���̃T�C�Y�ł���̂ŁA���̃A���P�[�^���Ă΂��
        //! ���[�J���̃T�C�Y�́AMPIsize�ŕ��z����B
        std::size_t totalsize  = n;

        //! ���[�J���T�C�Y��MPI�����N�T�C�Y�Ŋ���A�]����e�����N�Ɋ���U��悤�ɂ���
        std::size_t local_size = totalsize / MPIsize ;
        std::size_t rest = totalsize % MPIsize;
        for( auto ii = 0; ii < rest; ii++ )
        {
            if( ii == MyRank )
            {
                local_size++;
            }
        }

        //! MPI Win�p�̎��ʕϐ��B���Ԃ͕����Ȃ�int
        MPI_Win win;

        //! ���L�̕ϐ���MPI API�̓����ł͗��p����Ă��Ȃ��iMPICH3�ɂ����Ắj
        //! �K�i���ɂ̓x�[�X�|�C���^�Ƃ����L�ڂ���Ă��炸�AMPICH3�̎����ɂ����Ă��A
        //! �������̃`�F�b�N���s���Ă���̂݁H���ۂ�2015/12/06����
        T     buffer;
        //! MPI Window�p�̃������m��
        MPI_Win_allocate_shared( local_size * sizeof( T ), sizeof( T ), MPI_INFO_NULL, shmcomm, &buffer, &win );

        T* base_ptr;
        MPI_Aint    sharedsize;     // �������݂݂̂ŗ��p����
        int         shareddispunit; // �������݂݂̂ŗ��p����

        //!�@MPI_PROC_NULL���w�肵�ăO���[�o���ȃV�F�A�[�h�������|�C���^���擾����
        MPI_Win_shared_query( win, MPI_PROC_NULL, &sharedsize, &shareddispunit, &base_ptr );

        //! �����Ŋm�ۂ���Win���������Ǘ��p��Hashmap�ɕێ�����
        auto iter = MPIWinAllocMap.find( (void*)base_ptr );
        //! �����|�C���^������ꍇ�͑��d����̌����ɂȂ�̂ŁAbad_alloc
        //! �𓊂���B�ʏ�͂��蓾�Ȃ��B
        if( iter != MPIWinAllocMap.end() )
        {
            static std::bad_alloc ex;
            throw &ex;
        }
        else
        {
            //! ��肪�Ȃ����Hashmap�ɓo�^
            MPIWinAllocMap[ (void*)base_ptr ] = win;
        }

        return base_ptr;
    }

    //! ���̊֐���MPI�̗̈���Ŏ��s����Ȃ���΂Ȃ炢�̂ŁA
    //! �K���O���[�o���X�R�[�v�������͑S�����N���瓯���ɌĂ΂�
    //! �Ȃ��Ă͂Ȃ�Ȃ��B
    void deallocate(T* p, std::size_t n)
    {
        static_cast<void>(n); //�@�R���p�C���x���΍�

        auto iter = MPIWinAllocMap.find( (void*)p );

        if(  iter != MPIWinAllocMap.end() )
        {
            MPI_Win win_temp = iter->second;
            MPI_Win_free( &win_temp );
            MPIWinAllocMap.erase( iter );
        }
    }
};

//! ��r���Z�q
template <class T, class U>
bool operator==(const MPISharedAllocator<T>&, const MPISharedAllocator<U>&)
{
    return true;
}

template <class T, class U>
bool operator!=(const MPISharedAllocator<T>&, const MPISharedAllocator<U>&)
{
    return false;
}

#endif // MPISHAREDALLOCATOR_H

