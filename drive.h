#pragma once

/**
Intended usage:

    Drive d(robot);

    // forward, steer with center of curvature on left
    d.move({.forward=0.9, .steer=0.1});

*/
class Drive : Device {
public:
    struct move_args {
        float forward;
        float steer;
    };
    /**
    @param args.forward  non-dimensional linear speed: 1 is full speed
                         forwards, -1 is full speed backwards

    @param args.steer    non-dimensional rotational speed: 1 is full speed
                         CCW

    Should ensure that abs(args.forward) + abs(args.steer) <= 1
    */
    void move(move_args args);

    /// shorthand for no motion
    void stop() { move({.forward = 0, .steer = 0}); }
};


