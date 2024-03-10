#ifndef _BR_MUTEX_LOCKER_H_
#define _BR_MUTEX_LOCKER_H_


namespace br {
#if defined(__GNUC__) || defined(__GNUG__)
#include <time.h>
#define __br_compare_and_swap(x, o, n) __sync_val_compare_and_swap(x, o, n)
#define __br_fetch_and_add(x, v) __sync_fetch_and_add(x, v)
#define __br_fetch_and_sub(x, v) __sync_fetch_and_sub(x, v)
#define __br_set(x,v)   __sync_lock_test_and_set(x, v)
#elif defined(_MSC_VER)
#include <time.h>
#include <windows.h>
#define __br_compare_and_swap(x, o, n) InterlockedCompareExchange(x, n, o)   
#define __br_fetch_and_add(x, v) InterlockedExchangeAdd(x, v)
#define __br_fetch_and_sub(x, v) InterlockedExchangeAdd(x, -v) 
#define __br_set(x,v)   InterlockedExchange(x, v)
#endif

    static void msleep ( LONGLONG a )
    {
#ifdef WIN32
        /*
        UNREFERENCED_PARAMETER ( a );
        HANDLE timer;
        LARGE_INTEGER li;
        LONGLONG ns = 120;
        if ( !( timer = CreateWaitableTimer ( NULL, TRUE, NULL ) ) )
            return;

        li.QuadPart = -ns;
        if ( !SetWaitableTimer ( timer, &li, 0, NULL, NULL, FALSE ) ) {
            CloseHandle ( timer );
            return;
        }

        WaitForSingleObject ( timer, INFINITE );
        CloseHandle ( timer );*/
        
        Sleep ( 1 );
#else
        struct timespec ts = { ms / 1000, ( ms % 1000 ) * 1000 * 1000 };
        nanosleep ( &ts, NULL );
#endif
    }

    struct mutex
    {
        volatile long sem;
        volatile bool askwrite;
        mutex ( ) :sem ( 0 ), askwrite ( false ) {}
    };

    class locker
    {
    public:
        locker ( mutex& m, bool shared = false ) : _m ( m ), _write ( !shared ) {
            if ( _write ) { // big brother
                while ( __br_compare_and_swap ( &_m.sem, 0, -1 ) != 0 ) { //someone taking my toy
                    _m.askwrite = true; // I need the toy right now!!!
                    msleep ( 1 ); // wait
                }
                _m.askwrite = false; // your buddies can competing for it now
            }
            else {
                while ( 1 ) {
                    if ( _m.askwrite ) { // the big brother is looking for it, stand by
                        msleep ( 1 );
                    }
                    else if ( __br_fetch_and_add ( &_m.sem, 2 ) <= -1 ) { // big brother is enjoying
                        __br_fetch_and_sub ( &_m.sem, 2 ); // sorry, I'm not mean it, step back
                        msleep ( 1 );
                    }
                    else { // we can share it now
                        break;
                    }
                };
            }
        }
        ~locker ( ) {
            if ( _write ) {
                __br_fetch_and_add ( &_m.sem, 1 );
            }
            else {
                __br_fetch_and_sub ( &_m.sem, 2 );
            }
        }
    private:
        mutex& _m;
        bool _write;
    };

    class condition_variable
    {
    public:
        condition_variable ( ) : count ( 0 ), signal ( 0 ) {}
        ~condition_variable ( ) {
            while ( count > 0 ) {
                notify_one ( );
            }
        }

        bool wait ( unsigned long timeout_ms = -1 ) {
            bool ret = true;
            __br_fetch_and_add ( &count, 1 );
            clock_t start = clock ( );
            while ( __br_fetch_and_sub ( &signal, 1 ) <= 0 ) {
                __br_fetch_and_add ( &signal, 1 );
                clock_t now = clock ( );
                if ( (unsigned long)( ( now - start ) * 1000 / CLOCKS_PER_SEC ) > timeout_ms ) {
                    ret = false;
                    break;
                }
                msleep ( 1 );
            }
            __br_fetch_and_sub ( &count, 1 );
            return ret;
        }

        void notify_one ( ) {
            __br_fetch_and_add ( &signal, 1 );
        }

        void notify_all ( ) {
            __br_set ( &signal, count );
        }

    private:
        volatile long count;
        volatile long signal;
    };
}

#endif