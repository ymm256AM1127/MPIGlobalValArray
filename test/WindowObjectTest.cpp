#include "Test.h"

#include "../include/MPI/WindowObject.h"
#include <QDebug>

using namespace _MYNAMESPACE_::MPI;

int WindowObjectTest(  _MYNAMESPACE_::MPI::Environment::CommPtr &comm )
{
    comm->Barrier();

    std::size_t globalsize = 100;
    std::size_t localhalo  = 10;

    //! ここでインスタンスを作成する。
    //! 仮引数は、コミュニケータのシェアードポインタ、配列のグローバルサイズ、
    //! HALOサイズ（オーバラップの領域）、オブジェクト名となっている。
    WindowObject<double> RMA01( comm, globalsize, localhalo, "Test01" );

    //! ここで、作成したインスタンスに値を代入する。
    //! ここは並列領域であるので、ローカルポインタに直接アクセスすることで、
    //! 並列実行により全要素にアクセスしたことになる。
    for( auto ii = 0UL; ii < RMA01.GetLocalSize(); ii++ )
    {
        RMA01.GetBasePtr()[ ii ] = static_cast<double>( ii * ( comm->GetMPIRank() + 1 ) );
    }

    //! ちょっとデバッグ
    qDebug() << MPIEnvPtr->GetHostName().c_str() << RMA01.GetLocalSize() << RMA01.GetGlobalSize();

    comm->Barrier();

    //! ルートランクのみがすべての要素にアクセスし、値を表示させる。
    if( comm->IsRootRank() )
    {
        for( auto ii = 0UL; ii < RMA01.GetGlobalSize(); ii++ )
        {
            qDebug() << MPIEnvPtr->GetHostName().c_str() << " Index: " << ii << " value = " << RMA01[ii];
        }
    }

    comm->Barrier();
    //! 配列の書き込みテスト
    RMA01[ 10 ] = 0.1;
    RMA01[ 20 ] = 0.2;
    RMA01[ 30 ] = 0.3;
    RMA01[ 40 ] = 0.4;
    RMA01[ 50 ] = 0.5;
    RMA01[ 60 ] = 0.6;
    RMA01[ 70 ] = 0.7;
    RMA01[ 80 ] = 0.8;
    RMA01[ 90 ] = 0.9;

    //! 値の確認
    for( auto ii = 10UL; ii < RMA01.GetGlobalSize(); ii+=10 )
    {
        qDebug() << MPIEnvPtr->GetHostName().c_str() << "index: " << ii << RMA01[ ii ];
    }

    return 0;
}
