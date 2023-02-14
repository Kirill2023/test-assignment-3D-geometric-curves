#ifndef SHAPEAPI_H
#define SHAPEAPI_H



#include <iostream>

namespace crvs
{


    enum class Type_ID
    {
        non = 0,

        Point,
        Vector3d,
        Shape,
        Circle,
        Ellipse,
        Spiral,

        end = 255
    };






    class  Point
    {
    public:

        double x;
        double y;
        double z;

        Point();
        Point(double _x, double _y, double _z);

        Point& operator+= (const Point& p);
        const Point operator+ (const Point& p) const;
        Point& operator-= (const Point& p);
        const Point operator- (const Point& p) const;
        Point& operator/= (const double div);
        const Point operator/ (const double div) const;

        friend std::ostream& operator<<(std::ostream& os, const Point& prnt);
    };


    class  Vector3d
    {
    public:
        Point begin;
        Point end;

        Vector3d();
        Vector3d(Point _end);
        Vector3d(Point _begin, Point _end);

        Vector3d& operator+= (const Point& p);
        const Vector3d operator+ (const Point& p) const;
        Vector3d& operator-= (const Point& p);
        const Vector3d operator- (const Point& p) const;

        static double length(Point _begin, Point _end);
        double length() const;
        Vector3d normalize();
    };



    class  Circle;
    class  Ellipse;
    class  Spiral;

    class  Shape
    {
    protected:
        

        Shape() =delete;
        Shape(Type_ID t, std::string n);
        Shape(Type_ID t, std::string n, Point _ref);
    public:

        Point ref;
        const Type_ID type;
        const std::string name;

        Shape(const Shape&) = default;
        Shape & operator =(const Shape&) = default;
        Shape(Shape&&) = default;
        Shape& operator =(Shape&&) = default;
        virtual ~Shape() = default;


        virtual const Circle& usCircle() const;
        virtual const Ellipse& usEllipse() const;
        virtual const Spiral& usSpiral() const;

        virtual Type_ID get_Type() const =0;
        virtual std::string get_Name() const = 0;

        virtual Point get_ref() const = 0; // центр
        virtual Point get_point(double t) const = 0; // точка на кривой
        virtual Vector3d get_vector_D1(double t) const = 0; // направляющий вектор касательной (от {0, 0, 0})
        virtual void print(double t) const = 0;


    };


    class  Circle final : public Shape
    {
    protected:
        double radius;

    public:
        Circle();
        explicit  Circle(double _radius);
        Circle(double _radius, Point _ref);

        Circle(const Circle&) = default;
        Circle& operator =(const Circle&) = default;
        Circle(Circle&&) = default;
        Circle& operator =(Circle&&) = default;
        ~Circle() = default;

        const Circle& usCircle() const override;

        Type_ID get_Type() const override;
        std::string get_Name() const override;
        Point get_ref() const override; // центр
        Point get_point(double t) const override; // точка на кривой
        Vector3d get_vector_D1(double t) const override; // направляющий вектор касательной
        double get_radius() const;
        void print(double) const override;
    };


    class  Ellipse final : public Shape
    {
    protected:
        double radius_x;
        double radius_y;

    public:
        Ellipse();
        Ellipse(double radius_x, double radius_y);
        Ellipse(double radius_x, double radius_y, Point _ref);
        Ellipse(const Ellipse&) = default;
        Ellipse& operator =(const Ellipse&) = default;
        Ellipse(Ellipse&&) = default;
        Ellipse& operator =(Ellipse&&) = default;
        ~Ellipse() = default;

        const Ellipse& usEllipse() const override;

        Type_ID get_Type() const override;
        std::string get_Name() const override;
        Point get_ref() const override; // центр
        Point get_point(double t) const override; // точка на кривой
        Vector3d get_vector_D1(double t) const override; // направляющий вектор касательной
        void print(double) const override;
    };



    class  Spiral final : public Shape
    {
    protected:
        double radius;
        double step;

    public:
        Spiral();
        Spiral(double _radius, double _step);
        Spiral(double _radius, double _step, Point _ref);
        Spiral(const Spiral&) = default;
        Spiral& operator =(const Spiral&) = default;
        Spiral(Spiral&&) = default;
        Spiral& operator =(Spiral&&) = default;
        ~Spiral() = default;

        const Spiral& usSpiral() const override;

        Type_ID get_Type() const override;
        std::string get_Name() const override;
        Point get_ref() const override; // центр
        Point get_point(double t) const override; // точка на кривой
        Vector3d get_vector_D1(double t) const override; // направляющий вектор касательной
        void print(double) const override;
    };

}

#endif // SHAPEAPI_H
