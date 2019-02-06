#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <cmath>

#include <QtGui>
#include <QFile>
#include <QFileDialog>
#include <QByteArray>

#include "dcel/DCEL.hh"

enum State { INPUT, COMPUTED };

class Engine : public QObject
{

	Q_OBJECT


 	public:

                Engine(QObject *parent = 0);



	signals:

		void send_dcel(QVector<DCEL>& dc); // update the GUI



	public slots:

		void reset(void);
		void open_file(void);
		void save_file(void);
		void calculate_ch(void);		// <= you are expected to fill this method in with
						//    the 3D Convex Hull Algorithm.
						//
						//    Every new variable/method/class is welcome as
						//    long as it is useful for the completion of the
						//    exercise.
						//
						//    Try to keep the logical structure of the program
						//    clean (if you need a data structure create a separate
						//    class; if you want to add a functionality to this
						//    class think if that should be either public or private...)


	private:

		bool create_from_file(QString filename);

		State state;
		QVector<DCEL> meshes;
};
