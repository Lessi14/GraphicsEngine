#include "Room.h"
#include "Furniture.h"
#include <ostream>
#include <iostream>
#include <glm.hpp>

glm::vec3 Room::default_furniture_location = glm::vec3(0.0f, 0.01f, 0.0f);

Room::Room(glm::vec2&  dimensions, map<int, Furniture*>& room_objects) : room_dimensions(dimensions),objects(room_objects)
{
}


///Takes in the dimensions of the rooms from the user.
void Room::set_room_size()
{
	while (room_dimensions.x < 4 || room_dimensions.x > 30) {
		std::cout << "Enter room width(x): " << std::endl;
		std::cin >> room_dimensions.x;
		if (room_dimensions.x < 4) {
			std::cout << "Minimum accepted value is 4.0" << std::endl;
		}
		if (room_dimensions.x > 30) {
			std::cout << "Maximum accepted value is 30.0" << std::endl;
		}
	}
	while (room_dimensions.y < 4 || room_dimensions.y > 30) {
		std::cout << "Enter room length(z): " << std::endl;
		std::cin >> room_dimensions.y;
		if (room_dimensions.y < 4) {
			std::cout << "Minimum accepted value is 4.0" << std::endl;
		}
		if (room_dimensions.y > 30) {
			std::cout << "Maximum accepted value is 30.0" << std::endl;
		}
	}
}

///Adds a furniture object to the Object vector and returns the id.
int Room::add_furniture(const char * type, glm::vec3 const& position)
{
	Furniture *tempObject = new Furniture(0, type);
	tempObject->loadObjToMap(objects);
	objects[tempObject->id] = tempObject;

	glGenVertexArrays(1, &objects[tempObject->id]->VAO);

	glGenBuffers(1, &objects[tempObject->id]->vertices_VBO);
	glGenBuffers(1, &objects[tempObject->id]->normals_VBO);
	glGenBuffers(1, &objects[tempObject->id]->uvs_VBO);


	setIndividualBuffers(objects[tempObject->id]->VAO, objects[tempObject->id]->vertices_VBO, objects[tempObject->id]->normals_VBO, objects[tempObject->id]->uvs_VBO, tempObject->id);
	glBindVertexArray(0);

	objects[tempObject->id]->translate(objects, position);

	return tempObject->id;
}

///Extracted the method which creates the vbos.
void Room::setIndividualBuffers(GLuint localVAO, GLuint verticesVBO, GLuint normalsVBO, GLuint uvsVBO, int id)
{
	glGenBuffers(1, &verticesVBO);
	glGenBuffers(1, &normalsVBO);
	glGenBuffers(1, &uvsVBO);

	glBindVertexArray(localVAO);

	glBindBuffer(GL_ARRAY_BUFFER, verticesVBO);
	glBufferData(GL_ARRAY_BUFFER, objects[id]->vertices.size() * sizeof(glm::vec3), &objects[id]->vertices.front(), GL_DYNAMIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, normalsVBO);
	glBufferData(GL_ARRAY_BUFFER, objects[id]->normals.size() * sizeof(glm::vec3), &objects[id]->normals.front(), GL_DYNAMIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, uvsVBO);
	glBufferData(GL_ARRAY_BUFFER, objects[id]->uvs.size() * sizeof(glm::vec3), &objects[id]->uvs.front(), GL_DYNAMIC_DRAW);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
}


///Set the texture of the furniture and places it in the room.
void Room::set_furniture(const int texture_number, const char* furniture_name)
{
	int furniture = add_furniture(furniture_name, default_furniture_location);
	glm::vec3 randomLocation = Furniture::randomLocationGenerator(furniture, objects, room_dimensions);

	if (randomLocation != vec3(-1000, -1000, 1000)) {
		objects[furniture]->texture_number = texture_number;
		objects[furniture]->translate(objects, randomLocation);
	}
	else {
		cout << "Cannot find a free spot to spawn a bed in the room" << endl;
		objects.erase(furniture);
	}
}


Room::~Room()
{
}
