#include "catch2/catch.hpp"

#include "planin/structured_triangle_mesh_2d.hpp"
#include "planin/core.hpp"


SCENARIO( "Testing detection of triangle's ids given a point p in R^2", "[structured_triangle_mesh_2d]")
{
    GIVEN( "A default mesh with one block") {

        StructuredTriangleMesh2D<Point2> mesh;

        REQUIRE( mesh.numBlocks() == 1 );
        REQUIRE( mesh.numVertices() == 9 );
        REQUIRE( mesh.numTriangles() == 8 );

        WHEN( "Checking interior points" ) {
            THEN( "There is no ambiguity in the determination of triangles" ) {

                REQUIRE( mesh.triangleIndex({1./4., 1./8.}) == 0 );
                REQUIRE( mesh.triangleIndex({1./4., 3./8.}) == 1 );
                REQUIRE( mesh.triangleIndex({1./4., 5./8.}) == 4 );
                REQUIRE( mesh.triangleIndex({1./4., 7./8.}) == 5 );
                REQUIRE( mesh.triangleIndex({3./4., 1./8.}) == 2 );
                REQUIRE( mesh.triangleIndex({3./4., 3./8.}) == 3 );
                REQUIRE( mesh.triangleIndex({3./4., 5./8.}) == 6 );
                REQUIRE( mesh.triangleIndex({3./4., 7./8.}) == 7 );
            }
        }

        WHEN( "Checking boundary points" ) {
            THEN( "Points belonging to multiple triangle should adhere to *this* specified order" ) {

                REQUIRE( mesh.triangleIndex({0.   , 0.}   ) == 0 );
                REQUIRE( mesh.triangleIndex({1./2., 0.}   ) == 0 );
                REQUIRE( mesh.triangleIndex({1./2., 1./2.}) == 0 );
                REQUIRE( mesh.triangleIndex({1./4., 1./4.}) == 0 );
                REQUIRE( mesh.triangleIndex({0.   , 1./2.}) == 1 );
                REQUIRE( mesh.triangleIndex({0.   , 1.}   ) == 4 );
                REQUIRE( mesh.triangleIndex({1./2., 1.}   ) == 5 );
                REQUIRE( mesh.triangleIndex({1.   , 1.}   ) == 6 );
                REQUIRE( mesh.triangleIndex({1.   , 1./2.}) == 3 );
                REQUIRE( mesh.triangleIndex({1.   , 0.}   ) == 2 );
                REQUIRE( mesh.triangleIndex({3./4., 3./4.}) == 6 );
            }
        }
    }


    GIVEN( "A mesh with two blocks, discretized as (2, 1)" ) {

        StructuredTriangleMesh2D<Point2> mesh;

        mesh.setDiscretization(2, 1);
        mesh.setDomainLength({2., 1.});

        REQUIRE( mesh.numBlocks() == 2 );
        REQUIRE( mesh.numVertices() == 15 );
        REQUIRE( mesh.numTriangles() == 16 );

        WHEN( "Checking interior points" ) {
            THEN( "There is no ambiguity in the determination of triangles" ) {

                REQUIRE( mesh.triangleIndex({1./4., 1./8.}) == 0);
                REQUIRE( mesh.triangleIndex({1./4., 3./8.}) == 1);
                REQUIRE( mesh.triangleIndex({1./4., 5./8.}) == 4);
                REQUIRE( mesh.triangleIndex({1./4., 7./8.}) == 5);
                REQUIRE( mesh.triangleIndex({3./4., 1./8.}) == 2);
                REQUIRE( mesh.triangleIndex({3./4., 3./8.}) == 3);
                REQUIRE( mesh.triangleIndex({3./4., 5./8.}) == 6);
                REQUIRE( mesh.triangleIndex({3./4., 7./8.}) == 7);
            }
        }

        WHEN( "Checking boundary points" ) {
            THEN( "Points belonging to multiple triangle should adhere to *this* specified order" ) {

                REQUIRE( mesh.triangleIndex({0.   , 0.}   ) == 0);
                REQUIRE( mesh.triangleIndex({1./2., 0.}   ) == 0);
                REQUIRE( mesh.triangleIndex({1./2., 1./2.}) == 0);
                REQUIRE( mesh.triangleIndex({1./4., 1./4.}) == 0);
                REQUIRE( mesh.triangleIndex({0.   , 1./2.}) == 1);
                REQUIRE( mesh.triangleIndex({0.   , 1.}   ) == 4);
                REQUIRE( mesh.triangleIndex({1./2., 1.}   ) == 5);
                REQUIRE( mesh.triangleIndex({1.   , 1.}   ) == 12);
                REQUIRE( mesh.triangleIndex({1.   , 1./2.}) == 9);
                REQUIRE( mesh.triangleIndex({1.   , 0.}   ) == 8);
                REQUIRE( mesh.triangleIndex({3./4., 3./4.}) == 6);
            }
        }
    }

    GIVEN( "A mesh with two blocks, discretized as (1, 2)" ) {

        StructuredTriangleMesh2D<Point2> mesh;

        mesh.setDiscretization(1, 2);
        mesh.setDomainLength({1., 2.});

        REQUIRE( mesh.numBlocks() == 2 );
        REQUIRE( mesh.numVertices() == 15 );
        REQUIRE( mesh.numTriangles() == 16 );

        WHEN( "Checking interior points" ) {
            THEN( "There is no ambiguity in the determination of triangles" ) {

                REQUIRE( mesh.triangleIndex({1./4., 1./8.}) == 0);
                REQUIRE( mesh.triangleIndex({1./4., 3./8.}) == 1);
                REQUIRE( mesh.triangleIndex({1./4., 5./8.}) == 4);
                REQUIRE( mesh.triangleIndex({1./4., 7./8.}) == 5);
                REQUIRE( mesh.triangleIndex({3./4., 1./8.}) == 2);
                REQUIRE( mesh.triangleIndex({3./4., 3./8.}) == 3);
                REQUIRE( mesh.triangleIndex({3./4., 5./8.}) == 6);
                REQUIRE( mesh.triangleIndex({3./4., 7./8.}) == 7);
            }
        }

        WHEN( "Checking boundary points" ) {
            THEN( "Points belonging to multiple triangle should adhere to *this* specified order" ) {

                REQUIRE( mesh.triangleIndex({0.   , 0.}   ) == 0);
                REQUIRE( mesh.triangleIndex({1./2., 0.}   ) == 0);
                REQUIRE( mesh.triangleIndex({1./2., 1./2.}) == 0);
                REQUIRE( mesh.triangleIndex({1./4., 1./4.}) == 0);
                REQUIRE( mesh.triangleIndex({0.   , 1./2.}) == 1);
                REQUIRE( mesh.triangleIndex({0.   , 1.}   ) == 8);
                REQUIRE( mesh.triangleIndex({1./2., 1.}   ) == 8);
                REQUIRE( mesh.triangleIndex({1.   , 1.}   ) == 10);
                REQUIRE( mesh.triangleIndex({1.   , 1./2.}) == 3);
                REQUIRE( mesh.triangleIndex({1.   , 0.}   ) == 2);
                REQUIRE( mesh.triangleIndex({3./4., 3./4.}) == 6);
            }
        }
    }
}

SCENARIO( "Projecting a given point p in R^2 into a mesh", "[structured_triangle_mesh_2d]") {

    GIVEN( "A mesh with one block") {

        StructuredTriangleMesh2D<Point2> mesh;

        mesh.setDiscretization(1, 1);
        mesh.setDomainLength({1., 1.});

        REQUIRE( mesh.numBlocks() == 1 );
        REQUIRE( mesh.numVertices() == 9 );
        REQUIRE( mesh.numTriangles() == 8 );

        WHEN( "Projecting a point that lies inside the mesh" ) {
            THEN( "The projection returns the same point (it is the identify operator)" ) {

                Point2 p, q;

                p = {1./4., 1./8.};
                q = mesh.projectOntoDomain(p);
                REQUIRE( p[0] == Approx(q[0]) );
                REQUIRE( p[1] == Approx(q[1]) );

                p = {1./4., 3./8.};
                q = mesh.projectOntoDomain(p);
                REQUIRE( p[0] == Approx(q[0]) );
                REQUIRE( p[1] == Approx(q[1]) );

                p = {1./4., 5./8.};
                q = mesh.projectOntoDomain(p);
                REQUIRE( p[0] == Approx(q[0]) );
                REQUIRE( p[1] == Approx(q[1]) );

                p = {1./4., 7./8.};
                q = mesh.projectOntoDomain(p);
                REQUIRE( p[0] == Approx(q[0]) );
                REQUIRE( p[1] == Approx(q[1]) );

                p = {3./4., 1./8.};
                q = mesh.projectOntoDomain(p);
                REQUIRE( p[0] == Approx(q[0]) );
                REQUIRE( p[1] == Approx(q[1]) );

                p = {3./4., 3./8.};
                q = mesh.projectOntoDomain(p);
                REQUIRE( p[0] == Approx(q[0]) );
                REQUIRE( p[1] == Approx(q[1]) );

                p = {3./4., 5./8.};
                q = mesh.projectOntoDomain(p);
                REQUIRE( p[0] == Approx(q[0]) );
                REQUIRE( p[1] == Approx(q[1]) );

                p = {3./4., 7./8.};
                q = mesh.projectOntoDomain(p);
                REQUIRE( p[0] == Approx(q[0]) );
                REQUIRE( p[1] == Approx(q[1]) );
            }
        }

        WHEN( "Projecting a point that lies on the boundary" ) {
            THEN( "The projection returns the same point (it is the identify operator)" ) {

                Point2 p, q;

                p = {0.   , 0.};
                q = mesh.projectOntoDomain(p);
                REQUIRE( p[0] == Approx(q[0]) );
                REQUIRE( p[1] == Approx(q[1]) );

                p = {1./2., 0.};
                q = mesh.projectOntoDomain(p);
                REQUIRE( p[0] == Approx(q[0]) );
                REQUIRE( p[1] == Approx(q[1]) );

                p = {1./2., 1./2.};
                q = mesh.projectOntoDomain(p);
                REQUIRE( p[0] == Approx(q[0]) );
                REQUIRE( p[1] == Approx(q[1]) );

                p = {1./4., 1./4.};
                q = mesh.projectOntoDomain(p);
                REQUIRE( p[0] == Approx(q[0]) );
                REQUIRE( p[1] == Approx(q[1]) );

                p = {0.   , 1./2.};
                q = mesh.projectOntoDomain(p);
                REQUIRE( p[0] == Approx(q[0]) );
                REQUIRE( p[1] == Approx(q[1]) );

                p = {0.   , 1.};
                q = mesh.projectOntoDomain(p);
                REQUIRE( p[0] == Approx(q[0]) );
                REQUIRE( p[1] == Approx(q[1]) );

                p = {1./2., 1.};
                q = mesh.projectOntoDomain(p);
                REQUIRE( p[0] == Approx(q[0]) );
                REQUIRE( p[1] == Approx(q[1]) );

                p = {1.   , 1.};
                q = mesh.projectOntoDomain(p);
                REQUIRE( p[0] == Approx(q[0]) );
                REQUIRE( p[1] == Approx(q[1]) );

                p = {1.   , 1./2.};
                q = mesh.projectOntoDomain(p);
                REQUIRE( p[0] == Approx(q[0]) );
                REQUIRE( p[1] == Approx(q[1]) );

                p = {1.   , 0.};
                q = mesh.projectOntoDomain(p);
                REQUIRE( p[0] == Approx(q[0]) );
                REQUIRE( p[1] == Approx(q[1]) );

                p = {3./4., 3./4.};
                q = mesh.projectOntoDomain(p);
                REQUIRE( p[0] == Approx(q[0]) );
                REQUIRE( p[1] == Approx(q[1]) );

            }
        }

        WHEN( "Projecting a point that outside the mesh" ) {
            THEN( "The projection returns the closest point on the mesh's boundary" ) {

                Point2 p, q, expected;

                p = {-1.   , -1.};
                q = mesh.projectOntoDomain(p);
                expected = {0.0, 0.0};
                REQUIRE( expected[0] == Approx(q[0]) );
                REQUIRE( expected[1] == Approx(q[1]) );

                p = {1./2., -1./2.};
                q = mesh.projectOntoDomain(p);
                expected = {0.5, 0.0};
                REQUIRE( expected[0] == Approx(q[0]) );
                REQUIRE( expected[1] == Approx(q[1]) );

                p = {-1./2.   , 1./2.};
                q = mesh.projectOntoDomain(p);
                expected = {0.0, 0.5};
                REQUIRE( expected[0] == Approx(q[0]) );
                REQUIRE( expected[1] == Approx(q[1]) );

                p = {-0.5   , 1.5};
                q = mesh.projectOntoDomain(p);
                expected = {0.0, 1.0};
                REQUIRE( expected[0] == Approx(q[0]) );
                REQUIRE( expected[1] == Approx(q[1]) );

                p = {1./2., 1.5};
                q = mesh.projectOntoDomain(p);
                expected = {0.5, 1.0};
                REQUIRE( expected[0] == Approx(q[0]) );
                REQUIRE( expected[1] == Approx(q[1]) );

                p = {1.5   , 1.5};
                q = mesh.projectOntoDomain(p);
                expected = {1.0, 1.0};
                REQUIRE( expected[0] == Approx(q[0]) );
                REQUIRE( expected[1] == Approx(q[1]) );

                p = {0.5   , -1./2.};
                q = mesh.projectOntoDomain(p);
                expected = {0.5, 0.0};
                REQUIRE( expected[0] == Approx(q[0]) );
                REQUIRE( expected[1] == Approx(q[1]) );

                p = {1.5   , -0.5};
                q = mesh.projectOntoDomain(p);
                expected = {1.0, 0.0};
                REQUIRE( expected[0] == Approx(q[0]) );
                REQUIRE( expected[1] == Approx(q[1]) );
            }
        }
    }
}

SCENARIO( "Getting baricentric coordinates for a given point p in R^2", "[structured_triangle_mesh_2d]") {

    GIVEN( "A mesh with one block") {

        StructuredTriangleMesh2D<Point2> mesh;

        mesh.setDiscretization(1, 1);
        mesh.setDomainOrigin({0., 0.});
        mesh.setDomainLength({1., 1.});

        REQUIRE( mesh.numBlocks() == 1 );
        REQUIRE( mesh.numVertices() == 9 );
        REQUIRE( mesh.numTriangles() == 8 );

        WHEN( "Obtaining coordinates for a point inside a triangle" ) {
            THEN( "The coordinates lie on the interval (0, 1) and add to 1" ) {

                auto [tindex, coords] = mesh.baricentricCoordinates({1./4., 1./8.});
                REQUIRE( tindex == 0 ); 
                REQUIRE( coords[0] == Approx( 0.5 ) );
                REQUIRE( coords[1] == Approx( 0.25 ) );
                REQUIRE( coords[2] == Approx( 0.25 ) );

                std::tie(tindex, coords) = mesh.baricentricCoordinates({1./4., 3./8.});
                REQUIRE( tindex == 1 ); 
                REQUIRE( coords[0] == Approx( 0.25 ) );
                REQUIRE( coords[1] == Approx( 0.5 ) );
                REQUIRE( coords[2] == Approx( 0.25 ) );

                std::tie(tindex, coords) = mesh.baricentricCoordinates({1./4., 5./8.});
                REQUIRE( tindex == 4 ); 
                REQUIRE( coords[0] == Approx( 0.25 ) );
                REQUIRE( coords[1] == Approx( 0.5 ) );
                REQUIRE( coords[2] == Approx( 0.25 ) );

                std::tie(tindex, coords) = mesh.baricentricCoordinates({1./4., 7./8.});
                REQUIRE( tindex == 5 ); 
                REQUIRE( coords[0] == Approx( 0.25 ) );
                REQUIRE( coords[1] == Approx( 0.25 ) );
                REQUIRE( coords[2] == Approx( 0.5 ) );

                std::tie(tindex, coords) = mesh.baricentricCoordinates({3./4., 1./8.});
                REQUIRE( tindex == 2 ); 
                REQUIRE( coords[0] == Approx( 0.25 ) );
                REQUIRE( coords[1] == Approx( 0.5 ) );
                REQUIRE( coords[2] == Approx( 0.25 ) );

                std::tie(tindex, coords) = mesh.baricentricCoordinates({3./4., 3./8.});
                REQUIRE( tindex == 3 ); 
                REQUIRE( coords[0] == Approx( 0.25 ) );
                REQUIRE( coords[1] == Approx( 0.25 ) );
                REQUIRE( coords[2] == Approx( 0.5 ) );

                std::tie(tindex, coords) = mesh.baricentricCoordinates({3./4., 5./8.});
                REQUIRE( tindex == 6 ); 
                REQUIRE( coords[0] == Approx( 0.5 ) );
                REQUIRE( coords[1] == Approx( 0.25 ) );
                REQUIRE( coords[2] == Approx( 0.25 ) );

                std::tie(tindex, coords) = mesh.baricentricCoordinates({3./4., 7./8.});
                REQUIRE( tindex == 7 ); 
                REQUIRE( coords[0] == Approx( 0.25 ) );
                REQUIRE( coords[1] == Approx( 0.5 ) );
                REQUIRE( coords[2] == Approx( 0.25 ) );

            }
        }

        WHEN( "Obtaining coordinates for a point on the boundary of a triangle" ) {
            THEN( "The coordinates lie on the interval [0, 1] and add to 1" ) {

                auto [tindex, coords] = mesh.baricentricCoordinates({0.   , 0.});
                REQUIRE( tindex == 0 ); 
                REQUIRE( coords[0] == Approx( 1.0 ) );
                REQUIRE( coords[1] == Approx( 0.0 ) );
                REQUIRE( coords[2] == Approx( 0.0 ) );

                std::tie(tindex, coords) = mesh.baricentricCoordinates({1./2., 0.});
                REQUIRE( tindex == 0 ); 
                REQUIRE( coords[0] == Approx( 0.0 ) );
                REQUIRE( coords[1] == Approx( 1.0 ) );
                REQUIRE( coords[2] == Approx( 0.0 ) );

                std::tie(tindex, coords) = mesh.baricentricCoordinates({1./2., 1./2.});
                REQUIRE( tindex == 0 ); 
                REQUIRE( coords[0] == Approx( 0.0 ) );
                REQUIRE( coords[1] == Approx( 0.0 ) );
                REQUIRE( coords[2] == Approx( 1.0 ) );

                std::tie(tindex, coords) = mesh.baricentricCoordinates({1./4., 1./4.});
                REQUIRE( tindex == 0 ); 
                REQUIRE( coords[0] == Approx( 0.5 ) );
                REQUIRE( coords[1] == Approx( 0.0 ) );
                REQUIRE( coords[2] == Approx( 0.5 ) );

                std::tie(tindex, coords) = mesh.baricentricCoordinates({0.   , 1./2.});
                REQUIRE( tindex == 1 ); 
                REQUIRE( coords[0] == Approx( 0.0 ) );
                REQUIRE( coords[1] == Approx( 0.0 ) );
                REQUIRE( coords[2] == Approx( 1.0 ) );

                std::tie(tindex, coords) = mesh.baricentricCoordinates({0.   , 1.});
                REQUIRE( tindex == 4 ); 
                REQUIRE( coords[0] == Approx( 0.0 ) );
                REQUIRE( coords[1] == Approx( 0.0 ) );
                REQUIRE( coords[2] == Approx( 1.0 ) );

                std::tie(tindex, coords) = mesh.baricentricCoordinates({1./2., 1.});
                REQUIRE( tindex == 5 ); 
                REQUIRE( coords[0] == Approx( 0.0 ) );
                REQUIRE( coords[1] == Approx( 1.0 ) );
                REQUIRE( coords[2] == Approx( 0.0 ) );

                std::tie(tindex, coords) = mesh.baricentricCoordinates({1.   , 1.});
                REQUIRE( tindex == 6 ); 
                REQUIRE( coords[0] == Approx( 0.0 ) );
                REQUIRE( coords[1] == Approx( 0.0 ) );
                REQUIRE( coords[2] == Approx( 1.0 ) );

                std::tie(tindex, coords) = mesh.baricentricCoordinates({1.   , 1./2.});
                REQUIRE( tindex == 3 ); 
                REQUIRE( coords[0] == Approx( 0.0 ) );
                REQUIRE( coords[1] == Approx( 1.0 ) );
                REQUIRE( coords[2] == Approx( 0.0 ) );

                std::tie(tindex, coords) = mesh.baricentricCoordinates({1.   , 0.});
                REQUIRE( tindex == 2 ); 
                REQUIRE( coords[0] == Approx( 0.0 ) );
                REQUIRE( coords[1] == Approx( 1.0 ) );
                REQUIRE( coords[2] == Approx( 0.0 ) );

                std::tie(tindex, coords) = mesh.baricentricCoordinates({3./4., 3./4.});
                REQUIRE( tindex == 6 ); 
                REQUIRE( coords[0] == Approx( 0.5 ) );
                REQUIRE( coords[1] == Approx( 0.0 ) );
                REQUIRE( coords[2] == Approx( 0.5 ) );
            }
        }
    }
}
