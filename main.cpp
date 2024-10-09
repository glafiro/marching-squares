#include "raylib5.h"
#include "raymath.h"
#include <cmath>
#include <string>
#include <vector>
using std::string;
using std::vector;

#define SCR_W 640
#define SCR_H 640
#define N_CIRCLES 5
#define GRID_SIZE 8
#define GUIDE_COLOR Color{200, 200, 200, 255}

int floatToBinary(float v) {
	return v >= 1.0 ? 1 : 0;
}

float distance(Vector2 p1, Vector2 p2) {
	return pow(p1.x - p2.x, 2.0) + pow(p1.y - p2.y, 2.0);
}

struct Circle {
	Vector2 center;
	Vector2 velocity;
	float   radius;
	
	Circle() {
		center.x = GetRandomValue(SCR_W / 4, SCR_W / 2);
		center.y = GetRandomValue(SCR_W / 4, SCR_H / 2);
		radius = GetRandomValue(20, 100);
		velocity.x = GetRandomValue(1, 3);
		velocity.y = GetRandomValue(1, 3);
	}

	void move() {
		center.x += velocity.x;
		center.y += velocity.y;

		if (center.x + radius >= SCR_W || center.x - radius < 0) velocity.x *= -1;
		if (center.y + radius >= SCR_H || center.y - radius < 0) velocity.y *= -1;
	}

	void draw() {
		DrawCircleV(center, radius, GUIDE_COLOR);
	}
};

int main()
{
	const int screenWidth = SCR_W;
	const int screenHeight = SCR_H;

	SetConfigFlags(FLAG_MSAA_4X_HINT);
	InitWindow(screenWidth, screenHeight, "Marching Squares");
	SetTargetFPS(60);
	Color bgColor = Color{ 254, 219, 0, 255 };

	const int COLS = SCR_W / GRID_SIZE + 1;
	const int ROWS = SCR_W / GRID_SIZE + 1;
	float radius = 2.0f;
	float distanceValue;

	vector<Circle> circles;
	vector<vector<float>> grid( ROWS, vector<float>(COLS, 0.0f) );

	for (int i = 0; i < N_CIRCLES; i++) {
		Circle c;
		circles.push_back(c);
	}

	while (!WindowShouldClose()) 
	{	
		if (IsKeyPressed(KEY_R)) {
			circles.clear();
			for (int i = 0; i < N_CIRCLES; i++) {
				Circle c;
				circles.push_back(c);
			}
		}

		BeginDrawing();
		ClearBackground(bgColor);

		for (int y = 0; y < ROWS; y++) {
			for (int x = 0; x < COLS; x++) {
				distanceValue = 0.0f;
				for (int i = 0; i < N_CIRCLES; i++) {
					distanceValue += (circles[i].radius * circles[i].radius) / 
									  distance(Vector2{ (float)x * GRID_SIZE, (float)y * GRID_SIZE }, circles[i].center);
				}
				grid[x][y] = distanceValue;
			}
		}

		for (int i = 0; i < N_CIRCLES; i++) {
			circles[i].move();
		}

		
		for (int y = 0; y < ROWS - 1; y++) {
			for (int x = 0; x < COLS - 1; x++) {
				float f_a = grid[x][y];
				float f_b = grid[x+1][y];
				float f_c = grid[x+1][y+1];
				float f_d = grid[x][y+1];

				int a = floatToBinary(f_a);
				int b = floatToBinary(f_b);
				int c = floatToBinary(f_c);
				int d = floatToBinary(f_d);

				int config = 8 * a + 4 * b + 2 * c + d;

				Vector2 p1, p2, p3, p4;
				float t = 0.0f;

				t = (1.0 - f_a) / (f_b - f_a);
				p1.x = Lerp(x * GRID_SIZE, x * GRID_SIZE + GRID_SIZE, t);
				p1.y = y * GRID_SIZE;

				t = (1.0 - f_b) / (f_c - f_b);
				p2.x = x * GRID_SIZE + GRID_SIZE;
				p2.y = Lerp(y * GRID_SIZE, y * GRID_SIZE + GRID_SIZE, t);
				
				t = (1.0 - f_d) / (f_c - f_d);
				p3.x = Lerp(x * GRID_SIZE, x * GRID_SIZE + GRID_SIZE, t);
				p3.y = y * GRID_SIZE + GRID_SIZE;
				
				t = (1.0 - f_a) / (f_d - f_a);
				p4.x = x * GRID_SIZE;
				p4.y = Lerp(y * GRID_SIZE, y * GRID_SIZE + GRID_SIZE, t);

				Color lineColor = BLACK;

				switch (config) {
				case 0:
					break;
				case 1:
					DrawLineV(p3, p4, lineColor);
					break;
				case 2:
					DrawLineV(p3, p2, lineColor);
					break;
				case 3:
					DrawLineV(p2, p4, lineColor);
					break;
				case 4:
					DrawLineV(p1, p2, lineColor);
					break;
				case 5:
					DrawLineV(p1, p4, lineColor);
					DrawLineV(p2, p3, lineColor);
					break;
				case 6:
					DrawLineV(p1, p3, lineColor);
					break;
				case 7:
					DrawLineV(p1, p4, lineColor);
					break;
				case 8:
					DrawLineV(p1, p4, lineColor);
					break;
				case 9:
					DrawLineV(p1, p3, lineColor);
					break;
				case 10:
					DrawLineV(p1, p2, lineColor);
					DrawLineV(p3, p4, lineColor);
					break;
				case 11:
					DrawLineV(p1, p2, lineColor);
					break;
				case 12:
					DrawLineV(p2, p4, lineColor);
					break;
				case 13:
					DrawLineV(p2, p3, lineColor);
					break;
				case 14:
					DrawLineV(p3, p4, lineColor);
					break;
				case 15: 
					break;
				}
			}
		}

		//for (int y = 0; y < ROWS; y++) {
		//	for (int x = 0; x < COLS; x++) {
		//		Color color = floatToBinary(grid[x][y]) == 1 ? LIME : BLACK;
		//		DrawCircle(x * GRID_SIZE, y * GRID_SIZE, radius, color);
		//	}
		//}

		
		EndDrawing();
	}
}