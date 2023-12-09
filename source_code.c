#include <stdio.h>
#include <curses.h>
#include <time.h>
#include <unistd.h>
#include <sys/stat.h>
#include <stdlib.h>

void intro();
void show_rules();
void game_start();
void print_field(char nebo[16][16], int destroyed_targets, int not_captured);
void game();
void generate_target(char nebo[16][16]);
void move_targets(char nebo[16][16]);
int move_aim(char nebo[16][16], int *aim_x, int *aim_y, int *explosion_timer, int *destroyed_targets, int *not_captured);
void explode(char nebo[16][16], int x, int y, int *destroyed_targets, int *not_captured);
void clear_explosions(char nebo[16][16]);
void report(int destroyed_targets, int not_captured);
void save_result(int destroyed_targets, int not_captured);

int main() {
    initscr();
    start_color();
    resize_term(150, 150);
    nodelay(stdscr, TRUE);
    timeout(100);
    keypad(stdscr, TRUE);
    init_pair(1, COLOR_RED, COLOR_BLACK);
    init_pair(2, COLOR_GREEN, COLOR_BLACK);
    init_pair(3, COLOR_YELLOW, COLOR_BLACK);
    init_pair(4, COLOR_WHITE, COLOR_RED);
    attron(COLOR_PAIR(4));
    intro();
    attroff(COLOR_PAIR(4));
    attron(COLOR_PAIR(1));
    
    show_rules();
    
    getch();
    endwin();
    return 1;
}

// Display the name of game
void intro(){
	resize_term(50, 250);
	printw("\t $    $ $$$$$   $$$$  $$$$$$ $$  $$     $$$$  $$$$$$     $$$$   $$$$  $$$$$  \t\n");
	printw("\t $    $ $$  $$ $$  $$   $$   $$  $$    $$  $$ $$        $$     $$  $$ $$  $$ \t\n");
	printw("\t $ $$ $ $$$$$  $$$$$$   $$   $$$$$$    $$  $$ $$$$      $$ $$$ $$  $$ $$  $$ \t\n");
	printw("\t $$$$$$ $$  $$ $$  $$   $$   $$  $$    $$  $$ $$        $$  $$ $$  $$ $$  $$ \t\n");
	printw("\t  $$ $$ $$  $$ $$  $$   $$   $$  $$     $$$$  $$         $$$$   $$$$  $$$$$  \t\n");
	refresh();
	sleep(2);
	clear();
}


// Display game rules and countdown before starting the game
void show_rules() {
    int time = 15;
    attroff(COLOR_PAIR(1));
    clear();
    printw("\nRules:\nYou play as a strike drone operator.\nYour main task is to prevent the russians from reaching the Kyiv(other side of screen).\n");
    printw("If you manage to hit the target, it is marked as an D(like a coffin).");
    printw("\nYou can drop the bomb using the space bar, the area of impact is 3x3.\n"); 
    printw("Good luck and happy hunting!\n");
    printw("Aim: x | Target: < \n\n");
    printw("###\n");
    printw("#x# <-- explosion area\n");
    printw("###\n");
    printw("\n\t\tControls:\n\tArrows v, ^, <, >, to move the aim.\n\tSpace to capture the target and trigger explosion.");

    // Countdown before starting the game
    while (time != 3) {
        sleep(1);
        time--;
        mvprintw(17, 10, "Game starts in %d", time);
        refresh();
    }
    
    clear();
    game_start();
    clear();
}

// Display the introductory animation
void game_start() {
    attroff(COLOR_PAIR(1));
    attron(COLOR_PAIR(2));
    printw("\t\t\t\t\t #### \n");
    printw("\t\t\t\t\t#   ##\n");
    printw("\t\t\t\t\t  ### \n");
    printw("\t\t\t\t\t#   ##\n");
    printw("\t\t\t\t\t #### \n");
    attroff(COLOR_PAIR(2));
    refresh();
    sleep(1);
    clear();
    
    attron(COLOR_PAIR(3));
    printw("\t\t\t\t\t #### \n");
    printw("\t\t\t\t\t##  ##\n");
    printw("\t\t\t\t\t   ## \n");
    printw("\t\t\t\t\t ##   \n");
    printw("\t\t\t\t\t######\n");
    attroff(COLOR_PAIR(3));
    refresh();
    sleep(1);
    clear();
    
    attron(COLOR_PAIR(1));
    printw("\t\t\t\t\t  ##\n");
    printw("\t\t\t\t\t####\n");
    printw("\t\t\t\t\t  ##\n");
    printw("\t\t\t\t\t  ##\n");
    printw("\t\t\t\t\t  ##\n");
    refresh();
    sleep(1);
    clear();
    
    attron(COLOR_PAIR(2));
    game();
    attroff(COLOR_PAIR(2));
}

// Display the game field
void print_field(char nebo[16][16], int destroyed_targets, int not_captured) {
    clear();
    printw("Destroyed Targets: %d | Amount of targets: %d\n", destroyed_targets, not_captured);
    for (int i = 0; i < 15; i++) {
        for (int j = 0; j < 15; j++) {
            printw("%c ", nebo[i][j]);
        }
        printw("\n");
    }
}

// Generate a target on the game field
void generate_target(char nebo[16][16]) {
    int target_x = rand() % 15;
    int target_y = rand() % 15;

    if (nebo[target_y][target_x] == '.') {
        nebo[target_y][target_x] = '<';
    }
}

// Move targets on the game field
void move_targets(char nebo[16][16]) {
    for (int i = 14; i >= 0; i--) {
        for (int j = 0; j < 15; j++) {
            if (nebo[i][j] == '<') {
                nebo[i][j] = '.';
                if (j > 0) {
                    nebo[i][j - 1] = '<';
                } else {
                    nebo[i][14] = '<';
                }
            }
        }
    }
}

// Move the aim based on user input
int move_aim(char nebo[16][16], int *aim_x, int *aim_y, int *explosion_timer, int *destroyed_targets, int *not_captured) {
    static int counter = 0;
    nebo[*aim_y][*aim_x] = '.';
    int input = getch();
    if (input != ERR) {
        switch (input) {
            case KEY_UP:
                if (*aim_y > 0) {
                    (*aim_y)--;
                }
                break;
            case KEY_DOWN:
                if (*aim_y < 14) {
                    (*aim_y)++;
                }
                break;
            case KEY_LEFT:
                if (*aim_x > 0) {
                    (*aim_x)--;
                }
                break;
            case KEY_RIGHT:
                if (*aim_x < 14) {
                    (*aim_x)++;
                }
                break;
            case ' ':
                if (*explosion_timer <= 0) {
                    explode(nebo, *aim_x, *aim_y, destroyed_targets, not_captured);
                    *explosion_timer = 10;
                    counter = 0;
                }
                break;
            default:
                break;
        }
        counter++;
    }
    if (*explosion_timer > 0) {
        explode(nebo, *aim_x, *aim_y, destroyed_targets, not_captured);
        *explosion_timer = (*explosion_timer > 1) ? *explosion_timer - 1 : 0;
        counter = 0;
    }
    nebo[*aim_y][*aim_x] = 'x';
    if (counter <= 2) {
        return 1;
    }
    return 1;
}

// Trigger an explosion on the game field
void explode(char nebo[16][16], int x, int y, int *destroyed_targets, int *not_captured) {
    for (int i = y - 1; i <= y + 1; i++) {
        for (int j = x - 1; j <= x + 1; j++) {
            if (i >= 0 && i < 15 && j >= 0 && j < 15) {
                if (nebo[i][j] == '<') {
                    (*destroyed_targets)++;
                    nebo[i][j] = 'D';
                }
            }
        }
    }
    print_field(nebo, *destroyed_targets, *not_captured);
    clear_explosions(nebo);
    print_field(nebo, *destroyed_targets, *not_captured);
}

// Clear explosion effects on the game field
void clear_explosions(char nebo[16][16]) {
    for (int i = 0; i < 15; i++) {
        for (int j = 0; j < 15; j++) {
            if (nebo[i][j] == '#') {
                nebo[i][j] = '.';
            }
        }
    }
}

void report(int destroyed_targets, int not_captured) {
    clear();
    printw("Game Over!\n");
    printw("Destroyed Targets: %d | From: %d\n", destroyed_targets, not_captured);
    refresh();
    sleep(3);
}

void save_result(int destroyed_targets, int not_captured) {
    system("echo. > results.txt");
    FILE *file = fopen("results.txt", "w");
    if (file != NULL) {
        fprintf(file, "Results:\n");
        fprintf(file, "Destroyed Targets: %d | From: %d\n", destroyed_targets, not_captured);
        fclose(file);
    } else {
        printw("Error: Unable to open best.txt for writing\n");
    }
}

// Main game loop
void game() {
    resize_term(150, 150);

    char nebo[16][16];
    for (int i = 0; i != 15; i++) {
        for (int j = 0; j != 15; j++) {
            nebo[i][j] = '.';
        }
    }

    char aim = 'x';

    int aim_x = 16 / 2 - 1;
    int aim_y = 16 / 2 - 1;

    nebo[aim_y][aim_x] = aim;
    print_field(nebo, 0, 0);

    int timer = 0;
    int explosion_timer = 0;
    int destroyed_targets = 0;
    int not_captured = 0;

    while (timer < 600) {
        if (timer % 15 == 0) {
            generate_target(nebo);
            not_captured++;
        }

        if (explosion_timer > 0) {
            explosion_timer--;
        }

        if (move_aim(nebo, &aim_x, &aim_y, &explosion_timer, &destroyed_targets, &not_captured)) {
            move_targets(nebo);
            clear_explosions(nebo);
            print_field(nebo, destroyed_targets, not_captured);
            timer++;
            sleep(0.5);
        }
    }

    report(destroyed_targets, not_captured);
    save_result(destroyed_targets, not_captured);
}
