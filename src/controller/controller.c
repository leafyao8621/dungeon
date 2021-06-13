#include <ncurses.h>
#include "controller.h"

#define STATE_START_SCREEN 0

static char state = 0;
static char selection[10], *selection_ptr;

static void render_start_screen(void) {
    mvaddstr(1, 0, "New");
    mvaddstr(2, 0, "Load");
    move(*selection_ptr + 1, 0);
}

void controller_initialize(void) {
    initscr();
    keypad(stdscr, TRUE);
    noecho();
    selection_ptr = selection;
    *selection_ptr = 0;
    state = STATE_START_SCREEN;
    mvaddstr(0, 0, "Dungeon");
    render_start_screen();
}

void controller_finalize(void) {
    endwin();
}

int controller_handle(void) {
    int in = getch();
    switch (in) {
    case 'Q':
    case 'q':
        return 1;
    case KEY_UP:
    case 'W':
    case 'w':
        switch (state) {
        case STATE_START_SCREEN:
            ++*selection_ptr;
            *selection_ptr &= 1;
            move(*selection_ptr + 1, 0);
            break;
        }
        break;
    case KEY_DOWN:
    case 'S':
    case 's':
        switch (state) {
        case STATE_START_SCREEN:
            --*selection_ptr;
            *selection_ptr &= 1;
            move(*selection_ptr + 1, 0);
            break;
        }
        break;
    }
    return 0;
}
