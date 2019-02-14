#include "StdAfx.h"
#include "coordinate_computation.h"
#include "const.h"
#include <math.h>

coordinate_computation::coordinate_computation(void)
{
}

coordinate_computation::~coordinate_computation(void)
{
}

void coordinate_computation::load_identity_matrix(double  a[4][4])
{
	int i,j;
	for(i=0;i<4;i++){
		for(j=0;j<4;j++)
			a[i][j]=0;
		a[i][i]=1;
	}
	return;
}

void coordinate_computation::multiply_array(double  a[4][4], double  b[4][4], double  c[4][4])
{
	int i,j,k;
	double my_a[4][4],my_b[4][4];
	
	for(i=0;i<4;i++)
		for(j=0;j<4;j++){
			my_a[i][j]=a[i][j];
			my_b[i][j]=b[i][j];
		}
	for(i=0;i<4;i++)
		for(j=0;j<4;j++)
			for(c[i][j]=0,k=0;k<4;k++)
				c[i][j]+=my_a[i][k]*my_b[k][j];
	return;
}

void coordinate_computation::reset_move_array(double  a[4][4],double dx,double dy,double dz)
{
	load_identity_matrix(a);
	a[0][3]=dx;
	a[1][3]=dy;
	a[2][3]=dz;
}

void coordinate_computation::reset_x_rotate_array(double  a[4][4],double rx)
{
	load_identity_matrix(a);

	a[1][1]=cosl(rx*PI/180);
	a[2][1]=sinl(rx*PI/180);
	a[1][2]=(-a[2][1]);
	a[2][2]=a[1][1];
}

void coordinate_computation::reset_y_rotate_array(double a[4][4], double ry)
{
	load_identity_matrix(a);
	a[0][0]=cosl(ry*PI/180);
	a[0][2]=sinl(ry*PI/180);
	a[2][0]=(-a[0][2]);
	a[2][2]=a[0][0];
}

void coordinate_computation::reset_z_rotate_array(double a[4][4], double rz)
{
	load_identity_matrix(a);
	a[0][0]=cosl(rz*PI/180);
	a[1][0]=sinl(rz*PI/180);
	a[0][1]=(-a[1][0]);
	a[1][1]=a[0][0];
}
void coordinate_computation::get_array(double  a[], double dx , double dy , double dz , double rx , double ry , double rz)
{
	double x[4][4],y[4][4];

	load_identity_matrix(y);

	reset_x_rotate_array(x,rx);
	multiply_array(x,y,y);

	reset_y_rotate_array(x,ry);	
	multiply_array(x,y,y);

	reset_z_rotate_array(x,rz);
	multiply_array(x,y,y);

	reset_move_array(x,dx,dy,dz);
	multiply_array(x,y,y);

	change_4_4_to_16(y,a);	

}

void coordinate_computation::load_identity_matrix(double  a[])
{
	double b[4][4];
	load_identity_matrix(b);
	change_4_4_to_16(b,a);
}

void coordinate_computation::change_4_4_to_16(double a[4][4],double b[16])
{
	b[ 0]=a[ 0][ 0]	;	
	b[ 1]=a[ 1][ 0]	;	
	b[ 2]=a[ 2][ 0]	;	
	b[ 3]=a[ 3][ 0]	;	

	b[ 4]=a[ 0][ 1]	;	
	b[ 5]=a[ 1][ 1]	;	
	b[ 6]=a[ 2][ 1]	;	
	b[ 7]=a[ 3][ 1]	;	

	b[ 8]=a[ 0][ 2]	;	
	b[ 9]=a[ 1][ 2]	;	
	b[10]=a[ 2][ 2]	;	
	b[11]=a[ 3][ 2]	;	

	b[12]=a[ 0][ 3]	;	
	b[13]=a[ 1][ 3]	;	
	b[14]=a[ 2][ 3]	;	
	b[15]=a[ 3][ 3]	;	
}

void coordinate_computation::change_16_to_4_4(double a[16],double b[4][4])
{
	b[ 0][ 0]=a[ 0];
	b[ 1][ 0]=a[ 1];
	b[ 2][ 0]=a[ 2];
	b[ 3][ 0]=a[ 3];

	b[ 0][ 1]=a[ 4];
	b[ 1][ 1]=a[ 5];
	b[ 2][ 1]=a[ 6];
	b[ 3][ 1]=a[ 7];

	b[ 0][ 2]=a[ 8];
	b[ 1][ 2]=a[ 9];
	b[ 2][ 2]=a[10];
	b[ 3][ 2]=a[11];

	b[ 0][ 3]=a[12];
	b[ 1][ 3]=a[13];
	b[ 2][ 3]=a[14];
	b[ 3][ 3]=a[15];
}

void coordinate_computation::caculate_relative_location(double my_parent_location[], double my_location[], double my_relative_location[])
{
	int i,j,k;
	double a[4][4],b[4][4],c[4][4],p,x;

	change_16_to_4_4(my_parent_location,a);
	change_16_to_4_4(my_location,b);
	load_identity_matrix(c);

	for(i=0;i<3;i++){
		for(k=i,j=i+1;j<4;j++)
			if(fabs((float)(a[j][i]))>fabs((float)(a[k][i])))
				k=j;
		for(j=0;j<4;j++){
			x=a[i][j];		a[i][j]=a[k][j];		a[k][j]=x;
			x=c[i][j];		c[i][j]=c[k][j];		c[k][j]=x;
		}
		for(j=i+1;j<4;j++){
			p=a[j][i]/a[i][i];
			for(k=i+1;k<4;k++)
				a[j][k]-=p*a[i][k];
			for(k=0;k<4;k++)
				c[j][k]-=p*c[i][k];
		}
	}
	for(i=3;i>0;i--)
		for(j=0;j<i;j++){
			p=a[j][i]/a[i][i];
			for(k=0;k<4;k++)
				c[j][k]-=p*c[i][k];
			for(k=i+1;k<4;k++)
				a[j][k]-=p*a[i][k];
		}

	for(i=0;i<4;i++)
		for(j=0;j<4;j++)
			c[i][j]/=a[i][i];
	for(i=0;i<4;i++)
		for(j=0;j<4;j++){
			a[i][j]=0;
			for(k=0;k<4;k++)
				a[i][j]+=c[i][k]*b[k][j];
		}
	change_4_4_to_16(a,my_relative_location);
	return;
}