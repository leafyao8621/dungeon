#ifndef _MODEL_H_
#define _MODEL_H_

#include <stdio.h>

#define DIR_RIGHT 0
#define DIR_UP 1
#define DIR_DOWN 2
#define DIR_LEFT 3

struct Creature {
    int level;
    int hp;
    int mp;
    int atk;
    int def;
    int agi;
    struct {
        int type;
        struct {
            int level;
            int stat;
        } stats[5];
    } magic;
};

struct Map {
    int stage;
    int next_enemy;
    int next_hp;
    int next_mp;
    int next_magic;
    int exp;
    int req;
    struct {
        int row;
        int col;
        int idx;
        int dir;
    } pos;
    char map[225];
};

extern const char *dir_map;
extern const char **magic_map;

extern struct Creature player;
extern struct Creature enemy;
extern struct Map map;

void model_new(void);
int model_save(int n);
int model_load(int n);
void model_move(int dir);
void model_log(FILE *fout);

#endif
