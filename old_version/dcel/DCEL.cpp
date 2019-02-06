#include "DCEL.hh"
#include <iostream>

/** 
  * \param i L'indice del vertice di interesse.
  * \return il vertice.
  */
vertex DCEL::get_vertex( int i )	{
	return V[i];
};

/** 
  * \param i L'indice dell'half-edge di interesse.
  * \return l'half-edge.
  */
half_edge DCEL::get_half_edge( int i )	{
	return HE[i];
};

/** 
  * \param i L'indice della faccia di interesse.
  * \return la faccia.
  */
face DCEL::get_face( int i )	{
	return F[i];
};

/**
 * \param v Il vertice da aggiungere alla DCEL.
 * \return l'indice del vertice aggiunto.
 */
int DCEL::add_vertex( vertex v )	{
	V.push_back( v );
	return V.size() - 1;
};

/**
 * \param he L'half-edge da aggiungere alla DCEL.
 * \return l'indice dell'half-edge aggiunto.
 */
int DCEL::add_half_edge( half_edge he )	{
	HE.push_back( he );
	return HE.size() - 1;
};
	
/**
 * \param f La faccia da aggiungere alla DCEL.
 * \return l'indice della faccia aggiunta.
 */
int DCEL::add_face( face f )	{
	F.push_back( f );
	return F.size() - 1;
};

/** \return Il numero di vertici della DCEL. */
int DCEL::get_vertex_n ( void )	{
	return V.size();
};

/** \return Il numero di half-edge della DCEL. */
int DCEL::get_half_edge_n ( void )	{
	return HE.size();
};

/** \return Il numero di facce della DCEL. */
int DCEL::get_faces_n ( void )	{
	return F.size();
};

int  DCEL::get_vertex_card( int v )
{
	if ( v >= V.size() )	return -1;
	return V[v].get_card();
}

int DCEL::dec_vertex_card( int v )
{
	if ( v >= V.size() )	return -1;
	V[v].dec_card();
	return V[v].get_card();	
}

bool DCEL::set_vertex_card( int v , int card )
{
	if ( v >= V.size() )	return false;
	V[v].set_card(card);
	return true;	
}

/**
 * \param v l'indice del vertice
 * \param he l'indice dell'half-edge
 * \return true se l'inserimento è andato a buon fine, false altrimenti.
 */
bool DCEL::set_vertex_incident ( int v, int he )	{
	
	if ( v >= V.size() )	return false;
	if ( he >= HE.size() )	return false;
	V[v].set_incident_half_edge( he );
	return true;
}

/**
 * \param v l'indice del vertice
 * \param normal la normale al vertice
 * \return true se l'inserimento è andato a buon fine, false altrimenti.
 */
bool DCEL::set_vertex_normal ( int v, CGPointf normal )	{
	if ( v >= V.size() )	return false;
	V[v].set_normal( normal );
	return true;
}

/**
 * \param he l'indice dell'half-edge
 * \param from l'indice del from-vertex
 * \return true se l'inserimento è andato a buon fine, false altrimenti.
 */
bool DCEL::set_half_edge_from ( int he, int from )	{

	if ( he >= HE.size() )	return false;
	if ( from >= V.size() )	return false;
	HE[he].set_from_vertex( from );
	return true;
}

/**
 * \param he l'indice dell'half-edge
 * \param to l'indice del to-vertex
 * \return true se l'inserimento è andato a buon fine, false altrimenti.
 */
bool DCEL::set_half_edge_to ( int he, int to )	{

	if ( he >= HE.size() )	return false;
	if ( to >= V.size() )	return false;
	HE[he].set_to_vertex( to );
	return true;
}

/**
 * \param he l'indice dell'half-edge principale
 * \param twin l'indice dell'half-edge che diventerà twin
 * \return true se l'inserimento è andato a buon fine, false altrimenti.
 */
bool DCEL::set_half_edge_twin ( int he, int twin )	{

	if ( he >= HE.size() )		return false;
	if ( twin >= HE.size() )	return false;
	HE[he].set_twin( twin );
	return true;
}

/**
 * \param he l'indice dell'half-edge principale
 * \param prev l'indice dell'half-edge che diventerà prev
 * \return true se l'inserimento è andato a buon fine, false altrimenti.
 */
bool DCEL::set_half_edge_prev ( int he, int prev )	{

	if ( he >= HE.size() )		return false;
	if ( prev >= HE.size() )	return false;
	HE[he].set_prev( prev );
	return true;
}

/**
 * \param he l'indice dell'half-edge principale
 * \param next l'indice dell'half-edge che diventerà next
 * \return true se l'inserimento è andato a buon fine, false altrimenti.
 */
bool DCEL::set_half_edge_next ( int he, int next )	{

	if ( he >= HE.size() )		return false;
	if ( next >= HE.size() )	return false;
	HE[he].set_next( next );
	return true;
}

/**
 * \param he l'indice dell'half-edge
 * \param f l'indice della faccia
 * \return true se l'inserimento è andato a buon fine, false altrimenti.
 */
bool DCEL::set_half_edge_face (int he, int f )	{

	if ( he >= HE.size() )	return false;
	if ( f >= F.size() )	return false;
	HE[he].set_face( f );
	return true;
}

/**
 * \param f l'indice della faccia
 * \param he l'indice dell'inner half-edge
 * \return true se l'inserimento è andato a buon fine, false altrimenti.
 */
bool DCEL::set_face_inner_half_edge (int f, int he ) 	{

	if ( f >= F.size() )	return false;
	if ( he >= HE.size() )	return false;
	if (f == -1 ) return false; 
	else F[f].set_inner_half_edge( he );
	return true;
}

/**
 * \param f l'indice della faccia
 * \param he l'indice dell'outer half-edge
 * \return true se l'inserimento è andato a buon fine, false altrimenti.
 */
bool DCEL::set_face_outer_half_edge (int f, int he ) 	{

	if ( f >= F.size() )	return false;
	if ( he >= HE.size() )	return false;
	if (f == -1 ) return false; 
	else F[f].set_outer_half_edge( he );
	return true;
}

/** 
 * Rimuove un vertice dalla DCEL, cancellando
 * l'elemento corrispondente dalla lista e aggiornando
 * i riferimenti presenti negli altri elementi che potrebbero cambiare in seguito
 * alla rimozione. Non aggiorna riferimenti all'elemento cancellato; questo è compito
 * dell'utente; pertanto è consigliato utilizzare la funzione di cancellazione
 * di un elemento SOLO quando non vi è più alcun riferimento all'elemento stesso nella DCEL.
 *
 * \param v l'indice del vertice da rimuovere
 * \return true se la rimozione è andata a buon fine, false altrimenti
 */
bool DCEL::delete_vertex( int v )
{
	int i=0;

	if ( v >= V.size() ) return false;

    for(i=0; i<HE.size(); i++)
	{
		half_edge h = HE.at(i);
		int from = h.get_from_vertex();
		int to = h.get_to_vertex();
		if( from >= v ) h.set_from_vertex( from-1 );
		if( to >= v ) h.set_to_vertex( to-1 );
		from = h.get_from_vertex();
		to = h.get_to_vertex();
		HE.replace(i, h);
    }

	V.remove( v );
	return true;
}

/** 
 * Rimuove un half-edge dalla DCEL, cancellando
 * l'elemento corrispondente dalla lista e aggiornando
 * i riferimenti presenti negli altri elementi che potrebbero cambiare in seguito
 * alla rimozione. Non aggiorna riferimenti all'elemento cancellato; questo è compito
 * dell'utente; pertanto è consigliato utilizzare la funzione di cancellazione
 * di un elemento SOLO quando non vi è più alcun riferimento all'elemento stesso nella DCEL.
 *
 * \param he l'indice dell'half-edge da rimuovere
 * \return true se la rimozione è andata a buon fine, false altrimenti
 */
bool DCEL::delete_half_edge( int he )
{
	int i=0;

	if ( he >= HE.size() ) return false;

	for(i=0; i<V.size(); i++)
	{
		vertex v = V.at(i);
		int incident = v.get_incident_half_edge();
		if( incident >= he )
			v.set_incident_half_edge( incident-1 );
		V.replace(i, v);
	}

	for(i=0; i<F.size(); i++)
	{
		face f = F.at(i);

		int incident = f.get_inner_half_edge();
		if( incident >= he )
			f.set_inner_half_edge( incident-1 );
		int incident_out = f.get_outer_half_edge();
		if( incident_out >= he )
			f.set_outer_half_edge( incident_out-1 );
		F.replace(i, f);
	}

	for(i=(he==0)?1:0; i<HE.size(); i+=(i==he-1)?2:1)
	{
		half_edge h = HE.at(i);

		int next = h.get_next();
		if( next >= he )
			h.set_next( next-1 );
		int prev = h.get_prev();
		if( prev >= he )
			h.set_prev( prev-1 );
		int twin = h.get_twin();
		if( twin >= he )
			h.set_twin( twin-1 );
		HE.replace(i, h);
	}

	HE.remove( he );
	return true;	//è andato a buon fine
}

/** 
 * Rimuove una faccia dalla DCEL, cancellando
 * l'elemento corrispondente dalla lista e aggiornando
 * i riferimenti presenti negli altri elementi che potrebbero cambiare in seguito
 * alla rimozione. Non aggiorna riferimenti all'elemento cancellato; questo è compito
 * dell'utente; pertanto è consigliato utilizzare la funzione di cancellazione
 * di un elemento SOLO quando non vi è più alcun riferimento all'elemento stesso nella DCEL.
 *
 * \param f l'indice della faccia da rimuovere
 * \return true se la rimozione è andata a buon fine, false altrimenti
 */
bool DCEL::delete_face( int f )
{
	int i=0;

	if ( f >= F.size() ) return false;

    for(i=0; i<HE.size(); i++)
	{
		half_edge h = HE.at(i);
		int face = h.get_face();

		if( face >= f ) h.set_face( face-1 );
		HE.replace(i, h);
    }

	F.remove( f );
	return true;
}

/**
 * Per cancellazione si intende la rimozione di tutti i vertici,
 * gli half-edge e le facce precedentemente create, con relativa
 * perdita di tutte le informazioni in esse contenute. 
 */
void DCEL::reset( void )	{
	V.clear();
	HE.clear();	
	F.clear();	
}
