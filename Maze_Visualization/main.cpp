#include <opencv2/opencv.hpp>
#include <iostream>
#include <queue>
#include <vector>
using namespace cv;
using namespace std;

const int WINDOW_SIZE_Y = 500;
const int WINDOW_SIZE_X = 500;

String windowName = "Maze";
Mat maze;
vector < vector<Point>> parent(WINDOW_SIZE_Y, vector<Point>(WINDOW_SIZE_X, Point(-1, -1)));
Point pt1, pt2, start, finish;
int status = -1;

void display(Point p) {
	int x = parent[p.x][p.y].x;
	int y = parent[p.x][p.y].y;
	while (x != -1 && y != -1) {
		maze.at<Vec3b>(Point(x,y)) = Vec3b(0, 255, 0);
		/*maze.at<Vec3b>(Point(x+1, y)) = Vec3b(0, 255, 0);
		maze.at<Vec3b>(Point(x-1, y)) = Vec3b(0, 255, 0);
		maze.at<Vec3b>(Point(x, y+1)) = Vec3b(0, 255, 0);
		maze.at<Vec3b>(Point(x, y-1)) = Vec3b(0, 255, 0);
		maze.at<Vec3b>(Point(x+1, y+1)) = Vec3b(0, 255, 0);
		maze.at<Vec3b>(Point(x - 1, y - 1)) = Vec3b(0, 255, 0);
		maze.at<Vec3b>(Point(x + 1, y - 1)) = Vec3b(0, 255, 0);
		maze.at<Vec3b>(Point(x - 1, y + 1)) = Vec3b(0, 255, 0);*/
		imshow(windowName, maze);
		waitKey(1);
		Point temp = parent[x][y];
		x = temp.x;
		y = temp.y;
	}
	return;	
}

void CallBackFunc(int event, int x, int y, int flags, void* userdata) {
	if (event == EVENT_LBUTTONDOWN) {
		if (status == -1) {
			pt1.x = x;
			pt1.y = y;
		}
		else if (status == 0) {
			start.x = x;
			start.y = y;
			maze.at<Vec3b>(start) = Vec3b(255, 0, 0);
			imshow(windowName, maze);
		}
		else {
			finish.x = x;
			finish.y = y;
			maze.at<Vec3b>(finish) = Vec3b(0, 255, 0);
			imshow(windowName, maze);
		}
	}

	else if (event == EVENT_LBUTTONUP)
	{
		if (status == -1) {
			pt2.x = x;
			pt2.y = y;
			line(maze, pt1, pt2, Scalar(0, 0, 0));
			imshow(windowName, maze);
		}

	}
}

bool checkValid(Point p) {
	if (p.x == start.x && p.y == start.y)
		return false;
	if (!(p.x > 0 && p.x < WINDOW_SIZE_X && p.y > 0 && p.y < WINDOW_SIZE_Y))
		return false;

	auto color = maze.at<Vec3b>(p);
	if (!(color == Vec3b(0, 0, 255)) && !(color == Vec3b(0, 0, 0)))
		return true;
	else
		return false;
}

void BFS() {
	queue<Point> q;
	q.push(start);
	int count = 0;
	while (!q.empty()) {
		Point p = q.front();
		q.pop();
		if (p.x == finish.x && p.y == finish.y)
			break;

		if (checkValid(Point(p.x + 1, p.y))) {
			maze.at<Vec3b>(Point(p.x + 1, p.y)) = Vec3b(0, 0, 255);
			parent[p.x + 1][p.y] = Point(p.x, p.y);
			q.push(Point(p.x + 1, p.y));
		}

		if (checkValid(Point(p.x - 1, p.y))) {
			maze.at<Vec3b>(Point(p.x - 1, p.y)) = Vec3b(0, 0, 255);
			parent[p.x - 1][p.y] = Point(p.x, p.y);
			q.push(Point(p.x - 1, p.y));
		}

		if (checkValid(Point(p.x, p.y + 1))) {
			maze.at<Vec3b>(Point(p.x, p.y + 1)) = Vec3b(0, 0, 255);
			parent[p.x][p.y + 1] = Point(p.x, p.y);
			q.push(Point(p.x, p.y + 1));
		}
		if (checkValid(Point(p.x, p.y - 1))) {
			maze.at<Vec3b>(Point(p.x, p.y - 1)) = Vec3b(0, 0, 255);
			parent[p.x][p.y - 1] = Point(p.x, p.y);
			q.push(Point(p.x, p.y - 1));
		}
		count++;
		if (count == 200) {
			imshow(windowName, maze);
			waitKey(1);
			count = 0;
		}


	}
	display(finish);
}

int main(void) {
	namedWindow(windowName);
	maze = Mat::zeros(WINDOW_SIZE_Y, WINDOW_SIZE_X, CV_8UC3);
	maze = Scalar(255, 255, 255);
	setMouseCallback(windowName, CallBackFunc, NULL);
	imshow(windowName, maze);
	while (true) {
		int k = waitKey(1) & 0xFF;
		if (char(k) == 'm') {
			status = 0;
			break;
		}
	}
	while (true) {
		int k = waitKey(1) & 0xFF;
		if (char(k) == 's') {
			status = 1;
			break;
		}
	}
	while (true) {
		int k = waitKey(1) & 0xFF;
		if (char(k) == 'f') {
			break;
		}
	}
	BFS();
	waitKey(0);
	destroyAllWindows();
	return 0;
}
