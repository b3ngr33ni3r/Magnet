#include "Graphics.h"

/// ////////////////////////////////////
//  STATIC FUNCTIONS
/// ////////////////////////////////////
Vector Vector::Cross(const Vector& a, const Vector& b){

    ///**ADD: PIPELINE
    if(DEBUG){
        std::cout << "Crossing\n";
        a.debug_output();
        std::cout << "\tx\n";
        b.debug_output();
        std::cout << std::endl;
    }

    point_t nx = (a.y() * b.z()) - (a.z() * b.y());
    point_t ny = (a.z() * b.x()) - (a.x() * b.z());
    point_t nz = (a.x() * b.y()) - (a.y() * b.x());

    Vector norm(nx, ny, nz);
    norm.normalize();

    return Vector(nx, ny, nz);
}

Vector Vector::GetNormal(const Vector& x, const Vector& y){
    Vector norm = Cross(x, y);

    return norm;
}

angle_t Vector::GetAngle(const Vector& a, const Vector& b){
    angle_t det = (a * b) / (a.length() * b.length());
    angle_t angle = acos(det);
    return angle;
}


/// ////////////////////////////////////
//  CONSTRUCTORS
/// ////////////////////////////////////
Vector::Vector(point_t x, point_t y, point_t z)
:   POINT_MATRIX(4, 1)
{
    set(x, 0, 0);
    set(y, 1, 0);
    set(z, 2, 0);
    set(0, 3, 0);
}
Vector::Vector(const Point& pt)                          //Construct from a point
:    POINT_MATRIX(pt)
{}

Vector::Vector(const POINT_MATRIX& m)                          //Construct from a matrix
:    POINT_MATRIX(m)
{}

Vector::Vector(const Vector& oldvect)
:    POINT_MATRIX(oldvect)
{ std::cout << "Copy constructor\n"; }

/// ////////////////////////////////////
//  GETTERS
/// ////////////////////////////////////
point_t Vector::x() const{ return get(0, 0); }
point_t Vector::y() const{ return get(1, 0); }
point_t Vector::z() const{ return get(2, 0); }
point_t Vector::w() const{ return get(3, 0); }

point_t   Vector::length() const {
    if(x() == 0 && y() == 0 && z() == 0) return 0.0f;

    point_t nx = pow(x(), 2);
    point_t ny = pow(y(), 2);
    point_t nz = pow(z(), 2);
    point_t sum = nx+ny+nz;

    if(sum < 0 ){
        std::cout << "Sum is negative\n";
        sum *= -1;
    }

    point_t r = sqrt(sum);

    return r;
}

bool Vector::isNormal() const{
    if(length() == 1) return true;
    return false;
}

bool Vector::isZero() const{
    if(x() == 0 && y() == 0 && z() == 0) return true;
    return false;
}

/// ////////////////////////////////////
//  SETTERS
/// ////////////////////////////////////
void    Vector::normalize(){
    if(x() == 0 && y() == 0 && z() == 0) return;
    if(isNormal()) return;

    point_t len = length();

    ///**ADD: CONDITIONAL CONFIG
    ///**ADD: PIPELINE
    std::cout << "normalizing vector\n";
    debug_output();

    set((x()/len), 0, 0);
    set((y()/len), 1, 0);
    set((z()/len), 2, 0);

    ///**ADD: CONDITIONAL CONFIG
    ///**ADD: PIPELINE
    std::cout << "\nto\n";
    debug_output();

    std::cout << std::endl;
}

inline bool Vector::isCorrupt() const
{
    return ( w() != 0.f ) ? true : false;
}

inline void Vector::debug_output() const
{
    std::cout << "v{ " << x() << ", " << y() << ", " << z() << ", " << w() << " }";

    if(isCorrupt()){
        std::cout << " WARNING: Vector is corrupt!";
    }
}

