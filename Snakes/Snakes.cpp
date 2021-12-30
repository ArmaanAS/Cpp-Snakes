// Snakes.cpp : Defines the entry point for the console application.
#include "stdafx.h"
#include <iostream>
#include <stdio.h>
#include <string>
#include <windows.h>
#include <thread>

using namespace std;


int const WIDTH = 30, HEIGHT = 30;
enum SnakeType { HEAD = '\xCE', BODY = 1 };
enum Direction { UP = 0, RIGHT = 1, DOWN = 2, LEFT = 3 };
enum Colour {
	CYAN = 3, GREY = 8, DARK_BLUE = 9, GREEN = 10, BLUE = 11,
	RED = 12, PINK = 13, YELLOW = 14, DEFAULT = 15
};

void gotoXY(int x, int y)
{
	COORD coord;
	coord.X = x;
	coord.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void gotoDefault() {
	gotoXY(0, HEIGHT + 5);
}

void setColour(Colour c) {
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), c);
};

class SnakeBit {
public:
	void set(int i, int j, SnakeType k) {
		this->setX(i);
		this->setY(j);
		type = k;
	};
	void setXY(int i, int j) {
		this->setX(i);
		this->setY(j);
	};
	void setY(int i) {
		y = i;
	};
	void setX(int i) {
		x = i;
	};
	void addX(int i) {
		if (x + i < 0) {
			x = WIDTH;
		}
		else if (x + i > WIDTH) {
			x = 0;
		}
		else {
			x += i;
		}
	};
	void addY(int i) {
		if (y + i < 0) {
			y = HEIGHT;
		}
		else if (y + i > WIDTH) {
			y = 0;
		}
		else {
			y += i;
		}
	};
	void setSnakeType(SnakeType i) {
		type = i;
	};
	int getX() {
		return x;
	};
	int getY() {
		return y;
	};
	SnakeType getSnakeType() {
		return type;
	};
	void moveCursor() {
		gotoXY(x + 2, HEIGHT - y + 1);
	};
	void erase() {
		this->moveCursor();
		cout << " ";
		gotoXY(0, HEIGHT + 3);
	};
	void draw(Colour col, char t) {
		this->moveCursor();
		setColour(col);
		cout << t;
		setColour(DEFAULT);
		gotoDefault();
	};
private:
	SnakeType type;
	int x, y;
};

class Snake {
public:
	Snake() {
		body[0].set(WIDTH / 2, HEIGHT / 2, HEAD);
		for (int i = 1; i < 5; i++) {
			body[i].set(WIDTH / 2, HEIGHT / 2 - i, BODY);
		}
		length = 5;
	};
	void setDir(Direction d) {
		direction = d;
	};
	Direction getDir() {
		return direction;
	};
	int getLength() {
		return length;
	};
	void eat() {
		body[length].set(body[length - 1].getX(), body[length - 1].getY(), BODY);
		length++;
	};
	void drawSnake() {
		for (int i = length - 1; i >= 0; i--) {
			if (body[i].getSnakeType() == HEAD) {
				body[i].draw(col, HEAD);
			}
			else {
				if (i == length - 1) {
					int y0 = body[i].getY(),
						y1 = body[i - 1].getY(),
						x0 = body[i].getX(),
						x1 = body[i - 1].getX();

					if (y0 != y1) {
						body[i].draw(col, '\xBA');
					}
					else {
						body[i].draw(col, '\xCD');
					}
				}
				else {
					int y_1 = body[i - 1].getY(),
						y0 = body[i].getY(),
						y1 = body[i + 1].getY(),
						x_1 = body[i - 1].getX(),
						x0 = body[i].getX(),
						x1 = body[i + 1].getX();

					if (y_1 == y0 && y0 == y1) {
						body[i].draw(col, '\xCD');
					}
					else if (x_1 == x0 && x0 == x1) {
						body[i].draw(col, '\xBA');
					}
					else if ((y_1 < y0 || y1 < y0) && (x_1 < x0 || x1 < x0)) {
						body[i].draw(col, '\xBB');
					}
					else if ((y_1 > y0 || y1 > y0) && (x_1 < x0 || x1 < x0)) {
						body[i].draw(col, '\xBC');
					}
					else if ((y_1 < y0 || y1 < y0) && (x_1 > x0 || x1 > x0)) {
						body[i].draw(col, '\xC9');
					}
					else if ((y_1 > y0 || y1 > y0) && (x_1 > x0 || x1 > x0)) {
						body[i].draw(col, '\xC8');
					}
				}
			}
		}
	};
	void move() {
		for (int i = length - 1; i > 0; i--) {
			body[i].erase();
			body[i].setXY(
				body[i - 1].getX(),
				body[i - 1].getY()
			);
		}
		switch (direction) {
		case UP:
			body[0].addY(1);
			break;
		case DOWN:
			body[0].addY(-1);
			break;
		case LEFT:
			body[0].addX(-1);
			break;
		case RIGHT:
			body[0].addX(1);
			break;
		}

		this->drawSnake();
	};
	bool hasCollided() {
		int x = body[0].getX(),
			y = body[0].getY();

		for (int i = length - 1; i > 0; i--) {
			if (body[i].getX() == x && body[i].getY() == y) {
				return true;
			}
		}
		return false;
	};
	COORD getPos() {
		COORD c;
		c.X = body[0].getX();
		c.Y = body[0].getY();
		return c;
	};
private:
	SnakeBit body[100];
	Direction direction = UP;
	int length;
	Colour col = GREEN;
};

class food {
public:
	void randomise() {
		x = rand() % WIDTH;
		y = rand() % HEIGHT;
		Colour cols[7] = { CYAN, DARK_BLUE, GREEN, BLUE, RED, PINK, YELLOW };
		col = cols[rand() % 7];
	};
	void draw() {
		setColour(col);
		gotoXY(2 + x, HEIGHT - y + 1);
		cout << "\xDC";//'\xFE';
		gotoDefault();
		setColour(DEFAULT);
	};
	int getX() {
		return x;
	};
	int getY() {
		return y;
	};
private:
	int x, y;
	Colour col = RED;
};

void drawBorder() {
	for (int y = HEIGHT + 2; y >= 0; y--) {
		for (int x = 0; x <= WIDTH + 2; x++) {
			if (x == 0 || x == WIDTH + 2) {
				cout << "\xDB\xDB";
			}
			else if (y == 0 || y == HEIGHT + 2) {
				cout << "\xDB";
			}
			else {
				cout << " ";
			}
		}
		cout << endl;
	}
}

void getInput(Direction const& d, Snake const& s) {
	Direction& direction = const_cast<Direction&>(d);
	Snake& snake = const_cast<Snake&>(s);

	while (true) {
		if ((GetAsyncKeyState(VK_UP) || GetAsyncKeyState(0x57)) && snake.getDir() != DOWN) {
			direction = UP;
		}
		else if ((GetAsyncKeyState(VK_DOWN) || GetAsyncKeyState(0x53)) && snake.getDir() != UP) {
			direction = DOWN;
		}
		else if ((GetAsyncKeyState(VK_RIGHT) || GetAsyncKeyState(0x44)) && snake.getDir() != LEFT) {
			direction = RIGHT;
		}
		else if ((GetAsyncKeyState(VK_LEFT) || GetAsyncKeyState(0x41)) && snake.getDir() != RIGHT) {
			direction = LEFT;
		}
	}
}

int main()
{
	system("title Snakes!");
	setColour(DEFAULT);
	drawBorder();

	Snake snake;
	snake.drawSnake();

	Direction direction = UP;

	thread input(getInput, ref(direction), ref(snake));
	input.detach();

	food f;
	f.randomise();

	COORD s;

	bool collided = false;

	int score = 0;
	gotoXY(0, HEIGHT + 3);
	cout << "Score: " << score << "\nLength: " << snake.getLength();

	while (!collided) {
		snake.setDir(direction);
		snake.move();

		f.draw();

		s = snake.getPos();
		if (s.X == f.getX() && s.Y == f.getY()) {
			f.randomise();
			f.draw();
			snake.eat();
			score++;
			gotoXY(0, HEIGHT + 3);
			cout << "\rScore: " << score << "\n\rLength: " << snake.getLength();
		}

		collided = snake.hasCollided();
		Sleep(150);
	}

	setColour(RED);
	gotoXY(0, HEIGHT + 5);
	cout << "Game over!";

	cin.ignore();
	return 0;
}

