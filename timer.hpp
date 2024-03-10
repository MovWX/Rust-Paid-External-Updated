#pragma once
#include <chrono>

class Timer
{
public:
    explicit Timer ( )
    {
        timeCreated = std::chrono::duration_cast<std::chrono::nanoseconds>
            ( std::chrono::high_resolution_clock ( ).now ( ).time_since_epoch ( ) );
    };
    template<class duration>
    bool hasElapsed ( duration time )
    {
        return this->elapsed<std::chrono::nanoseconds> ( ).count ( ) >= std::chrono::duration_cast<std::chrono::nanoseconds>( time ).count ( );
    }

    template<class duration>
    duration elapsed ( )
    {
        std::chrono::nanoseconds currentTime = std::chrono::duration_cast<std::chrono::nanoseconds>
            ( std::chrono::high_resolution_clock ( ).now ( ).time_since_epoch ( ) );

        return std::chrono::duration_cast<duration>( currentTime - timeCreated );
    }

    void reset ( ) {
        timeCreated = std::chrono::duration_cast<std::chrono::nanoseconds>
            ( std::chrono::high_resolution_clock ( ).now ( ).time_since_epoch ( ) );
    }

private:
    std::chrono::nanoseconds timeCreated;
};