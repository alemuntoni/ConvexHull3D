/** 
 * \page strutture_dati Strutture Dati Essenziali
 * Nella sezione "Strutture Dati" sono raccolte le strutture dati essenziali per la corretta
 * implementazione della DCEL, vale a dire vertici, half-edge e facce. Ciascuna
 * di esse contiene le informazioni necessarie per una corretta gestione delle
 * relazioni topologiche di adiacenza e di incidenza. \n 
 * Alle informazioni è possibile
 * accedere tramite i metodi appositi get_something() dove something è genericamente
 * riferito all'entità a cui vogliamo accedere, mentre tramite i metodi set_something()
 * è possibile passare all'entità di interesse uno o più parametri così da modificare
 * le informazioni in essa contenute. Sono inoltre implementati svariati operatori per
 * il confronto tra due vertex o tra due half-edge, o per la somma di CGPoints, moltiplicazioni
 * per una costante etc.
 * 
 * \note Benchè sia possibile accedere alle informazioni direttamente (es. v.coord consente
 * di accedere alle informazioni sulle coordinate di un vertice) è preferibile utilizzare i
 * metodi predefiniti per la lettura e la modifica dei valori.
 *
 * \author Stefano Marras, Fabio Guggeri. 
 * 
 */
#ifndef STRUCTURES
#define STRUCTURES

#include <QMessageBox>
#include <QString>
#include <sstream>

/**
 * \brief Struttura rappresentante un punto nell spazio.
 *
 * Punto in 3 dimensioni, con precisione dati variabile definita dal tipo T.
 * In particolare, è possibile avere punti con precisione intera, float o double.
 */
template <class T> struct CGPoint {
	/// Coordinata x	
	T x;
	/// Coordinata y
	T y;
	/// Coordinata z
	T z;

	/** 
         *  Costruttore; inizializza il punto alle coordinate specificate.
	 *  Di default, inizializza il punto a (0, 0, 0).
         */
	CGPoint(T nx=0, T ny=0, T nz=0)	{x=nx; y=ny; z=nz;}

	virtual ~CGPoint(void){};

	/**
	 * Rende la coordinata x.
	 * \return La coordinata x.
	 */
	virtual T getX( void )	{ return x; }

	/**
	 * Rende la coordinata y.
	 * \return La coordinata y.
	 */
	virtual T getY( void )	{ return y; }

	/**
	 * Rende la coordinata z.
	 * \return La coordinata z.
	 */
	virtual T getZ( void )	{ return z; }

};

typedef CGPoint<float> CGPointf;
typedef CGPoint<double> CGPointd;
typedef CGPoint<int> CGPointi;	

/** 
 * \brief Struttura rappresentante un vertice della DCEL.
 *
 * La struttura vertex rappresenta un \b vertice della DCEL (due dimensioni).\n
 * Essa contiene al suo interno le informazioni essenziali per la
 * manipolazione del vertice, vale a dire le \b coordinate del punto (x, y)
 * e l'indice dell' \b half-edge \b incidente.
 */
struct vertex	{
	
	///Coordinate intere del punto
	CGPointf	coord;	
	///Normale al vertice
	CGPointf	normal;

	///half-edge incidente, uscente dal punto 	
	int	incident_half_edge;	
	
	int card;

	virtual ~vertex(void){};
	
	virtual void set_card( int c ) { card = c;    }	
	virtual int  get_card( void  ) { return card; }
	virtual void dec_card( void  ) { card--;      }
	
	/** Rende le normale al vertice.
         *  \return La normale al vertice.
         */
	virtual CGPointf get_normal()	{
		return normal;
	};

	/** Imposta la normale al vertice
	 *  \param n La normale al vertice.	
 	 */
	virtual void set_normal( CGPointf n ) {
		normal = n;
	};

	/** Rende le coordinate del vertice.
         *  \return La coordinate del vertice.
         */
	virtual CGPointf get_coord()	{
		return coord;
	};

	/** Rende l'indice dell half-edge incidente nel vertice. 
	 * \return L'indice dell'incident half-edge.
 	 */
	virtual int get_incident_half_edge()	{
		return incident_half_edge;
	};

	/** Imposta le coordinate del vertice.
	 *  \param c Le coordinate del vertice
	 */
	virtual void set_coord( CGPointf c )	{
		coord = c;
	};

	/** Imposta l'incident half-edge.
	 *  \param ihe L'indice dell'half-edge incidente al vertice.	
 	 */	
	virtual void set_incident_half_edge( int ihe )	{
		incident_half_edge = ihe;	
	};
};

/** 
 * \brief Struttura rappresentante un half-edge della DCEL.
 *
 * La struttura half_edge implementa un half-edge della DCEL.\n 
 * L'half-edge è un segmento \e orientato, che parte da un vertice di origine
 * identificato come \b from_vertex e arriva ad un vertice detto \b to_vertex.
 * Ogni half-edge è caratterizzato dall'avere un half-edge gemello, identificato
 * come \b twin half-edge, che ha orientamento opposto a quello dell'half-edge originale.
 * Inoltre, per la gestione corretta della topologia, ogni half-edge deve poter conoscere
 * il proprio \b prev half-edge (l'half-edge entrante nel from_vertex) e il proprio
 * \b next half-edge (l'half-edge uscente dal to_vertex). Tutte queste informazioni
 * sono contenute nella struttura, e possono essere lette e manipolate mediante gli 
 * appositi metodi.
 *
 * \note Si noti che degli elementi viene memorizzato solo l'indice all'interno della DCEL
 * e non un puntatore o un riferimento, per cui per accedere alle informazioni relative,
 * ad esempio, al prev, è necessario sfruttare l'indice per accedere all'elemento memorizzato
 * nella DCEL e, da quì, accedere alle informazioni richieste. La modifica delle informazioni
 * deve invece effettuarsi mediante gli opportuni metodi.
 */
struct half_edge	{

	///Vertice da cui l'half-edge esce.
	int 	from_vertex;	
	///Vertice in cui l'half-edge entra.
	int 	to_vertex;	

	///Indice del twin half-edge.
	int 	twin;	
	///Indice del prev half-edge.	
	int	prev;	
	///Indice del next half-edge.	
	int	next;	
	///Indice della faccia su cui incide l'half-edge.
	int 	face;		

	virtual ~half_edge(void){};

	/** Rende l'indice del from_vertex. 
	 * \return L'indice del from_vertex. */
	virtual int get_from_vertex()	{
		return from_vertex;
	};

	/** Rende l'indice del to_vertex.
  	 * \return L'indice del to_vertex*/
	virtual int get_to_vertex()	{
		return to_vertex;
	};

	/** Rende l'indice del twin half-edge.
 	 * \return L'indice del twin half-edge */
	virtual int get_twin()	{
		return twin;
	};

	/** Rende l'indice del prev half-edge.
	 * \return L'indice del prev half-edge */
	virtual int get_prev()	{
		return prev;
	};

	/** Rende l'indice del next half-edge.
	 * \return L'indice del next half-edge */
	virtual int get_next()	{
		return next;
	};

	/** Rende l'indice della faccia su cui indice l'half-edge.
	 * \return L'indice della faccia */
	virtual int get_face()	{
		return face;
	};

	/** Imposta il from vertex. 
	 * \param fv Il nuovo from_vertex. */
	virtual void set_from_vertex( int fv )	{
		from_vertex = fv;	
	};

	/** Imposta il to vertex. 
	 * \param tv Il nuovo to_vertex. */
	virtual void set_to_vertex( int tv )	{
		to_vertex = tv;	
	};

	/** Imposta il twin half-edge. 
	 * \param t Il nuovo twin half-edge. */
	virtual void set_twin( int t )	{
		twin = t;	
	};

	/** Imposta il prev half-edge. 
	 * \param p Il nuovo prev half-edge. */
	virtual void set_prev( int p )	{
		prev = p;	
	};

	/** Imposta il next half-edge. 
	 * \param n Il nuovo next half-edge. */
	virtual void set_next( int n )	{
		next = n;	
	};

	/** Imposta la faccia su cui incide l'half-edge.
	 *  \param f La nuova faccia su cui incide l'half-edge */
	virtual void set_face( int f )	{
		face = f;	
	};
};

/**
 * \brief Struttura rappresentante una faccia della DCEL.
 *
 * La struttura face implementa una faccia della DCEL. \n
 * Si tratta di una struttura in realtà poco sviluppata, poichè essa
 * contiene semplicemente gli indici dell' half-edge \b inner e dell'
 * half-edge \b outer. Può comunque essere estesa per includere ulteriori informazioni
 * quali colore della faccia, estensione, etc. 
 */
struct face	{
	
	///Indice dell'half-edge inner-component
	int 	inner_half_edge;	
	///Indice dell'half-edge outer-component
	int 	outer_half_edge;	
	
	/// Normale alla faccia
	CGPointf normal;

	virtual ~face(void){};

	/** Rende la normale della faccia
	 * \return La normale della faccia */
	virtual CGPointf get_normal()		{
		return normal;
	};
	
	/** Imposta la normale alla faccia
	 * \param new_normal La normale alla faccia */
	virtual void set_normal(CGPointf new_normal)		{
		normal = new_normal;
	};

	/** Rende l'indice dell'inner half-edge.
	 * \return L'indice dell'inner half-edge */
	virtual int get_inner_half_edge()	{
		return inner_half_edge;
	};		

	/** Imposta l'inner half-edge .
	 * \param ihe Il nuovo inner half-edge */
	virtual void set_inner_half_edge( int ihe )	{
		inner_half_edge = ihe;	
	};

	/** Rende l'indice dell'outer half-edge.
	 * \return L'indice dell'outer half-edge */
	virtual int get_outer_half_edge()	{
		return outer_half_edge;
	};		

	/** Imposta l'outer half-edge .
	 * \param ohe Il nuovo outer half-edge */
	virtual void set_outer_half_edge( int ohe )	{
		outer_half_edge = ohe;	
	};
};

template <class T> bool inline operator==( const CGPoint<T>& p1, const CGPoint<T>& p2 )	{
	if ( p1.x != p2.x )	return false;
	if ( p1.y != p2.y )	return false;
	if ( p1.z != p2.z )	return false;
	return true;
};

template <class T> bool inline operator!=( const CGPoint<T>& p1, const CGPoint<T>& p2 )	{
	if ( p1.x != p2.x )	return true;
	if ( p1.y != p2.y )	return true;
	if ( p1.z != p2.z )	return true;
	return false;
};

template <class T> CGPoint<T> inline operator + ( const CGPoint<T>& p1, const CGPoint<T>& p2 ){
	return CGPoint<T>(p1.x+p2.x, p1.y+p2.y, p1.z+p2.z);
};

template <class T> CGPoint<T> inline operator - ( const CGPoint<T>& p1, const CGPoint<T>& p2 ){
	return CGPoint<T>(p1.x-p2.x, p1.y-p2.y, p1.z-p2.z);
};

template <class T> CGPoint<T> inline operator * ( const CGPoint<T>& p1, T scalar ){
	return CGPoint<T>(p1.x * scalar, p1.y * scalar, p1.z * scalar);
};

template <class T> CGPoint<T> inline operator * ( T scalar, const CGPoint<T>& p1 ){
	return CGPoint<T>(p1.x * scalar, p1.y * scalar, p1.z * scalar);
};

template <class T> CGPoint<T> inline operator / ( const CGPoint<T>& p1, T scalar ){
	return CGPoint<T>(p1.x / scalar, p1.y / scalar, p1.z / scalar);
};

template <class T> CGPoint<T> inline operator / ( T scalar, const CGPoint<T>& p1 ){
	return CGPoint<T>(p1.x / scalar, p1.y / scalar, p1.z / scalar);
};

template <class T> CGPoint<T> inline cross( const CGPoint<T>& p1, const CGPoint<T>& p2 ) {
	T i, j, k;
	i = p1.y * p2.z - p1.z * p2.y;
	j = -( p1.x * p2.z - p1.z * p2.x );
	k = p1.x * p2.y - p1.y * p2.x;

	return CGPoint<T>(i,j,k);
};

// Due vertici sono uguali se hanno le medesime coordinate... 
bool inline operator==( const vertex& v1, const vertex& v2 )	{
	if ( v1.coord == v2.coord )	return true;	//ridefinizione ad opera di QT
	return false;
};
// ... altrimenti sono diversi. 
bool inline operator!=( const vertex& v1, const vertex& v2 )	{
	if ( v1.coord == v2.coord )	return false;	//ridefinizione ad opera di QT
	return true;
};
// Due halfedge sono uguali se hanno lo stesso from e lo stesso to... 
bool inline operator==( const half_edge& he1, const half_edge& he2 )	{
	if ( he1.from_vertex == he2.from_vertex &&
	     he1.to_vertex   == he2.to_vertex	)
	return true;
	return false;	
};

// ...altrimenti sono diversi 
bool inline operator!=( const half_edge& he1, const half_edge& he2 )	{
	if ( he1.from_vertex == he2.from_vertex &&
	     he1.to_vertex   == he2.to_vertex	)
	return false;
	return true;	
};

#endif //STRUCTURES
