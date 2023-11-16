// this is test C++ source file for vi command practice
#include <iosteam>

using namespace std;

class Point
{
private:
	int x;
	int y;
	static int cntObj;

public:
	Point() : x(0), y(0)
	{
		cntObj++;
	}
	Point(int _x, int _y) : x(_x), y(_y) { cntObj++; }
	~Point()
	{
		cout << "Destructed.." << endl;
	}
	void setXY(int _x, int _y);
	int getX() const { return this->x; }
	int getY() const { return this->y; }
	static int getCntObj() {return cntObj;}
	Point operator+(const Point& pt);
	Point& operator=(const Point& pt);
	friend ostream& operator <<(ostream& cout, const Point& pt);
	friend class SpyPoint;
};

void print_pt(const Point& pt)
{
	cout << pt.get() << ", " << pt.get() << endl;
}

ostream& operator<<(ostream& cout, const Point& pt)
{
	cout << pt.x << ", " << pt.y;
	return cout;
}

class SpyPoint
{
public:
	void hacking(const Point& pt)
	{
		cout << "x: " << pt.x << endl;
		cout << "y: " << pt.y << endl;
		cout << "CntObj: " << pt.cntObj << endl << endl;
	}
};


int main(){
	
}
