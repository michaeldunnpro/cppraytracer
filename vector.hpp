#pragma once


class Point { // Point definition
    public:
        Point();
    private: 
        int const x;
        int const y; 
        int const z;

};

class Vector : public Point {
    public:
        Vector();
    private:
        int const x;
        int const y;
        int const z;
        
};

