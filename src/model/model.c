#include <time.h>
#include <string.h>

#include "model.h"
#include "../util/mt19937.h"

#define MAGIC_NONE -1
#define MAGIC_EARTH 0
#define MAGIC_ICE 1
#define MAGIC_FIRE 2
#define MAGIC_WATER 3
#define MAGIC_WIND 4

const char *dir_map = ">^v<";

static const char *magic_types[6] = {
    "None",
    "Earth",
    "Ice",
    "Fire",
    "Water",
    "Wind"
};
const char **magic_map = magic_types + 1;

struct Creature player;
struct Creature enemy;
struct Map map;

static struct MT19937 gen;


void model_new(void) {
    mt19937_initialize(&gen, time(0));
    map.stage = 1;
    memset(map.map, 0, 225);
    int cur = mt19937_gen(&gen) % 9;
    map.pos.row = cur;
    map.pos.col = 0;
    map.pos.dir = DIR_RIGHT;
    map.pos.idx = cur;
    map.exp = 0;
    map.req = 1000;
    player.level = 1;
    player.hp = 10000;
    player.mp = 1000;
    player.atk = 1000;
    player.def = 500;
    player.agi = 500;
    player.magic.type = MAGIC_NONE;
    for (char i = 0; i < 5; ++i) {
        player.magic.stats[i].level = 1;
        player.magic.stats[i].stat = 100;
    }
    for (; cur / 9 < 25;) {
        int idx = 0;
        do {
            int dir = mt19937_gen(&gen) % 3;
            switch (dir) {
            case DIR_RIGHT:
                idx = cur + 9;
                break;
            case DIR_UP:
                idx = cur - 1;
                break;
            case DIR_DOWN:
                idx = cur + 1;
                break;
            }
        } while (map.map[idx]);
        map.map[idx] = 1;
        cur = idx;
    }
    for (unsigned char i = 0, *ii = map.map; i < 225; ++i, ++ii) {
        if (mt19937_gen(&gen) & 1) {
            *ii = 1;
        }
    }
    enemy.level = 1;
    enemy.hp = 1000;
    enemy.mp = 100;
    enemy.atk = 500;
    enemy.def = 200;
    enemy.agi = 200;
    enemy.magic.type = mt19937_gen(&gen) % 5;
    enemy.magic.stats[enemy.magic.type].level = 1;
    enemy.magic.stats[enemy.magic.type].stat = 100;
}

int model_save(int n) {
    static char dir[12];
    if (n < 0 || n > 9) {
        return 1;
    }
    snprintf(dir, 12, "saves/%d.sav", n);
    FILE *fout = fopen(dir, "wb");
    fwrite(&player, sizeof(player), 1, fout);
    fwrite(&enemy, sizeof(enemy), 1, fout);
    fwrite(&map, sizeof(map), 1, fout);
    fwrite(&gen, sizeof(gen), 1, fout);
    fclose(fout);
    return 0;
}

int model_load(int n) {
    static char dir[12];
    if (n < 0 || n > 9) {
        return 1;
    }
    snprintf(dir, 12, "saves/%d.sav", n);
    FILE *fin = fopen(dir, "rb");
    if (!fin) {
        return 2;
    }
    fread(&player, sizeof(player), 1, fin);
    fread(&enemy, sizeof(enemy), 1, fin);
    fread(&map, sizeof(map), 1, fin);
    fread(&gen, sizeof(gen), 1, fin);
    fclose(fin);
    return 0;
}

void model_move(int dir) {

}

void model_log(FILE *fout) {
    static const char *dir_map_log = "v><^";
    fputs("Player:\n", fout);
    fprintf(fout, "Level: %d\n", player.level);
    fprintf(fout, "HP: %d\n", player.hp);
    fprintf(fout, "MP: %d\n", player.mp);
    fprintf(fout, "ATK: %d\n", player.atk);
    fprintf(fout, "DEF: %d\n", player.def);
    fprintf(fout, "AGI: %d\n", player.agi);
    fputs("Magic:\n", fout);
    fprintf(fout, "Type: %s\n", magic_map[player.magic.type]);
    fputs("Stats:\n", fout);
    for (char i = 0; i < 5; ++i) {
        fprintf(fout, "%s:\n", magic_types[i]);
        fprintf(fout, "Level: %d\n", player.magic.stats[i].level);
        fprintf(fout, "Stat: %d\n", player.magic.stats[i].stat);
    }
    fprintf(fout, "EXP: %d\n", map.exp);
    fprintf(fout, "Next Level: %d\n\n", map.req);

    fputs("Enemy:\n", fout);
    fprintf(fout, "Level: %d\n", enemy.level);
    fprintf(fout, "HP: %d\n", enemy.hp);
    fprintf(fout, "MP: %d\n", enemy.mp);
    fprintf(fout, "ATK: %d\n", enemy.atk);
    fprintf(fout, "DEF: %d\n", enemy.def);
    fprintf(fout, "AGI: %d\n", enemy.agi);
    fputs("Magic:\n", fout);
    fprintf(fout, "Type: %s\n", magic_map[enemy.magic.type]);
    fprintf(fout, "Level: %d\n", enemy.magic.stats[enemy.magic.type].level);
    fprintf(fout, "Stat: %d\n\n", enemy.magic.stats[enemy.magic.type].stat);

    fputs("Steps:\n", fout);
    fprintf(fout, "Enemy: %d\n", map.next_enemy);
    fprintf(fout, "HP: %d\n", map.next_hp);
    fprintf(fout, "MP: %d\n", map.next_mp);
    fprintf(fout, "Magic: %d\n\n", map.next_magic);

    fputs("Map:\n", fout);
    fprintf(fout, "Stage: %d\n", map.stage);
    char *iter = map.map;
    for (char i = 0; i < 25; ++i) {
        for (char j = 0; j < 9; ++j, ++iter) {
            if (i == map.pos.col && j == map.pos.row) {
                fputc(dir_map_log[map.pos.dir], fout);
            } else {
                fputc(*iter ? ' ' : '*', fout);
            }
        }
        fputc(10, fout);
    }
}
