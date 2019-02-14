#include "StdAfx.h"
#include "Part_graphics_object.h"
#include "Location.h"

int Part_graphics_object::caculate_vertex_data_from_part(int map[],bool frame_object_flag,Part *p,float *buffer_pointer)
{
	int i,j,k,m,n,x,y,start_offset;
	Body *b;
	Face *f;
	Triangle *t;

	for(m=0,start_offset=0;m<(p->body_number);m++){
		for(n=0,b=&(p->body_array[m]);n<(b->face_number);n++){
			f=&(b->face_array[n]);
			if((f->texture_number==texture_number)||frame_object_flag){
				for(i=0;i<(f->triangles.triangle_number);i++){
					t=&(f->triangles.triangle_array[i]);
					start_offset++;
					if(buffer_pointer!=NULL){
						for(j=0;map[j]>=0;j++){
							switch(map[j]){
							default:
							case 0:
								x=t->vertex.p0;y=t->normal.p0;break;
							case 1:
								x=t->vertex.p1;y=t->normal.p1;break;
							case 2:
								x=t->vertex.p2;y=t->normal.p2;break;
							}

							*(buffer_pointer++)=(float)(f->vertices.vertex_array[x].x);
							*(buffer_pointer++)=(float)(f->vertices.vertex_array[x].y);
							*(buffer_pointer++)=(float)(f->vertices.vertex_array[x].z);
							*(buffer_pointer++)=1.0;

							*(buffer_pointer++)=(float)(f->normals.vertex_array[y].x);
							*(buffer_pointer++)=(float)(f->normals.vertex_array[y].y);
							*(buffer_pointer++)=(float)(f->normals.vertex_array[y].z);
							*(buffer_pointer++)=1.0;

							*(buffer_pointer++)=((float)m)+(float)0.25;
							*(buffer_pointer++)=((float)n)+(float)0.25;
							*(buffer_pointer++)=((float)(3*i+j))+(float)0.25;
							*(buffer_pointer++)=((float)(t->material_id))+(float)0.25;
							
							*(buffer_pointer++)=(float)(i)+(float)0.25;
							*(buffer_pointer++)=(float)(j)+(float)0.25;
							*(buffer_pointer++)=(float)(frame_object_flag?1.0:0.0);

							*(buffer_pointer++)=(float)(texture_number)+(float)0.25;

							for(k=0;k<texture_number;k++){
								switch(map[j]){
								default:
								case 0:
									x=t->texture[k].p0;break;
								case 1:
									x=t->texture[k].p1;break;
								case 2:
									x=t->texture[k].p2;break;
								}
								*(buffer_pointer++)=(float)(f->textures[k].vertex_array[x].x);
								*(buffer_pointer++)=(float)(f->textures[k].vertex_array[x].y);
								*(buffer_pointer++)=(float)(f->textures[k].vertex_array[x].z);
								*(buffer_pointer++)=1.0;
							}
						}
					}
				}
			}
		}
	}
	return start_offset;
}

void Part_graphics_object::caculate_vertex_data_from_box(
	Point &p0,Point &p1,Point &p2,Point &p3,Point &p4,Point &p5,Point &p6,Point &p7)
{
	texture_number=0;
	triangle_number=12;
	
	double my_vertex_data[]={
		//vertex						normal					color					secondarycolor

		p0.x,p0.y,p0.z,1.0,				-1.0,0.0,0.0,1.0,		0.0,0.0,0.0,0.0,		0.0,0.0,0.0,0.0,
		p2.x,p2.y,p2.z,1.0,				-1.0,0.0,0.0,1.0,		0.0,0.0,0.0,0.0,		0.0,0.0,0.0,0.0,
		p3.x,p3.y,p3.z,1.0,				-1.0,0.0,0.0,1.0,		0.0,0.0,0.0,0.0,		0.0,0.0,0.0,0.0,

		p3.x,p3.y,p3.z,1.0,				-1.0,0.0,0.0,1.0,		0.0,0.0,0.0,0.0,		0.0,0.0,0.0,0.0,
		p1.x,p1.y,p1.z,1.0,				-1.0,0.0,0.0,1.0,		0.0,0.0,0.0,0.0,		0.0,0.0,0.0,0.0,
		p0.x,p0.y,p0.z,1.0,				-1.0,0.0,0.0,1.0,		0.0,0.0,0.0,0.0,		0.0,0.0,0.0,0.0,



		p4.x,p4.y,p4.z,1.0,				1.0,0.0,0.0,1.0,		0.0,0.0,0.0,0.0,		0.0,0.0,0.0,0.0,
		p5.x,p5.y,p5.z,1.0,				1.0,0.0,0.0,1.0,		0.0,0.0,0.0,0.0,		0.0,0.0,0.0,0.0,
		p7.x,p7.y,p7.z,1.0,				1.0,0.0,0.0,1.0,		0.0,0.0,0.0,0.0,		0.0,0.0,0.0,0.0,

		p7.x,p7.y,p7.z,1.0,				1.0,0.0,0.0,1.0,		0.0,0.0,0.0,0.0,		0.0,0.0,0.0,0.0,
		p6.x,p6.y,p6.z,1.0,				1.0,0.0,0.0,1.0,		0.0,0.0,0.0,0.0,		0.0,0.0,0.0,0.0,
		p4.x,p4.y,p4.z,1.0,				1.0,0.0,0.0,1.0,		0.0,0.0,0.0,0.0,		0.0,0.0,0.0,0.0,



		p0.x,p0.y,p0.z,1.0,				0.0,-1.0,0.0,1.0,		0.0,0.0,0.0,0.0,		0.0,0.0,0.0,0.0,
		p1.x,p1.y,p1.z,1.0,				0.0,-1.0,0.0,1.0,		0.0,0.0,0.0,0.0,		0.0,0.0,0.0,0.0,
		p5.x,p5.y,p5.z,1.0,				0.0,-1.0,0.0,1.0,		0.0,0.0,0.0,0.0,		0.0,0.0,0.0,0.0,

		p5.x,p5.y,p5.z,1.0,				0.0,-1.0,0.0,1.0,		0.0,0.0,0.0,0.0,		0.0,0.0,0.0,0.0,
		p4.x,p4.y,p4.z,1.0,				0.0,-1.0,0.0,1.0,		0.0,0.0,0.0,0.0,		0.0,0.0,0.0,0.0,
		p0.x,p0.y,p0.z,1.0,				0.0,-1.0,0.0,1.0,		0.0,0.0,0.0,0.0,		0.0,0.0,0.0,0.0,



		p2.x,p2.y,p2.z,1.0,				0.0,1.0,0.0,1.0,		0.0,0.0,0.0,0.0,		0.0,0.0,0.0,0.0,
		p6.x,p6.y,p6.z,1.0,				0.0,1.0,0.0,1.0,		0.0,0.0,0.0,0.0,		0.0,0.0,0.0,0.0,
		p7.x,p7.y,p7.z,1.0,				0.0,1.0,0.0,1.0,		0.0,0.0,0.0,0.0,		0.0,0.0,0.0,0.0,

		p7.x,p7.y,p7.z,1.0,				0.0,1.0,0.0,1.0,		0.0,0.0,0.0,0.0,		0.0,0.0,0.0,0.0,
		p3.x,p3.y,p3.z,1.0,				0.0,1.0,0.0,1.0,		0.0,0.0,0.0,0.0,		0.0,0.0,0.0,0.0,
		p2.x,p2.y,p2.z,1.0,				0.0,1.0,0.0,1.0,		0.0,0.0,0.0,0.0,		0.0,0.0,0.0,0.0,



		p1.x,p1.y,p1.z,1.0,				0.0,0.0,-1.0,1.0,		0.0,0.0,0.0,0.0,		0.0,0.0,0.0,0.0,
		p3.x,p3.y,p3.z,1.0,				0.0,0.0,-1.0,1.0,		0.0,0.0,0.0,0.0,		0.0,0.0,0.0,0.0,
		p7.x,p7.y,p7.z,1.0,				0.0,0.0,-1.0,1.0,		0.0,0.0,0.0,0.0,		0.0,0.0,0.0,0.0,

		p7.x,p7.y,p7.z,1.0,				0.0,0.0,-1.0,1.0,		0.0,0.0,0.0,0.0,		0.0,0.0,0.0,0.0,
		p5.x,p5.y,p5.z,1.0,				0.0,0.0,-1.0,1.0,		0.0,0.0,0.0,0.0,		0.0,0.0,0.0,0.0,
		p1.x,p1.y,p1.z,1.0,				0.0,0.0,-1.0,1.0,		0.0,0.0,0.0,0.0,		0.0,0.0,0.0,0.0,



		p0.x,p0.y,p0.z,1.0,				0.0,0.0,1.0,1.0,		0.0,0.0,0.0,0.0,		0.0,0.0,0.0,0.0,
		p4.x,p4.y,p4.z,1.0,				0.0,0.0,1.0,1.0,		0.0,0.0,0.0,0.0,		0.0,0.0,0.0,0.0,
		p6.x,p6.y,p6.z,1.0,				0.0,0.0,1.0,1.0,		0.0,0.0,0.0,0.0,		0.0,0.0,0.0,0.0,

		p6.x,p6.y,p6.z,1.0,				0.0,0.0,1.0,1.0,		0.0,0.0,0.0,0.0,		0.0,0.0,0.0,0.0,
		p2.x,p2.y,p2.z,1.0,				0.0,0.0,1.0,1.0,		0.0,0.0,0.0,0.0,		0.0,0.0,0.0,0.0,
		p0.x,p0.y,p0.z,1.0,				0.0,0.0,1.0,1.0,		0.0,0.0,0.0,0.0,		0.0,0.0,0.0,0.0,

	};
	vertex_data=new float[sizeof(my_vertex_data)/sizeof(my_vertex_data[0])];

	for(int i=0;i<(sizeof(my_vertex_data)/sizeof(my_vertex_data[0]));i++)
		vertex_data[i]=(float)(my_vertex_data[i]);
	return;
}

void Part_graphics_object::caculate_edge_data_from_part(Part *p)
{
	int edge_number=0;

	for(int i=0;i<p->body_number;i++){
		Body *bp=&(p->body_array[i]);
		for(int j=0;j<bp->face_number;j++){
			Face *fp=&(bp->face_array[j]);
			for(int k=0;k<(fp->face_curve.face_loop_number);k++){
				Face_loop *fl=&(fp->face_curve.face_loop[k]);
				for(int m=0;m<(fl->edge_number);m++){
					Face_edge *fe=&(fl->edge[m]);
					if(fe->tessellation_point_number>1)
						edge_number+=(fe->tessellation_point_number)-1;
				}
			}
		}
	}

	if(edge_number<=0){
		vertex_data=NULL;
		texture_number=0;
		triangle_number=0;
		return;
	}
	if((edge_number%3)!=0)
		edge_number=3*(1+(edge_number/3));
	
	texture_number=0;
	triangle_number=2*edge_number/3;

	vertex_data=new float [2*edge_number*4*4];
	float *data_pointer=vertex_data,*end_data_pointer=vertex_data+2*edge_number*4*4;

	for(int i=0;i<p->body_number;i++){
		Body *bp=&(p->body_array[i]);
		for(int j=0;j<bp->face_number;j++){
			Face *fp=&(bp->face_array[j]);
			for(int k=0;k<(fp->face_curve.face_loop_number);k++){
				Face_loop *fl=&(fp->face_curve.face_loop[k]);
				for(int m=0;m<(fl->edge_number);m++){
					Face_edge *fe=&(fl->edge[m]);
					for(int n=0;n<(fe->tessellation_point_number-1);n++){
						Point *fp=&(fe->tessellation[n]),*fq=&(fe->tessellation[n+1]);
						Point np=(*fq)-(*fp);

						int mi=bp->min_material_id;

						data_pointer[ 0]=(float)(fp->x);		data_pointer[ 1]=(float)(fp->y);		data_pointer[ 2]=(float)(fp->z);	data_pointer[ 3]=1.0;
						data_pointer[ 4]=(float)(np.x);			data_pointer[ 5]=(float)(np.y);			data_pointer[ 6]=(float)(np.z);		data_pointer[ 7]=1.0;
						data_pointer[ 8]=(float)0.25+(float)i;	data_pointer[ 9]=(float)0.25+(float)j;	data_pointer[10]=0.0;				data_pointer[11]=(float)0.25+(float)mi;
						data_pointer[12]=(float)0.25+(float)k;	data_pointer[13]=(float)0.25+(float)m;	data_pointer[14]=1.0;				data_pointer[15]=0.0;


						data_pointer[16]=(float)(fq->x);		data_pointer[17]=(float)(fq->y);		data_pointer[18]=(float)(fq->z);	data_pointer[19]=1.0;
						data_pointer[20]=(float)(np.x);			data_pointer[21]=(float)(np.y);			data_pointer[22]=(float)(np.z);		data_pointer[23]=1.0;
						data_pointer[24]=(float)0.25+(float)i;	data_pointer[25]=(float)0.25+(float)j;	data_pointer[26]=0.0;				data_pointer[27]=(float)0.25+(float)mi;
						data_pointer[28]=(float)0.25+(float)k;	data_pointer[29]=(float)0.25+(float)m;	data_pointer[30]=1.0;				data_pointer[31]=0.0;

						data_pointer+=32;
					}
				}
			}
		}
	}
	for(;data_pointer!=end_data_pointer;data_pointer+=32)
		for(int i=0;i<32;i++)
			data_pointer[i]=data_pointer[i-32];
	return;
}


void Part_graphics_object::caculate_point_data_from_part(Part *p)
{
	int point_number=0;

	for(int i=0;i<(p->body_number);i++){
		Body *bp=&(p->body_array[i]);
		for(int j=0;j<bp->face_number;j++){
			Face *fp=&(bp->face_array[j]);
			for(int k=0;k<(fp->face_curve.face_loop_number);k++){
				Face_loop *fl=&(fp->face_curve.face_loop[k]);
				for(int m=0;m<(fl->edge_number);m++){
					Face_edge *fe=&(fl->edge[m]);
					if(fe->start_effective_flag)
						point_number++;
					if(fe->end_effective_flag)
						point_number++;

					switch(fe->curve_type){
					default:
					case 0:
							break;
					case 1:
							point_number++;
							break;
					case 2:
							break;
					}
				}
			}
		}
	}

	if(point_number<=0){
		vertex_data=NULL;
		texture_number=0;
		triangle_number=0;
		return;
	}

	if((point_number%3)!=0)
		point_number=3*(1+(point_number/3));
	
	texture_number=0;
	triangle_number=point_number/3;

	vertex_data=new float [point_number*4*4];
	float *data_pointer=vertex_data,*end_data_pointer=vertex_data+point_number*4*4;


	for(int i=0;i<(p->body_number);i++){
		Body *bp=&(p->body_array[i]);
		for(int j=0;j<bp->face_number;j++){
			Face *fp=&(bp->face_array[j]);
			for(int k=0;k<(fp->face_curve.face_loop_number);k++){
				Face_loop *fl=&(fp->face_curve.face_loop[k]);
				for(int m=0;m<(fl->edge_number);m++){
					Face_edge *fe=&(fl->edge[m]);
					int mi=bp->min_material_id;

					if(fe->start_effective_flag){
								data_pointer[ 0]=(float)(fe->start_point.x);		data_pointer[ 1]=(float)(fe->start_point.y);		data_pointer[ 2]=(float)(fe->start_point.z);		data_pointer[ 3]=1.0;
								data_pointer[ 4]=0.0;								data_pointer[ 5]=0.0;								data_pointer[ 6]=0.0;								data_pointer[ 7]=1.0;
								data_pointer[ 8]=(float)0.25+(float)i;				data_pointer[ 9]=(float)0.25+(float)j;				data_pointer[10]=0.25;								data_pointer[11]=(float)0.25+(float)mi;
								data_pointer[12]=(float)0.25+(float)k;				data_pointer[13]=(float)0.25+(float)m;				data_pointer[14]=2.0;								data_pointer[15]=0.0;

								data_pointer+=4*4;
					}
					if(fe->end_effective_flag){
								data_pointer[ 0]=(float)(fe->end_point.x);			data_pointer[ 1]=(float)(fe->end_point.y);			data_pointer[ 2]=(float)(fe->end_point.z);			data_pointer[ 3]=1.0;
								data_pointer[ 4]=0.0;								data_pointer[ 5]=0.0;								data_pointer[ 6]=0.0;								data_pointer[ 7]=1.0;
								data_pointer[ 8]=(float)0.25+(float)i;				data_pointer[ 9]=(float)0.25+(float)j;				data_pointer[10]=1.25;								data_pointer[11]=(float)0.25+(float)mi;
								data_pointer[12]=(float)0.25+(float)k;				data_pointer[13]=(float)0.25+(float)m;				data_pointer[14]=2.0;								data_pointer[15]=0.0;

								data_pointer+=4*4;
					}
					switch(fe->curve_type){
					default:
					case 0:
						break;
					case 1://circle
						{
								data_pointer[ 0]=(float)(fe->curve_parameter[0]);	data_pointer[ 1]=(float)(fe->curve_parameter[1]);	data_pointer[ 2]=(float)(fe->curve_parameter[2]);	data_pointer[ 3]=1.0;
								data_pointer[ 4]=0.0;								data_pointer[ 5]=0.0;								data_pointer[ 6]=0.0;								data_pointer[ 7]=1.0;
								data_pointer[ 8]=(float)0.25+(float)i;				data_pointer[ 9]=(float)0.25+(float)j;				data_pointer[10]=2.25;								data_pointer[11]=(float)0.25+(float)mi;
								data_pointer[12]=(float)0.25+(float)k;				data_pointer[13]=(float)0.25+(float)m;				data_pointer[14]=2.0;								data_pointer[15]=0.0;

								data_pointer+=4*4;
						}
						break;
					case 2:
						break;
					}
				}
			}
		}
	}
	for(;data_pointer!=end_data_pointer;data_pointer+=16)
		for(int i=0;i<16;i++)
			data_pointer[i]=data_pointer[i-16];
	return;
}