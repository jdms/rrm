#include "CoordinateAxes.h"

CoordinateAxes::CoordinateAxes()
{

}


CoordinateAxes::~CoordinateAxes()
{
	resetBuffers();
}

void CoordinateAxes::init()
{
	vertex_array_coneaxes = 0;
	vertex_buffer_faces_indices = 0;
	vertex_buffer_vertices = 0;
	vertex_buffer_normals = 0;
	vertex_buffer_colors = 0;
}

void CoordinateAxes::initShader( std::string directory )
{
	shader_axes = new Tucano::Shader("shader_axes", (directory + "shaders/orientationAxes.vert"), (directory + "shaders/orientationAxes.frag"), (directory + "shaders/orientationAxes.geom"), "", "");
    shader_axes->initialize();

    current_directory = directory;

	float left		 = -1.0f;
	float right		 =  1.0f;
	float bottom	 = -1.0f;
	float top		 =  1.0f; 
	float near_plane = 0.1f;
	float far_plane  = 100.0f;

	this->projetionMatrix = Eigen::Matrix4f::Zero();

	projetionMatrix(0, 0) = 2.0f / (right - left);
	projetionMatrix(1, 1) = 2.0f / (top - bottom);
	projetionMatrix(2, 2) = -2.0f / (far_plane - near_plane);
	projetionMatrix(3, 3) = 1.0f;
	projetionMatrix(0, 3) = -(right + left) / (right - left);
	projetionMatrix(1, 3) = -(top + bottom) / (top - bottom);
	projetionMatrix(2, 3) = -(far_plane + near_plane) / (far_plane - near_plane);

	/// The correct manner to defaine an identity matrix
	this->viewMatrix_ = Eigen::Affine3f::Identity();
	this->viewMatrix_.translate(Eigen::Vector3f(0.0f, 0.0f, -4.0f));
	this->viewMatrix_.rotate(Eigen::Quaternion<float>::Identity());
}

void CoordinateAxes::reloadShader()
{
	if (shader_axes)
	{
		shader_axes->reloadShaders();
	}
}

void CoordinateAxes::load()
{

	std::string filename = current_directory + "models/coordinateSystem.obj";
	std::string material_path = current_directory + "models/";

	std::cout << "Loading " << filename << std::endl;

	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	std::string err;

	//// For Vertex Buffer Intervealed @see https://www.khronos.org/opengl/wiki/Vertex_Specification_Best_Practices
	/// (VNC-VNC-VNC ...)

	std::vector<GLfloat> attributes;
	std::vector<int> indices;


	if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &err, filename.c_str(), material_path.c_str())) 
	{
		throw std::runtime_error(err);
	}
	else
	{
		attributes.clear();
		attributes.resize(attrib.vertices.size()*3);

		for ( auto shape : shapes) 
		{
			// Loop over faces(polygon)			
			std::cout << "# of shapes  name  : " << shape.name << std::endl;
			for (auto i : shape.mesh.indices)
			{
				indices.push_back(i.vertex_index);
			}

			size_t index_offset = 0;

			for (size_t f = 0; f < shape.mesh.num_face_vertices.size(); f++) 
			{
				size_t fv = shape.mesh.num_face_vertices[f];
				// Loop over vertices in the face.
				for (size_t v = 0; v < fv; v++) 
				{
					// access to vertex
					/// Use idx.vertex_index             | Use idx.vertex_index
					tinyobj::index_t idx = shape.mesh.indices[index_offset + v];
					attributes[9 * idx.vertex_index + 0] = attrib.vertices[3 * idx.vertex_index + 0];
					attributes[9 * idx.vertex_index + 1] = attrib.vertices[3 * idx.vertex_index + 1];
					attributes[9 * idx.vertex_index + 2] = attrib.vertices[3 * idx.vertex_index + 2];
					///	Use idx.vertex_index			 | Use idx.normal_index
					attributes[9 * idx.vertex_index + 3] = attrib.normals[3 * idx.normal_index + 0];
					attributes[9 * idx.vertex_index + 4] = attrib.normals[3 * idx.normal_index + 1];
					attributes[9 * idx.vertex_index + 5] = attrib.normals[3 * idx.normal_index + 2];
					///	Use idx.vertex_index			 | shape.mesh.material_ids[f]
					attributes[9 * idx.vertex_index + 6] = materials[shape.mesh.material_ids[f]].diffuse[0];
					attributes[9 * idx.vertex_index + 7] = materials[shape.mesh.material_ids[f]].diffuse[1];
					attributes[9 * idx.vertex_index + 8] = materials[shape.mesh.material_ids[f]].diffuse[2];
				}
				index_offset += fv;
			}
		}

	}
		//std::cout << "# Indices  : " << indices.size() << std::endl;
		//std::cout << "# Vertices : " << attrib.vertices.size() << std::endl;
	for (auto m : materials)
	{
		std::cout << "# Materials Name : " << m.name << std::endl;
		std::cout << "# Materia Ambient  KA: " << m.ambient[0] << "," << m.ambient[1] << "," << m.ambient[2] << "," << std::endl;
		std::cout << "# Materia Diffuse  KD: " << m.diffuse[0] << "," << m.diffuse[1] << "," << m.diffuse[2] << "," << std::endl;
		std::cout << "# Materia Specular KS: " << m.specular[0] << "," << m.specular[1] << "," << m.specular[2] << "," << std::endl;
	}
	
	// If exit, clean up the buffer on the server side (OpenGL - GPU) and in the client side (C++ buffers id)
	this->requestOpenGLResources();

	glBindVertexArray(vertex_array_coneaxes);

		//// - Vertex Information  (v.layout = 0, n.layout = 1, c.layout = 2)
		glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_vertices);
		glBufferData(GL_ARRAY_BUFFER, attributes.size() * sizeof(attributes[0]), attributes.data(), GL_STATIC_DRAW);

		/// Size of the per vertex attributes, aka (Vertex(V), Normal(N), Color(C)), each 3 floats
		int size_of_vertice = 3 * sizeof(float); 
		/// Size of per triagle attributes [ (V.x,V.y,V.z;N.x,N.y,N.z;C.x,C.y,C.z) ], each 3 floats to vertice, 3 to normal, and 3 to color
		int size_of_struct  = 3 * size_of_vertice;

		////! @link - http://www.opengl.org/wiki/Vertex_Specification
		//// Assign for vertex attribute location 0 - 3 
		for (int location = 0; location < 3; location++)
		{
			glEnableVertexAttribArray(location);
			glVertexAttribPointer(location, 3, GL_FLOAT, GL_FALSE, size_of_struct, reinterpret_cast<void*>(size_of_vertice * location));
		}

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertex_buffer_faces_indices);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(indices[0]), indices.data(), GL_STATIC_DRAW);
		indices_size_ = indices.size();

	glBindVertexArray(0);
}
void CoordinateAxes::draw( const Eigen::Quaternion<float>& _orientation, const Eigen::Vector4f& _viewport)
{

	/// Coordinate Axes enviroment setup

	float integral = 0.0f;

	integral = std::trunc(static_cast<float>(_viewport[2]*0.1f));
	GLsizei w = static_cast<GLsizei>(integral);
	integral = std::trunc(static_cast<float>(_viewport[3]*0.1f));
	GLsizei h = static_cast<GLsizei>(integral);

	float aspect_ratio_ = static_cast<float>(w) / static_cast<float>(h);

	h = static_cast<GLsizei>(h*aspect_ratio_);

	glViewport(0, 0, w, h);

	this->viewMatrix_ = Eigen::Affine3f::Identity();
	this->viewMatrix_.translate(Eigen::Vector3f(0.0f, 0.0f, -4.0f));
	this->viewMatrix_.rotate(Eigen::Quaternion<float>::Identity());
	this->viewMatrix_.rotate(_orientation);
	

	shader_axes->bind();

	shader_axes->setUniform("ModelMatrix", this->viewMatrix_.Identity());
	shader_axes->setUniform("ViewMatrix", this->viewMatrix_);
	shader_axes->setUniform("ProjectionMatrix", this->projetionMatrix);

		glBindVertexArray(vertex_array_coneaxes);

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertex_buffer_faces_indices);
			glDrawElements(GL_TRIANGLES, indices_size_, GL_UNSIGNED_INT, 0);

		glBindVertexArray(0);

	shader_axes->unbind();

	w = static_cast<GLsizei>(_viewport[2]);
	h = static_cast<GLsizei>(_viewport[3]);
	
	glViewport(0, 0, w, h);

}

void CoordinateAxes::resetBuffers()
{
	/// Delete Shaders
	if (shader_axes)
	{
		shader_axes->deleteShaders();
		shader_axes = nullptr;
	}

	this->releaseOpenGLResources();
}

void CoordinateAxes::requestOpenGLResources()
{
	this->releaseOpenGLResources();

	// Cuboid
	glGenVertexArrays(1, &vertex_array_coneaxes);		// Vertex Array
		glGenBuffers(1, &vertex_buffer_faces_indices);  // Face Indices
		glGenBuffers(1, &vertex_buffer_vertices);		// Vertices
		glGenBuffers(1, &vertex_buffer_normals);		// Normals
		glGenBuffers(1, &vertex_buffer_colors);			// Colors

}

void CoordinateAxes::releaseOpenGLResources()
{
	if (vertex_array_coneaxes)
	{
		glDeleteVertexArrays(1, &vertex_array_coneaxes);

		/// face indices
		if (vertex_buffer_faces_indices)
		{
			glDeleteBuffers(1, &vertex_buffer_faces_indices);
		}
		/// vertices
		if (vertex_buffer_vertices)
		{
			glDeleteBuffers(1, &vertex_buffer_vertices);
		}
		/// normals
		if (vertex_buffer_normals)
		{
			glDeleteBuffers(1, &vertex_buffer_normals);
		}
		/// colors
		if (vertex_buffer_colors)
		{
			glDeleteBuffers(1, &vertex_buffer_colors);
		}
	}

	this->init();
}

