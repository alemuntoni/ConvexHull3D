#include "engine.h"
#include <assert.h>
#include "timer.h"

#include <unistd.h>

#include "utils.h"
#include "conflict_graph/conflict_graph.h"
#include "dcel_operations.h"

Engine::Engine(QObject *parent) : QObject(parent)
{
	reset();
}


void Engine::reset(void)
{
	state = INPUT;
	meshes.clear();
}


void Engine::calculate_ch(void)
{
	Timer timer("3D Convex Hull");
	timer.start();

    DCEL convex_hull;                           //DCEL contenente il convex hull
    QVector<CGPointf> set_points;               //insieme dei punti
    int last = meshes.size() - 1;               //indice dell'ultima mesh caricata
    int n_vertex = meshes[last].get_vertex_n(); //numero di vertici dell'ultima mesh
    int i;
    point_pointer point;
    float determinant=0;                        //variabile per il calcolo del determinante
    CGPointf p0, p1, p2, p3;                    //i 4 punti iniziali che andranno a formare il tetraedro
    face_id f0, f1, f2, f3;                     //id delle prime 4 facce della DCEL
    conflict_graph cg;
    face_pointer cgf0, cgf1, cgf2, cgf3;        //puntatori ai nodi nel conflict graph delle prime 4 facce nella DCEL
    CGPointf next_point;                        //punto del conflict graph
    QVector<face_id> * visible_faces ;          //array delle facce visibili dal punto attuale
    QVector<vertex_id> horizon_vertex;          //array di vertici sull'orizzonte attuale
    QVector<face_id> horizon_faces;             //array di facce incidenti sull'orizzonte attuale
    QVector<edge_id> horizon;                   //array ordinato di edge sull'orizzonte attuale
    QVector< QVector<point_pointer> > P;        //per ogni edge sull'orizzonte, vi è la lista di punti che vedono l'edge
    QVector<vertex_id> deleted_vertex;
    QVector<face_id> deleted_faces;
    QVector<edge_id> deleted_edges;

    /**
     * Carico i punti nell'insieme di punti, e ne calcolo una permutazione
     * casuale.
     */
    for (i=0; i<n_vertex; i++){
       set_points.push_back( meshes[last].get_vertex(i).get_coord() );
    }
    random_shuffle(std::begin(set_points), std::end(set_points));

    /**
     * Seleziono 4 dell'insieme di punti che non siano complanari.
     */
    i=3;
    p0 = set_points[0];
    p1 = set_points[1];
    p2 = set_points[2];
    while (determinant == 0) { //finchè p0, p1, p2, p3 sono complanari
        p3 = set_points[i];
        set_points[i] = set_points[3];
        set_points[3] = p3;
        determinant = complanar(p0, p1, p2, p3);
        i++;
    }

    /**
     * Creo il tetraedro sui primi 4 punti, e inserisco le 4 facce nel conflict graph
     */
    //p3 vede p0, p1, p2 ordinati in senso orario
    if (determinant < 0) convex_hull = insert_tethraedrus(p0, p1, p2, p3, &f0, &f1, &f2, &f3);
    //p3 vede p0, p1, p2 ordinati in senso antiorario, quindi vede p1, p0, p2 ordinati in senso orario.
    else convex_hull = insert_tethraedrus(p1, p0, p2, p3, &f0, &f1, &f2, &f3);

    cgf0 = cg.add_new_face(f0);
    cgf1 = cg.add_new_face(f1);
    cgf2 = cg.add_new_face(f2);
    cgf3 = cg.add_new_face(f3);

    /**
     * Inserisco tutti gli altri n-4 punti nel conflict graph, aggiungo i conflitti con le
     * relative facce del tetraedro che vedono.
     */
    for (i=4; i<set_points.size(); i++){

        point = cg.add_new_point(set_points[i]);

        if(see(set_points[i], f0, convex_hull)) { //se point vede f0
            cg.add_new_conflict(point, cgf0);
        }

        if(see(set_points[i], f1, convex_hull)) { //se point vede f1
            cg.add_new_conflict(point, cgf1);
        }

        if(see(set_points[i], f2, convex_hull)) { //se point vede f2
            cg.add_new_conflict(point, cgf2);
        }

        if(see(set_points[i], f3, convex_hull)) { //se point vede f3
            cg.add_new_conflict(point, cgf3);
        }
    }



    /**
     * Per conflict graph vuoto si intende un grafo che non contiene più nodi punto.
     * Ad ogni iterazione, rimuoviamo un punto dal conflict_graph e lo aggiungiamo al
     * convex hull, finché i punti non sono terminati
     */
    while (!cg.is_empty()){

        /**
         * Se il punto è interno al convex hull, nel conflict graph il nodo associato al punto non
         * ha archi uscenti: l'unica operazione da fare è rimuovere il nodo dal grafo.
         */
        if (cg.next_point_has_no_conflicts()) cg.delete_next_point(); //Il punto è interno al Convex Hull

        else {
            next_point = cg.get_next_point();

            /**
             * Calcolo l'array (ordinato per face_id!) delle facce sul convex hull viste da next_point
             */
            visible_faces = cg.get_next_conflict_faces();

            /**
             * Calcolo la lista ordinata degli edge che stanno sul boundary delle facce visibili (orizzonte)
             */
            horizon = horizon_edge_list(convex_hull, visible_faces, &horizon_vertex, &horizon_faces, next_point);

            /**
             * Per ogni edge sull'orizzonte, calcolo i punti non ancora inseriti sul convex hull che vedono l'edge,
             * ossia l'unione tra gli insiemi di punti che vedono le due facce adiacenti sull'edge.
             * Sono tutti i possibili punti che potranno vedere la nuova faccia che verrà costruita unendo l'edge
             * sull'orizzonte con next_point.
             * P è quindi un array di array: ogni riga i corrisponde all'i-esimo elemento di horizon.
             */
            P = calculate_P(convex_hull, cg, horizon);

            /**
             * Elimino dal convex hull tutte le facce di visible_faces e tutti gli half edge ed i vece ad esse
             * incidenti, tranne i vertici che stanno sull'orizzonte.
             */
            delete_visible_faces(convex_hull, horizon_faces, horizon_vertex, *visible_faces, deleted_faces, deleted_vertex, deleted_edges, cg);

            /**
             * Rimuovo next_point dal conflict graph. è fondamentale farlo subito dopo aver rimosso le facce dal
             * convex hull: vengono ricalcolati gli indici dei nodi esattamente come vengono ricalcolati quando
             * viene rimosso un elemento dalla DCEL.
             */
            cg.remove_next_point();

            /**
             * Inserisco le nuove facce nel convex hull, che andranno a collegare gli edge di horizon con
             * next_point. Sempre in questa funzione vengono anche calcolati e aggiunti i nuovi conflitti
             * tra le nuove facce e i punti presenti nel conflict graph.
             */
            insert_new_faces(convex_hull, horizon, next_point, cg, P, horizon_faces);

            /**
             * Per pura curiosità, decommentando le successive 4 righe di codice, è possibile osservare "dal vivo"
             * la costruzione del convex hull.
             */

            meshes.push_back(convex_hull);
            send_dcel(meshes);
            usleep(200);
            meshes.pop_back();
        }
    }
    DCEL real_convex_hull;                           //DCEL contenente il convex hull
    copy_real_convex_hull(convex_hull, real_convex_hull, deleted_faces, deleted_vertex, deleted_edges);

    timer.stop_and_print();
    std::cout << "f: " << real_convex_hull.get_faces_n() << " v: " << real_convex_hull.get_vertex_n() << " e: " << real_convex_hull.get_half_edge_n() << std::endl;
    meshes.push_back(real_convex_hull);
    send_dcel(meshes);

}


// Let the open file dialog show up and call the OFF loader
//
void Engine::open_file(void)
{
	QString filename = QFileDialog::getOpenFileName( NULL, "Open mesh", "examples", "3D meshes (*.off)");

	if (!filename.isNull())
	{
		if(!create_from_file(filename)) return;
		state = COMPUTED;
	}
}


// Save a DCEL object to a .off file. This may be useful if you want to store
// your convex hulls into the file system...
//
void Engine::save_file(void)
{
	QFileDialog saveDialog( NULL, "Save mesh", "examples", "3D meshes (*.off)");
	saveDialog.setDefaultSuffix("off");
	saveDialog.setAcceptMode(QFileDialog::AcceptSave);
	saveDialog.setConfirmOverwrite(true);
	saveDialog.setFileMode(QFileDialog::AnyFile);
	
	if (saveDialog.exec())
	{
		QString filename = saveDialog.selectedFiles().takeFirst();
	
		QFile file(filename);

		if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
		{
			std::cout << "Error saving file " << file.error() << std::endl;
			return;
		}

		QTextStream out(&file);
		
		out << "OFF\n";
		
		int last = meshes.size() - 1;
		int nv = meshes[last].get_vertex_n(), nf = meshes[last].get_faces_n();
		out << nv << " " << nf << " 0\n";

		for(int i=0; i<nv; i++)
		{
			CGPointf v = meshes[last].get_vertex(i).get_coord();
			out << v.x << " " << v.y << " " << v.z << "\n";
		}

		for(int i=0; i<nf; i++)
		{
			int v1, v2, v3, h;
			h = meshes[last].get_face(i).get_inner_half_edge();
			v1 = meshes[last].get_half_edge(h).get_from_vertex();
			v2 = meshes[last].get_half_edge(h).get_to_vertex();
			v3 = meshes[last].get_half_edge(meshes[last].get_half_edge(h).get_next()).get_to_vertex();
			out << "3 " << v1 << " " << v2 << " " << v3 << "\n";
		}
	}
}


// Takes in input a .off file and fills the DCEL up
//
bool Engine::create_from_file(QString filename)
{
	int nv, // number of vertices
	    nh, // number of half-edges
	    nf; // number of facets

	DCEL tmp_d;	
	meshes.push_back(tmp_d);
	int last = meshes.size() - 1;

	std::cout << meshes.size() << " models loaded" << std::endl;
	
	QFile file(filename);
	if (!file.open(QIODevice::ReadWrite | QIODevice::Text)) return false;

	QByteArray line = file.readLine();
	char *tmp = line.data();
	char t[5];
	
	//
	// check the .off header
	//

	int res = sscanf(tmp, "%s", t);
	if (res != 1 || strcmp(t, "OFF"))
	{
		std::cout << res << " element read, first is " << t << std::endl;
		file.close();
		return false;
	}
	line = file.readLine();
	tmp = line.data();
	res = sscanf(tmp, "%d %d %d", &nv, &nf, &nh); //ignorare nh
    if( res < 2 )
	{
		std::cout << "ERROR IN SECOND LINE (expected nv nf [ne])" << std::endl;
		file.close();
		return false;
	}

	//
	// load vertices
	//

    for( int i=0; i < nv; i++ )
	{
		float x, y, z;
		line = file.readLine();
		char *tmp = line.data();

        /*std::cout << tmp << std::endl;
		res = sscanf(tmp, "%f %f %f", &x, &y, &z);
        std::cout << x << "; " << y << "; " << z << std::endl; */

        /// sscanf non funge, ho modificato usando stringstream del c++
        std::stringstream s(tmp);
        s >> x >> y >> z;

        if( s.fail() == true )
		{
			std::cout << "Error in vertex " << i << ": " << res << " elements read, first is " << x << std::endl;
			file.close();
			return false;
		}

		vertex v;
		v.set_coord( CGPointf( x, y, z ) );
		v.set_incident_half_edge( -1 );

		meshes[last].add_vertex( v );
	}

	QVector<QVector<QPair<int, int> > > record_table(nv);

	//
	// load facets
	//

	for( int i=0; i < nf; i++ )
	{
		int v1, v2, v3, nvf;
		line = file.readLine();
		char *tmp = line.data();
		res = sscanf(tmp, "%d %d %d %d", &nvf, &v1, &v2, &v3);

        /*std::cout << tmp << std::endl;
        std::cout << nvf << "; " << v1 << "; " << v2 << "; " << v3 << std::endl;*/
	
		if( res != 4 || nvf != 3 )
		{
			std::cout << "Error in face " << i << ": " << res << " elements read, first is " << nvf << std::endl;
			file.close();
			return false;
		}
		face f;

		//
		// make connectivity
		//

		half_edge he1, he2, he3; //he1 da v1, he2 da v2, he3 da v3
		int he1_id, he2_id, he3_id;		
		he1.set_from_vertex( v1 );
		he1.set_to_vertex( v2 );
		he2.set_from_vertex( v2 );
		he2.set_to_vertex( v3 );
		he3.set_from_vertex( v3 );

		he1_id = meshes[last].add_half_edge( he1 );
		he2_id = meshes[last].add_half_edge( he2 );
		he3_id = meshes[last].add_half_edge( he3 );

		meshes[last].set_half_edge_prev( he1_id, he3_id );
		meshes[last].set_half_edge_prev( he3_id, he2_id );
		meshes[last].set_half_edge_prev( he2_id, he1_id );
		meshes[last].set_half_edge_next( he1_id, he2_id );
		meshes[last].set_half_edge_next( he2_id, he3_id );
		meshes[last].set_half_edge_next( he3_id, he1_id );

		QVector<QPair<int,int> > list = record_table.at(v2);

		int position = -1;
		for(int seek=0; seek<list.size(); seek++)
		{
			QPair<int,int> record = list.at(seek);
			if( record.first == v1 ) //v2->v1 messo
			{
				position = seek;
				seek = list.size()+1;				
			}
		}

		if( position != -1 )
		{
			//recupera record
			QPair<int,int> record = list.at(position);
			int twin_id = record.second;
			//setta twin (v2->v1)
			meshes[last].set_half_edge_twin( twin_id, he1_id );
			meshes[last].set_half_edge_twin( he1_id, twin_id );
			//rimuovi elemento
			list.remove(position);
			record_table.replace(v2, list);
		}
		else
		{
			//list diventa la lista per v1 (aggiungiamo v1->v2)
			list = record_table.at(v1);
			//creiamo il record v1->v2
			QPair<int,int> record( v2, he1_id );
			list.push_back(record);
			record_table.replace(v1, list);
		}

		//PER V2->V3
		list = record_table.at(v3);

		//cercare v2 in list
		position = -1;
		for(int seek=0; seek<list.size(); seek++)
		{
			QPair<int,int> record = list.at(seek);
			if( record.first == v2 ) //v3->v2 messo
			{
				position = seek;
				seek = list.size()+1;				
			}
		}

		if( position != -1 )
		{
			//recupera record
			QPair<int,int> record = list.at(position);
			int twin_id = record.second;
			//setta twin (v3->v2)
			meshes[last].set_half_edge_twin( twin_id, he2_id );
			meshes[last].set_half_edge_twin( he2_id, twin_id );
			//rimuovi elemento
			list.remove(position);
			record_table.replace(v3, list);
		}
		else
		{
			//list diventa la lista per v2 (aggiungiamo v2->v3)
			list = record_table.at(v2);
			//creiamo il record v2->v3
			QPair<int,int> record( v3, he2_id );
			list.push_back(record);
			record_table.replace(v2, list);
		}

		//PER V3->V1
		list = record_table.at(v1);

		//cercare v3 in list
		position = -1;
		for(int seek=0; seek<list.size(); seek++)
		{
			QPair<int,int> record = list.at(seek);
			if( record.first == v3 ) //v1->v3 messo
			{
				position = seek;
				seek = list.size()+1;				
			}
		}

		if( position != -1 )
		{
			//recupera record
			QPair<int,int> record = list.at(position);
			int twin_id = record.second;
			//setta twin (v1->v3)
			meshes[last].set_half_edge_twin( twin_id, he3_id );
			meshes[last].set_half_edge_twin( he3_id, twin_id );
			//rimuovi elemento
			list.remove(position);
			record_table.replace(v1, list);
		}
		else
		{
			//list diventa la lista per v3 (aggiungiamo v3->v1)
			list = record_table.at(v3);
			//creiamo il record v3->v1
			QPair<int,int> record( v1, he3_id );
			list.push_back(record);
			record_table.replace(v3, list);
		}

		if( meshes[last].get_vertex( v1 ).get_incident_half_edge() == -1 ) meshes[last].set_vertex_incident( v1, he1_id );
		if( meshes[last].get_vertex( v2 ).get_incident_half_edge() == -1 ) meshes[last].set_vertex_incident( v2, he2_id );
		if( meshes[last].get_vertex( v3 ).get_incident_half_edge() == -1 ) meshes[last].set_vertex_incident( v3, he3_id );

		//
		// compute face normals
		//

		CGPointf vec1, vec2, norm;
		vec1 = meshes[last].get_vertex(v2).get_coord() - meshes[last].get_vertex(v1).get_coord();
		vec2 = meshes[last].get_vertex(v3).get_coord() - meshes[last].get_vertex(v1).get_coord();
		norm = cross(vec1, vec2);
		float norma = sqrt(pow(norm.x,2)+pow(norm.y,2)+pow(norm.z,2));
		norm.x /= norma;
		norm.y /= norma;
		norm.z /= norma;

		f.set_normal(norm);

		f.set_inner_half_edge( he1_id );
		int face_id = meshes[last].add_face( f );

		meshes[last].set_half_edge_face( he1_id, face_id );
		meshes[last].set_half_edge_face( he2_id, face_id );
		meshes[last].set_half_edge_face( he3_id, face_id );
	}
	file.close();

	//
	// compute vertex normals
	//

	for(int i=0; i<nv && meshes[last].get_faces_n(); i++)
	{
		vertex v_i = meshes[last].get_vertex(i);
		CGPointf normal_i;
		int start_he_id = v_i.get_incident_half_edge();
		int circulator = start_he_id;
		int faces_n = 0;
		do
		{
			half_edge circ_h = meshes[last].get_half_edge(circulator);
			face f = meshes[last].get_face( circ_h.get_face() );
			normal_i = normal_i + f.get_normal();
			faces_n ++;

			circulator = meshes[last].get_half_edge( circ_h.get_twin() ).get_next();

		}
		while (circulator != start_he_id);

		normal_i = normal_i / (float)faces_n;
		float normal_norm = sqrt(pow(normal_i.x,2)+pow(normal_i.y,2)+pow(normal_i.z,2));
		normal_i = normal_i / normal_norm;
		meshes[last].set_vertex_normal( i, normal_i );
	}

	//
	// update GUI
	//

	emit send_dcel(meshes);

	return true;
}
