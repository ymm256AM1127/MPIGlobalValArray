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

    // メモリ確保
    T* allocate(std::size_t n)
    {
        int MyRank  = MPIRank();
        int MPIsize = MPISize();

        //! 新しいコミュニケータの作成
        MPI_Comm    shmcomm;
        MPI_Comm_split_type( MPI_COMM_WORLD, MPI_COMM_TYPE_SHARED, 0, MPI_INFO_NULL, &shmcomm );

        //! メモリサイズの計算
        //! 入力のnの使用はグローバルのサイズであるので、このアロケータが呼ばれる
        //! ローカルのサイズは、MPIsizeで分配する。
        std::size_t totalsize  = n;

        //! ローカルサイズはMPIランクサイズで割り、余りを各ランクに割り振るようにする
        std::size_t local_size = totalsize / MPIsize ;
        std::size_t rest = totalsize % MPIsize;
        for( auto ii = 0; ii < rest; ii++ )
        {
            if( ii == MyRank )
            {
                local_size++;
            }
        }

        //! MPI Win用の識別変数。実態は符号なしint
        MPI_Win win;

        //! 下記の変数はMPI APIの内部では利用されていない（MPICH3においては）
        //! 規格書にはベースポインタとしか記載されておらず、MPICH3の実装においても、
        //! メモリのチェックを行っているのみ？っぽい2015/12/06現在
        T     buffer;
        //! MPI Window用のメモリ確保
        MPI_Win_allocate_shared( local_size * sizeof( T ), sizeof( T ), MPI_INFO_NULL, shmcomm, &buffer, &win );

        T* base_ptr;
        MPI_Aint    sharedsize;     // 書き込みのみで利用せず
        int         shareddispunit; // 書き込みのみで利用せず

        //!　MPI_PROC_NULLを指定してグローバルなシェアードメモリポインタを取得する
        MPI_Win_shared_query( win, MPI_PROC_NULL, &sharedsize, &shareddispunit, &base_ptr );

        //! ここで確保したWinをメモリ管理用のHashmapに保持する
        auto iter = MPIWinAllocMap.find( (void*)base_ptr );
        //! 同じポインタがいる場合は多重解放の原因になるので、bad_alloc
        //! を投げる。通常はあり得ない。
        if( iter != MPIWinAllocMap.end() )
        {
            static std::bad_alloc ex;
            throw &ex;
        }
        else
        {
            //! 問題がなければHashmapに登録
            MPIWinAllocMap[ (void*)base_ptr ] = win;
        }

        return base_ptr;
    }

    //! この関数はMPIの領域内で実行されなければならいので、
    //! 必ずグローバルスコープもしくは全ランクから同時に呼ばれ
    //! なくてはならない。
    void deallocate(T* p, std::size_t n)
    {
        static_cast<void>(n); //　コンパイラ警告対策

        auto iter = MPIWinAllocMap.find( (void*)p );

        if(  iter != MPIWinAllocMap.end() )
        {
            MPI_Win win_temp = iter->second;
            MPI_Win_free( &win_temp );
            MPIWinAllocMap.erase( iter );
        }
    }
};

//! 比較演算子
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

