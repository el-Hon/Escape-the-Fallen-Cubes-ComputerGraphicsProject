#include <GL/glut.h>
#include <iostream>
#include <sstream>
#include <string>
#include <cmath>
#include <vector>
#include <ctime>

const int WIDTH = 800;
const int HEIGHT = 600;

float playerX = WIDTH / 2.0f;
float playerSize = 40.0f;
float playerSpeed = 10.0f;

int seconds = 0;
int minutes = 0;

int score = 0;

struct Obstacle {
    float x;
    float y;
    float size;
    float speed;

    Obstacle(float _x, float _y, float _size, float _speed) : x(_x), y(_y), size(_size), speed(_speed) {}
};

std::vector<Obstacle> obstacles;

bool gameStarted = false;
bool inMainMenu = true;

void drawPlayer() {
    glColor3f(1.0f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glVertex2f(playerX - playerSize / 2, 50);
    glVertex2f(playerX + playerSize / 2, 50);
    glVertex2f(playerX + playerSize / 2, 10);
    glVertex2f(playerX - playerSize / 2, 10);
    glEnd();
}

void drawObstacles() {
    glColor3f(0.0f, 0.0f, 1.0f);
    for (std::vector<Obstacle>::iterator it = obstacles.begin(); it != obstacles.end(); ++it) {
        Obstacle& obstacle = *it;
        glBegin(GL_QUADS);
        glVertex2f(obstacle.x - obstacle.size / 2, obstacle.y - obstacle.size / 2);
        glVertex2f(obstacle.x + obstacle.size / 2, obstacle.y - obstacle.size / 2);
        glVertex2f(obstacle.x + obstacle.size / 2, obstacle.y + obstacle.size / 2);
        glVertex2f(obstacle.x - obstacle.size / 2, obstacle.y + obstacle.size / 2);
        glEnd();
    }
}

void drawText(float x, float y, std::string text) {
    glColor3f(1.0f, 1.0f, 1.0f); // White color
    glRasterPos2f(x, y);
    for (std::string::iterator it = text.begin(); it != text.end(); ++it) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *it);
    }
}

void drawCloud() {
    for (int i = 0; i < 3; ++i) {
        glBegin(GL_POLYGON);
        for (int j = 0; j < 360; j += 30) {
            float angle = j * 3.1415926f / 180.0f;
            glVertex2f(i * 30 * cos(angle), 20 * sin(angle));
        }
        glEnd();
    }
}

void drawBackground() {
    glColor3f(0.4f, 0.6f, 1.0f);
    glBegin(GL_QUADS);
    glVertex2f(0, HEIGHT);
    glVertex2f(WIDTH, HEIGHT);
    glVertex2f(WIDTH, 0);
    glVertex2f(0, 0);
    glEnd();

    glPushMatrix();
    glTranslatef(100, 500, 0);
    glColor3f(1.0f, 1.0f, 1.0f);
    drawCloud();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(400, 550, 0);
    drawCloud();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(650, 520, 0);
    drawCloud();
    glPopMatrix();
}

void drawMainMenu() {
    // Draw Game Title
    glColor3f(1.0f, 1.0f, 1.0f); // White color
    drawText(WIDTH / 2 - 120, HEIGHT - 100, "Escape The Fallen Cubes");
    // Draw Start Button
    glColor3f(0.0f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glVertex2f(WIDTH / 2 - 50, HEIGHT / 2 + 30);
    glVertex2f(WIDTH / 2 + 50, HEIGHT / 2 + 30);
    glVertex2f(WIDTH / 2 + 50, HEIGHT / 2 - 10);
    glVertex2f(WIDTH / 2 - 50, HEIGHT / 2 - 10);
    glEnd();
    drawText(WIDTH / 2 - 35, HEIGHT / 2 + 15, "Start");

    // Draw Exit Button
    glColor3f(0.0f, 0.0f, 0.0f); // Same color as start button
    glBegin(GL_QUADS);
    glVertex2f(WIDTH / 2 - 50, HEIGHT / 2 - 60);
    glVertex2f(WIDTH / 2 + 50, HEIGHT / 2 - 60);
    glVertex2f(WIDTH / 2 + 50, HEIGHT / 2 - 100);
    glVertex2f(WIDTH / 2 - 50, HEIGHT / 2 - 100);
    glEnd();
    drawText(WIDTH / 2 - 35, HEIGHT / 2 - 75, "Exit");
}


void handleMainMenuClick(int x, int y) {
    if (x >= WIDTH / 2 - 50 && x <= WIDTH / 2 + 50) {
        if (y >= HEIGHT / 2 - 10 && y <= HEIGHT / 2 + 30) {
            // Start Button Clicked
            inMainMenu = false;
            gameStarted = true;
        } else if (y >= HEIGHT / 2 - 100 && y <= HEIGHT / 2 - 60) {
            // Exit Button Clicked
            exit(0);
        }
    }
}

void handleKeypress(unsigned char key, int x, int y) {
    if (gameStarted) {
        switch (key) {
            case 27:
                exit(0);
                break;
        }
    }
}

void handleArrowKeypress(int key, int x, int y) {
    if (gameStarted) {
        switch (key) {
            case GLUT_KEY_LEFT:
                if (playerX > playerSize / 2) {
                    playerX -= playerSpeed;
                }
                break;
            case GLUT_KEY_RIGHT:
                if (playerX < WIDTH - playerSize / 2) {
                    playerX += playerSpeed;
                }
                break;
        }
    }
}

void update(int value) {
    if (!gameStarted) {
        glutPostRedisplay();
        glutTimerFunc(16, update, 0);
        return;
    }

    for (std::vector<Obstacle>::iterator it = obstacles.begin(); it != obstacles.end(); ++it) {
        Obstacle& obstacle = *it;
        obstacle.y -= obstacle.speed;
        if (obstacle.y < 0) {
            obstacle.y = HEIGHT + obstacle.size / 2;
            obstacle.x = rand() % (WIDTH - 100) + 50;
            score++;
            obstacle.speed += 0.5f;
        }
    }

    seconds++;
    if (seconds >= 60) {
        minutes++;
        seconds = 0;
    }

    for (std::vector<Obstacle>::iterator it = obstacles.begin(); it != obstacles.end(); ++it) {
        Obstacle& obstacle = *it;
        if (std::abs(obstacle.x - playerX) < (playerSize / 2 + obstacle.size / 2) &&
            std::abs(obstacle.y - 30) < (playerSize / 2 + obstacle.size / 2)) {
            std::cout << "Game Over! Your score: " << score << std::endl;
            exit(0);
        }
    }

    // Redraw
    glutPostRedisplay();
    glutTimerFunc(16, update, 0);
}

void drawScene() {
    glClear(GL_COLOR_BUFFER_BIT);

    if (inMainMenu) {
        drawMainMenu();
    } else {
        drawBackground();
        drawPlayer();
        drawObstacles();

        std::stringstream timerStream;
        timerStream << "Timer: " << minutes << ":" << seconds;
        drawText(10, HEIGHT - 20, timerStream.str());

        std::stringstream scoreStream;
        scoreStream << "Score: " << score;

        drawText(WIDTH - 100, HEIGHT - 20, scoreStream.str());
    }

    glutSwapBuffers();
}

void initOpenGL() {
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, WIDTH, 0, HEIGHT);
    glMatrixMode(GL_MODELVIEW);
}

void handleMouseClick(int button, int state, int x, int y) {
    if (inMainMenu && button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        handleMainMenuClick(x, HEIGHT - y);
    }
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(WIDTH, HEIGHT);
    glutCreateWindow("Challenging Game");

    glutDisplayFunc(drawScene);
    glutKeyboardFunc(handleKeypress);
    glutSpecialFunc(handleArrowKeypress);
    glutMouseFunc(handleMouseClick);

    initOpenGL();

    srand(time(0));

    for (int i = 0; i < 5; ++i) {
        obstacles.push_back(Obstacle(rand() % (WIDTH - 100) + 50, rand() % (HEIGHT - 50) + HEIGHT, 30, 2)); // Increased initial obstacle speed
    }

    glutTimerFunc(16, update, 0);

    glutMainLoop();

    return 0;
}
