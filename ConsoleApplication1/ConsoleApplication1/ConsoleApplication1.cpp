// ConsoleApplication1.cpp : Defines the entry point for the console application.
//


#include "stdafx.h"
#include <string>
#include <Windows.h>
#include <iostream>
#include <conio.h>
#include <sstream>
#include <math.h>
#include <gl\GL.h>
#include <gl\GLU.h>
#include "GL/freeglut.h"
#pragma comment(lib, "OpenGL32.lib")

//keycodes
#define VK_W 0x57
#define VK_S 0x53


void draw();

void update(int value);

void enable2D(int width, int height);

void drawText(float x, float y, std::string text);

void drawRect(float x, float y, float width, float height);

void keyboard();

void updateBall();

void vec2_norm(float &x, float &y);

std::string int2str(int x);

//window size and interval rate (60fps)
const int width = 500;
const int height = 500;
const int interval = 1000 / 60;

//scores
int score_left = 0;
int score_right = 0;

//ball
float ball_pos_x = width / 2;
float ball_pos_y = height / 2;
float ball_dir_x = -1.0f;
float ball_dir_y = 0.0f;
int ball_size = 8;
int ball_speed = 10;

//racket properties
const int racket_width = 10;
const int racket_height = 80;
int racket_speed = 5;

//left racket position
float racket_left_x = 10.0f;
float racket_left_y = 50.0f;

//right racket position
float racket_right_x = width - racket_width - 10;
float racket_right_y = 50;

int main(int argc, char** argv)
{
	//initialize opengl (via glut)

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(width, height);
	glutCreateWindow("Pong.exe");


	glutDisplayFunc(draw);
	glutTimerFunc(interval, update, 0);

	//Enable 2D mode and set draw color to white
	enable2D(width, height);
	glColor3f(1.0f, 1.0f, 1.0f);

	//start the whole thing
	glutMainLoop();
    return 0;
}

void draw() {
	//clear 
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	//draw ball
	drawRect(ball_pos_x - ball_size / 2, ball_pos_y - ball_size / 2, ball_size, ball_size);

	//draw rackets
	drawRect(racket_left_x, racket_left_y, racket_width, racket_height);
	drawRect(racket_right_x, racket_right_y, racket_width, racket_height);

	drawText(width / 2 - 20, height - 30, int2str(score_left) + ":" + int2str(score_right));
	//swap buffers (has to be done at the end)

	
	glutSwapBuffers();
}

void update(int value) {

	keyboard();
	//call update() again in 'interval' milliseconds

	updateBall();

	glutTimerFunc(interval, update, 0);

	//Redisplay frame
	glutPostRedisplay();

}

void enable2D(int width, int height) {
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0f, width, 0.0f, height, 0.0f, 1.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void drawText(float x, float y, std::string text) {
	glRasterPos2f(x, y);
	glutBitmapString(GLUT_BITMAP_8_BY_13, (const unsigned char*)text.c_str());
}

void drawRect(float x, float y, float width, float height) {
	glBegin(GL_QUADS);
		glVertex2f(x, y);
		glVertex2f(x + width ,y);
		glVertex2f(x + width, y + height);
		glVertex2f(x, y + height);
	glEnd();
}

void keyboard() {
	//left racket
	if (GetAsyncKeyState(VK_W)){
		if (racket_left_y <= height - racket_height) {
			racket_left_y += racket_speed;
		}
		}
	if (GetAsyncKeyState(VK_S))
	{
		if (racket_left_y >= 0) {
			racket_left_y -= racket_speed;
		}
	}

	//right racket
	if (GetAsyncKeyState(VK_UP)) 
		if (racket_right_y <= height - racket_height) {
			racket_right_y += racket_speed;
		}
	if (GetAsyncKeyState(VK_DOWN)) {
		if (racket_right_y >= 0) {
			racket_right_y -= racket_speed;
		}
	}

}

void updateBall() {
	ball_pos_x += ball_dir_x * ball_speed;
	ball_pos_y += ball_dir_y * ball_speed;

	// hit by left racket?
	if (ball_pos_x < racket_left_x + racket_width &&
		ball_pos_x > racket_left_x &&
		ball_pos_y < racket_left_y + racket_height &&
		ball_pos_y > racket_left_y) {
		// set fly direction depending on where it hit the racket
		// (t is 0.5 if hit at top, 0 at center, -0.5 at bottom)
		float t = ((ball_pos_y - racket_left_y) / racket_height) - 0.5f;
		ball_dir_x = fabs(ball_dir_x); // force it to be positive
		ball_dir_y = t;
	}

	// hit by right racket?
	if (ball_pos_x > racket_right_x &&
		ball_pos_x < racket_right_x + racket_width &&
		ball_pos_y < racket_right_y + racket_height &&
		ball_pos_y > racket_right_y) {
		// set fly direction depending on where it hit the racket
		// (t is 0.5 if hit at top, 0 at center, -0.5 at bottom)
		float t = ((ball_pos_y - racket_right_y) / racket_height) - 0.5f;
		ball_dir_x = -fabs(ball_dir_x); // force it to be negative
		ball_dir_y = t;
	}

	// hit left wall?
	if (ball_pos_x < 0) {
		++score_right;
		ball_pos_x = width / 2;
		ball_pos_y = height / 2;
		ball_dir_x = fabs(ball_dir_x); // force it to be positive
		ball_dir_y = 0;
	}

	// hit right wall?
	if (ball_pos_x > width) {
		++score_left;
		ball_pos_x = width / 2;
		ball_pos_y = height / 2;
		ball_dir_x = -fabs(ball_dir_x); // force it to be negative
		ball_dir_y = 0;
	}

	// hit top wall?
	if (ball_pos_y > height) {
		ball_dir_y = -fabs(ball_dir_y); // force it to be negative
	}

	// hit bottom wall?
	if (ball_pos_y < 0) {
		ball_dir_y = fabs(ball_dir_y); // force it to be positive
	}

	// make sure that length of dir stays at 1
	vec2_norm(ball_dir_x, ball_dir_y);
}

void vec2_norm(float& x, float &y) {
	// sets a vectors length to 1 (which means that x + y == 1)
	float length = sqrt((x * x) + (y * y));
	if (length != 0.0f) {
		length = 1.0f / length;
		x *= length;
		y *= length;
	}
}

std::string int2str(int x) {
	std::stringstream ss;
	ss << x;
	return ss.str();
}