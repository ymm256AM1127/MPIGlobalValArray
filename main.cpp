#include <iostream>
#include <mpi.h>
#include <vector>
#include <QElapsedTimer>
#include <QDebug>
#include <valarray>

#include "test/Test.h"

#include "include/MPI/GlobalValArray.h"

QDebug operator<<(QDebug dbg, const std::complex<double> &c)
{
    dbg.nospace() << "(" << c.real() << ", " << c.imag() << ")";

    return dbg.space();
}

using namespace MyMPI::MPI;

int main(int argc, char *argv[])
{
    //! MPIの初期化
    MPIEnvPtr->Init( argc, argv );
    //! コミュニケータの取得
    auto comm = MPIEnvPtr->CreateCommunicator( MPI_COMM_WORLD );

    //! GlobalValArrayのインスタンスを作成。
    GlobalValArray< WindowObject< std::complex< double > > > array1( comm, 10000, 0 );

    //! 各ランクのローカル配列のサイズを確認
    qDebug() << "Rank " << comm->GetMPIRank() << " : " << array1.GetLocalSize();

    //! 標準出力のための同期
    comm->Barrier();

    std::complex< double > CorrectVal( 100.0, -100.0 );
    //! Rank0がRnak1のメモリ領域に書き込む
    //! 片方通信となる
    if( comm->GetMPIRank() == 0 )
    {
        auto localsize = array1.GetLocalSize();
        auto val       = CorrectVal;
        qDebug() << "Rank " << comm->GetMPIRank() << " wrties " << val << " to " << "Rank " << comm->GetMPIRank() + 1 << ".";
        array1[ localsize + 10 ] = val;
    }

    //! 標準出力のための同期
    comm->Barrier();

    if( comm->GetMPIRank() == 1 )
    {
        auto localsize = array1.GetLocalSize();
        qDebug() << "Rank " << comm->GetMPIRank() << " reads " << array1[ localsize + 10 ] << " from " << "Rank " << comm->GetMPIRank() + 1 << ".";
    }

//    //! 標準出力のための同期
    comm->Barrier();

    //! array1のディープコピーをそれぞれ作成。
    auto array2 = array1;
    auto array3 = array1;

    array1[ 20 ] = 10.0;
    array1[ 80 ] = 30.0;
    array2[ 20 ] = 1.0;
    array2[ 80 ] = 3.0;
    array3[ 20 ] = -13.0;
    array3[ 80 ] = -19.0;

    array1.Fence();

    array1 = Pow( array1, 2.0 ) + Sqrt( array2 + Cos( array3 ) ) ;

    array1.Fence();
    comm->Barrier();

    auto sum = array1.sum();

    qDebug() << array1[ 20 ] << array1[ 80 ] << sum << array1.inner_product( array2 ) << array1.norm() << array1.norm2();

    comm->Barrier();
    auto localarray = array1.GetLocalValArray();
    auto localarray1 = array2.GetLocalValArray();

    qDebug() << localarray[ 20 ] << localarray.sum() << localarray.innter_product( localarray1 );


    return 0;
}

