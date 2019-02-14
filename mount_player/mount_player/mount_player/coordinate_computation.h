#pragma once

class coordinate_computation
{
public:
	coordinate_computation(void);
	~coordinate_computation(void);

private:
	void load_identity_matrix(double  a[4][4]);
	void multiply_array(double  a[4][4], double  b[4][4], double  c[4][4]);
	void reset_move_array(double  a[4][4],double dx,double dy,double dz);
	void reset_x_rotate_array(double  a[4][4],double rx);
	void reset_y_rotate_array(double a[4][4], double ry);
	void reset_z_rotate_array(double a[4][4], double rz);
	
public:
	void load_identity_matrix(double  a[16]);	
	void change_4_4_to_16(double a[4][4],double b[16]);
	void change_16_to_4_4(double a[16],double b[4][4]);
	void get_array(double  a[], double dx , double dy , double dz , double rx , double ry , double rz);	

	void caculate_relative_location(double my_parent_location[], double my_location[], double my_relative_location[]);
	
};
