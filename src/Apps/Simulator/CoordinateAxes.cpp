#include "CoordinateAxes.h"

CoordinateAxes::CoordinateAxes()
{
    //init();
}


void CoordinateAxes::init()
{
    //va_coneaxes = 0;
    //bf_faces_coneaxes = 0;
    //nfaces_cone = 0;

    //va_cylinderaxes = 0;
    //bf_faces_cylinderaxes = 0;
    //nfaces_cylinder = 0;

}


void CoordinateAxes::initShader( std::string directory )
{


    //shader_axes = new Tucano::Shader( "shader_axes", ( directory + "shaders/vertex_instanced_shader.vert" ), ( directory + "shaders/fragment_shader.frag" ), "", "", "" );
    //shader_axes->initialize();

    current_directory = directory;

}


void CoordinateAxes::reloadShader()
{
   //if( shader_axes )
   //     shader_axes->reloadShaders();
}


static void PrintInfo(const tinyobj::attrib_t &attrib, const std::vector<tinyobj::shape_t>& shapes, const std::vector<tinyobj::material_t>& materials, bool triangulate = true)
{
	std::cout << "# of vertices  : " << (attrib.vertices.size() / 3) << std::endl;
	std::cout << "# of normals   : " << (attrib.normals.size() / 3) << std::endl;
	std::cout << "# of texcoords : " << (attrib.texcoords.size() / 2) << std::endl;

	std::cout << "# of shapes    : " << shapes.size() << std::endl;
	std::cout << "# of materials : " << materials.size() << std::endl;

	for (size_t v = 0; v < attrib.vertices.size() / 3; v++) {
		printf("  v[%ld] = (%f, %f, %f)\n", v,
			static_cast<const double>(attrib.vertices[3 * v + 0]),
			static_cast<const double>(attrib.vertices[3 * v + 1]),
			static_cast<const double>(attrib.vertices[3 * v + 2]));
	}

	for (size_t v = 0; v < attrib.normals.size() / 3; v++) {
		printf("  n[%ld] = (%f, %f, %f)\n", v,
			static_cast<const double>(attrib.normals[3 * v + 0]),
			static_cast<const double>(attrib.normals[3 * v + 1]),
			static_cast<const double>(attrib.normals[3 * v + 2]));
	}

	for (size_t v = 0; v < attrib.texcoords.size() / 2; v++) {
		printf("  uv[%ld] = (%f, %f)\n", v,
			static_cast<const double>(attrib.texcoords[2 * v + 0]),
			static_cast<const double>(attrib.texcoords[2 * v + 1]));
	}

	for (size_t i = 0; i < shapes.size(); i++) {
		printf("shape[%ld].name = %s\n", i, shapes[i].name.c_str());
		printf("Size of shape[%ld].indices: %ld\n", i, shapes[i].mesh.indices.size());

		if (triangulate)
		{
			printf("Size of shape[%ld].material_ids: %ld\n", i, shapes[i].mesh.material_ids.size());
			assert((shapes[i].mesh.indices.size() % 3) == 0);
			for (size_t f = 0; f < shapes[i].mesh.indices.size() / 3; f++) {
				tinyobj::index_t i0 = shapes[i].mesh.indices[3 * f + 0];
				tinyobj::index_t i1 = shapes[i].mesh.indices[3 * f + 1];
				tinyobj::index_t i2 = shapes[i].mesh.indices[3 * f + 2];
				printf("  idx[%ld] = %d/%d/%d, %d/%d/%d, %d/%d/%d. mat_id = %d\n", f,
					i0.vertex_index, i0.normal_index, i0.texcoord_index,
					i1.vertex_index, i1.normal_index, i1.texcoord_index,
					i2.vertex_index, i2.normal_index, i2.texcoord_index,
					shapes[i].mesh.material_ids[f]);
			}
		}
		else {
			for (size_t f = 0; f < shapes[i].mesh.indices.size(); f++) {
				tinyobj::index_t idx = shapes[i].mesh.indices[f];
				printf("  idx[%ld] = %d/%d/%d\n", f, idx.vertex_index, idx.normal_index, idx.texcoord_index);
			}

			printf("Size of shape[%ld].material_ids: %ld\n", i, shapes[i].mesh.material_ids.size());
			assert(shapes[i].mesh.material_ids.size() == shapes[i].mesh.num_face_vertices.size());
			for (size_t m = 0; m < shapes[i].mesh.material_ids.size(); m++) {
				printf("  material_id[%ld] = %d\n", m,
					shapes[i].mesh.material_ids[m]);
			}

		}

		printf("shape[%ld].num_faces: %ld\n", i, shapes[i].mesh.num_face_vertices.size());
		for (size_t v = 0; v < shapes[i].mesh.num_face_vertices.size(); v++) {
			printf("  num_vertices[%ld] = %ld\n", v,
				static_cast<long>(shapes[i].mesh.num_face_vertices[v]));
		}

		//printf("shape[%ld].vertices: %ld\n", i, shapes[i].mesh.positions.size());
		//assert((shapes[i].mesh.positions.size() % 3) == 0);
		//for (size_t v = 0; v < shapes[i].mesh.positions.size() / 3; v++) {
		//  printf("  v[%ld] = (%f, %f, %f)\n", v,
		//    static_cast<const double>(shapes[i].mesh.positions[3*v+0]),
		//    static_cast<const double>(shapes[i].mesh.positions[3*v+1]),
		//    static_cast<const double>(shapes[i].mesh.positions[3*v+2]));
		//}

		printf("shape[%ld].num_tags: %ld\n", i, shapes[i].mesh.tags.size());
		for (size_t t = 0; t < shapes[i].mesh.tags.size(); t++) {
			printf("  tag[%ld] = %s ", t, shapes[i].mesh.tags[t].name.c_str());
			printf(" ints: [");
			for (size_t j = 0; j < shapes[i].mesh.tags[t].intValues.size(); ++j)
			{
				printf("%ld", static_cast<long>(shapes[i].mesh.tags[t].intValues[j]));
				if (j < (shapes[i].mesh.tags[t].intValues.size() - 1))
				{
					printf(", ");
				}
			}
			printf("]");

			printf(" floats: [");
			for (size_t j = 0; j < shapes[i].mesh.tags[t].floatValues.size(); ++j)
			{
				printf("%f", static_cast<const double>(shapes[i].mesh.tags[t].floatValues[j]));
				if (j < (shapes[i].mesh.tags[t].floatValues.size() - 1))
				{
					printf(", ");
				}
			}
			printf("]");

			printf(" strings: [");
			for (size_t j = 0; j < shapes[i].mesh.tags[t].stringValues.size(); ++j)
			{
				printf("%s", shapes[i].mesh.tags[t].stringValues[j].c_str());
				if (j < (shapes[i].mesh.tags[t].stringValues.size() - 1))
				{
					printf(", ");
				}
			}
			printf("]");
			printf("\n");
		}
	}

	for (size_t i = 0; i < materials.size(); i++) {
		printf("material[%ld].name = %s\n", i, materials[i].name.c_str());
		printf("  material.Ka = (%f, %f ,%f)\n", static_cast<const double>(materials[i].ambient[0]), static_cast<const double>(materials[i].ambient[1]), static_cast<const double>(materials[i].ambient[2]));
		printf("  material.Kd = (%f, %f ,%f)\n", static_cast<const double>(materials[i].diffuse[0]), static_cast<const double>(materials[i].diffuse[1]), static_cast<const double>(materials[i].diffuse[2]));
		printf("  material.Ks = (%f, %f ,%f)\n", static_cast<const double>(materials[i].specular[0]), static_cast<const double>(materials[i].specular[1]), static_cast<const double>(materials[i].specular[2]));
		printf("  material.Tr = (%f, %f ,%f)\n", static_cast<const double>(materials[i].transmittance[0]), static_cast<const double>(materials[i].transmittance[1]), static_cast<const double>(materials[i].transmittance[2]));
		printf("  material.Ke = (%f, %f ,%f)\n", static_cast<const double>(materials[i].emission[0]), static_cast<const double>(materials[i].emission[1]), static_cast<const double>(materials[i].emission[2]));
		printf("  material.Ns = %f\n", static_cast<const double>(materials[i].shininess));
		printf("  material.Ni = %f\n", static_cast<const double>(materials[i].ior));
		printf("  material.dissolve = %f\n", static_cast<const double>(materials[i].dissolve));
		printf("  material.illum = %d\n", materials[i].illum);
		printf("  material.map_Ka = %s\n", materials[i].ambient_texname.c_str());
		printf("  material.map_Kd = %s\n", materials[i].diffuse_texname.c_str());
		printf("  material.map_Ks = %s\n", materials[i].specular_texname.c_str());
		printf("  material.map_Ns = %s\n", materials[i].specular_highlight_texname.c_str());
		printf("  material.map_bump = %s\n", materials[i].bump_texname.c_str());
		printf("  material.map_d = %s\n", materials[i].alpha_texname.c_str());
		printf("  material.disp = %s\n", materials[i].displacement_texname.c_str());
		std::map<std::string, std::string>::const_iterator it(materials[i].unknown_parameter.begin());
		std::map<std::string, std::string>::const_iterator itEnd(materials[i].unknown_parameter.end());

		for (; it != itEnd; it++) {
			printf("  material.%s = %s\n", it->first.c_str(), it->second.c_str());
		}
		printf("\n");
	}
}


void CoordinateAxes::load()
{

	std::string filename = current_directory + "models/cornell_box.obj";
	std::string material_path = current_directory + "models/";

	std::cout << "Loading " << filename << std::endl;

	
	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	std::string err;

	if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &err, filename.c_str(), material_path.c_str())) {
		throw std::runtime_error(err);
	}
	else
	{
		std::cout << "# of shapes    : " << shapes.size() << std::endl;
		std::cout << "# of materials : " << materials.size() << std::endl;
		PrintInfo(attrib, shapes, materials, true);
	}



    //loadCylinderAxes();
    //loadConeAxes();

}


void CoordinateAxes::loadConeAxes()
{



    //vector< float > vertices;
    //vector< float > colors;
    //vector< float > normals;
    //vector< unsigned int > faces;


    //readPlyFile( ( current_directory + "models/cone.ply" ), vertices, colors, faces, normals );


    //glGenVertexArrays( 1, &va_coneaxes );
    //glBindVertexArray( va_coneaxes );



    //    if( vertices.empty() == false )
    //    {

    //        size_t vvertices_size = vertices.size();
    //        GLint nvertices =  (GLint) vvertices_size/3;


    //        vector< Eigen::Vector3f > vertices_eigen;
    //        for( int it = 0; it < nvertices; ++it )
    //            vertices_eigen.push_back( Eigen::Vector3f( vertices[ 3*it ], vertices[ 3*it + 1 ], vertices[ 3*it + 2 ] ) );

    //        Celer::BoundingBox3< float > box;
    //        box.fromPointCloud( vertices_eigen.begin(), vertices_eigen.end() );


    //        for( int it = 0; it < nvertices; ++it )
    //            vertices_eigen[ it ] = ( vertices_eigen[ it ] - box.center() )/box.diagonal();

    //        box.fromPointCloud( vertices_eigen.begin(), vertices_eigen.end() );


    //        GLuint bf_points_coneaxes;
    //        glGenBuffers( 1, &bf_points_coneaxes );
    //        glBindBuffer( GL_ARRAY_BUFFER, bf_points_coneaxes );
    //        glBufferData( GL_ARRAY_BUFFER, vvertices_size*sizeof( GLfloat ), vertices_eigen.data(), GL_STATIC_DRAW );
    //        glVertexAttribPointer( 0, 3, GL_FLOAT, GL_TRUE, 0, NULL );
    //        glEnableVertexAttribArray( 0 );

    //    }


    //    if( faces.empty() == false )
    //    {
    //        std::size_t vfaces_size = faces.size();
    //        nfaces_cone = (GLint) vfaces_size;

    //        glGenBuffers( 1, &bf_faces_coneaxes );
    //        glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, bf_faces_coneaxes );
    //        glBufferData( GL_ELEMENT_ARRAY_BUFFER, vfaces_size*sizeof( GLuint ), faces.data(), GL_STATIC_DRAW );

    //    }


    //    if( normals.empty() == false )
    //    {

    //        std::size_t vnormals_size = normals.size();

    //        GLuint bf_normals_coneaxes;
    //        glGenBuffers( 1, &bf_normals_coneaxes );
    //        glBindBuffer( GL_ARRAY_BUFFER, bf_normals_coneaxes );
    //        glBufferData( GL_ARRAY_BUFFER, vnormals_size*sizeof(  GLfloat ), normals.data(), GL_STATIC_DRAW );
    //        glVertexAttribPointer( 2, 3, GL_FLOAT, GL_TRUE, 0, NULL );
    //        glEnableVertexAttribArray( 2 );
    //    }


    //    const std::size_t NAXES = 3;

    //    Eigen::Affine3f* M = new Eigen::Affine3f[ NAXES ];

    //    for( std::size_t it = 0; it < NAXES; ++it )
    //    {
    //        Eigen::Affine3f matrix;
    //        matrix.setIdentity();

    //        if( it == 0 )
    //        {
    //            matrix.translate( Eigen::Vector3f( 0.0f, 0.38f, 0.0f ) );
    //            matrix.rotate( Eigen::AngleAxisf( (float)M_PI, Eigen::Vector3f( 1.0f, 0.0f, 0.0f ) ) );
    //        }
    //        else if( it == 1 )
    //        {
    //            matrix.translate( Eigen::Vector3f( 0.38f , 0.0f, 0.0f ) );
    //            matrix.rotate( Eigen::AngleAxisf( (float)M_PI*0.5f, Eigen::Vector3f( 0.0f, 0.0f, 1.0f ) ) );
    //        }
    //        else
    //        {
    //            matrix.translate( Eigen::Vector3f( 0.0f , 0.0f, 0.38f ) );
    //            matrix.rotate( Eigen::AngleAxisf((float) -M_PI*0.5f, Eigen::Vector3f( 1.0f, 0.0f, 0.0f ) ) );
    //        }

    //        matrix.scale(  Eigen::Vector3f( 0.08f, 0.18f, 0.08f )  );


    //        M[ it ] = matrix;
    //    }

    //    GLuint bf_mmatrix_axes;
    //    glGenBuffers( 1, &bf_mmatrix_axes );
    //    glBindBuffer( GL_ARRAY_BUFFER, bf_mmatrix_axes );
    //    glBufferData( GL_ARRAY_BUFFER, NAXES*sizeof( Eigen::Affine3f ), &M[ 0 ], GL_STATIC_DRAW );

    //    glEnableVertexAttribArray( 3 );
    //    glVertexAttribPointer( 3, 4, GL_FLOAT, GL_FALSE, sizeof( Eigen::Affine3f ), (GLvoid*)0 );

    //    glEnableVertexAttribArray(4);
    //    glVertexAttribPointer( 4, 4, GL_FLOAT, GL_FALSE, sizeof( Eigen::Affine3f ), (GLvoid*)( sizeof( Eigen::Vector4f ) ) );

    //    glEnableVertexAttribArray( 5 );
    //    glVertexAttribPointer( 5, 4, GL_FLOAT, GL_FALSE, sizeof( Eigen::Affine3f ), (GLvoid*)( 2 * sizeof( Eigen::Vector4f ) ) );

    //    glEnableVertexAttribArray( 6 );
    //    glVertexAttribPointer( 6, 4, GL_FLOAT, GL_FALSE, sizeof( Eigen::Affine3f ), (GLvoid*)( 3 * sizeof( Eigen::Vector4f ) ) );

    //    glVertexAttribDivisor( 3, 1 );
    //    glVertexAttribDivisor( 4, 1 );
    //    glVertexAttribDivisor( 5, 1 );
    //    glVertexAttribDivisor( 6, 1 );


    //    glBindBuffer( GL_ARRAY_BUFFER, 0 );
    //    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );


    //glBindVertexArray( 0 );


}


void CoordinateAxes::loadCylinderAxes()
{


    //vector< float > vertices;
    //vector< float > colors;
    //vector< float > normals;
    //vector< unsigned int > faces;


    //readPlyFile( ( current_directory + "Models/cylinder.ply" ), vertices, colors, faces, normals );


    //glGenVertexArrays( 1, &va_cylinderaxes );
    //glBindVertexArray( va_cylinderaxes );



    //    if( vertices.empty() == false )
    //    {

    //        size_t vvertices_size = vertices.size();
    //        GLint nvertices =  (GLint) vvertices_size/3;


    //        vector< Eigen::Vector3f > vertices_eigen;
    //        for( int it = 0; it < nvertices; ++it )
    //            vertices_eigen.push_back( Eigen::Vector3f( vertices[ 3*it ], vertices[ 3*it + 1 ], vertices[ 3*it + 2 ] ) );

    //        Celer::BoundingBox3< float > box;
    //        box.fromPointCloud( vertices_eigen.begin(), vertices_eigen.end() );


    //        for( int it = 0; it < nvertices; ++it )
    //            vertices_eigen[ it ] = ( vertices_eigen[ it ] - box.center() )/box.diagonal();

    //        box.fromPointCloud( vertices_eigen.begin(), vertices_eigen.end() );


    //        GLuint bf_points_cylinderaxes;
    //        glGenBuffers( 1, &bf_points_cylinderaxes );
    //        glBindBuffer( GL_ARRAY_BUFFER, bf_points_cylinderaxes );
    //        glBufferData( GL_ARRAY_BUFFER, vvertices_size*sizeof( GLfloat ), vertices_eigen.data(), GL_STATIC_DRAW );
    //        glVertexAttribPointer( 0, 3, GL_FLOAT, GL_TRUE, 0, NULL );
    //        glEnableVertexAttribArray( 0 );

    //    }


    //    if( faces.empty() == false )
    //    {
    //        std::size_t vfaces_size = faces.size();
    //        nfaces_cylinder = (GLint) vfaces_size;

    //        glGenBuffers( 1, &bf_faces_cylinderaxes );
    //        glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, bf_faces_cylinderaxes );
    //        glBufferData( GL_ELEMENT_ARRAY_BUFFER, vfaces_size*sizeof( GLuint ), faces.data(), GL_STATIC_DRAW );

    //    }


    //    if( normals.empty() == false )
    //    {

    //        std::size_t vnormals_size = normals.size();

    //        GLuint bf_normals_cylinderaxes;
    //        glGenBuffers( 1, &bf_normals_cylinderaxes );
    //        glBindBuffer( GL_ARRAY_BUFFER, bf_normals_cylinderaxes );
    //        glBufferData( GL_ARRAY_BUFFER, vnormals_size*sizeof(  GLfloat ), normals.data(), GL_STATIC_DRAW );
    //        glVertexAttribPointer( 2, 3, GL_FLOAT, GL_TRUE, 0, NULL );
    //        glEnableVertexAttribArray( 2 );
    //    }



    //    const std::size_t NAXES = 3;

    //    Eigen::Affine3f* M = new Eigen::Affine3f[ NAXES ];

    //    for( std::size_t it = 0; it < NAXES; ++it )
    //    {
    //        Eigen::Affine3f matrix;
    //        matrix.setIdentity();


    //        Eigen::Vector3f vector_translation( 0.0f, 0.0f, 0.0f );
    //        if( it == 0 )
    //            vector_translation[ 1 ] += 0.18f;

    //        else if( it == 1 )
    //            vector_translation[ 0 ] += 0.18f;

    //        else
    //            vector_translation[ 2 ] += 0.18f;

    //        matrix.translate( vector_translation );

    //        Eigen::Vector3f axis_rotation( 0.0f, 0.0f, 0.0f );
    //        axis_rotation[ it ] = 1.0f;

    //        matrix.rotate( Eigen::AngleAxisf( (float)0.5f*M_PI, axis_rotation ) );
    //        matrix.scale( Eigen::Vector3f( 0.032f, 0.032f, 0.8f ) );

    //        M[ it ] = matrix;
    //    }


    //    GLuint bf_mmatrix_axes;
    //    glGenBuffers( 1, &bf_mmatrix_axes );
    //    glBindBuffer( GL_ARRAY_BUFFER, bf_mmatrix_axes );
    //    glBufferData( GL_ARRAY_BUFFER, NAXES*sizeof( Eigen::Affine3f ), &M[ 0 ], GL_STATIC_DRAW );

    //    glEnableVertexAttribArray( 3 );
    //    glVertexAttribPointer( 3, 4, GL_FLOAT, GL_FALSE, sizeof( Eigen::Affine3f ), (GLvoid*)0 );

    //    glEnableVertexAttribArray(4);
    //    glVertexAttribPointer( 4, 4, GL_FLOAT, GL_FALSE, sizeof( Eigen::Affine3f ), (GLvoid*)( sizeof( Eigen::Vector4f ) ) );

    //    glEnableVertexAttribArray( 5 );
    //    glVertexAttribPointer( 5, 4, GL_FLOAT, GL_FALSE, sizeof( Eigen::Affine3f ), (GLvoid*)( 2 * sizeof( Eigen::Vector4f ) ) );

    //    glEnableVertexAttribArray( 6 );
    //    glVertexAttribPointer( 6, 4, GL_FLOAT, GL_FALSE, sizeof( Eigen::Affine3f ), (GLvoid*)( 3 * sizeof( Eigen::Vector4f ) ) );

    //    glVertexAttribDivisor( 3, 1 );
    //    glVertexAttribDivisor( 4, 1 );
    //    glVertexAttribDivisor( 5, 1 );
    //    glVertexAttribDivisor( 6, 1 );


    //    glBindBuffer( GL_ARRAY_BUFFER, 0 );
    //    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );


    //glBindVertexArray( 0 );


}


void CoordinateAxes::draw(const Eigen::Affine3f& V, const Eigen::Matrix4f& P)
{



    //Eigen::Affine3f V;
    //V.setIdentity();

    //for( int i = 0; i < 3; ++i )
    //    for( int j = 0; j < 3; ++j )
    //        V( i, j ) = R( i, j );

    //V( 0, 3 ) = T( 0 );
    //V( 1, 3 ) = T( 1 );
    //V( 2, 3 ) = T( 2 );


    //Eigen::Affine3f matrix;
    //matrix.setIdentity();
    //matrix.translate( Eigen::Vector3f( 3.1f, -1.7f, 0.0f ) );


    //QMatrix4x4 Q;
    //Q.perspective( 60.0 , (float) 1024/(float) 600 , 0.1f , 10000.0f );
//    Q.ortho( 0, 1024, 0, 600, 0.1, 1000 );

    //Eigen::Matrix4f P1;
    //for( int i = 0; i < 4; ++i )
    //    for( int j = 0; j < 4; ++j )
    //        P1( i, j ) = Q( i, j );




    //shader_axes->bind();

    //shader_axes->setUniform( "vmatrix", V );
    //shader_axes->setUniform( "pmatrix", P );

    //    glBindVertexArray( va_coneaxes );


    //        glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, bf_faces_coneaxes );
    //        glDrawElementsInstanced( GL_TRIANGLES, nfaces_cone, GL_UNSIGNED_INT, 0, 3 );

    //    glBindVertexArray( 0 );


    //    glBindVertexArray( va_cylinderaxes );


    //        glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, bf_faces_cylinderaxes );
    //        glDrawElementsInstanced( GL_TRIANGLES, nfaces_cylinder, GL_UNSIGNED_INT, 0, 3 );


    //    glBindVertexArray( 0 );

    //shader_axes->unbind();


}


void CoordinateAxes::resetBuffers()
{
    //deleteShaders();

    //if( va_coneaxes )
    //{
    //    glDeleteVertexArrays( 1, &va_coneaxes );
    //    if( bf_faces_coneaxes )
    //        glDeleteBuffers(1, &bf_faces_coneaxes);

    //}

    //if( va_cylinderaxes )
    //{
    //    glDeleteVertexArrays( 1, &va_cylinderaxes );
    //    if( bf_faces_cylinderaxes )
    //        glDeleteBuffers(1, &bf_faces_cylinderaxes);

    //}


    //va_coneaxes = 0;
    //bf_faces_coneaxes = 0;
    //bf_faces_cylinderaxes = 0;
    //va_cylinderaxes = 0;;

    //nfaces_cone = 0;
    //nfaces_cylinder = 0;


}

void CoordinateAxes::deleteShaders()
{

    //if (shader_axes)
    //{
    //    delete (shader_axes);
    //    shader_axes = nullptr;
    //}


}

