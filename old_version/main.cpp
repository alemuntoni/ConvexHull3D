#include <QApplication>
#include <QWidget>
#include <QDialog>
#include <QMenu>
#include <QMenuBar>
#include "window_gl.h"
#include "engine.h"
#include "dcel/DCEL.hh"

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
	QWidget container;
	container.resize(640, 680);
	container.setFocusPolicy ( Qt::NoFocus );
	Engine engine(NULL);

	QMenuBar open_menu_bar(&container);
	QMenu open_menu("&File", &container);

	QAction open("&Open", &container);
	open.setShortcut(QKeySequence(Qt::CTRL + Qt::Key_O));
	open_menu.addAction(&open);

	QAction reset("&Reset", &container);
	reset.setShortcut(QKeySequence(Qt::CTRL + Qt::Key_R));
	open_menu.addAction(&reset);

	QAction save("&Save", &container);
	save.setShortcut(QKeySequence(Qt::CTRL + Qt::Key_S));
	open_menu.addAction(&save);

	QAction close("&Close", &container);
	close.setShortcut(QKeySequence(Qt::ALT + Qt::Key_F4));
	open_menu.addAction(&close);

	QMenu draw_menu("&Drawmodes", &container);

	QAction points("&Points", &container);
	points.setShortcut(QKeySequence(Qt::CTRL + Qt::Key_P));
	draw_menu.addAction(&points);

	QAction wire("&Wireframe", &container);
	wire.setShortcut(QKeySequence(Qt::CTRL + Qt::Key_W));
	draw_menu.addAction(&wire);

	QAction flat("&Flat shaded", &container);
	flat.setShortcut(QKeySequence(Qt::CTRL + Qt::Key_F));
	draw_menu.addAction(&flat);

	QAction smooth("&Smooth shaded", &container);
	smooth.setShortcut(QKeySequence(Qt::CTRL + Qt::Key_M));
	draw_menu.addAction(&smooth);

	/**** MENU CONVEX HULL ****/
	QMenu convex_hull("Convex Hull", &container);

	QAction calc("&Calculate CH", &container);
	calc.setShortcut(QKeySequence(Qt::CTRL + Qt::Key_C));
	convex_hull.addAction(&calc);

	/**** MENU HELP ****/
	QMenu help_menu("&?", &container);

	QAction help("&Help", &container);
	help.setShortcut(QKeySequence(Qt::CTRL + Qt::Key_H));
	help_menu.addAction(&help);

	QAction about("&About", &container);
	about.setShortcut(QKeySequence(Qt::CTRL + Qt::Key_A));
	help_menu.addAction(&about);
	
	open_menu_bar.addMenu(&open_menu);
	open_menu_bar.addMenu(&draw_menu);
	open_menu_bar.addMenu(&convex_hull);
	open_menu_bar.addMenu(&help_menu);
	
	Window_gl window(&container);
	window.setGeometry(0,22,640, 680);

	container.show();
	
	QDialog instructions( NULL );
	instructions.setFixedSize(300,180);
	instructions.setWindowTitle("Help");
	QLabel instr_text("\nUp - Sposta l'osservatore verso l'alto\nDown - Sposta l'osservatore verso il basso\nLeft - Ruota verso sinistra\nRight - Ruota verso destra\nShift+Up - Zoom In\nShift+Down - Zoom out\n\nSi ricorda che il programma e' in grado di gestire\nsolo files di tipo .OFF.\nAltri formati non sono attualmente supportati.", &instructions);
	instr_text.setTextFormat(Qt::AutoText);
	instr_text.setWordWrap(true);
	instructions.hide();

	QDialog credits( NULL );
	credits.setFixedSize(300,100);
	credits.setWindowTitle("Credits");
	QLabel cred_text("\tCGVew v.1.7\n\nA cura di Fabio Guggeri (guga2112@gmail.com)\ne Stefano Marras (mr.marras@gmail.com).\n", &credits);
	cred_text.setTextFormat(Qt::AutoText);
	cred_text.setWordWrap(true);
	credits.hide();

	QObject::connect( &open, SIGNAL(triggered()), &engine, SLOT(open_file()) );
	QObject::connect( &reset, SIGNAL(triggered()), &window, SLOT(reset()) );
	QObject::connect( &reset, SIGNAL(triggered()), &engine, SLOT(reset()) );
	QObject::connect( &save, SIGNAL(triggered()), &engine, SLOT(save_file()) );
	QObject::connect( &points, SIGNAL(triggered()), &window, SLOT(set_p_drawmode()) );
	QObject::connect( &wire, SIGNAL(triggered()), &window, SLOT(set_w_drawmode()) );
	QObject::connect( &flat, SIGNAL(triggered()), &window, SLOT(set_f_drawmode()) );
	QObject::connect( &smooth, SIGNAL(triggered()), &window, SLOT(set_s_drawmode()) );
	QObject::connect( &help, SIGNAL(triggered()), &instructions, SLOT(show()) );
	QObject::connect( &about, SIGNAL(triggered()), &credits, SLOT(show()) );
	QObject::connect( &close, SIGNAL(triggered()), &app, SLOT(quit()) );
	QObject::connect( &engine, SIGNAL(send_dcel(QVector<DCEL>&)), &window, SLOT(add_dcel(QVector<DCEL>&)) );
	QObject::connect( &calc, SIGNAL(triggered()), &engine, SLOT(calculate_ch()) );

	window.setFocus();

	return app.exec();
}
