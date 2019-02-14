#pragma once

#include "Part_graphics_object.h"

class Location
{
	double a[4][4];
public:
	
	inline static Location standard_positive()
	{
		return Location(Point(0,0,0),Point(1,0,0),Point(0,1,0),Point(0,0,1));
	}
	inline static Location standard_negative()
	{
		double x[4][4];

		x[0][0]=-1;		x[0][1]=-1;		x[0][2]=-1;		x[0][3]=1;
		x[1][0]=1;		x[1][1]=0;		x[1][2]=0;		x[1][3]=0;
		x[2][0]=0;		x[2][1]=1;		x[2][2]=0;		x[2][3]=0;
		x[3][0]=0;		x[3][1]=0;		x[3][2]=1;		x[3][3]=0;
		return Location(x);
	}

	inline Location(void)
	{
		for(int i=0;i<4;i++){
			for(int j=0;j<4;j++)
				a[j][i]=0;
			a[i][i]=1;
		}
	}
	inline Location(double b[16])
	{
		for(int i=0,k=0;i<4;i++)
			for(int j=0;j<4;j++,k++)
				a[j][i]=b[k];
	}
	inline Location(ifstream &f)
	{
		double b[16];
		for(int i=0;i<16;i++)
			f>>(b[i]);
		(*this)=Location(b);
	}
	inline Location(double b[4][4])
	{
		for(int i=0;i<4;i++)
			for(int j=0;j<4;j++)
				a[j][i]=b[j][i];
	}
	inline Location(Location &b)
	{
		for(int i=0;i<4;i++)
			for(int j=0;j<4;j++)
				a[j][i]=b.a[j][i];
	}
	inline Location(Point &p0, Point &p1, Point &p2, Point &p3)
	{
		a[0][0]=p0.x;		a[0][1]=p1.x;		a[0][2]=p2.x;		a[0][3]=p3.x;	
		a[1][0]=p0.y;		a[1][1]=p1.y;		a[1][2]=p2.y;		a[1][3]=p3.y;
		a[2][0]=p0.z;		a[2][1]=p1.z;		a[2][2]=p2.z;		a[2][3]=p3.z;
		a[3][0]=1.0;		a[3][1]=1.0;		a[3][2]=1.0;		a[3][3]=1.0;	
	}
	inline Location(Box &b)
	{
		a[0][0]=(b.p[1].x)-(b.p[0].x);	a[0][1]=0;						a[0][2]=0;						a[0][3]=b.p[0].x;	
		a[1][0]=0;						a[1][1]=(b.p[1].y)-(b.p[0].y);	a[1][2]=0;						a[1][3]=b.p[0].y;
		a[2][0]=0;						a[2][1]=0;						a[2][2]=(b.p[1].z)-(b.p[0].z);	a[2][3]=b.p[0].z;
		a[3][0]=0;						a[3][1]=0;						a[3][2]=0;						a[3][3]=1.0;
	}
	inline void get_location_float_data(float *data)
	{
		for(int i=0,j=0;i<4;i++)
			for(int k=0;k<4;j++,k++)
				data[j]=(float)(a[k][i]);
	}
	inline void get_location_double_data(double *data)
	{
		for(int i=0,j=0;i<4;i++)
			for(int k=0;k<4;j++,k++)
				data[j]=a[k][i];
	}	
	inline Location operator -()
	{
		const int n=4;
		int i,j,k;
		double p,b[n][n],c[n][n];
	
		for(i=0;i<n;i++){
			for(j=0;j<n;j++){
				b[i][j]=a[i][j];
				c[i][j]=0;
			}
			c[i][i]=1;
		}
		for(i=0;i<(n-1);i++){
			for(k=i,j=i+1;j<n;j++)
				if(::abs(b[j][i])>::abs(b[k][i]))
					k=j;
			for(j=i;j<n;j++){
				p=b[i][j];
				b[i][j]=b[k][j];
				b[k][j]=p;
			}
			for(j=0;j<n;j++){
				p=c[i][j];
				c[i][j]=c[k][j];
				c[k][j]=p;
			}
			for(j=i+1;j<n;j++){
				p=b[j][i]/b[i][i];
				for(k=i+1;k<n;k++)
					b[j][k]-=p*b[i][k];
				for(k=0;k<n;k++)
					c[j][k]-=p*c[i][k];
			}
		}
		for(i=(n-1);i>0;i--)
			for(j=0;j<i;j++){
				p=b[j][i]/b[i][i];
				for(k=0;k<n;k++)
					c[j][k]-=p*c[i][k];
			}
		for(i=0;i<n;i++){
			p=b[i][i];
			for(j=0;j<n;j++)
				c[i][j]/=p;
		}
		return Location(c);
	}
	inline void caculate_local_plane(double &A,double &B,double &C,double &D)
	{
		double aa=A,bb=B,cc=C,dd=D;
		A=a[0][0]*aa+a[1][0]*bb+a[2][0]*cc+a[3][0]*dd;
		B=a[0][1]*aa+a[1][1]*bb+a[2][1]*cc+a[3][1]*dd;
		C=a[0][2]*aa+a[1][2]*bb+a[2][2]*cc+a[3][2]*dd;
		D=a[0][3]*aa+a[1][3]*bb+a[2][3]*cc+a[3][3]*dd;
	}
	inline void caculate_world_plane(double &A,double &B,double &C,double &D)
	{
		(-(*this)).caculate_local_plane(A,B,C,D);
	}
	inline Point operator *( Point &b)
	{
		return Point(	a[0][0]*(b.x)+a[0][1]*(b.y)+a[0][2]*(b.z)+a[0][3],
						a[1][0]*(b.x)+a[1][1]*(b.y)+a[1][2]*(b.z)+a[1][3],
						a[2][0]*(b.x)+a[2][1]*(b.y)+a[2][2]*(b.z)+a[2][3],
						a[3][0]*(b.x)+a[3][1]*(b.y)+a[3][2]*(b.z)+a[3][3]
					);
	}
	inline Box operator *( Box &b)
	{
		Box ret_val;
		for(int i=0;i<2;i++)
			for(int j=0;j<2;j++)
				for(int k=0;k<2;k++){
					Box a((*this)*Point(b.p[i].x,b.p[j].y,b.p[k].z));
					if((i+j+k)==0)
						ret_val=a;
					else
						ret_val=ret_val+a;
				}
		return ret_val;
	}
	inline Location operator * ( Location &b)
	{
		double x[4][4];	
		for(int i=0;i<4;i++)
			for(int j=0;j<4;j++){
				x[i][j]=0;
				for(int k=0;k<4;k++)
					x[i][j]+=a[i][k]*(b.a[k][j]);
			}
		return Location(x);
	}
	inline Location normalize()
	{
		Point s0=(*this)*Point(0,0,0);
		Point sx=((*this)*Point(1,0,0))-s0;
		Point sy=((*this)*Point(0,1,0))-s0;
		Point sz=((*this)*Point(0,0,1))-s0;

		double dx2=sx.distance2(),dy2=sy.distance2(),dz2=sz.distance2();

		if((dx2>=dy2)&&(dx2>=dz2)){//keep X
			double a_xy=sx.dot(sy);
			double a_xz=sx.dot(sz);
			double a_xy2=(a_xy*a_xy)/(dx2*dy2);
			double a_xz2=(a_xz*a_xz)/(dx2*dz2);

			if(a_xy2>a_xz2){
					sy=sz.cross(sx);
					sz=sx.cross(sy);
			}else{
					sz=sx.cross(sy);
					sy=sz.cross(sx);
			}
		}else if((dy2>=dx2)&&(dy2>=dz2)){//keep Y
			double a_xy=sx.dot(sy);
			double a_yz=sy.dot(sz);
			double a_xy2=(a_xy*a_xy)/(dx2*dy2);
			double a_yz2=(a_yz*a_yz)/(dy2*dz2);

			if(a_xy2>a_yz2){
					sx=sy.cross(sz);
					sz=sx.cross(sy);
			}else{
					sz=sx.cross(sy);
					sx=sy.cross(sz);
			}
		}else {//keep Z
			double a_xz=sx.dot(sz);
			double a_yz=sy.dot(sz);
			double a_xz2=(a_xz*a_xz)/(dx2*dz2);
			double a_yz2=(a_yz*a_yz)/(dy2*dz2);

			if(a_xz2>a_yz2){
					sx=sy.cross(sz);
					sy=sz.cross(sx);
			}else{
					sy=sz.cross(sx);
					sx=sy.cross(sz);					
			}
		}

		return Location(s0,s0+sx.expand(1),s0+sy.expand(1),s0+sz.expand(1))*(Location::standard_negative());
	}

	inline Location mix(Location &b,double p)
	{
		double new_location[4][4];
		for(int i=0;i<4;i++)
			for(int j=0;j<4;j++)
				new_location[i][j]=(1-p)*a[i][j]+p*(b.a[i][j]);
		return Location(new_location);
	}
	inline static Location rotation(Point &ss,Point &tt)
	{
		Point s(ss),t(tt);
		if(s.distance2()<MIN_VALUE)
			return Location();

		if(t.distance2()<MIN_VALUE)
			return Location();

		s=s.expand(1);
		t=t.expand(1);
		Point aix=s.cross(t);
		if(aix.distance2()<MIN_VALUE)
			return Location();

		aix=aix.expand(1);
		Point old_p0(0,0,0),					new_p0(0,0,0);
		Point old_p1=aix,						new_p1=aix;
		Point old_p2=s,							new_p2=t;
		Point old_p3=s.cross(aix).expand(1),	new_p3=t.cross(aix).expand(1);
	
		Location x(old_p0,old_p1,old_p2,old_p3),y(new_p0,new_p1,new_p2,new_p3);

		return (y*(-x));
	}

	inline static Location move(double dx,double dy,double dz)
	{
		double x[4][4];

		for(int i=0;i<4;i++){
			for(int j=0;j<4;j++)
				x[j][i]=0;
			x[i][i]=1;
		}
		x[0][3]=dx;
		x[1][3]=dy;
		x[2][3]=dz;

		return Location(x);
	}
	inline static Location move(Point& mov) 
	{
		return move(mov.x,mov.y,mov.z);
	}
	inline static Location x_rotate(double rx)
	{
		double x[4][4];

		for(int i=0;i<4;i++){
			for(int j=0;j<4;j++)
				x[j][i]=0;
			x[i][i]=1;
		}
		x[1][1]=::cosl(rx*M_PI/180);
		x[2][1]=::sinl(rx*M_PI/180);
		x[1][2]=(-x[2][1]);
		x[2][2]=x[1][1];

		return Location(x);
	}
	inline static Location y_rotate(double ry)
	{
		double x[4][4];

		for(int i=0;i<4;i++){
			for(int j=0;j<4;j++)
				x[j][i]=0;
			x[i][i]=1;
		}

		x[0][0]=::cosl(ry*M_PI/180);
		x[0][2]=::sinl(ry*M_PI/180);
		x[2][0]=(-x[0][2]);
		x[2][2]=x[0][0];

		return Location(x);
	}
	inline static Location z_rotate(double rz)
	{
		double x[4][4];

		for(int i=0;i<4;i++){
			for(int j=0;j<4;j++)
				x[j][i]=0;
			x[i][i]=1;
		}

		x[0][0]=::cosl(rz*M_PI/180);
		x[1][0]=::sinl(rz*M_PI/180);
		x[0][1]=(-x[1][0]);
		x[1][1]=x[0][0];

		return Location(x);
	}
	inline static Location frustem(double left_value,double right_value,double bottom_value,double top_value,double near_value,double far_value)
	{
		double x[4][4];

		x[0][0]=2.0*near_value/(right_value-left_value);		
		x[0][1]=0.0;			
		x[0][2]=(right_value+left_value)/(right_value-left_value);
		x[0][3]=0.0;

		x[1][0]=0.0;
		x[1][1]=2.0*near_value/(top_value-bottom_value);
		x[1][2]=(top_value+bottom_value)/(top_value-bottom_value);
		x[1][3]=0.0;

		x[2][0]=0.0;
		x[2][1]=0.0;
		x[2][2]=(near_value+far_value)/(near_value-far_value);
		x[2][3]=2.0*near_value*far_value/(near_value-far_value);

		x[3][0]=0.0;
		x[3][1]=0.0;
		x[3][2]=(-1.0);
		x[3][3]=0.0;

		return Location(x);
	}

	inline static Location orthographic(double left_value,double right_value,double bottom_value,double top_value,double near_value,double far_value)
	{
		double x[4][4];

		x[0][0]=2.0/(right_value-left_value);		
		x[0][1]=0.0;			
		x[0][2]=0.0;
		x[0][3]=(right_value+left_value)/(left_value-right_value);

		x[1][0]=0.0;
		x[1][1]=2.0/(top_value-bottom_value);
		x[1][2]=0.0;
		x[1][3]=(top_value+bottom_value)/(bottom_value-top_value);

		x[2][0]=0.0;
		x[2][1]=0.0;
		x[2][2]=2.0/(near_value-far_value);
		x[2][3]=(near_value+far_value)/(near_value-far_value);

		x[3][0]=0.0;
		x[3][1]=0.0;
		x[3][2]=0.0;
		x[3][3]=1.0;

		return Location(x);
	}

	inline static Location caculate_project_matrix(double half_fovy_tanl,double aspect_value,double near_value,double far_value,double distance,bool type_flag)
	{
		double top_value=(type_flag?near_value:distance)*half_fovy_tanl;
		double bottom_value=(-top_value);
		double right_value=top_value*aspect_value;
		double left_value=(-right_value);
		
		if(type_flag)
			return frustem(left_value,right_value,bottom_value,top_value,near_value,far_value);
		else
			return orthographic(left_value,right_value,bottom_value,top_value,near_value,far_value);
	}

	inline static Location lookat(Point eye_point,Point center_point,Point up_point)
	{
		Point y_direction=up_point-center_point;
		Point z_direction=(eye_point-center_point).expand(1);
		Point x_direction=y_direction.cross(z_direction).expand(1);
		y_direction=z_direction.cross(x_direction).expand(1);

		Location a(eye_point,eye_point+x_direction,eye_point+y_direction,eye_point+z_direction);
		
		return standard_positive()*(-a);
	}
	inline static Location lookat(Location &loca,double distance)
	{
		return Location::move(0,0,-distance)*(-loca);
	}
};