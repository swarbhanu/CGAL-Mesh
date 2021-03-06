#include <CGAL/Simple_cartesian.h>
#include <CGAL/Polyhedron_incremental_builder_3.h>
#include <CGAL/Polyhedron_3.h>
#include <iostream>
#include <CGAL/IO/Polyhedron_iostream.h>

// A modifier creating a triangle with the incremental builder.
template <class HDS>
class Build_triangle : public CGAL::Modifier_base<HDS> {
public:
    Build_triangle() {}
    void operator()( HDS& hds) {
        // Postcondition: `hds' is a valid polyhedral surface.
        CGAL::Polyhedron_incremental_builder_3<HDS> B( hds, true);
        B.begin_surface( 4, 2, 12);
        typedef typename HDS::Vertex   Vertex;
        typedef typename Vertex::Point Point;
        B.add_vertex( Point( 0, 0, 0));
        B.add_vertex( Point( 1, 0, 0));
        B.add_vertex( Point( 0, 1, 0));
        B.add_vertex( Point( 1, 1, 1));

        B.begin_facet();
        B.add_vertex_to_facet( 0);
        B.add_vertex_to_facet( 1);
        B.add_vertex_to_facet( 3); // for unknown reason, there is a segmentation fault when vertex 2 is added instead of 3
        B.end_facet();

        B.begin_facet();
		B.add_vertex_to_facet( 1);
		B.add_vertex_to_facet( 2);
		B.add_vertex_to_facet( 3);
		B.end_facet();

        B.end_surface();
    }
};

typedef CGAL::Simple_cartesian<double>     Kernel;
typedef CGAL::Polyhedron_3<Kernel>         Polyhedron;
typedef Polyhedron::HalfedgeDS             HalfedgeDS;

// a helper method for running different iterators
void running_iterators( Polyhedron& P) {
    if ( P.size_of_facets() == 0)
        return;

    std::size_t nv = P.size_of_vertices();

    std::cout << "The number of vertices in the Polyhedron: " << nv << std::endl;
    std::cout << "The number of facets in the Polyhedron: " << P.size_of_facets() << std::endl;
    std::cout << "The number of half edges in the Polyhedron: " << P.size_of_halfedges() << std::endl;

    std::cout << std:: endl;

    Polyhedron::Vertex_iterator last_v = P.vertices_end();
    -- last_v;  // the last of the old vertices

    Polyhedron::Edge_iterator last_e = P.edges_end();
    -- last_e;  // the last of the old edges

    Polyhedron::Facet_iterator last_f = P.facets_end();
    -- last_f;  // the last of the old facets

    int k = 0;
    Polyhedron::Facet_iterator f = P.facets_begin();

    do {
    	std::cout << "Printing a facet index: " << k++ <<  std::endl;

    	f->halfedge();

    } while ( f++ != last_f);

    std::cout  << std::endl;

    // -------------------------------------------------
    // traverse the vertices
    // -------------------------------------------------

    std::cout << "Printing the vertex indices: " << std::endl;

     int n=0;
     for (Polyhedron::Vertex_iterator vi = P.vertices_begin(); vi != P.vertices_end(); ++vi)
     {
    	 Kernel::Point_3 p;
    	 p = vi->point();
    	 std::cout << "Vertex index: "  << n++ << std::endl;
    	 std::cout << "p.x() = "  << p.x() << std::endl;
    	 std::cout << "p.y() = "  << p.y() << std::endl;
    	 std::cout << "p.z() = "  << p.z() << std::endl;

     }

     std::cout  << std::endl;

     // -------------------------------------------------
     // traverse the edges
     // -------------------------------------------------

     std::cout << "Iterating over the edges.... " << std::endl;

     n=0;
     for (Polyhedron::Edge_iterator ei = P.edges_begin(); ei != P.edges_end(); ++ei)
     {
    	 ei->next();
    	 Kernel::Point_3 p;
    	 p =  ei->vertex()->point();
    	 std::cout << "For edge index: " << n++ << std::endl;
    	 std::cout << "p.x() = "  << p.x() << std::endl;
		 std::cout << "p.y() = "  << p.y() << std::endl;
		 std::cout << "p.z() = "  << p.z() << std::endl;

     }
     std::cout  << std::endl;

	 // -----------------------------------------------
	 // Do something else with the edge iterators
	 // -----------------------------------------------

    Polyhedron::Edge_iterator e = P.edges_begin();
    ++ last_e; // make it the past-the-end position again

    while ( e != last_e) {
    	Polyhedron::Halfedge_handle h = e;
        ++e;
    };

    CGAL_postcondition( P.is_valid());
}


int main() {
    Polyhedron P;
    Build_triangle<HalfedgeDS> triangle;
    P.delegate( triangle);
    CGAL_assertion( P.is_triangle( P.halfedges_begin()));
    
    CGAL::set_pretty_mode(std::cout);
    
    std::cout << "Success creating two triangles: "
                  << std::endl;
    
    std::cout << "The polyhedron created at this stage" << P << std::endl;
    
    running_iterators(P);

    return 0;
}
