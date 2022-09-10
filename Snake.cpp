#include <iostream>
#include <cstdio>
#include <conio.h>
#include <cstring>
#include <ctime>
#include <windows.h>
using namespace std;

#define ROW_NUM 25
#define COL_NUM 50

struct snake {
	COORD pos;
	char op;
	bool flag;
	int fx; //方向：1为上，2为下，3为左，4为右 
};

struct Food {
	COORD pos;
	bool flag;
	char op;
};

struct Game {
	snake head, body[2505];
	Food food;
	int body_num;
	void init();
	bool GameOver();
	bool Same(COORD a, COORD b);
	void moveSnake(char op);
	bool checkfood();
	void creatfood();
	void eatfood();
	void drawfood();
	void creatbody();
	snake movesnake(snake t);
	void drawSnake();
	void playing(int t);
};

void SetPos(int i, int j) {
	swap(i, j);
	COORD pos = {i, j};
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

void SetPos(COORD pos) {
	swap(pos.X, pos.Y);
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);	
}

bool Game::Same(COORD a, COORD b) {
	return (a.X == b.X && a.Y == b.Y);
}

void Game::init() {
	body_num = 3;
	head.pos.X = 12;
	head.pos.Y = 25;
	head.op = '#';
	head.fx = 1;
	for(int i=0; i<=2500; i++) {
		body[i].op = '#';
		body[i].fx = 1;
	}
	body[1].pos.X = 13;
	body[1].pos.Y = 25;
	body[2].pos.X = 14;
	body[2].pos.Y = 25;
	body[3].pos.X = 15;
	body[3].pos.Y = 25;
	for(int j=2; j<=49; j++) {
		SetPos(1, j);
		cout << '-';
		SetPos(25, j);
		cout << '-';
	}
	for(int i=2; i<=24; i++) {
		SetPos(i, 1);
		cout << '|';
		SetPos(i, 50);
		cout << '|';
	}
}

bool Game::GameOver() {
	if(head.pos.X <= 1 || head.pos.X >= ROW_NUM || head.pos.Y <= 1 || head.pos.Y >= COL_NUM) {
		return true;
	} else {
		for(int i=1; i<=body_num; i++) {
			if(Same(head.pos, body[i].pos)) {
				return true;
			}
		}
	}
	return false;
}

void Game::drawSnake() {
	SetPos(head.pos);
	cout << head.op;
	for(int i=1; i<=body_num; i++) {
		SetPos(body[i].pos);
		cout << body[i].op;
	}
}

snake Game::movesnake(snake t) {
	switch(t.fx) {
		case 1:
			t.pos.X -= 1;
			break;
		case 2:
			t.pos.X += 1;
			break;
		case 3:
			t.pos.Y -= 1;
			break;
		case 4:
			t.pos.Y += 1;
			break;
	}
	return t;
}

void Game::moveSnake(char op) {
	switch(op) {
		case 'w':
			head.fx = 1;
			break;
		case 's':
			head.fx = 2;
			break;
		case 'a':
			head.fx = 3;
			break;
		case 'd':
			head.fx = 4;
			break;
	}
	SetPos(head.pos);
	cout << " ";
	for(int i=1; i<=body_num; i++) {
		SetPos(body[i].pos);
		cout << " ";
	}
	head = movesnake(head);
	body[1] = movesnake(body[1]);
	for(int i=2; i<=body_num; i++) {
		body[i] = movesnake(body[i]);
	}
	int p[1005], cnt = 0;
	for(int i=2; i<=body_num; i++) {
		if(body[i].fx != body[i-1].fx) {
			cnt ++;
			p[cnt] = i;
		}
	}
	for(int i=cnt; i>=1; i--) {
		body[p[i]].fx = body[p[i]-1].fx;
	}
	if(body[1].fx != head.fx) {
		body[1].fx = head.fx;
	}
}

void Game::creatfood() {
	COORD p;
	while(true) {
		p = {rand() % 25 + 1, rand() % 50 + 1};
		bool flag = true;
		if(Same(head.pos, p)) {
			flag = false;
		}
		for(int i=1; i<=body_num; i++) {
			if(Same(body[i].pos, p)) {
				flag = false;
			}
		}
		if(p.X > 1 && p.X < 25 && p.Y > 1 && p.Y < 50 && flag) {
			break;
		}
	}
	food.pos = p;
	food.op = '@';
	food.flag = true;
}

void Game::drawfood() {
	SetPos(food.pos);
	cout << food.op;
}

bool Game::checkfood() {
	return Same(head.pos, food.pos);
}

void Game::eatfood() {
	food.flag = false;
	creatbody();
}

void Game::creatbody() {
	snake s = body[body_num];
	body_num ++;
	body[body_num].op = '#';
	body[body_num].fx = s.fx;
	if(s.fx == 1) {
		body[body_num].pos = {s.pos.X+1, s.pos.Y};
	} else if(s.fx == 2) {
		body[body_num].pos = {s.pos.X-1, s.pos.Y};
	} else if(s.fx == 3) {
		body[body_num].pos = {s.pos.X, s.pos.Y+1};
	} else if(s.fx == 4) {
		body[body_num].pos = {s.pos.X, s.pos.Y-1};
	}
} 

void Game::playing(int t) {
	init();
	while(!GameOver()) {
		char x;
		if(_kbhit()) {
			x = _getch();		
		} else {
			x = ' ';
		}
		moveSnake(x);
		drawSnake();
		if(checkfood()) {
			eatfood();
		}
		if(!food.flag) {
			creatfood();
		}
		drawfood();
		Sleep(t);
	}
}

int main() {
	int n;
	Game game;
	cout << "请输入游戏难度（1~9）：";
	cin >> n;
	n = 9-n+1;
	game.playing(n*100);
	return 0;
}

