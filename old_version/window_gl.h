#include <QtOpenGL/QGLWidget>
#include <GL/glu.h>
#include <QKeyEvent>
#include <QtOpenGL/QtOpenGL>
#include <iostream>
#include <cmath>
#include "dcel/DCEL.hh" 

#define THETA M_PI/36.0

enum Drawmode { CGPOINTS, WIRE, FLAT, SMOOTH };
// CGPOINTS     draw vertices
// WIRE         draw half-edges
// FLAT         draw facets (flat shaded);
// SMOOTH       draw facets (smooth shaded);


class Window_gl : public QGLWidget
{

	Q_OBJECT

	public:

		Window_gl(QWidget *parent = 0);



	public slots:

		void add_dcel(QVector<DCEL>& dc);
		void reset( void );

		void set_p_drawmode( void );
		void set_w_drawmode( void );
		void set_f_drawmode( void );
		void set_s_drawmode( void );



	protected:

		void initializeGL(void);		// init GL environment (lights, shading, clipping planes, ...)
		void resizeGL(int w, int h);	// handle canvas' resizing
		void paintGL(void);		// render DCELs

		void keyPressEvent( QKeyEvent *event );
		void mousePressEvent( QMouseEvent *event );



	private:

		Drawmode style;

		CGPointf eye,	 // camera position
			 center, // point observed by the camera (tipically the DCEL's centroid)
			 up;	 // up direction

		QVector<DCEL> mesh;

		double diagonal; // bbox diagonal

		QVector<CGPointf> centroid; // DCEL centroids

		float cos_theta;
		float sin_theta;

		int last; // pointer to the last DCEL in the vector
};
