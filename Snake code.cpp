#include <bits/stdc++.h>
#include <conio.h>
#include <windows.h>
using namespace std;

#define MAX 1000

const char UP = 'u';
const char DOWN = 'd';
const char LEFT = 'l';
const char RIGHT = 'r';

int consoleWidth, consoleHeight;

void initialscreen() {
    HANDLE hconsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(hconsole, &csbi);
    consoleHeight = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
    consoleWidth = csbi.srWindow.Right - csbi.srWindow.Left + 1;
}

struct Point {
    int xCoord;
    int yCoord;
    Point() {}
    Point(int x, int y) {
        xCoord = x;
        yCoord = y;
    }
};

class Snake {
    int length;
    char direction;

public:
    Point body[MAX];

    Snake(int x, int y) {
        length = 1;
        body[0] = Point(x, y);
        direction = RIGHT;
    }

    int getLength() {
        return length;
    }

    void changeDirection(char newDirection) {
        if (newDirection == UP && direction != DOWN)
            direction = newDirection;
        else if (newDirection == DOWN && direction != UP)
            direction = newDirection;
        else if (newDirection == LEFT && direction != RIGHT)
            direction = newDirection;
        else if (newDirection == RIGHT && direction != LEFT)
            direction = newDirection;
    }

    bool move(Point food) {
        
        for (int i = length - 1; i > 0; i--) {
            body[i] = body[i - 1];
        }

        
        switch (direction) {
        case UP:
            body[0].yCoord--;
            break;
        case DOWN:
            body[0].yCoord++;
            break;
        case RIGHT:
            body[0].xCoord++;
            break;
        case LEFT:
            body[0].xCoord--;
            break;
        }

        
        if (body[0].xCoord < 0 || body[0].xCoord >= consoleWidth ||
            body[0].yCoord < 1 || body[0].yCoord >= consoleHeight) {
            return false;
        }

        
        for (int i = 1; i < length; i++) {
            if (body[0].xCoord == body[i].xCoord && body[0].yCoord == body[i].yCoord)
                return false;
        }

        
        if (food.xCoord == body[0].xCoord && food.yCoord == body[0].yCoord) {
            if (length < MAX - 1) {
                body[length] = body[length - 1]; 
                length++;
            }
            return true;
        }

        return true;
    }
};

class Board {
    Snake* snake;
    const char SNAKE_BODY = 'O';
    Point food;
    const char FOOD = '*';
    int score;

public:
    Board() {
        snake = new Snake(10, 10);
        score = 0;
        spawnFood();
    }

    ~Board() {
        delete snake;
    }

    int getScore() {
        return score;
    }

    void spawnFood() {
       
            int x = rand() % consoleWidth;
            int y = rand() % consoleHeight;
                food = Point(x, y);
            
    }

    void gotoxy(int x, int y) {
        COORD coord;
        coord.X = x;
        coord.Y = y;
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
    }

    void display() {
        gotoxy(consoleWidth / 2 - 5, 0);
        cout << "Score = " << score;
    }

    void draw() {
        system("cls");
        for (int i = 0; i < snake->getLength(); i++) {
            gotoxy(snake->body[i].xCoord, snake->body[i].yCoord);
            cout << SNAKE_BODY;
        }
        gotoxy(food.xCoord, food.yCoord);
        cout << FOOD;
        display();
    }

    bool update() {
        bool ateFood = snake->move(food);
        if (!ateFood)
            return false;

        // If snake head is on food
        if (food.xCoord == snake->body[0].xCoord && food.yCoord == snake->body[0].yCoord) {
            score++;
            spawnFood();
        }

        return true;
    }

    void getInput() {
        if (_kbhit()) {
            int key = _getch();
            if (key == 'w' || key == 'W')
                snake->changeDirection(UP);
            else if (key == 'a' || key == 'A')
                snake->changeDirection(LEFT);
            else if (key == 's' || key == 'S')
                snake->changeDirection(DOWN);
            else if (key == 'd' || key == 'D')
                snake->changeDirection(RIGHT);
        }
    }
};

int main() {
    srand(time(0));
    initialscreen();
    Board* board = new Board();

    while (board->update()) {
        board->getInput();
        board->draw();
        Sleep(200); // milliseconds
    }

    cout << "\nGame over" << endl;
    cout << "Final score = " << board->getScore() << endl;

    return 0;
}

