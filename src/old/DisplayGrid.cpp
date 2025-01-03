#include "DisplayGrid.h"

DisplayGrid::DisplayGrid(double x_pos, double y_pos, double x_spacing, double y_spacing,
	double width, double height) : position(x_pos,y_pos) {

	this->x_spacing = x_spacing;
	this->y_spacing = y_spacing;

	this->width = width;
	this->height = height;

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &IBO);
}

void DisplayGrid::update() {
	uint8_t x_points = width / x_spacing;
	uint8_t y_points = height / y_spacing;

	uint32_t index = 0;
	for (float x = 0.0; x <= width; x += x_spacing) {
		vertices.push_back(glm::vec2(position.x + x, position.y));
		vertices.push_back(glm::vec2(position.x + x, position.y+height));
		indices.push_back(glm::uvec2(index, index++));
	}
	for (float y = 0.0; y <= height; y += y_spacing) {
		vertices.push_back(glm::vec2(position.x, position.y + y));
		vertices.push_back(glm::vec2(position.x + width, position.y+y));
		indices.push_back(glm::uvec2(index, index++));
	}

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec2), glm::value_ptr(vertices[0]), GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);

	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(glm::uvec2), glm::value_ptr(indices[0]), GL_STATIC_DRAW);

}

void DisplayGrid::render() {

	update();

	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	//glBindVertexArray(VAO);

	//glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);


	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	//glBindBuffer(GL_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	//glBindVertexArray(0);

	glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
	glDrawArrays(GL_LINES, 0, vertices.size());


}

DisplayGrid::~DisplayGrid() {
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &IBO);
}
