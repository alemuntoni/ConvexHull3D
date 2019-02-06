#include "window_gl.h"

Window_gl::Window_gl(QWidget *parent) : QGLWidget(parent)
{
	reset();
}

void Window_gl::reset( void )
{
	eye.x = 0.0;
	eye.y = 0.0;
	eye.z = 5.0;

	center.x = 0;
	center.y = 0;
	center.z = 0;

	up.x = 0;
	up.y = 1;
	up.z = 0;

	style = FLAT;

	cos_theta = cos(THETA);
	sin_theta = sin(THETA);

	diagonal = 6.0;

	last = 0;

	updateGL();
}


void Window_gl::paintGL(void)
{
	// clean both color and depth buffer
	//
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// set the viewing system
	//
	glLoadIdentity();
	gluLookAt(eye.x, eye.y, eye.z, center.x, center.y, center.z, up.x, up.y, up.z);
	glPushMatrix();


	for(int j=0; j < mesh.size(); j ++)
	{

		// each DCEL will take a different color (progressive transparency)
		//
		glColor4f(0.0f, 0.2f, 0.85f - j/5.0, (GLfloat) 1.0 - j/2.0);


		if (mesh[j].get_faces_n() == 0)
		{
			for(int i=0; i < mesh[j].get_vertex_n(); i++)
			{
				vertex v = mesh[j].get_vertex( i );

				glBegin(GL_POINTS);
				glVertex3f((GLfloat) v.get_coord().x, (GLfloat) v.get_coord().y, (GLfloat) v.get_coord().z );
				glEnd();

			}
		}

		else

		{
			for (int i=0; i < mesh[j].get_faces_n(); i++)
			{
				face f = mesh[j].get_face(i);

				half_edge tmp = mesh[j].get_half_edge(f.get_inner_half_edge());

				vertex v1, v2, v3;
				v1 = mesh[j].get_vertex( tmp.get_from_vertex() );
				v2 = mesh[j].get_vertex( tmp.get_to_vertex() );
				v3 = mesh[j].get_vertex( mesh[j].get_half_edge( tmp.get_next() ).get_to_vertex() );

				if (style == SMOOTH)
				{
					glBegin(GL_TRIANGLES);
					glNormal3f( (GLfloat)v1.get_normal().x, (GLfloat)v1.get_normal().y, (GLfloat)v1.get_normal().z);
					glVertex3f( (GLfloat)v1.get_coord().x, (GLfloat)v1.get_coord().y, (GLfloat)v1.get_coord().z);
					glNormal3f( (GLfloat)v2.get_normal().x, (GLfloat)v2.get_normal().y, (GLfloat)v2.get_normal().z);
					glVertex3f((GLfloat)v2.get_coord().x, (GLfloat)v2.get_coord().y, (GLfloat)v2.get_coord().z);
					glNormal3f( (GLfloat)v3.get_normal().x, (GLfloat)v3.get_normal().y, (GLfloat)v3.get_normal().z);
					glVertex3f( (GLfloat)v3.get_coord().x, (GLfloat)v3.get_coord().y, (GLfloat)v3.get_coord().z);
					glEnd();
				}
				else if (style == FLAT)
				{
					glBegin(GL_TRIANGLES);
					glNormal3f( (GLfloat)f.get_normal().x, (GLfloat)f.get_normal().y, (GLfloat)f.get_normal().z);
					glVertex3f( (GLfloat)v1.get_coord().x, (GLfloat)v1.get_coord().y, (GLfloat)v1.get_coord().z);
					glVertex3f((GLfloat)v2.get_coord().x, (GLfloat)v2.get_coord().y, (GLfloat)v2.get_coord().z);
					glVertex3f( (GLfloat)v3.get_coord().x, (GLfloat)v3.get_coord().y, (GLfloat)v3.get_coord().z);
					glEnd();
				}
				else if (style == WIRE)
				{
					glBegin(GL_LINE_LOOP);
					glNormal3f( (GLfloat)v1.get_normal().x, (GLfloat)v1.get_normal().y, (GLfloat)v1.get_normal().z);
					glVertex3f( (GLfloat)v1.get_coord().x, (GLfloat)v1.get_coord().y, (GLfloat)v1.get_coord().z);
					glNormal3f( (GLfloat)v2.get_normal().x, (GLfloat)v2.get_normal().y, (GLfloat)v2.get_normal().z);
					glVertex3f((GLfloat)v2.get_coord().x, (GLfloat)v2.get_coord().y, (GLfloat)v2.get_coord().z);
					glNormal3f( (GLfloat)v3.get_normal().x, (GLfloat)v3.get_normal().y, (GLfloat)v3.get_normal().z);
					glVertex3f( (GLfloat)v3.get_coord().x, (GLfloat)v3.get_coord().y, (GLfloat)v3.get_coord().z);
					glEnd();
				}
				else
				if (style == CGPOINTS)
				{
					glBegin(GL_POINTS);
					glNormal3f( (GLfloat)v1.get_normal().x, (GLfloat)v1.get_normal().y, (GLfloat)v1.get_normal().z);
					glVertex3f( (GLfloat)v1.get_coord().x, (GLfloat)v1.get_coord().y, (GLfloat)v1.get_coord().z);
					glNormal3f( (GLfloat)v2.get_normal().x, (GLfloat)v2.get_normal().y, (GLfloat)v2.get_normal().z);
					glVertex3f((GLfloat)v2.get_coord().x, (GLfloat)v2.get_coord().y, (GLfloat)v2.get_coord().z);
					glNormal3f( (GLfloat)v3.get_normal().x, (GLfloat)v3.get_normal().y, (GLfloat)v3.get_normal().z);
					glVertex3f( (GLfloat)v3.get_coord().x, (GLfloat)v3.get_coord().y, (GLfloat)v3.get_coord().z);
					glEnd();
				}

			}
		}
	}

	glPopMatrix();
	setFocus();
}

void Window_gl::initializeGL()
{
	QColor background(160, 160, 160);
	qglClearColor( background );

	glShadeModel(GL_SMOOTH);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	float v[]={1,1,1,0};
	glLightfv(GL_LIGHT0,GL_SPOT_DIRECTION,v);
	glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, (GLfloat) 25);
	float light_specular[4] = {1.0, 1.0, 1.0, 0.7};
	float light_ambient[4] = {1.0, 1.0, 1.0, 0.7};
	glLightfv(GL_LIGHT1, GL_SPECULAR, light_specular);
	glLightfv(GL_LIGHT2, GL_AMBIENT, light_ambient);
	float y[] = {0, 0, -1, 0};
	glLightfv(GL_LIGHT3,GL_SPOT_DIRECTION,y);
	glLightf(GL_LIGHT3, GL_LINEAR_ATTENUATION, (GLfloat) 25);

	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
	glEnable(GL_LIGHT2);
	glEnable(GL_LIGHT3);
	glEnable(GL_LIGHTING);
	glEnable(GL_NORMALIZE);
	glEnable(GL_COLOR_MATERIAL);

	// enable transparency
	glEnable (GL_BLEND);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Window_gl::resizeGL(int w, int h)
{
	glViewport(0, 0, (GLint)w, (GLint)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60, 1, 0.5, 2*diagonal);
	glMatrixMode(GL_MODELVIEW);
}


void Window_gl::keyPressEvent( QKeyEvent *event )
{
	if (event->key() == Qt::Key_Left)
	{
		float tmpx = eye.x - center.x;
		float tmpy = eye.y - center.y;
		float tmpz = eye.z - center.z;
		
		float newx = tmpx * cos_theta + tmpz * sin_theta;
		float newy = tmpy;
		float newz = -tmpx * sin_theta + tmpz * cos_theta;
		
		eye.x = center.x + newx;
		eye.y = center.y + newy;
		eye.z = center.z + newz;
	}

	if (event->key() == Qt::Key_Right)
	{
		float tmpx = eye.x - center.x;
		float tmpy = eye.y - center.y;
		float tmpz = eye.z - center.z;
		
		float newx = tmpx * cos_theta - tmpz * sin_theta;
		float newy = tmpy;
		float newz = tmpx * sin_theta + tmpz * cos_theta;
		
		eye.x = center.x + newx;
		eye.y = center.y + newy;
		eye.z = center.z + newz;
	}

	if (event->matches( QKeySequence::SelectNextLine ) )
	{
		float tmpx = eye.x - center.x;
		float tmpy = eye.y - center.y;
		float tmpz = eye.z - center.z;

		float newx = tmpx * 1.02;
		float newy = tmpy * 1.02;
		float newz = tmpz * 1.02;
		
		eye.x = center.x + newx;
		eye.y = center.y + newy;
		eye.z = center.z + newz;
	}
	else
	if (event->matches( QKeySequence::SelectPreviousLine ) )
	{
		float tmpx = eye.x - center.x;
		float tmpy = eye.y - center.y;
		float tmpz = eye.z - center.z;

		float newx = tmpx * 0.98;
		float newy = tmpy * 0.98;
		float newz = tmpz * 0.98;
		
		eye.x = center.x + newx;
		eye.y = center.y + newy;
		eye.z = center.z + newz;
	}
	else
	if (event->key() == Qt::Key_Down)
	{
		eye.y -= 0.1;
	}
	else
	if (event->key() == Qt::Key_Up)
	{
		eye.y += 0.1;
	}

	updateGL();
}

void Window_gl::mousePressEvent( QMouseEvent *event )
{
    setFocus();
    updateGL();
}

void Window_gl::add_dcel( QVector<DCEL>& dc )
{
	mesh = dc;
	float maxx, maxy, maxz, minx, miny, minz;
	for (int j = last; j < mesh.size(); j++)
	{
		CGPointf origin(0,0,0);
		centroid.push_back(origin);

		for(int i=0; i<mesh[j].get_vertex_n(); i++)
		{
			vertex v = mesh[j].get_vertex(i);
			CGPointf v_coord = v.get_coord();
			centroid[j] = centroid[j] + v_coord;

			if(i==0 && j==0)
			{
				maxx = minx = centroid[j].x;
				maxy = miny = centroid[j].y;
				maxz = minz = centroid[j].z;
			}
			else
			{
				if(v_coord.x > maxx) maxx = v_coord.x;
				if(v_coord.x < minx) minx = v_coord.x;
				if(v_coord.y > maxy) maxy = v_coord.y;
				if(v_coord.y < miny) miny = v_coord.y;
				if(v_coord.z > maxz) maxz = v_coord.z;
				if(v_coord.z < minz) minz = v_coord.z;
			}
		}
		centroid[j].x = centroid[j].x / mesh[j].get_vertex_n();
		centroid[j].z = centroid[j].z / mesh[j].get_vertex_n();
		centroid[j].y = centroid[j].y / mesh[j].get_vertex_n();
	}

	diagonal = 1.5 * sqrt(pow(maxx-minx,2)+pow(maxy-miny,2)+pow(maxz-minz,2));
	
	center.x = 0;
	center.y = 0;
	center.z = 0;
	
	eye.x = center.x;
	eye.y = center.y;
	eye.z = center.z + diagonal;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60, (float)height()/width(), 0.5, 2*diagonal);
	glMatrixMode(GL_MODELVIEW);

	updateGL();
}

void Window_gl::set_p_drawmode( void )
{
	style = CGPOINTS;
	updateGL();
}

void Window_gl::set_f_drawmode( void )
{
	style = FLAT;
	updateGL();
}

void Window_gl::set_w_drawmode( void )
{
	style = WIRE;
	updateGL();
}

void Window_gl::set_s_drawmode( void )
{
	style = SMOOTH;
	updateGL();
}
