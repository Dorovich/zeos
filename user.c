#include <libc.h>
#include <colors.h>

#define NULL (void *)0

#define SCREEN_HEIGHT   25
#define SCREEN_WIDTH    80

struct sem_t *sem_input, *sem_update, *sem_main;

#define EMPTY   0
#define PLAYER  1
#define ENEMY   2
#define ROCK    3
#define BULLET  4

// TODO crear matriz para el logo
int screen[SCREEN_HEIGHT][SCREEN_WIDTH];
char c;
int hasRead = 0;

enum moveStates { IDLE, LEFT, RIGHT };

int move = IDLE;
int enemyMove = RIGHT;
int numEnemies = 0;

void error() {
    char str[64] = "(ERROR!)";
    write(1, str, sizeof(str));
}

void print(const char *s) {
    write(1, s, strlen(s));
}

void input(void *ingored) {
    int err;
    while (1) {
        err = waitKey(&c, 1); // hacer que si hasRead == 1 no lea
        if (!err) hasRead = 1;
    }
}

void update(void *ignored) {
    while (1) {
        // HANDLES INPUTS
        if (hasRead) {
            switch (c) {
            case 'w':
                // disparar
                break;
            case 'a':
                move = 1;
                break;
            case 'd':
                move = 2;
                break;
            }
            hasRead = 0;
        }

        // HANDLES ENEMY DIRACTION CHANGES
        for (int i = 0; i < SCREEN_HEIGHT; ++i) {
            if (enemyMove == LEFT && screen[i][0]) {
                enemyMove = RIGHT;
                break;
            }
            else if (enemyMove == RIGHT && screen[i][SCREEN_WIDTH-1]) {
                enemyMove = LEFT;
                break;
            }
        }

        // UPDATES THE SCREEN
        for (int i = 0; i < SCREEN_HEIGHT; ++i) {
            for (int j = 0; j < SCREEN_WIDTH; ++j) {
                switch (screen[i][j]) {
                case PLAYER:
                    if (move == LEFT) {
                        screen[i][j] = EMPTY;
                        screen[i][j-1] = PLAYER;
                        move = IDLE;
                    }
                    else if (move == RIGHT) {
                        screen[i][j] = EMPTY;
                        screen[i][j+1] = PLAYER;
                        ++j;
                        move = IDLE;
                    }
                    break;
                case ENEMY:
                    if (enemyMove == LEFT) {
                        screen[i][j] = EMPTY;
                        screen[i][j-1] = ENEMY;
                    }
                    else if (enemyMove == RIGHT) {
                        screen[i][j] = EMPTY;
                        screen[i][j+1] = ENEMY;
                        ++j;
                    }
                    break;
                case BULLET:
                    if (screen[i-1][j] == ENEMY) {
                        screen[i-1][j] = EMPTY;
                        screen[i][j] = EMPTY;
                        --numEnemies;
                    }
                    else if (screen[i-1][j] == ROCK) {
                        screen[i-1][j] = EMPTY;
                        screen[i][j] = EMPTY;
                    }
                    else {
                        screen[i][j] = EMPTY;
                        screen[i-1][j] = BULLET;
                    }
                    break;
                }
            }
        }
    semSignal(sem_update);
    }
}

void draw(void *ignored) {
    while (1) {
        semWait(sem_update);
        for (int i = 0; i < SCREEN_HEIGHT; ++i) {
            for (int j = 0; j < SCREEN_WIDTH; ++j) {
                switch (screen[i][j]) {
                case EMPTY:
                    changeColor(C_BLACK, C_BLACK);
                    print(" ");
                    break;
                case PLAYER:
                    changeColor(C_BLACK, C_YELLOW);
                    print(" ");
                    break;
                case ENEMY:
                    changeColor(C_BLACK, C_RED);
                    print(" ");
                    break;
                case ROCK:
                    changeColor(C_BLACK, C_BROWN);
                    print(" ");
                    break;
                case BULLET:
                    changeColor(C_BLACK, C_CYAN);
                    print(" ");
                    break;
                }
            }
        }
    }
}

int __attribute__ ((__section__(".text.main")))
main(void)
{
    print("   Codigo de usuario ejecutandose:\n");

    /* sem_read = semCreate(0); */
    sem_update = semCreate(0);
    sem_main = semCreate(0);

    int ret;

    ret = threadCreateWithStack(input, 1, NULL);
    if (ret<0) error();
    ret = threadCreateWithStack(update, 1, NULL);
    if (ret<0) error();
    ret = threadCreateWithStack(draw, 1, NULL);
    if (ret<0) error();

    print("   Empezando juego:\n");
    // Imprimir Logo
    /* changeColor(C_BLACK, C_BLACK); */
    /* clrscr(screen); */

    semWait(sem_main);

    while(1);
}
