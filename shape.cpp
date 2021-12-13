#include "shape.hpp"




//Base class
//Please implement Shape's member functions
//constructor, getName()
//
//Base class' constructor should be called in derived classes'
//constructor to initizlize Shape's private variable

Shape::Shape(string name) {name_ = name;}
string Shape::getName() {return name_;}


//Rectangle
//Please implement the member functions of Rectangle:
//constructor, getArea(), getVolume(), operator+, operator-
//@@Insert your code here

// Constructor
Rectangle::Rectangle(double width, double length) : Shape("Rectangle")
{
	width_ = width;
	length_ = length;
}

// getArea
double Rectangle::getArea() const{return length_ * width_;}

// getVolume
double Rectangle::getVolume() const{return 0;}

// operator+
Rectangle Rectangle::operator+(const Rectangle &rec)
{
	Rectangle temp(width_ + rec.getWidth(), length_ + rec.getLength());
	return temp;
}

// operator -
Rectangle Rectangle::operator-(const Rectangle &rec)
{
	Rectangle temp(fmax(0, width_ - rec.getWidth()), fmax(0, length_ - rec.getLength()));
	return temp;
}

double Rectangle::getWidth()const{return width_;}
double Rectangle::getLength()const{return length_;}


//Circle
//Please implement the member functions of Circle:
//constructor, getArea(), getVolume(), operator+, operator-
//@@Insert your code here

// Constructor
Circle::Circle(double radius) : Shape("Circle") {radius_ = radius;}

// getArea
double Circle::getArea() const {return (M_PI * radius_ * radius_);}

// getVolume
double Circle::getVolume() const {return 0;}

// operator + 
Circle Circle::operator+(const Circle &cir)
{
	Circle temp(radius_ + cir.getRadius());
	return temp;
}

// operator-
Circle Circle::operator-(const Circle &cir)
{
	Circle temp(fmax(0, radius_ - cir.getRadius()));
	return temp;
}

double Circle::getRadius()const{return radius_;}

//Sphere
//Please implement the member functions of Sphere:
//constructor, getArea(), getVolume(), operator+, operator-
//@@Insert your code here

// Constructor
Sphere::Sphere(double radius) : Shape("Sphere") {radius_ = radius;}

// getArea
double Sphere::getArea()const
{
	return (4 * M_PI * radius_ * radius_);
}

// getVolume
double Sphere::getVolume()const
{
	return ((4.0 / 3.0) * radius_ * radius_ * radius_ * M_PI);
}

// operator+
Sphere Sphere::operator+(const Sphere &sph)
{
	Sphere temp(radius_ + sph.getRadius());
	return temp;
}

// operator-
Sphere Sphere::operator-(const Sphere &sph)
{
	Sphere temp(fmax(0, radius_ - sph.getRadius()));
	return temp;
}

double Sphere::getRadius()const{return radius_;}

//Rectprism
//Please implement the member functions of RectPrism:
//constructor, getArea(), getVolume(), operator+, operator-
//@@Insert your code here

// Constructor
RectPrism::RectPrism(double width, double length, double height) : Shape("RectPrism")
{
	width_ = width;
	length_ = length;
	height_ = height;
}

// getArea
double RectPrism::getArea()const
{
	return (2 * (length_ *  width_  + length_ * height_ + width_ * height_));
}

// getVolume
double RectPrism::getVolume()const
{
	return (length_ * width_ * height_);
}

// operator+
RectPrism RectPrism::operator+(const RectPrism &rectp)
{
	RectPrism temp(width_ + rectp.getWidth(), length_ + rectp.getLength(), height_ + rectp.getHeight());
	return temp;
}

// operator-
RectPrism RectPrism::operator-(const RectPrism &rectp)
{
	RectPrism temp(fmax(0, width_ - rectp.getWidth()), fmax(0, length_ - rectp.getLength()), fmax(0, height_ - rectp.getHeight()));
	return temp;
}

double RectPrism::getWidth()const{return width_;}
double RectPrism::getHeight()const{return height_;}
double RectPrism::getLength()const{return length_;}


 
// Read shapes from test.txt and initialize the objects
// Return a vector of pointers that points to the objects 
vector<Shape*> CreateShapes(char* file_name){
	//@@Insert your code here
	int shape_count;
	ifstream ifs(file_name);
	
	vector<Shape*> shape_vect;
	Shape *shape_ptr;
	ifs >> shape_count;

	for (int i = 0; i < shape_count; i++)
	{
		string name;
		ifs >> name;

		if (name == "Rectangle")
		{
			double width;
			double length;
			ifs >> width >> length;
			shape_ptr = new Rectangle(width, length);
		}

		else if (name == "Circle")
		{
			double radius;
			ifs >> radius;
			shape_ptr = new Circle(radius);
		}

		else if (name == "Sphere")
		{
			double radius;
			ifs >> radius;
			shape_ptr = new Sphere(radius);
		}

		else if (name == "RectPrism")
		{
			double width;
			double length;
			double height;
			ifs >> width >> length >> height;
			shape_ptr = new RectPrism(width, length, height);
		}
		shape_vect.push_back(shape_ptr);
	}
	ifs.close();
	return shape_vect; // please remeber to modify this line to return the correct value
}

// call getArea() of each object 
// return the max area
double MaxArea(vector<Shape*> shapes){
	double max_area = 0;
	//@@Insert your code here
	double cur_max;
	for (int i = 0; i<shapes.size(); i++)
	{
		cur_max = shapes[i]->getArea();
		if (cur_max > max_area)
			max_area = cur_max;
	}
	return max_area;
}


// call getVolume() of each object 
// return the max volume
double MaxVolume(vector<Shape*> shapes){
	double max_volume = 0;
	//@@Insert your code here
	double cur_max;
	for (int i = 0; i<shapes.size(); i++)
	{
		cur_max = shapes[i]->getVolume();
		if (cur_max > max_volume)
			max_volume= cur_max;
	}
	return max_volume;
}
