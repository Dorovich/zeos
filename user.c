#include <libc.h>
#include <colors.h>

#define NULL (void *)0

#define SCREEN_HEIGHT   23
#define SCREEN_WIDTH    80

struct sem_t *sem_draw, *sem_update, *sem_main;

#define EMPTY   0
#define PLAYER  1
#define ENEMY   2
#define ROCK    3
#define BULLET  4

// TODO crear matriz para el logo
int screen[SCREEN_HEIGHT][SCREEN_WIDTH];
char c;
char msg[64];
int hasRead = 0;

enum moveStates { IDLE, LEFT, RIGHT };

int move = IDLE;
int shoot = 0;
int reload = 0;
int enemyMove = RIGHT;
int numEnemies = 0;
int end_game = 0;
int score = 0;

void error() {
    char str[64] = "(ERROR!)";
    write(1, str, sizeof(str));
    end_game = 1;
    exit();
}

void print(const char *s) {
    write(1, s, strlen(s));
}

void input(void *ingored) {
    int err;
    while (1) {
        if (end_game) exit();
        err = waitKey(&c, 1);
        if (!err) hasRead = 1;
    }
}

void update(void *ignored) {
    while (1) {
        if (end_game) exit();
        semWait(sem_draw);
        // HANDLES INPUTS
        if (hasRead) {
            switch (c) {
            case 'w':
                if (reload == 0) {
                    shoot = 1;
                    reload = 10;
                }
                break;
            case 'a':
                move = LEFT;
                break;
            case 'd':
                move = RIGHT;
                break;
            case 'k':
                semSignal(sem_main);
                break;
            }
            hasRead = 0;
        }

        // HANDLE RELOAD
        if (reload > 0) --reload;

        // HANDLES ENEMY DIRECTION CHANGES
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
                    if (move == LEFT && j>0) {
                        if (j>0) {
                            screen[i][j] = EMPTY;
                            screen[i][j-1] = PLAYER;
                        }
                        move = IDLE;
                    }
                    else if (move == RIGHT) {
                        if (j<SCREEN_WIDTH-1) {
                            screen[i][j] = EMPTY;
                            screen[i][j+1] = PLAYER;
                            ++j;
                        }
                        move = IDLE;
                    }
                    if (shoot) {
                        screen[i-1][j] = BULLET;
                        shoot = 0;
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
                        score += 100;
                    }
                    else if (screen[i-1][j] == ROCK) {
                        screen[i-1][j] = EMPTY;
                        screen[i][j] = EMPTY;
                        score -= 5;
                    }
                    else {
                        screen[i][j] = EMPTY;
                        screen[i-1][j] = BULLET;
                    }
                    break;
                default:
                    break;
                }
            }
        }
        semSignal(sem_update);
    }
}

void draw(void *ignored) {
    while (1) {
        if (end_game) exit();
        semWait(sem_update);
        gotoXY(0, 2);
        for (int i = 0; i < SCREEN_HEIGHT; ++i) {
            for (int j = 0; j < SCREEN_WIDTH; ++j) {
                switch (screen[i][j]) {
                case EMPTY:
                    changeColor(C_BLACK, C_BLACK);
                    print(" ");
                    break;
                case PLAYER:
                    changeColor(C_GREEN, C_BLACK);
                    print("A");
                    break;
                case ENEMY:
                    changeColor(C_RED, C_BLACK);
                    print("W");
                    break;
                case ROCK:
                    changeColor(C_BLACK, C_BROWN);
                    print(" ");
                    break;
                case BULLET:
                    changeColor(C_WHITE, C_BLACK);
                    print("i");
                    break;
                default:
                    break;
                }
            }
        }
        gotoXY(3, 0);
        changeColor(C_GREEN, C_BLACK);
        print("Score: ");
        itoa(score, msg);
        changeColor(C_WHITE, C_BLACK);
        print(msg);
        semSignal(sem_draw);
    }
}

void init_screen () {
    for (int i = 0; i < SCREEN_HEIGHT; ++i) {
        for (int j = 0; j < SCREEN_WIDTH; ++j) {
            if (i%2 == 1 && i < 10 && j%2 == 1 && j < 30)
                screen[i][j] = ENEMY;
            else if ((i == 16 || i == 15) && (j%10 == 3 || j%10 == 4 || j%10 == 5))
                screen[i][j] = ROCK;
            else if (i == 20 && j == 40) screen[i][j] = PLAYER;
            else screen[i][j] = EMPTY;
        }
    }
}

int __attribute__ ((__section__(".text.main")))
main(void)
{
    clrscr(NULL);

    sem_update = semCreate(1);
    sem_draw = semCreate(0);
    sem_main = semCreate(0);

    int ret;
    init_screen();
    ret = threadCreateWithStack(input, 1, NULL);
    if (ret<0) error();
    ret = threadCreateWithStack(update, 1, NULL);
    if (ret<0) error();
    ret = threadCreateWithStack(draw, 1, NULL);
    if (ret<0) error();

    semWait(sem_main);

    print("MAIN HA VUELTO\n");
    end_game = 1;

    while(1);
}
