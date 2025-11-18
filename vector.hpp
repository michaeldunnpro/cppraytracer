#pragma once


class Point { // Point definition
    public:
        Point();
        int const x;
        int const y; 
        int const z;

};

class Vector : public Point {
    public:
        Vector();
        int const x;
        int const y;
        int const z;
        
};

