#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <math.h>

#include "Shaders/LoadShaders.h"
GLuint h_ShaderProgram; // handle to shader program
GLint loc_ModelViewProjectionMatrix, loc_primitive_color; // indices of uniform variables

// include glm/*.hpp only if necessary
//#include <glm/glm.hpp> 
#include <glm/gtc/matrix_transform.hpp> //translate, rotate, scale, ortho, etc.
glm::mat4 ModelViewProjectionMatrix;
glm::mat4 ViewMatrix, ProjectionMatrix, ViewProjectionMatrix;

#define TO_RADIAN 0.01745329252f  
#define TO_DEGREE 57.295779513f
#define BUFFER_OFFSET(offset) ((GLvoid *) (offset))

#define LOC_VERTEX 0

int win_width = 0, win_height = 0; 
float centerx = 0.0f, centery = 0.0f, rotate_angle = 0.0f;

// Suyeong added variables start here.
float all_clock=0, boomball_clock=0, airplane_clock=0, shirt_clock=0,house_clock=0,car1_clock=0,cocktail_clock=0,car2_clock=0;
// Each object's size variable.
float boomball_size=1, airplane_size=1, shirt_size = 1, house_size = 1, car1_size = 1, cocktail_size = 1, car2_size = 1;
// Each object's coordinate change flag.
int boomball_flag=0,boomball_initialflag=0, boomball_x_flag=0, boomball_y_flag=0, airplane_flag=0, shirt_flag = 0, house_flag = 0, car1_flag = 0, car1_flag_x=0, cocktail_flag = 0, car2_flag = 0, car2_flag_x = 0;;
// Each object's coordinate informaion variables.
float boomball_x=0, boomball_y=0, airplane_x=0,airplane_y=0,shirt_x=0, shirt_y=0, house_x=0, house_y=0, car1_x=0, car1_y=0, cocktail_x=0, cocktail_y=0, car2_x=0, car2_y=0;
//
// Suyeong added variables end here.

GLfloat axes[4][2];
GLfloat axes_color[3] = { 0.0f, 0.0f, 0.0f };
GLuint VBO_axes, VAO_axes;

void prepare_axes(void) { // Draw axes in their MC.
	axes[0][0] = -win_width / 2.5f; axes[0][1] = 0.0f;
	axes[1][0] = win_width / 2.5f; axes[1][1] = 0.0f;
	axes[2][0] = 0.0f; axes[2][1] = -win_height / 2.5f;
	axes[3][0] = 0.0f; axes[3][1] = win_height / 2.5f;

	// Initialize vertex buffer object.
	glGenBuffers(1, &VBO_axes);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_axes);
	glBufferData(GL_ARRAY_BUFFER, sizeof(axes), axes, GL_STATIC_DRAW);

	// Initialize vertex array object.
	glGenVertexArrays(1, &VAO_axes);
	glBindVertexArray(VAO_axes);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_axes);
	glVertexAttribPointer(LOC_VERTEX, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void update_axes(void) {
	axes[0][0] = -win_width / 2.25f; axes[1][0] = win_width / 2.25f; 
	axes[2][1] = -win_height / 2.25f;
	axes[3][1] = win_height / 2.25f;

	glBindBuffer(GL_ARRAY_BUFFER, VBO_axes);
	glBufferData(GL_ARRAY_BUFFER, sizeof(axes), axes, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void draw_axes(void) {
	glUniform3fv(loc_primitive_color, 1, axes_color);
	glBindVertexArray(VAO_axes);
	glDrawArrays(GL_LINES, 0, 4);
	glBindVertexArray(0);
}

GLfloat line[2][2];
GLfloat line_color[3] = { 1.0f, 0.0f, 0.0f };
GLuint VBO_line, VAO_line;

void prepare_line(void) { 	// y = x - win_height/4
	line[0][0] = (1.0f / 4.0f - 1.0f / 2.5f)*win_height; 
	line[0][1] = (1.0f / 4.0f - 1.0f / 2.5f)*win_height - win_height / 4.0f;
	line[1][0] = win_width / 2.5f; 
	line[1][1] = win_width / 2.5f - win_height / 4.0f;

	// Initialize vertex buffer object.
	glGenBuffers(1, &VBO_line);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_line);
	glBufferData(GL_ARRAY_BUFFER, sizeof(line), line, GL_STATIC_DRAW);

	// Initialize vertex array object.
	glGenVertexArrays(1, &VAO_line);
	glBindVertexArray(VAO_line);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_line);
	glVertexAttribPointer(LOC_VERTEX, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void update_line(void) { 	// y = x - win_height/4
	line[0][0] = (1.0f / 4.0f - 1.0f / 2.5f)*win_height; 
	line[0][1] = (1.0f / 4.0f - 1.0f / 2.5f)*win_height - win_height / 4.0f;
	line[1][0] = win_width / 2.5f; 
	line[1][1] = win_width / 2.5f - win_height / 4.0f;

	glBindBuffer(GL_ARRAY_BUFFER, VBO_line);
	glBufferData(GL_ARRAY_BUFFER, sizeof(line), line, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void draw_line(void) { // Draw line in its MC.
	// y = x - win_height/4
	glUniform3fv(loc_primitive_color, 1, line_color);
	glBindVertexArray(VAO_line);
	glDrawArrays(GL_LINES, 0, 2);
	glBindVertexArray(0);
}

#define AIRPLANE_BIG_WING 0
#define AIRPLANE_SMALL_WING 1
#define AIRPLANE_BODY 2
#define AIRPLANE_BACK 3
#define AIRPLANE_SIDEWINDER1 4
#define AIRPLANE_SIDEWINDER2 5
#define AIRPLANE_CENTER 6
GLfloat big_wing[6][2] = { { 0.0, 0.0 }, { -20.0, 15.0 }, { -20.0, 20.0 }, { 0.0, 23.0 }, { 20.0, 20.0 }, { 20.0, 15.0 } };
GLfloat small_wing[6][2] = { { 0.0, -18.0 }, { -11.0, -12.0 }, { -12.0, -7.0 }, { 0.0, -10.0 }, { 12.0, -7.0 }, { 11.0, -12.0 } };
GLfloat body[5][2] = { { 0.0, -25.0 }, { -6.0, 0.0 }, { -6.0, 22.0 }, { 6.0, 22.0 }, { 6.0, 0.0 } };
GLfloat back[5][2] = { { 0.0, 25.0 }, { -7.0, 24.0 }, { -7.0, 21.0 }, { 7.0, 21.0 }, { 7.0, 24.0 } };
GLfloat sidewinder1[5][2] = { { -20.0, 10.0 }, { -18.0, 3.0 }, { -16.0, 10.0 }, { -18.0, 20.0 }, { -20.0, 20.0 } };
GLfloat sidewinder2[5][2] = { { 20.0, 10.0 }, { 18.0, 3.0 }, { 16.0, 10.0 }, { 18.0, 20.0 }, { 20.0, 20.0 } };
GLfloat center[1][2] = { { 0.0, 0.0 } };
GLfloat airplane_color[7][3] = {
	{ 150 / 255.0f, 129 / 255.0f, 183 / 255.0f },  // big_wing
	{ 245 / 255.0f, 211 / 255.0f,   0 / 255.0f },  // small_wing
	{ 111 / 255.0f,  85 / 255.0f, 157 / 255.0f },  // body
	{ 150 / 255.0f, 129 / 255.0f, 183 / 255.0f },  // back
	{ 245 / 255.0f, 211 / 255.0f,   0 / 255.0f },  // sidewinder1
	{ 245 / 255.0f, 211 / 255.0f,   0 / 255.0f },  // sidewinder2
	{ 255 / 255.0f,   0 / 255.0f,   0 / 255.0f }   // center
};

GLuint VBO_airplane, VAO_airplane;

//int airplane_clock = 0;
float airplane_s_factor = 1.0f;

void prepare_airplane() {
	GLsizeiptr buffer_size = sizeof(big_wing)+sizeof(small_wing)+sizeof(body)+sizeof(back)
		+sizeof(sidewinder1)+sizeof(sidewinder2)+sizeof(center);

	// Initialize vertex buffer object.
	glGenBuffers(1, &VBO_airplane);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_airplane);
	glBufferData(GL_ARRAY_BUFFER, buffer_size, NULL, GL_STATIC_DRAW); // allocate buffer object memory

	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(big_wing), big_wing);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(big_wing), sizeof(small_wing), small_wing);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(big_wing)+sizeof(small_wing), sizeof(body), body);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(big_wing)+sizeof(small_wing)+sizeof(body), sizeof(back), back);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(big_wing)+sizeof(small_wing)+sizeof(body)+sizeof(back),
		sizeof(sidewinder1), sidewinder1);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(big_wing)+sizeof(small_wing)+sizeof(body)+sizeof(back)
		+sizeof(sidewinder1), sizeof(sidewinder2), sidewinder2);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(big_wing)+sizeof(small_wing)+sizeof(body)+sizeof(back)
		+sizeof(sidewinder1)+sizeof(sidewinder2), sizeof(center), center);

	// Initialize vertex array object.
	glGenVertexArrays(1, &VAO_airplane);
	glBindVertexArray(VAO_airplane);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_airplane);
	glVertexAttribPointer(LOC_VERTEX, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void draw_airplane() { // Draw airplane in its MC.
	glBindVertexArray(VAO_airplane);

	glUniform3fv(loc_primitive_color, 1, airplane_color[AIRPLANE_BIG_WING]);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 6);

	glUniform3fv(loc_primitive_color, 1, airplane_color[AIRPLANE_SMALL_WING]);
	glDrawArrays(GL_TRIANGLE_FAN, 6, 6);

	glUniform3fv(loc_primitive_color, 1, airplane_color[AIRPLANE_BODY]);
	glDrawArrays(GL_TRIANGLE_FAN, 12, 5);

	glUniform3fv(loc_primitive_color, 1, airplane_color[AIRPLANE_BACK]);
	glDrawArrays(GL_TRIANGLE_FAN, 17, 5);

	glUniform3fv(loc_primitive_color, 1, airplane_color[AIRPLANE_SIDEWINDER1]);
	glDrawArrays(GL_TRIANGLE_FAN, 22, 5);

	glUniform3fv(loc_primitive_color, 1, airplane_color[AIRPLANE_SIDEWINDER2]);
	glDrawArrays(GL_TRIANGLE_FAN, 27, 5);

	glUniform3fv(loc_primitive_color, 1, airplane_color[AIRPLANE_CENTER]);
	glPointSize(5.0);
	glDrawArrays(GL_POINTS, 32, 1);
	glPointSize(1.0);
	glBindVertexArray(0);
}

//shirt
#define SHIRT_LEFT_BODY 0
#define SHIRT_RIGHT_BODY 1
#define SHIRT_LEFT_COLLAR 2
#define SHIRT_RIGHT_COLLAR 3
#define SHIRT_FRONT_POCKET 4
#define SHIRT_BUTTON1 5
#define SHIRT_BUTTON2 6
#define SHIRT_BUTTON3 7
#define SHIRT_BUTTON4 8
GLfloat left_body[6][2] = { { 0.0, -9.0 }, { -8.0, -9.0 }, { -11.0, 8.0 }, { -6.0, 10.0 }, { -3.0, 7.0 }, { 0.0, 9.0 } };
GLfloat right_body[6][2] = { { 0.0, -9.0 }, { 0.0, 9.0 }, { 3.0, 7.0 }, { 6.0, 10.0 }, { 11.0, 8.0 }, { 8.0, -9.0 } };
GLfloat left_collar[4][2] = { { 0.0, 9.0 }, { -3.0, 7.0 }, { -6.0, 10.0 }, { -4.0, 11.0 } };
GLfloat right_collar[4][2] = { { 0.0, 9.0 }, { 4.0, 11.0 }, { 6.0, 10.0 }, { 3.0, 7.0 } };
GLfloat front_pocket[6][2] = { { 5.0, 0.0 }, { 4.0, 1.0 }, { 4.0, 3.0 }, { 7.0, 3.0 }, { 7.0, 1.0 }, { 6.0, 0.0 } };
GLfloat button1[3][2] = { { -1.0, 6.0 }, { 1.0, 6.0 }, { 0.0, 5.0 } };
GLfloat button2[3][2] = { { -1.0, 3.0 }, { 1.0, 3.0 }, { 0.0, 2.0 } };
GLfloat button3[3][2] = { { -1.0, 0.0 }, { 1.0, 0.0 }, { 0.0, -1.0 } };
GLfloat button4[3][2] = { { -1.0, -3.0 }, { 1.0, -3.0 }, { 0.0, -4.0 } };

GLfloat shirt_color[9][3] = {
	{ 255 / 255.0f, 255 / 255.0f, 255 / 255.0f },
	{ 255 / 255.0f, 255 / 255.0f, 255 / 255.0f },
	{ 206 / 255.0f, 173 / 255.0f, 184 / 255.0f },
	{ 206 / 255.0f, 173 / 255.0f, 184 / 255.0f },
	{ 206 / 255.0f, 173 / 255.0f, 184 / 255.0f },
	{ 206 / 255.0f, 173 / 255.0f, 184 / 255.0f },
	{ 206 / 255.0f, 173 / 255.0f, 184 / 255.0f },
	{ 206 / 255.0f, 173 / 255.0f, 184 / 255.0f },
	{ 206 / 255.0f, 173 / 255.0f, 184 / 255.0f }
};

GLuint VBO_shirt, VAO_shirt;
void prepare_shirt() {
	GLsizeiptr buffer_size = sizeof(left_body)+sizeof(right_body)+sizeof(left_collar)+sizeof(right_collar)
		+sizeof(front_pocket)+sizeof(button1)+sizeof(button2)+sizeof(button3)+sizeof(button4);

	// Initialize vertex buffer object.
	glGenBuffers(1, &VBO_shirt);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_shirt);
	glBufferData(GL_ARRAY_BUFFER, buffer_size, NULL, GL_STATIC_DRAW); // allocate buffer object memory

	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(left_body), left_body);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(left_body), sizeof(right_body), right_body);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(left_body)+sizeof(right_body), sizeof(left_collar), left_collar);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(left_body)+sizeof(right_body)+sizeof(left_collar), sizeof(right_collar), right_collar);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(left_body)+sizeof(right_body)+sizeof(left_collar)+sizeof(right_collar),
		sizeof(front_pocket), front_pocket);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(left_body)+sizeof(right_body)+sizeof(left_collar)+sizeof(right_collar)
		+sizeof(front_pocket), sizeof(button1), button1);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(left_body)+sizeof(right_body)+sizeof(left_collar)+sizeof(right_collar)
		+sizeof(front_pocket)+sizeof(button1), sizeof(button2), button2);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(left_body)+sizeof(right_body)+sizeof(left_collar)+sizeof(right_collar)
		+sizeof(front_pocket)+sizeof(button1)+sizeof(button2), sizeof(button3), button3);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(left_body)+sizeof(right_body)+sizeof(left_collar)+sizeof(right_collar)
		+sizeof(front_pocket)+sizeof(button1)+sizeof(button2)+sizeof(button3), sizeof(button4), button4);

	// Initialize vertex array object.
	glGenVertexArrays(1, &VAO_shirt);
	glBindVertexArray(VAO_shirt);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_shirt);
	glVertexAttribPointer(LOC_VERTEX, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void draw_shirt() {
	glBindVertexArray(VAO_shirt);

	glUniform3fv(loc_primitive_color, 1, shirt_color[SHIRT_LEFT_BODY]);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 6);

	glUniform3fv(loc_primitive_color, 1, shirt_color[SHIRT_RIGHT_BODY]);
	glDrawArrays(GL_TRIANGLE_FAN, 6, 6);

	glUniform3fv(loc_primitive_color, 1, shirt_color[SHIRT_LEFT_COLLAR]);
	glDrawArrays(GL_TRIANGLE_FAN, 12, 4);

	glUniform3fv(loc_primitive_color, 1, shirt_color[SHIRT_RIGHT_COLLAR]);
	glDrawArrays(GL_TRIANGLE_FAN, 16, 4);

	glUniform3fv(loc_primitive_color, 1, shirt_color[SHIRT_FRONT_POCKET]);
	glDrawArrays(GL_TRIANGLE_FAN, 20, 6);

	glUniform3fv(loc_primitive_color, 1, shirt_color[SHIRT_BUTTON1]);
	glDrawArrays(GL_TRIANGLE_FAN, 26, 3);

	glUniform3fv(loc_primitive_color, 1, shirt_color[SHIRT_BUTTON2]);
	glDrawArrays(GL_TRIANGLE_FAN, 29, 3);

	glUniform3fv(loc_primitive_color, 1, shirt_color[SHIRT_BUTTON3]);
	glDrawArrays(GL_TRIANGLE_FAN, 32, 3);

	glUniform3fv(loc_primitive_color, 1, shirt_color[SHIRT_BUTTON4]);
	glDrawArrays(GL_TRIANGLE_FAN, 35, 3);
	glBindVertexArray(0);
}

//house
#define HOUSE_ROOF 0
#define HOUSE_BODY 1
#define HOUSE_CHIMNEY 2
#define HOUSE_DOOR 3
#define HOUSE_WINDOW 4

GLfloat roof[3][2] = { { -12.0, 0.0 }, { 0.0, 12.0 }, { 12.0, 0.0 } };
GLfloat house_body[4][2] = { { -12.0, -14.0 }, { -12.0, 0.0 }, { 12.0, 0.0 }, { 12.0, -14.0 } };
GLfloat chimney[4][2] = { { 6.0, 6.0 }, { 6.0, 14.0 }, { 10.0, 14.0 }, { 10.0, 2.0 } };
GLfloat door[4][2] = { { -8.0, -14.0 }, { -8.0, -8.0 }, { -4.0, -8.0 }, { -4.0, -14.0 } };
GLfloat window[4][2] = { { 4.0, -6.0 }, { 4.0, -2.0 }, { 8.0, -2.0 }, { 8.0, -6.0 } };

GLfloat house_color[5][3] = {
	{ 200 / 255.0f, 39 / 255.0f, 42 / 255.0f },
	{ 235 / 255.0f, 225 / 255.0f, 196 / 255.0f },
	{ 255 / 255.0f, 0 / 255.0f, 0 / 255.0f },
	{ 233 / 255.0f, 113 / 255.0f, 23 / 255.0f },
	{ 44 / 255.0f, 180 / 255.0f, 49 / 255.0f }
};

GLuint VBO_house, VAO_house;
void prepare_house() {
	GLsizeiptr buffer_size = sizeof(roof)+sizeof(house_body)+sizeof(chimney)+sizeof(door)
		+sizeof(window);

	// Initialize vertex buffer object.
	glGenBuffers(1, &VBO_house);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_house);
	glBufferData(GL_ARRAY_BUFFER, buffer_size, NULL, GL_STATIC_DRAW); // allocate buffer object memory

	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(roof), roof);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(roof), sizeof(house_body), house_body);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(roof)+sizeof(house_body), sizeof(chimney), chimney);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(roof)+sizeof(house_body)+sizeof(chimney), sizeof(door), door);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(roof)+sizeof(house_body)+sizeof(chimney)+sizeof(door),
		sizeof(window), window);

	// Initialize vertex array object.
	glGenVertexArrays(1, &VAO_house);
	glBindVertexArray(VAO_house);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_house);
	glVertexAttribPointer(LOC_VERTEX, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void draw_house() {
	glBindVertexArray(VAO_house);

	glUniform3fv(loc_primitive_color, 1, house_color[HOUSE_ROOF]);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 3);

	glUniform3fv(loc_primitive_color, 1, house_color[HOUSE_BODY]);
	glDrawArrays(GL_TRIANGLE_FAN, 3, 4);

	glUniform3fv(loc_primitive_color, 1, house_color[HOUSE_CHIMNEY]);
	glDrawArrays(GL_TRIANGLE_FAN, 7, 4);

	glUniform3fv(loc_primitive_color, 1, house_color[HOUSE_DOOR]);
	glDrawArrays(GL_TRIANGLE_FAN, 11, 4);

	glUniform3fv(loc_primitive_color, 1, house_color[HOUSE_WINDOW]);
	glDrawArrays(GL_TRIANGLE_FAN, 15, 4);

	glBindVertexArray(0);
}

//car
#define CAR_BODY 0
#define CAR_FRAME 1
#define CAR_WINDOW 2
#define CAR_LEFT_LIGHT 3
#define CAR_RIGHT_LIGHT 4
#define CAR_LEFT_WHEEL 5
#define CAR_RIGHT_WHEEL 6

GLfloat car_body[4][2] = { { -16.0, -8.0 }, { -16.0, 0.0 }, { 16.0, 0.0 }, { 16.0, -8.0 } };
GLfloat car_frame[4][2] = { { -10.0, 0.0 }, { -10.0, 10.0 }, { 10.0, 10.0 }, { 10.0, 0.0 } };
GLfloat car_window[4][2] = { { -8.0, 0.0 }, { -8.0, 8.0 }, { 8.0, 8.0 }, { 8.0, 0.0 } };
GLfloat car_left_light[4][2] = { { -9.0, -6.0 }, { -10.0, -5.0 }, { -9.0, -4.0 }, { -8.0, -5.0 } };
GLfloat car_right_light[4][2] = { { 9.0, -6.0 }, { 8.0, -5.0 }, { 9.0, -4.0 }, { 10.0, -5.0 } };
GLfloat car_left_wheel[4][2] = { { -10.0, -12.0 }, { -10.0, -8.0 }, { -6.0, -8.0 }, { -6.0, -12.0 } };
GLfloat car_right_wheel[4][2] = { { 6.0, -12.0 }, { 6.0, -8.0 }, { 10.0, -8.0 }, { 10.0, -12.0 } };

GLfloat car_color[7][3] = {
	{ 0 / 255.0f, 149 / 255.0f, 159 / 255.0f },
	{ 0 / 255.0f, 149 / 255.0f, 159 / 255.0f },
	{ 216 / 255.0f, 208 / 255.0f, 174 / 255.0f },
	{ 249 / 255.0f, 244 / 255.0f, 0 / 255.0f },
	{ 249 / 255.0f, 244 / 255.0f, 0 / 255.0f },
	{ 21 / 255.0f, 30 / 255.0f, 26 / 255.0f },
	{ 21 / 255.0f, 30 / 255.0f, 26 / 255.0f }
};

GLuint VBO_car, VAO_car;
void prepare_car() {
	GLsizeiptr buffer_size = sizeof(car_body)+sizeof(car_frame)+sizeof(car_window)+sizeof(car_left_light)
		+sizeof(car_right_light)+sizeof(car_left_wheel)+sizeof(car_right_wheel);

	// Initialize vertex buffer object.
	glGenBuffers(1, &VBO_car);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_car);
	glBufferData(GL_ARRAY_BUFFER, buffer_size, NULL, GL_STATIC_DRAW); // allocate buffer object memory

	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(car_body), car_body);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(car_body), sizeof(car_frame), car_frame);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(car_body)+sizeof(car_frame), sizeof(car_window), car_window);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(car_body)+sizeof(car_frame)+sizeof(car_window), sizeof(car_left_light), car_left_light);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(car_body)+sizeof(car_frame)+sizeof(car_window)+sizeof(car_left_light),
		sizeof(car_right_light), car_right_light);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(car_body)+sizeof(car_frame)+sizeof(car_window)+sizeof(car_left_light)
		+sizeof(car_right_light), sizeof(car_left_wheel), car_left_wheel);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(car_body)+sizeof(car_frame)+sizeof(car_window)+sizeof(car_left_light)
		+sizeof(car_right_light)+sizeof(car_left_wheel), sizeof(car_right_wheel), car_right_wheel);

	// Initialize vertex array object.
	glGenVertexArrays(1, &VAO_car);
	glBindVertexArray(VAO_car);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_car);
	glVertexAttribPointer(LOC_VERTEX, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void draw_car() {
	glBindVertexArray(VAO_car);

	glUniform3fv(loc_primitive_color, 1, car_color[CAR_BODY]);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

	glUniform3fv(loc_primitive_color, 1, car_color[CAR_FRAME]);
	glDrawArrays(GL_TRIANGLE_FAN, 4, 4);

	glUniform3fv(loc_primitive_color, 1, car_color[CAR_WINDOW]);
	glDrawArrays(GL_TRIANGLE_FAN, 8, 4);

	glUniform3fv(loc_primitive_color, 1, car_color[CAR_LEFT_LIGHT]);
	glDrawArrays(GL_TRIANGLE_FAN, 12, 4);

	glUniform3fv(loc_primitive_color, 1, car_color[CAR_RIGHT_LIGHT]);
	glDrawArrays(GL_TRIANGLE_FAN, 16, 4);

	glUniform3fv(loc_primitive_color, 1, car_color[CAR_LEFT_WHEEL]);
	glDrawArrays(GL_TRIANGLE_FAN, 20, 4);

	glUniform3fv(loc_primitive_color, 1, car_color[CAR_RIGHT_WHEEL]);
	glDrawArrays(GL_TRIANGLE_FAN, 24, 4);

	glBindVertexArray(0);
}

//draw cocktail
#define COCKTAIL_NECK 0
#define COCKTAIL_LIQUID 1
#define COCKTAIL_REMAIN 2
#define COCKTAIL_STRAW 3
#define COCKTAIL_DECO 4

GLfloat neck[6][2] = { { -6.0, -12.0 }, { -6.0, -11.0 }, { -1.0, 0.0 }, { 1.0, 0.0 }, { 6.0, -11.0 }, { 6.0, -12.0 } };
GLfloat liquid[6][2] = { { -1.0, 0.0 }, { -9.0, 4.0 }, { -12.0, 7.0 }, { 12.0, 7.0 }, { 9.0, 4.0 }, { 1.0, 0.0 } };
GLfloat remain[4][2] = { { -12.0, 7.0 }, { -12.0, 10.0 }, { 12.0, 10.0 }, { 12.0, 7.0 } };
GLfloat straw[4][2] = { { 7.0, 7.0 }, { 12.0, 12.0 }, { 14.0, 12.0 }, { 9.0, 7.0 } };
GLfloat deco[8][2] = { { 12.0, 12.0 }, { 10.0, 14.0 }, { 10.0, 16.0 }, { 12.0, 18.0 }, { 14.0, 18.0 }, { 16.0, 16.0 }, { 16.0, 14.0 }, { 14.0, 12.0 } };

GLfloat cocktail_color[5][3] = {
	{ 235 / 255.0f, 225 / 255.0f, 196 / 255.0f },
	{ 0 / 255.0f, 63 / 255.0f, 122 / 255.0f },
	{ 235 / 255.0f, 225 / 255.0f, 196 / 255.0f },
	{ 191 / 255.0f, 255 / 255.0f, 0 / 255.0f },
	{ 218 / 255.0f, 165 / 255.0f, 32 / 255.0f }
};

GLuint VBO_cocktail, VAO_cocktail;
void prepare_cocktail() {
	GLsizeiptr buffer_size = sizeof(neck)+sizeof(liquid)+sizeof(remain)+sizeof(straw)
		+sizeof(deco);

	// Initialize vertex buffer object.
	glGenBuffers(1, &VBO_cocktail);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_cocktail);
	glBufferData(GL_ARRAY_BUFFER, buffer_size, NULL, GL_STATIC_DRAW); // allocate buffer object memory

	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(neck), neck);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(neck), sizeof(liquid), liquid);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(neck)+sizeof(liquid), sizeof(remain), remain);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(neck)+sizeof(liquid)+sizeof(remain), sizeof(straw), straw);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(neck)+sizeof(liquid)+sizeof(remain)+sizeof(straw),
		sizeof(deco), deco);

	// Initialize vertex array object.
	glGenVertexArrays(1, &VAO_cocktail);
	glBindVertexArray(VAO_cocktail);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_cocktail);
	glVertexAttribPointer(LOC_VERTEX, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void draw_cocktail() {
	glBindVertexArray(VAO_cocktail);

	glUniform3fv(loc_primitive_color, 1, cocktail_color[COCKTAIL_NECK]);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 6);

	glUniform3fv(loc_primitive_color, 1, cocktail_color[COCKTAIL_LIQUID]);
	glDrawArrays(GL_TRIANGLE_FAN, 6, 6);

	glUniform3fv(loc_primitive_color, 1, cocktail_color[COCKTAIL_REMAIN]);
	glDrawArrays(GL_TRIANGLE_FAN, 12, 4);

	glUniform3fv(loc_primitive_color, 1, cocktail_color[COCKTAIL_STRAW]);
	glDrawArrays(GL_TRIANGLE_FAN, 16, 4);

	glUniform3fv(loc_primitive_color, 1, cocktail_color[COCKTAIL_DECO]);
	glDrawArrays(GL_TRIANGLE_FAN, 20, 8);

	glBindVertexArray(0);
}

//draw car2
#define CAR2_BODY 0
#define CAR2_FRONT_WINDOW 1
#define CAR2_BACK_WINDOW 2
#define CAR2_FRONT_WHEEL 3
#define CAR2_BACK_WHEEL 4
#define CAR2_LIGHT1 5
#define CAR2_LIGHT2 6

GLfloat car2_body[8][2] = { { -18.0, -7.0 }, { -18.0, 0.0 }, { -13.0, 0.0 }, { -10.0, 8.0 }, { 10.0, 8.0 }, { 13.0, 0.0 }, { 18.0, 0.0 }, { 18.0, -7.0 } };
GLfloat car2_front_window[4][2] = { { -10.0, 0.0 }, { -8.0, 6.0 }, { -2.0, 6.0 }, { -2.0, 0.0 } };
GLfloat car2_back_window[4][2] = { { 0.0, 0.0 }, { 0.0, 6.0 }, { 8.0, 6.0 }, { 10.0, 0.0 } };
GLfloat car2_front_wheel[8][2] = { { -11.0, -11.0 }, { -13.0, -8.0 }, { -13.0, -7.0 }, { -11.0, -4.0 }, { -7.0, -4.0 }, { -5.0, -7.0 }, { -5.0, -8.0 }, { -7.0, -11.0 } };
GLfloat car2_back_wheel[8][2] = { { 7.0, -11.0 }, { 5.0, -8.0 }, { 5.0, -7.0 }, { 7.0, -4.0 }, { 11.0, -4.0 }, { 13.0, -7.0 }, { 13.0, -8.0 }, { 11.0, -11.0 } };
GLfloat car2_light1[3][2] = { { -18.0, -1.0 }, { -17.0, -2.0 }, { -18.0, -3.0 } };
GLfloat car2_light2[3][2] = { { -18.0, -4.0 }, { -17.0, -5.0 }, { -18.0, -6.0 } };

GLfloat car2_color[7][3] = {
	{ 100 / 255.0f, 141 / 255.0f, 159 / 255.0f },
	{ 235 / 255.0f, 219 / 255.0f, 208 / 255.0f },
	{ 235 / 255.0f, 219 / 255.0f, 208 / 255.0f },
	{ 0 / 255.0f, 0 / 255.0f, 0 / 255.0f },
	{ 0 / 255.0f, 0 / 255.0f, 0 / 255.0f },
	{ 249 / 255.0f, 244 / 255.0f, 0 / 255.0f },
	{ 249 / 255.0f, 244 / 255.0f, 0 / 255.0f }
};

GLuint VBO_car2, VAO_car2;
void prepare_car2() {
	GLsizeiptr buffer_size = sizeof(car2_body)+sizeof(car2_front_window)+sizeof(car2_back_window)+sizeof(car2_front_wheel)
		+sizeof(car2_back_wheel)+sizeof(car2_light1)+sizeof(car2_light2);

	// Initialize vertex buffer object.
	glGenBuffers(1, &VBO_car2);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_car2);
	glBufferData(GL_ARRAY_BUFFER, buffer_size, NULL, GL_STATIC_DRAW); // allocate buffer object memory

	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(car2_body), car2_body);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(car2_body), sizeof(car2_front_window), car2_front_window);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(car2_body)+sizeof(car2_front_window), sizeof(car2_back_window), car2_back_window);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(car2_body)+sizeof(car2_front_window)+sizeof(car2_back_window), sizeof(car2_front_wheel), car2_front_wheel);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(car2_body)+sizeof(car2_front_window)+sizeof(car2_back_window)+sizeof(car2_front_wheel),
		sizeof(car2_back_wheel), car2_back_wheel);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(car2_body)+sizeof(car2_front_window)+sizeof(car2_back_window)+sizeof(car2_front_wheel)
		+sizeof(car2_back_wheel), sizeof(car2_light1), car2_light1);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(car2_body)+sizeof(car2_front_window)+sizeof(car2_back_window)+sizeof(car2_front_wheel)
		+sizeof(car2_back_wheel)+sizeof(car2_light1), sizeof(car2_light2), car2_light2);

	// Initialize vertex array object.
	glGenVertexArrays(1, &VAO_car2);
	glBindVertexArray(VAO_car2);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_car2);
	glVertexAttribPointer(LOC_VERTEX, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void draw_car2() {
	glBindVertexArray(VAO_car2);

	glUniform3fv(loc_primitive_color, 1, car2_color[CAR2_BODY]);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 8);

	glUniform3fv(loc_primitive_color, 1, car2_color[CAR2_FRONT_WINDOW]);
	glDrawArrays(GL_TRIANGLE_FAN, 8, 4);

	glUniform3fv(loc_primitive_color, 1, car2_color[CAR2_BACK_WINDOW]);
	glDrawArrays(GL_TRIANGLE_FAN, 12, 4);

	glUniform3fv(loc_primitive_color, 1, car2_color[CAR2_FRONT_WHEEL]);
	glDrawArrays(GL_TRIANGLE_FAN, 16, 8);

	glUniform3fv(loc_primitive_color, 1, car2_color[CAR2_BACK_WHEEL]);
	glDrawArrays(GL_TRIANGLE_FAN, 24, 8);

	glUniform3fv(loc_primitive_color, 1, car2_color[CAR2_LIGHT1]);
	glDrawArrays(GL_TRIANGLE_FAN, 32, 3);

	glUniform3fv(loc_primitive_color, 1, car2_color[CAR2_LIGHT2]);
	glDrawArrays(GL_TRIANGLE_FAN, 35, 3);

	glBindVertexArray(0);
}

// draw boomball
#define BOOMBALL_UPBODY 0
#define BOOMBALL_DOWNBODY 1
#define BOOMBALL_LEFTEYE 2
#define BOOMBALL_RIGHTEYE 3
#define BOOMBALL_MOUTH 4
#define BOOMBALL_EFFECT1 5
#define BOOMBALL_EFFECT2 6
#define BOOMBALL_LEFTEYEBROW 7
#define BOOMBALL_RIGHTEYEBROW 8


GLfloat boomball_upbody[8][2] = { { -20.0, 0.0 }, { -15.0, 13.5 }, { -10.0, 17.5 }, { -2.5, 19.5 }, { 2.5, 19.5 }, { 10.0, 17.5}, { 15.0, 13.5}, { 20.0, 0.0 } };
GLfloat boomball_downbody[8][2] = { { -20.0, 0.0 }, { -15.0, -13.5 }, { -10.0, -17.5 }, { -2.5, -19.5 }, { 2.5, -19.5 }, { 10.0, -17.5}, { 15.0, -13.5 }, { 20.0, 0.0 } };
GLfloat boomball_lefteye[4][2] = { { -5.3, 5.0 }, { -4.7, 5.0 }, { -5.0, 8.0 }, { -5.0, 2.0 } };
GLfloat boomball_righteye[4][2] = { { 5.3, 5.0 }, { 4.7, 5.0 }, { 5.0 , 8.0 }, { 5.0, 2.0 } };
GLfloat boomball_mouth[3][2] = { { -15.0, -3.5 }, { -10.0, -11.5 }, { 15.0, -5.5 } };
GLfloat boomball_effect1[6][2] = { { 20.0, 20.0 }, { 24.0, 28.0 }, { 21.0, 21.0 }, { 26.0, 21.0 }, { 24.0, 12.0 }, { 23.0, 20.0 } };
GLfloat boomball_effect2[6][2] = { { -20.0, -20.0 },{ -24.0, -28.0 },{ -21.0, -21.0 },{ -26.0, -21.0 },{ -24.0, -12.0 },{ -23.0, -20.0 } };
GLfloat boomball_lefteyebrow[4][2] = { { -3.0, 8.0 }, { -2.0, 11.0 }, { -4.0, 10.0 }, { -10.0, 15.0 } };
GLfloat boomball_righteyebrow[3][2] = { { 3.0, 8.0 }, { 9.0, 15.0 }, { 10.0, 13.0 } };


GLfloat boomball_color[9][3] = {
	{ 255 / 255.0f, 255 / 255.0f, 255 / 255.0f },    // boomball_upbody
	{ 255 / 255.0f, 0 / 255.0f, 0 / 255.0f },        // boomball_downbody
	{ 0 / 255.0f, 0 / 255.0f, 0 / 255.0f },          // boomball_lefteye
	{ 0 / 255.0f, 0 / 255.0f, 0 / 255.0f },          // boomball_righteye
	{ 255 / 255.0f, 255 / 255.0f, 255 / 255.0f },    // boomball_mouth
	{ 255 / 255.0f, 255 / 255.0f, 0 / 255.0f },      // boomball_effect1
	{ 255 / 255.0f, 255 / 255.0f, 0 / 255.0f },      // boomball_effect2
	{ 0 / 255.0f, 0 / 255.0f, 0 / 255.0f },          // boomball_lefteyebrow
	{ 0 / 255.0f, 0 / 255.0f, 0 / 255.0f }           // boomball_righteyebrow
};

GLuint VAO_boomball, VBO_boomball;

void prepare_boomball() {
	GLsizeiptr buffer_size = sizeof(boomball_upbody) + sizeof(boomball_downbody) + sizeof(boomball_lefteye) +sizeof(boomball_righteye) + sizeof(boomball_mouth) + sizeof(boomball_effect1) + sizeof(boomball_effect2) + sizeof(boomball_lefteyebrow) + sizeof(boomball_righteyebrow);

	// Initialize vertex buffer object.
	glGenBuffers(1, &VBO_boomball);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_boomball);
	glBufferData(GL_ARRAY_BUFFER, buffer_size, NULL, GL_STATIC_DRAW); // allocate buffer object memory


	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(boomball_upbody), boomball_upbody);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(boomball_upbody), sizeof(boomball_downbody), boomball_downbody);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(boomball_upbody) + sizeof(boomball_downbody), sizeof(boomball_lefteye), boomball_lefteye);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(boomball_upbody) + sizeof(boomball_downbody) + sizeof(boomball_lefteye), sizeof(boomball_righteye), boomball_righteye);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(boomball_upbody) + sizeof(boomball_downbody) + sizeof(boomball_lefteye) + sizeof(boomball_righteye), sizeof(boomball_mouth), boomball_mouth);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(boomball_upbody) + sizeof(boomball_downbody) + sizeof(boomball_lefteye) + sizeof(boomball_righteye) + sizeof(boomball_mouth), sizeof(boomball_effect1), boomball_effect1);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(boomball_upbody) + sizeof(boomball_downbody) + sizeof(boomball_lefteye) + sizeof(boomball_righteye) + sizeof(boomball_mouth) + sizeof(boomball_effect1), sizeof(boomball_effect2), boomball_effect2);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(boomball_upbody) + sizeof(boomball_downbody) + sizeof(boomball_lefteye) + sizeof(boomball_righteye) + sizeof(boomball_mouth) + sizeof(boomball_effect1) + sizeof(boomball_effect2), sizeof(boomball_lefteyebrow), boomball_lefteyebrow);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(boomball_upbody) + sizeof(boomball_downbody) + sizeof(boomball_lefteye) + sizeof(boomball_righteye) + sizeof(boomball_mouth) + sizeof(boomball_effect1) + sizeof(boomball_effect2) + sizeof(boomball_lefteyebrow), sizeof(boomball_righteyebrow), boomball_righteyebrow);

	
	// Initialize vertex array object.
	glGenVertexArrays(1, &VAO_boomball);
	glBindVertexArray(VAO_boomball);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_boomball);
	glVertexAttribPointer(LOC_VERTEX, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void draw_boomball() {
	glBindVertexArray(VAO_boomball);

	glUniform3fv(loc_primitive_color, 1, boomball_color[BOOMBALL_UPBODY]);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 8);

	glUniform3fv(loc_primitive_color, 1, boomball_color[BOOMBALL_DOWNBODY]);
	glDrawArrays(GL_TRIANGLE_FAN, 8, 8);

	glUniform3fv(loc_primitive_color, 1, boomball_color[BOOMBALL_LEFTEYE]);
	glDrawArrays(GL_TRIANGLE_FAN, 16, 4);

	glUniform3fv(loc_primitive_color, 1, boomball_color[BOOMBALL_RIGHTEYE]);
	glDrawArrays(GL_TRIANGLE_FAN, 20, 4);

	glUniform3fv(loc_primitive_color, 1, boomball_color[BOOMBALL_MOUTH]);
	glDrawArrays(GL_TRIANGLE_FAN, 24, 3);

	glUniform3fv(loc_primitive_color, 1, boomball_color[BOOMBALL_EFFECT1]);
	glDrawArrays(GL_TRIANGLE_FAN, 27, 6);

	glUniform3fv(loc_primitive_color, 1, boomball_color[BOOMBALL_EFFECT2]);
	glDrawArrays(GL_TRIANGLE_FAN, 33, 6);

	glUniform3fv(loc_primitive_color, 1, boomball_color[BOOMBALL_LEFTEYEBROW]);
	glDrawArrays(GL_TRIANGLE_FAN, 39, 4);

	glUniform3fv(loc_primitive_color, 1, boomball_color[BOOMBALL_RIGHTEYEBROW]);
	glDrawArrays(GL_TRIANGLE_FAN, 43, 3);

	glBindVertexArray(0);
}
//

float count_var_airplane=0, count_var_shirt=0, count_var_car1=0, count_var_cocktail=0, count_var_car2=0, count_var_house=0;
float count_var_boomball = 0;

void display(void) {
	//int i;
	//float x, r, s, delx, delr, dels;
	glm::mat4 ModelMatrix;

	glClear(GL_COLOR_BUFFER_BIT);
	
	/* Background axes no need.
	ModelMatrix = glm::mat4(1.0f);
	ModelViewProjectionMatrix = ViewProjectionMatrix * ModelMatrix;
	glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
	draw_axes();
	*/
	if (all_clock < 1000)
	{
		// draw boomball start here.
		count_var_boomball += 5;
		if (boomball_initialflag == 0)
		{
			boomball_x = 0.0f + count_var_boomball;
			boomball_y = 0.0f;
			if (boomball_x < 500.0f)
				ModelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(boomball_x, boomball_y, 0.0f));
			else
			{
				boomball_initialflag = 1;
				count_var_boomball = 0;
			}
		}
		// if boomball_x_flag == 0 : move left // == 1 : move right
		// if boomball_y_flag == 0 : move up   // == 1 : move down
		else
		{
			if (boomball_x_flag == 0 && boomball_y_flag == 0)
			{
				boomball_x = 500.0f - count_var_boomball;
				boomball_y = 0.0f + count_var_boomball * 3 / 5;

				if (boomball_x > 0.0f && boomball_y < 300.0f)
					ModelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(boomball_x, boomball_y, 0.0f));
				else
				{
					boomball_y_flag = 1;
					count_var_boomball = 0;
				}
			}
			else if (boomball_x_flag == 0 && boomball_y_flag == 1)
			{
				boomball_x = 0.0f - count_var_boomball;
				boomball_y = 300.0f - count_var_boomball * 3 / 5;

				if (boomball_x > -500.0f && boomball_y > 0.0f)
					ModelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(boomball_x, boomball_y, 0.0f));
				else
				{
					boomball_x_flag = 1;
					count_var_boomball = 0;
				}
			}
			else if (boomball_x_flag == 1 && boomball_y_flag == 1)
			{
				boomball_x = -500.0f + count_var_boomball;
				boomball_y = 0.0f - count_var_boomball * 3 / 5;

				if (boomball_x < 0.0f && boomball_y > -300.0f)
					ModelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(boomball_x, boomball_y, 0.0f));
				else
				{
					boomball_y_flag = 0;
					count_var_boomball = 0;
				}
			}
			else if (boomball_x_flag == 1 && boomball_y_flag == 0)
			{
				boomball_x = 0.0f + count_var_boomball;
				boomball_y = -300.0f + count_var_boomball * 3 / 5;
				if (boomball_x < 500.0f && boomball_y < 0.0f)
					ModelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(boomball_x, boomball_y, 0.0f));
				else
				{
					boomball_x_flag = 0;
					count_var_boomball = 0;
				}
			}
		}
		ModelMatrix = glm::scale(ModelMatrix, glm::vec3(5.0f, 5.0f, 1.0f));
		ModelMatrix = glm::rotate(ModelMatrix, 2 * TO_RADIAN*boomball_clock, glm::vec3(0.0f, 0.0f, 1.0f));
		ModelViewProjectionMatrix = ViewProjectionMatrix * ModelMatrix;
		glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
		draw_boomball();
		// draw boomball end here.

		// draw airplane start here.
		count_var_airplane += 3;
		if (airplane_flag == 0)    // airplane is on the top half of circle movement.
		{
			// move leftward.
			airplane_x = -300.0f + count_var_airplane;
			airplane_y = sqrt(90000 - pow(airplane_x, 2));
			ModelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(airplane_x, airplane_y, 0.0f));
			ModelMatrix = glm::rotate(ModelMatrix, TO_RADIAN*airplane_clock, glm::vec3(0.0f, 0.0f, 1.0f));

			if (airplane_x >= 299.0f)
			{
				airplane_flag = 1;
				count_var_airplane = 0;
			}
		}
		else // airplane_flag == 1  // airplane is on the bottom half of circle movement.
		{
			// move right ward.
			airplane_x = 300.0f - count_var_airplane;
			airplane_y = -sqrt(90000 - pow(airplane_x, 2));
			ModelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(airplane_x, airplane_y, 0.0f));
			ModelMatrix = glm::rotate(ModelMatrix, TO_RADIAN*airplane_clock, glm::vec3(0.0f, 0.0f, 1.0f));

			if (airplane_x <= -299.0f)
			{
				airplane_flag = 0;
				count_var_airplane = 0;
			}
		}

		ModelViewProjectionMatrix = ViewProjectionMatrix * ModelMatrix;
		glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
		draw_airplane();
		// draw airplane end here.


		// draw shirt start here.
		count_var_shirt += 1;
		if (shirt_flag == 0)  // move x-rightward.
		{
			shirt_x = -250.0f + count_var_shirt;
			shirt_y = pow(shirt_x, 2) / 100 - 350;
			ModelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(shirt_x, shirt_y, 0.0f));

			if (shirt_x >= 249.0f)
			{
				shirt_flag = 1;
				count_var_shirt = 0;
			}
		}
		else // shirt_flag == 1 // move x-leftward.
		{
			shirt_x = 250.0f - count_var_shirt;
			shirt_y = pow(shirt_x, 2) / 100 - 350;
			ModelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(shirt_x, shirt_y, 0.0f));

			if (shirt_x <= -249.0f)
			{
				shirt_flag = 0;
				count_var_shirt = 0;
			}
		}
		ModelMatrix = glm::scale(ModelMatrix, glm::vec3(2.0f, 2.0f, 1.0f));

		ModelViewProjectionMatrix = ViewProjectionMatrix * ModelMatrix;
		glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
		draw_shirt();
		// draw shirt end here.


		// draw house start here.
		count_var_house += 0.5;
		if (house_flag == 0)  // move x-rightward.
		{
			house_x = -300.0f + count_var_house;
			house_y = 50 * sin(house_x * 90 * TO_DEGREE);
			ModelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(house_x, house_y, 0.0f));

			if (house_x >= 299.9f)
			{
				house_flag = 1;
				count_var_house = 0;
			}
		}
		else // house_flag == 1 // move x-leftward.
		{
			house_x = 300.0f - count_var_house;
			house_y = 50 * sin(house_x * 90 * TO_DEGREE);
			ModelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(house_x, house_y, 0.0f));

			if (house_x <= -299.9f)
			{
				house_flag = 0;
				count_var_house = 0;
			}

		}
		ModelMatrix = glm::scale(ModelMatrix, glm::vec3(2.0f, 2.0f, 1.0f));
		ModelViewProjectionMatrix = ViewProjectionMatrix * ModelMatrix;
		glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
		draw_house();
		// draw house end here.

		// draw car start here.
		if (car1_flag == 0) // move downward.
		{
			car1_x += 5;
			car1_y += 1;
			if (car1_flag_x == 0)      // move rightward.
			{
				ModelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(-500.0f + car1_x, 200.0f - car1_y, 0.0f));

				if (-500.0f + car1_x >= 499.9f) // touch the left end(I defined.).
				{
					car1_flag_x = 1;
					car1_x = 0;
				}
			}
			else  // car2_flag_x == 1   // move rigthward.
			{
				ModelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(500.0f - car1_x, 200.0f - car1_y, 0.0f));

				if (500.0f - car1_x <= -499.9f)
				{
					car1_flag_x = 0;
					car1_x = 0;
				}
			}

			if (200.0f - car1_y <= -199.9f) // touch the floor.
			{
				car1_flag = 1;
				car1_y = 0;
			}
		}
		else // car1_flag == 1    // move upward.
		{
			car1_y++;
			ModelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(-500.0f, -200.0f + car1_y, 0.0f));

			if (-200.0f + car1_y >= 200.0f) // touch the ceilling.
			{
				car1_flag = 0;
				car1_y = 0;
			}
		}
		ModelMatrix = glm::scale(ModelMatrix, glm::vec3(2.0f, 2.0f, 1.0f));
		ModelMatrix = glm::rotate(ModelMatrix, TO_RADIAN*car1_clock, glm::vec3(0.0f, 0.0f, 1.0f));
		ModelViewProjectionMatrix = ViewProjectionMatrix * ModelMatrix;
		glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
		draw_car();
		// draw car end here.

		// draw cocktail start here.
		count_var_cocktail += 1;
		if (cocktail_flag == 0)
		{
			cocktail_x = 300.0f - count_var_cocktail;
			cocktail_y = 0.0f;
			ModelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(cocktail_x, cocktail_y, 0.0f));

			if (cocktail_x <= -299.9f)
			{
				cocktail_flag = 1;
				count_var_cocktail = 0;
			}
		}
		else // cocktail_flag == 1  
		{
			cocktail_x = -300.0f + count_var_cocktail;
			cocktail_y = 0.0f;
			ModelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(cocktail_x, cocktail_y, 0.0f));

			if (cocktail_x >= 299.9f)
			{
				cocktail_flag = 0;
				count_var_cocktail = 0;
			}
		}
		ModelMatrix = glm::scale(ModelMatrix, glm::vec3(2.0f, 2.0f, 1.0f));
		ModelMatrix = glm::rotate(ModelMatrix, TO_RADIAN*cocktail_clock, glm::vec3(0.0f, 0.0f, 1.0f));
		ModelViewProjectionMatrix = ViewProjectionMatrix * ModelMatrix;
		glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
		draw_cocktail();
		// draw cocktail  end here.


		// draw car2 start here.
		if (car2_flag == 0) // move downward.
		{
			car2_x += 5;
			car2_y += 1;
			if (car2_flag_x == 0)      // move leftward.
			{
				ModelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(500.0f - car2_x, 200.0f - car2_y, 0.0f));

				if (500.0f - car2_x <= -499.9f) // touch the left end(I defined.).
				{
					car2_flag_x = 1;
					car2_x = 0;
				}
			}
			else  // car2_flag_x == 1   // move rigthward.
			{
				ModelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(-500.0f + car2_x, 200.0f - car2_y, 0.0f));

				if (-500.0f + car2_x >= 499.9f)
				{
					car2_flag_x = 0;
					car2_x = 0;
				}
			}

			if (200.0f - car2_y <= -199.9f) // touch the floor.
			{
				car2_flag = 1;
				car2_y = 0;
			}
		}
		else // car2_flag == 1    // move upward.
		{
			car2_y++;
			ModelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(500.0f, -200.0f + car2_y, 0.0f));

			if (-200.0f + car2_y >= 200.0f) // touch the ceilling.
			{
				car2_flag = 0;
				car2_y = 0;
			}
		}
		ModelMatrix = glm::scale(ModelMatrix, glm::vec3(2.0f, 2.0f, 1.0f));
		ModelMatrix = glm::rotate(ModelMatrix, TO_RADIAN*car2_clock, glm::vec3(0.0f, 0.0f, 1.0f));
		ModelViewProjectionMatrix = ViewProjectionMatrix * ModelMatrix;
		glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
		draw_car2();
		// draw car2 end here.

	}
	else // all_clock tick completed. draw boomball and scale it.
	{
		if (boomball_size < 10)
		{
			boomball_size += 0.1;
			ModelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
			ModelMatrix = glm::scale(ModelMatrix, glm::vec3(1.0f*boomball_size, 1.0f*boomball_size, 1.0f));
			ModelMatrix = glm::rotate(ModelMatrix, 2 * TO_RADIAN*boomball_clock, glm::vec3(0.0f, 0.0f, 1.0f));
			ModelViewProjectionMatrix = ViewProjectionMatrix * ModelMatrix;
			glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
			draw_boomball();
		}
		else
		{
			ModelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
			ModelMatrix = glm::scale(ModelMatrix, glm::vec3(1.0f*boomball_size, 1.0f*boomball_size, 1.0f));
			ModelViewProjectionMatrix = ViewProjectionMatrix * ModelMatrix;
			glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
			if ((int)all_clock % 10 % 2 != 0)
				draw_boomball();
		}
	}
	glFlush();	
}   

void keyboard(unsigned char key, int x, int y) {
	switch (key) {
	case 27: // ESC key
		glutLeaveMainLoop(); // Incur destuction callback for cleanups.
		break;
	}
}

void reshape(int width, int height) {
	win_width = width, win_height = height;
	
  	glViewport(0, 0, win_width, win_height);
	ProjectionMatrix = glm::ortho(-win_width / 2.0, win_width / 2.0, 
		-win_height / 2.0, win_height / 2.0, -1000.0, 1000.0);
	ViewProjectionMatrix = ProjectionMatrix * ViewMatrix;

	update_axes();
	update_line();

	glutPostRedisplay();
}

void cleanup(void) {
	glDeleteVertexArrays(1, &VAO_axes);
	glDeleteBuffers(1, &VBO_axes);

	glDeleteVertexArrays(1, &VAO_line);
	glDeleteBuffers(1, &VBO_line);

	glDeleteVertexArrays(1, &VAO_airplane);
	glDeleteBuffers(1, &VBO_airplane);
}

void timer(int value)
{
	all_clock++;
	boomball_clock++;
	airplane_clock++;
	car1_clock++;
	car2_clock++;
	cocktail_clock++;
	house_clock++;

	glutPostRedisplay();
	glutTimerFunc(5, timer, 0);
}

void register_callbacks(void) {
	glutTimerFunc(25, timer, 0);
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutReshapeFunc(reshape);
	glutCloseFunc(cleanup);
}

void prepare_shader_program(void) {
	ShaderInfo shader_info[3] = {
		{ GL_VERTEX_SHADER, "Shaders/simple.vert" },
		{ GL_FRAGMENT_SHADER, "Shaders/simple.frag" },
		{ GL_NONE, NULL }
	};

	h_ShaderProgram = LoadShaders(shader_info);
	glUseProgram(h_ShaderProgram);

	loc_ModelViewProjectionMatrix = glGetUniformLocation(h_ShaderProgram, "u_ModelViewProjectionMatrix");
	loc_primitive_color = glGetUniformLocation(h_ShaderProgram, "u_primitive_color");
}

void initialize_OpenGL(void) {
	glEnable(GL_MULTISAMPLE); 
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	
	glClearColor(153 / 255.0f, 255 / 255.0f, 153 / 255.0f, 1.0f);
	ViewMatrix = glm::mat4(1.0f);
}

void prepare_scene(void) {
	prepare_axes();
	prepare_line();
	prepare_boomball();
	prepare_airplane();
	prepare_shirt();
	prepare_house();
	prepare_car();
	prepare_cocktail();
	prepare_car2();
}

void initialize_renderer(void) {
	register_callbacks();
	prepare_shader_program(); 
	initialize_OpenGL();
	prepare_scene();
}

void initialize_glew(void) {
	GLenum error;

	glewExperimental = GL_TRUE;

    error = glewInit();
	if (error != GLEW_OK) { 
		fprintf(stderr, "Error: %s\n", glewGetErrorString(error));
		exit(-1);
	}
	fprintf(stdout, "*********************************************************\n");
	fprintf(stdout, " - GLEW version supported: %s\n", glewGetString(GLEW_VERSION));
	fprintf(stdout, " - OpenGL renderer: %s\n", glGetString(GL_RENDERER));
	fprintf(stdout, " - OpenGL version supported: %s\n", glGetString(GL_VERSION));
	fprintf(stdout, "*********************************************************\n\n");
}

void greetings(char *program_name, char messages[][256], int n_message_lines) {
	fprintf(stdout, "**************************************************************\n\n");
	fprintf(stdout, "  PROGRAM NAME: %s\n\n", program_name);
	fprintf(stdout, "    This program was coded for CSE4170 students\n");
	fprintf(stdout, "      of Dept. of Comp. Sci. & Eng., Sogang University.\n\n");

	for (int i = 0; i < n_message_lines; i++)
		fprintf(stdout, "%s\n", messages[i]);
	fprintf(stdout, "\n**************************************************************\n\n");

	initialize_glew();
}

#define N_MESSAGE_LINES 1
void main(int argc, char *argv[]) {
	char program_name[64] = "Sogang CSE4170 2DObjects_GLSL_3.1";
	char messages[N_MESSAGE_LINES][256] = {
		"    - Keys used: 'ESC' "
	};

	glutInit (&argc, argv);
 	glutInitDisplayMode(GLUT_RGBA | GLUT_MULTISAMPLE);
	glutInitWindowSize (1200, 800);
	glutInitContextVersion(4, 0);
	glutInitContextProfile(GLUT_CORE_PROFILE);
	glutCreateWindow(program_name);

	greetings(program_name, messages, N_MESSAGE_LINES);
	initialize_renderer();

	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);
	glutMainLoop ();
}


