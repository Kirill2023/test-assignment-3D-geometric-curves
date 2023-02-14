#include <typeinfo>
#include <cmath>
#include <iomanip>
#include <corecrt_math_defines.h>
#include <sstream>
#include <string>
#include "shapeapi.h"

namespace crvs
{

    //++++++++++++ Point ++++++++++++

    Point::Point()
    {
        x = 0.0;
        y = 0.0;
        z = 0.0;
    }

    Point::Point(double _x, double _y, double _z)
    {
        x = _x;
        y = _y;
        z = _z;
    }

    Point& Point::operator+=(const Point& p)
    {
        x += p.x;
        y += p.y;
        z += p.z;
        return *this;
    }

    const Point Point::operator+(const Point& p) const
    {
        return Point(*this) += p;
    }

    Point& Point::operator-=(const Point& p)
    {
        x -= p.x;
        y -= p.y;
        z -= p.z;
        return *this;
    }

    const Point Point::operator-(const Point& p) const
    {
        return Point(*this) -= p;
    }

    Point& Point::operator/=(const double div)
    {
        x /= div;
        y /= div;
        z /= div;
        return *this;
    }

    const Point Point::operator/(const double div) const
    {
        return Point(*this) /= div;
    }

    std::ostream& operator<<(std::ostream& os, const Point& prnt)
    {
        os << " " << std::setw(12) << prnt.x << ", " << std::setw(12) << prnt.y << ", " << std::setw(12) << prnt.z << " ";
        return os;
    }



    //++++++++++++ Vector3d ++++++++++++
    Vector3d::Vector3d()
    {
        begin = Point{ 0.0, 0.0, 0.0 };
        end = Point{ 0.0, 0.0, 0.0 };
    }

    Vector3d::Vector3d(Point _end)
    {
        begin = Point{ 0.0, 0.0, 0.0 };
        end = _end;
    }

    Vector3d::Vector3d(Point _begin, Point _end)
    {
        begin = _begin;
        end = _end;
    }


    Vector3d& Vector3d::operator+=(const Point& p)
    {
        begin += p;
        end += p;
        return *this;
    }

    const Vector3d Vector3d::operator+(const Point& p) const
    {
        return Vector3d(*this) += p;
    }

    Vector3d& Vector3d::operator-=(const Point& p) 
    {
        begin -= p;
        end -= p;
        return *this;
    }

    const Vector3d Vector3d::operator-(const Point& p) const
    {
        return Vector3d(*this) -= p;
    }

    double Vector3d::length(Point _begin, Point _end)
    {
        Point dif = _end - _begin;
        return sqrt(pow(dif.x, 2) + pow(dif.y, 2) + pow(dif.z, 2));
    }

    double Vector3d::length() const
    {
        return Vector3d::length(begin, end);
    }

    Vector3d Vector3d::normalize()
    {
        return Vector3d(begin, ((end - begin) / length()) + begin);
    }




    //++++++++++++ Shape ++++++++++++
    //Shape::Shape() :type(Type_ID::non)
    //{
    //    ref = Point();
    //}
    Shape::Shape(Type_ID t, std::string n) : type(t), name(n)
    {
        ref = Point();
    }
    Shape::Shape(Type_ID t, std::string n, Point _ref) : type(t), name(n), ref(_ref)
    {
       ;
    }

    const Circle& Shape::usCircle() const
    {
        std::cerr << "Error: Attempt to use the " + name + " object as a Circle\n";
        throw "Error: Attempt to use the " + name + " object as a Circle";
        return static_cast<Circle&>(*const_cast<Shape*>(this));
    }

    const Ellipse& Shape::usEllipse() const
    {
        std::cerr << "Error: Attempt to use the " + name + " object as an Ellipse\n";
        throw "Error: Attempt to use the " + name + " object as an Ellipse";
        return static_cast<Ellipse&>(*const_cast<Shape*>(this));
    }

    const Spiral& Shape::usSpiral() const
    {
        std::cerr << "Error: Attempt to use the " + name + " object as a Spiral\n";
        throw "Error: Attempt to use the " + name + " object as a Spiral";
        return static_cast<Spiral&>(*const_cast<Shape*>(this));
    }




    //++++++++++++ Circle ++++++++++++
    Circle::Circle() : Shape(Type_ID::Circle, "Circle")
    {
        radius = 0.0;
    }

    Circle::Circle(double _radius) : Shape(Type_ID::Circle, "Circle")
    {
        if (_radius < 0)
        {
            throw "Error: radius < 0";
        }
        radius = _radius;
    }

    Circle::Circle(double _radius, Point _ref) : Shape(Type_ID::Circle, "Circle", _ref)
    {
        if (_radius < 0)
        {
            throw "Error: radius < 0";
        }
        radius = _radius;
    }

    const Circle& Circle::usCircle() const
    {
        return *this;
    }

    Type_ID Circle::get_Type() const
    {
        return type;
    }

    std::string Circle::get_Name() const
    {
        return name;
    }


    Point Circle::get_ref() const
    {
        return ref;
    }

    Point Circle::get_point(double t) const
    {
        Point tmp;
        tmp.x = radius * cos(t) + ref.x;
        tmp.y = radius * sin(t) + ref.y;
        tmp.z = ref.z;
        return tmp;
    }

    Vector3d Circle::get_vector_D1(double t) const
    {
        Point tmp;
        tmp.x = -1.0 * radius * sin(t);
        tmp.y = radius * cos(t);
        tmp.z = 0;
        return Vector3d(tmp);
    }


    double Circle::get_radius() const
    {
        return radius;
    }

    void Circle::print(double t)  const
    {
        std::string sp = "   ";
        //std::cout << typeid(*this).name() << "\t";
        std::cout << (*this).name << "\t\t";

        std::cout << "Point = {" << get_point(t) << "}" << sp;

        std::cout << "D1 = {" << get_vector_D1(t).end << "}" << sp;

        std::cout << "Radius   = " << std::setw(10) << radius << sp;

        std::cout << std::endl;
    }




    //++++++++++++ Ellipse ++++++++++++
    Ellipse::Ellipse() : Shape(Type_ID::Ellipse, "Ellipse")
    {
        radius_x = 0.0;
        radius_y = 0.0;
    }

    Ellipse::Ellipse(double _radius_x, double _radius_y) : Shape(Type_ID::Ellipse, "Ellipse")
    {
        if (_radius_x < 0)
        {
            throw "Error: radius1 < 0";
        }
        radius_x = _radius_x;

        if (_radius_y < 0)
        {
            throw "Error: radius2 < 0";
        }
        radius_y = _radius_y;
    }

    Ellipse::Ellipse(double _radius_x, double _radius_y, Point _ref) : Shape(Type_ID::Ellipse, "Ellipse", _ref)
    {
        if (_radius_x < 0)
        {
            throw "Error: radius1 < 0";
        }
        radius_x = _radius_x;

        if (_radius_y < 0)
        {
            throw "Error: radius2 < 0";
        }
        radius_y = _radius_y;
    }


    const Ellipse& Ellipse::usEllipse() const
    {
        return *this;
    }

    Type_ID Ellipse::get_Type() const
    {
        return type;
    }

    std::string Ellipse::get_Name() const
    {
        return name;
    }

    Point Ellipse::get_ref() const
    {
        return ref;
    }

    Point Ellipse::get_point(double t) const
    {
        Point tmp;
        tmp.x = radius_x * cos(t) + ref.x;
        tmp.y = radius_y * sin(t) + ref.y;
        tmp.z = ref.z;
        return tmp;
    }

    Vector3d Ellipse::get_vector_D1(double t) const
    {
        Point tmp;
        tmp.x = -1.0 * radius_x * sin(t);
        tmp.y = radius_y * cos(t);
        tmp.z = 0.0;
        return Vector3d(tmp);
    }


    void Ellipse::print(double t)  const
    {
        std::string sp = "   ";
        //std::cout << typeid(*this).name() << "\t";
        std::cout << (*this).name << "\t\t";

        std::cout << "Point = {" << get_point(t) << "}" << sp;

        std::cout << "D1 = {" << get_vector_D1(t).end << "}" << sp;

        std::cout << "Radius_x = " << std::setw(10) << radius_x << sp;
        std::cout << "Radius_y = " << std::setw(10) << radius_y << sp;

        std::cout << std::endl;
    }


    //++++++++++++ Spiral ++++++++++++
    Spiral::Spiral() : Shape(Type_ID::Spiral, "Spiral")
    {
        radius = 0.0;
        step = 0.0;
    }

    Spiral::Spiral(double _radius, double _step) : Shape(Type_ID::Spiral, "Spiral")
    {
        if (_radius < 0)
        {
            throw "Error: radius < 0";
        }
        radius = _radius;
        step = _step;
    }

    Spiral::Spiral(double _radius, double _step, Point _ref) : Shape(Type_ID::Spiral, "Spiral", _ref)
    {
        if (_radius < 0)
        {
            throw "Error: radius < 0";
        }
        radius = _radius;
        step = _step;
    }



    const Spiral& Spiral::usSpiral() const
    {
        return *this;
    }

    Type_ID Spiral::get_Type() const
    {
        return type;
    }

    std::string Spiral::get_Name() const
    {
        return name;
    }

    Point Spiral::get_ref() const
    {
        return ref;
    }

    Point Spiral::get_point(double t) const
    {
        Point tmp;
        tmp.x = radius * cos(t) + ref.x;
        tmp.y = radius * sin(t) + ref.y;
        tmp.z = step / (2 * M_PI) * t + ref.z;
        return tmp;
    }

    Vector3d Spiral::get_vector_D1(double t) const
    {
        Point tmp;
        tmp.x = -1.0 * radius * sin(t);
        tmp.y = radius * cos(t);
        tmp.z = step / (2 * M_PI);
        return Vector3d(tmp);
    }


    void Spiral::print(double t)  const
    {
        std::string sp = "   ";
        //std::cout << typeid(*this).name() << "\t";
        std::cout << (*this).name << "\t\t";

        std::cout << "Point = {" << get_point(t) << "}" << sp;

        std::cout << "D1 = {" << get_vector_D1(t).end << "}" << sp;

        std::cout << "Radius   = " << std::setw(10) << radius << sp;
        std::cout << "Step     = " << std::setw(10) << step << sp;

        std::cout << std::endl;
    }
}