#pragma once

#include "File_data.h"

#include "Network.h"

class Point
{
public:
	double x,y,z;

	inline Point()
	{
		x=0;		y=0;		z=0;;
	};
	inline Point( Point &b)
	{
		x=b.x;		y=b.y;		z=b.z;;
	}
	inline Point(double my_x,double my_y,double my_z)
	{
		x=my_x;		y=my_y;		z=my_z;
	}
	inline Point(double my_x,double my_y,double my_z,double my_w)
	{
		x=my_x/my_w;		y=my_y/my_w;		z=my_z/my_w;
	}
	inline Point(ifstream &f)
	{
		x=0;	y=0;	z=0;	f>>x>>y>>z;	
	};
	virtual ~Point()
	{
	};
	inline Point operator =(ifstream &f)
	{
		Point a(f);
		*this=a;
		return a;
	};
	inline Point operator =(Point &b)
	{
		return Point(x=b.x,	y=b.y,	z=b.z);	
	}
	inline double *operator >>(double b[])
	{
		b[0]=x;
		b[1]=y;
		b[2]=z;
		b[3]=1.0;
		return b;
	};
	inline float *operator >>(float b[])
	{
		b[0]=(float)x;
		b[1]=(float)y;
		b[2]=(float)z;
		b[3]=1.0;		
		return b;
	};
	inline bool operator ==(Point &b)
	{
		return ((x==b.x)&&(y==b.y)&&(z==b.z))?true:false;
	}
	inline Point operator +(Point &b)
	{
		return Point(x+b.x,	y+b.y,	z+b.z);	
	}
	inline Point operator -(Point &b)
	{
		return Point(x-b.x,y-b.y,z-b.z);	
	}
	inline Point operator *(double scale)
	{
		return Point(scale*x,scale*y,scale*z);
	}
	inline Point operator -()
	{
		return Point(-x,-y,-z);	
	}
	inline bool is_zero()
	{
		return (x==0)&&(y==0)&&(z==0);
	}
	inline double distance2()
	{
		return (x*x+y*y+z*z);
	}
	inline double distance()
	{
		return ::sqrtl(distance2());
	}
	inline Point expand(double length)
	{
		length/=distance();
		return Point(x*length,y*length,z*length);
	}
	inline double dot(Point &b)
	{
		return (x*(b.x)+y*(b.y)+z*(b.z));
	}
	inline Point cross(Point &b)
	{
		double x1=b.x,	y1=b.y,	z1=b.z;

		return Point(y*z1-y1*z,z*x1-x*z1,x*y1-y*x1);
	}
	inline Point mix(Point &b,double p)
	{
		double x1=b.x,	y1=b.y,	z1=b.z;

		return Point(p*x+(1-p)*x1,p*y+(1-p)*y1,p*z+(1-p)*z1);
	}
	inline Point mirror(double A,double B,double C,double D)
	{
		double t=2.0*(A*x+B*y+C*z+D)/(A*A+B*B+C*C);
		return Point(x-A*t,y-B*t,z-C*t);
	};
};

class Box
{
public :
	Point p[2];
	inline Box()
	{
		p[0].x=0;p[0].y=0;p[0].z=0;
		p[1].x=0;p[1].y=0;p[1].z=0;
	}
	inline Box( Point pp)
	{
		p[0]=pp;p[1]=pp;
	}
	inline Box( Point pp[])
	{
		p[0].x=(pp[0].x<pp[1].x)?pp[0].x:pp[1].x;
		p[0].y=(pp[0].y<pp[1].y)?pp[0].y:pp[1].y;
		p[0].z=(pp[0].z<pp[1].z)?pp[0].z:pp[1].z;

		p[1].x=(pp[0].x>pp[1].x)?pp[0].x:pp[1].x;
		p[1].y=(pp[0].y>pp[1].y)?pp[0].y:pp[1].y;
		p[1].z=(pp[0].z>pp[1].z)?pp[0].z:pp[1].z;
	}
	inline Box(Point p0,Point p1)
	{
		p[0].x=(p0.x<p1.x)?p0.x:p1.x;
		p[0].y=(p0.y<p1.y)?p0.y:p1.y;
		p[0].z=(p0.z<p1.z)?p0.z:p1.z;

		p[1].x=(p0.x>p1.x)?p0.x:p1.x;
		p[1].y=(p0.y>p1.y)?p0.y:p1.y;
		p[1].z=(p0.z>p1.z)?p0.z:p1.z;		
	}
	inline Box(double x0,double y0,double z0,double x1,double y1,double z1)
	{
		p[0]=Point(x0,y0,z0);
		p[1]=Point(x1,y1,z1);
	}
	inline Box operator + ( Box &b)
	{
		double x0=p[0].x<b.p[0].x?p[0].x:b.p[0].x;
		double y0=p[0].y<b.p[0].y?p[0].y:b.p[0].y;
		double z0=p[0].z<b.p[0].z?p[0].z:b.p[0].z;
		
		double x1=p[1].x>b.p[1].x?p[1].x:b.p[1].x;
		double y1=p[1].y>b.p[1].y?p[1].y:b.p[1].y;
		double z1=p[1].z>b.p[1].z?p[1].z:b.p[1].z;

		return Box(x0,y0,z0,x1,y1,z1);
	}
	inline Point center()
	{
		double x0=p[0].x,y0=p[0].y,z0=p[0].z;
		double x1=p[1].x,y1=p[1].y,z1=p[1].z;
		return Point(x0+x1,y0+y1,z0+z1,2);
	}
	inline double distance()
	{
		return (p[1]-p[0]).distance();
	}
	inline double distance2()
	{
		return (p[1]-p[0]).distance2();
	}
	inline Box operator + ( Point &b)
	{
		return (*this)+Box(b);
	}

	inline bool move_point_into_box(Point &pp)
	{
		int flag=0;
		if((pp.x-p[0].x)<(-MIN_VALUE)){
			pp.x=p[0].x;
			flag++;
		}
		if((pp.y-p[0].y)<(-MIN_VALUE)){
			pp.y=p[0].y;
			flag++;
		}
		if((pp.z-p[0].z)<(-MIN_VALUE)){
			pp.z=p[0].z;
			flag++;
		}
		if((pp.x-p[1].x)>MIN_VALUE){
			pp.x=p[1].x;
			flag++;
		}
		if((pp.y-p[1].y)>MIN_VALUE){
			pp.y=p[1].y;
			flag++;
		}
		if((pp.z-p[1].z)>MIN_VALUE){
			pp.z=p[1].z;
			flag++;
		}
		return (flag>0)?true:false;
	}
	inline Box operator = ( Box &b)
	{
		p[0]=(b.p[0]);
		p[1]=(b.p[1]);
		return Box(p);
	}
	inline virtual ~Box()
	{
	}
};
class Point_set
{
public:
	int point_number;
	Point *vertex_array;

	inline bool release_point_set_memory()
	{
		if(vertex_array!=NULL){
			delete []vertex_array;
			vertex_array=NULL;
			return true;
		}
		return false;
	}

	inline void load(ifstream &f)
	{
		char buffer[MAX_BUFFER_LENGTH];
		
		if(vertex_array!=NULL)
			delete []vertex_array;
		f>>buffer>>point_number;
		if(point_number<=0){
			point_number=0;
			vertex_array=NULL;
		}else{
			vertex_array=new Point[point_number];
			for(int i=0;i<point_number;i++){				
				f>>buffer>>buffer>>buffer;
				vertex_array[i]=Point(f);
			}
		}
		caculate_box();
	}
	inline Point_set()
	{
		point_number=0;
		vertex_array=NULL;
		caculate_box();
	}
	inline virtual ~Point_set()
	{
		if(vertex_array!=NULL)
			delete []vertex_array;
		point_number=0;
	}
	bool has_box_flag;
	Box box;
	inline void caculate_box()
	{
		if(vertex_array==NULL)
			return;
		has_box_flag=false;
		box=Box();
		for(int i=0,j=0;i<point_number;i++){
			has_box_flag=true;
			if((j++)==0)
				box=Box(vertex_array[i]);
			else
				box=box+Box(vertex_array[i]);
		}
	}
};