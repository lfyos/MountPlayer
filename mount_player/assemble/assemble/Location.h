#pragma once

#include <math.h>

class Location
{
public:
	double a[4][4];

	Location load_identity_matrix()
	{
		int i,j;
		for(i=0;i<4;i++)
			for(j=0;j<4;j++)
				a[j][i]=0;
		for(i=0;i<4;i++)
			a[i][i]=1;

		return Location(a);
	}
	double *get_data(double *b)
	{
		for(int i=0,k=0;i<4;i++)
			for(int j=0;j<4;j++,k++)
				b[k]=a[j][i];
		return b;
	}
	Location(void)
	{
		load_identity_matrix();
	}
	Location( double b[16])
	{
		for(int i=0,k=0;i<4;i++)
			for(int j=0;j<4;j++,k++)
				a[j][i]=b[k];

	}
	
	Location( double b[4][4])
	{
		for(int i=0;i<4;i++)
			for(int j=0;j<4;j++)
				a[j][i]=b[j][i];
	}
	Location( Location &b)
	{
		for(int i=0;i<4;i++)
			for(int j=0;j<4;j++)
				a[j][i]=b.a[j][i];

	}
	
	Location operator = ( Location &b)
	{
		for(int i=0;i<4;i++)
			for(int j=0;j<4;j++)
				a[j][i]=b.a[j][i];
		return Location(a);
	}
	Location operator *( double &b)
	{
		Location c;	
		c.load_identity_matrix();
		for(int i=0;i<4;i++)
			for(int j=0;j<4;j++)
				c.a[i][j]=b*c.a[i][j]+(1-b)*a[i][j];
		return c;
	}
	Location operator *(float &b)
	{
		double bb=b;
		return (*this)*bb;
	}


	Location operator -()
	{
		int i,j,k;
		double b[4][4],c[4][4],p;

		for(i=0;i<4;i++)
			for(j=0;j<4;j++)
				b[i][j]=a[i][j];
		for(i=0;i<4;i++)
			for(j=0;j<4;j++)
				c[j][i]=0;
		for(i=0;i<4;i++)
			c[i][i]=1;

		for(i=0;i<3;i++){
			for(k=i,j=i+1;j<4;j++)
				if(fabs((float)(b[j][i]))>fabs((float)(b[k][i])))
					k=j;
			for(j=0;j<4;j++){
				p=b[i][j];		b[i][j]=b[k][j];		b[k][j]=p;
				p=c[i][j];		c[i][j]=c[k][j];		c[k][j]=p;
			}
			for(j=i+1;j<4;j++){
				p=b[j][i]/b[i][i];
				for(k=i+1;k<4;k++)
					b[j][k]-=p*b[i][k];
				for(k=0;k<4;k++)
					c[j][k]-=p*c[i][k];
			}
		}
		for(i=3;i>0;i--)
			for(j=0;j<i;j++){
				p=b[j][i]/b[i][i];
				for(k=0;k<4;k++)
					c[j][k]-=p*c[i][k];
				for(k=i+1;k<4;k++)
					b[j][k]-=p*b[i][k];
			}

		for(i=0;i<4;i++)
			for(j=0;j<4;j++)
				c[i][j]/=b[i][i];

		return Location(c);

	}	
	Location operator * ( Location &b)
	{
		int i,j,k;
		double my_a[4][4],my_b[4][4],my_c[4][4];
	
		for(i=0;i<4;i++)
			for(j=0;j<4;j++){
				my_a[i][j]=a[i][j];
				my_b[i][j]=b.a[i][j];
			}
		for(i=0;i<4;i++)
			for(j=0;j<4;j++)
				for(my_c[i][j]=0,k=0;k<4;k++)
					my_c[i][j]+=my_a[i][k]*my_b[k][j];

		return Location(my_c);
	}
	Location operator / ( Location &b)
	{
		return (*this)*(-Location(b));
	}
};