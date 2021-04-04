#include <stdio.h>
#include <stddef.h>
#include "arcanoid.h"
#include <ncurses.h>
#include <unistd.h>
#include <math.h>


void initializePlayingField(t_player *player, int level)
{
    for (int i = 0; i < 5; i++)
    {
        for (int j = 0; j < 15; j++)
        {
            player->blocks[i * 15 + j] = malloc(sizeof(block));
            player->blocks[i * 15 + j]->start = malloc(sizeof(vector2i));
            player->blocks[i * 15 + j]->end = malloc(sizeof(vector2i));
            player->blocks[i * 15 + j]->start->x = 8 + j * 16;
            player->blocks[i * 15 + j]->start->y = 4 * i + 8;
            player->blocks[i * 15 + j]->end->x = 19 + j * 16;
            player->blocks[i * 15 + j]->end->y = 4 * i + 10;
            int ran = rand() % 100;
            int h = level < 6 ? 1 : level > 10 ? 5 : level - 5;
            while (ran > 50)
            {
                h = h >= 5 ? 5 : h == level ? level : h + 1;
                ran = rand() % 100;
            }
            player->blocks[i * 15 + j]->health = h;
            int giftPercent = rand() % 100;
            player->blocks[i * 15 + j]->gift = giftPercent < 90 ? 0 : giftPercent < 93 ? 4 : giftPercent < 96 ? 5 : giftPercent < 98 ? 1 : giftPercent < 99 ? 2 : 3;
        }
    }
}

void initializePlayer(t_player *player, int maxX, int maxY)
{
    player->power = 1;
    player->message = malloc(100);
    player->platform = malloc(sizeof(t_platform));
    player->platform->position = malloc(sizeof(vector2d));
    player->platform->position->x = maxX / 2;
    player->platform->position->y = maxY - 5;
    player->platform->stashedPosition = malloc(sizeof(vector2d));
    player->platform->stashedPosition->x = player->platform->position->x;
    player->platform->stashedPosition->y = player->platform->position->y;
    player->platform->length = standardSize;
    player->platform->effectId = 0;
    player->platform->buffTimer = 0;
    player->platform->speed = 5;
    for (int i = 0; i < 3; i++)
    {
        player->balls[i] = malloc(sizeof(ball));
        player->balls[i]->position = malloc(sizeof(vector2d));
        player->balls[i]->stashedPosition = malloc(sizeof(vector2d));
        player->balls[i]->direction = malloc(sizeof(vector2i));
        player->balls[i]->alive = false;
        player->balls[i]->speed = 1;
        player->balls[i]->lastPlatformTouch = 0;
    }
    for (int i = 0; i < 100; i++)
    {
        player->blocks[i] = malloc(sizeof(block));
        player->blocks[i]->start = malloc(sizeof(vector2i));
        player->blocks[i]->end = malloc(sizeof(vector2i));
        player->balls[i]->position = malloc(sizeof(vector2d));
        player->balls[i]->direction = malloc(sizeof(vector2i));
        player->blocks[i]->start->x = 0;
        player->blocks[i]->start->y = 0;
        player->blocks[i]->end->x = 0;
        player->blocks[i]->end->y = 0;
        player->blocks[i]->health = 0;
        player->blocks[i]->gift = 0;
    }
    spawnBall(player);
    initializePlayingField(player, 1);
}

void drawBlocks(block *blocks[])
{
    for (int i = 0; i < 100; i++)
    {
        drawBlock(blocks[i]);
    }
}

void drawBorder(int maxX, int maxY)
{
    for (int i = 3; i < maxX - 5; i++)
    {
        mvprintw(2, i, "-");
    }
    mvprintw(2, 2, "\U0001F577");
    mvprintw(2, maxX - 5, "\U0001F577");
    mvprintw(maxY - 3, 2, "\U0001F577");
    mvprintw(maxY - 3, maxX - 5, "\U0001F577");

    for (int i = 3; i < maxY - 3; i++)
    {
        mvprintw(i, 2, "|");
        mvprintw(i, maxX - 5, "|");
    }
        mvprintw(maxY - 5, 2, "<");
        mvprintw(maxY - 5, maxX - 5, ">");
}

void drawPlatform(t_platform *platform)
{
    for (int i = platform->stashedPosition->x - platform->length / 2; i <= platform->stashedPosition->x + platform->length / 2; i++)
    {
        mvprintw(platform->position->y, i, " ");
    }
    platform->stashedPosition->x = platform->position->x;
    platform->stashedPosition->y = platform->position->y;
    for (int i = platform->position->x - platform->length / 2 + 1; i < platform->position->x + platform->length / 2; i++)
    {
        mvprintw(platform->position->y, i, "=");
    }
    mvprintw(platform->position->y, platform->position->x - platform->length / 2, "<");
    mvprintw(platform->position->y, platform->position->x + platform->length / 2, ">");
}



int main()
{
    char *locale;
    locale = setlocale(LC_ALL, "");

    int maxX = 253;
    int maxY = 76;
    int level = 1;
    int currentPanel = 0; // 0 - main menu, 1 - game, 2 - info panel, 3 - after the game panel
    int menuSelectId = 0;

    WINDOW* mainwin = initscr();
    scrollok(mainwin, 1);

    start_color();
    init_pair(1, COLOR_BLACK, COLOR_BLACK);
    init_pair(2, COLOR_RED, COLOR_BLACK);
    init_pair(3, COLOR_YELLOW, COLOR_BLACK);
    init_pair(4, COLOR_GREEN, COLOR_BLACK);
    init_pair(5, COLOR_BLUE, COLOR_BLACK);
    init_pair(6, COLOR_CYAN, COLOR_BLACK);
    init_pair(7, COLOR_MAGENTA, COLOR_BLACK);
    init_pair(8, COLOR_WHITE, COLOR_BLACK);
    init_pair(9, COLOR_BLACK, COLOR_WHITE);
    curs_set(false);
    noecho();

    t_player *player = malloc(sizeof(t_player));

    while (1)
    {
        switch (currentPanel)
        {
            case 0:
                drawStart(mainwin, maxX / 2 - 45, maxY / 2 - 14, menuSelectId == 0);
                drawInfo(mainwin, maxX / 2 - 45, maxY / 2, menuSelectId == 1);
                drawQuit(mainwin, maxX / 2 - 45, maxY / 2 + 14, menuSelectId == 2);
                switch (getch())
                {
                    case '\033':
                        getch();
                        switch (getch())
                        {
                            case 'A': menuSelectId--;
                                break;
                            case 'B': menuSelectId++;
                                break;
                        }
                        menuSelectId = (menuSelectId + 3) % 3;
                        break;
                    case 0x0A:
                        if (menuSelectId == 2)
                        {
                            exit(0);
                        }
                        if (menuSelectId == 0)
                        {
                            clear();
                            initializePlayer(player, maxX, maxY);
                            player->points = 0;
                            player->health = 3;
                        }
                        currentPanel = menuSelectId + 1;
                        menuSelectId = 0;
                        break;
                }
                break;

            case 1:
                drawPlatform(player->platform);
                drawBlocks(player->blocks);
                for (int i = 1; i < maxX - 1; i++) mvprintw(maxY - 1, i, " ");
                mvprintw(1, 10, "Points:");
                mvprintw(1, 20, "%d", player->points);
                mvprintw(1, 40, "Level:");
                mvprintw(1, 50, "%d", level);
                mvprintw(1, 70, "Health:");
                mvprintw(1, 80, "%d", player->health);
                mvprintw(0, 5, "                                                                    ");
                if (clock() - player->messageFade < 500000)
                {
                    mvprintw(0, 5, player->message);
                }
                for (int i = 0; i < 3; i++)
                {
                    if (!player->balls[i]->alive) continue;
                    mvprintw(player->balls[i]->stashedPosition->y, player->balls[i]->stashedPosition->x, " ");
                    mvprintw(player->balls[i]->position->y, player->balls[i]->position->x, "\U0001F577");
                    player->balls[i]->stashedPosition->x = player->balls[i]->position->x;
                    player->balls[i]->stashedPosition->y = player->balls[i]->position->y;
                    if (player->balls[i]->position->y < player->platform->position->y) 
                        player->balls[i]->lastPlatformTouch = false;
                    currentPanel = checkForCollisionBorder(player, player->balls[i], maxX, maxY) == 1 ? 3 : currentPanel;
                    checkForPlatformCollision(player, player->balls[i]);
                    for (int j = 0; j < 100; j++)
                    {
                        if (player->blocks[j]->health > 0)
                            checkForCollisionBlock(player, player->balls[i], player->blocks[j]);
                    }
                }

                switch (getch())
                {
                    case 'q':
                        exit(0);
                        break;
                    case '\033':
                        getch();
                        switch (getch())
                        {
                            case 'D':
                                if (player->platform->position->x < 2 + player->platform->length / 2) break;
                                player->platform->position->x -= player->platform->speed;
                                if (player->platform->position->x < 2 + player->platform->length / 2) 
                                    player->platform->position->x = 2 + player->platform->length / 2;
                                break;
                            case 'C':
                                if (player->platform->position->x > maxX - 5 - player->platform->length / 2) break;
                                player->platform->position->x += player->platform->speed;
                                if (player->platform->position->x > maxX - 5 - player->platform->length / 2) 
                                    player->platform->position->x = maxX - 5 - player->platform->length / 2;
                                break;
                        }
                        break;
                }

                if (player->platform->effectId != 0 && clock() - player->platform->buffTimer > 30000)
                {
                    player->platform->length = standardSize;
                    player->platform->effectId = 0;
                }
                for (int i = 0; i < 3; i++)
                {
                    if (!player->balls[i]->alive) continue;
                    moveBall(player->balls[i]);
                }

                bool trigger = false;

                for (int i = 0; i < 100; i++)
                {
                    if (player->blocks[i]->health > 0)
                    {
                        timeout(TIMEOUT);
                        trigger = true;
                        break;
                    }
                }

                if (!trigger)
                {
                    level++;
                    for (int i = 0; i < 3; i++)
                    {
                        mvwprintw(mainwin, player->balls[i]->position->y, player->balls[i]->position->x, " ");
                        mvwprintw(mainwin, player->balls[i]->stashedPosition->y, player->balls[i]->stashedPosition->x, " ");
                    }
                    initializePlayer(player, maxX, maxY);
                    initializePlayingField(player, level);
                }
                break;

            case 2:
                outputInfo(mainwin, maxX, maxY);
                currentPanel = 0;
                break;

            case 3:
                for (int i = 0; i < 100; i++)
                {
                    for (int j = 0; j < 30; j++)
                    {
                        mvwprintw(mainwin, maxY / 2 - 15 + j, maxX / 2 - 50 + i, " ");
                    }
                }

                char *gameOverText[8] = { " _______   _______   _______   _______     _______             _______   _______ ",
                                          "(  ____ \\ (  ___  ) (       ) (  ____ \\   (  ___  ) |\\     /| (  ____ \\ (  ____ )",
                                          "| (    \\/ | (   ) | | /\\ /\\ | | (    \\/   | (   ) | | )   ( | | (    \\/ | (    )|",
                                          "| |       | (___) | | || || | | (__       | |   | | | |   | | | (__     | (____)|",
                                          "| | ____  |  ___  | | | V | | |  __)      | |   | | ( (   ) ) |  __)    |     __)",
                                          "| | \\_  ) | (   ) | | |   | | | (         | |   | |  \\ \\_/ /  | (       | (\\ (   ",
                                          "| (___) | | )   ( | | )   ( | | (____/\\   | (___) |   \\   /   | (____/\\ | ) \\ \\__",
                                          "(_______) |/     \\| |/     \\| (_______/   (_______)    \\_/    (_______/ |/   \\__/" };

                char *numbers[10][8] = 
                {
                    {
                        " _______  ",
                        "(  __   ) ",
                        "| (  )  | ",
                        "| | /   | ",
                        "| (/ /) | ",
                        "|   / | | ",
                        "|  (__) | ",
                        "(_______) "
                    },
                    {
                        "  __      ",
                        " /  \\     ",
                        " \\/) )    ",
                        "   | |    ",
                        "   | |    ",
                        "   | |    ",
                        " __) (_   ",
                        " \\____/   "
                    },
                    {
                        " _______  ",
                        "/ ___   ) ",
                        "\\/   )  | ",
                        "    /   ) ",
                        "  _/   /  ",
                        " /   _/   ",
                        "(   (__/\\ ",
                        "\\_______/ "
                    },
                    {
                        " ______   ",
                        "/ ___  \\  ",
                        "\\/   \\  \\ ",
                        "   ___) / ",
                        "  (___ (  ",
                        "      ) \\ ",
                        "/\\___/  / ",
                        "\\______/  "
                    },
                    {
                        "    ___   ",
                        "   /   )  ",
                        "  / /) |  ",
                        " / (_) (_ ",
                        "(____   _)",
                        "     ) (  ",
                        "     | |  ",
                        "     (_)  "
                    },
                    {
                        " _______  ",
                        "(  ____ \\ ",
                        "| (    \\/ ",
                        "| (____   ",
                        "(_____ \\  ",
                        "      ) ) ",
                        "/\\____) ) ",
                        "\\______/  "
                    },
                    {
                        "  ______  ",
                        " / ____ \\ ",
                        "( (    \\/ ",
                        "| (____   ",
                        "|  ___ \\  ",
                        "| (   ) ) ",
                        "( (___) ) ",
                        " \\_____/  "
                    },
                    {
                        " ______   ",
                        "/ ___  \\  ",
                        "\\/   )  ) ",
                        "    /  /  ",
                        "   /  /   ",
                        "  /  /    ",
                        " /  /     ",
                        " \\_/      "
                    },
                    {
                        "  _____   ",
                        " / ___ \\  ",
                        "( (___) ) ",
                        " \\     /  ",
                        " / ___ \\  ",
                        "( (   ) ) ",
                        "( (___) ) ",
                        " \\_____/  "
                    },
                    {
                        "  _____   ",
                        " / ___ \\  ",
                        "( (   ) ) ",
                        "( (___) | ",
                        " \\____  | ",
                        "      ) | ",
                        "/\\____) ) ",
                        "\\______/  "
                    }
                };

                for (int i = 0; i < 8; i++)
                {
                    mvwprintw(mainwin, maxY / 2 - 10 + i, maxX / 2 - 41, gameOverText[i]);
                }

                char *result = numberToString(player->points);

                for (int r = 0; r < 8; r++)
                {
                    for (unsigned long i = 0; i < strlen(result); i++)
                    {
                        mvwprintw(mainwin, maxY / 2 + 2 + r, maxX / 2 - strlen(result) * 6 + i * 12, numbers[result[i] - 48][r]);
                    }
                }

                mvwprintw(mainwin, maxY / 2 + 15, maxX / 2 - 9, "Press 'R' to retry.");
                mvwprintw(mainwin, maxY / 2 + 16, maxX / 2 - 16, "Press 'B' to return to main menu.");
                mvwprintw(mainwin, maxY / 2 + 17, maxX / 2 - 9, "Press 'Q' to quit.");

                switch (getch())
                {
                    case 'b':
                        clear();
                        currentPanel = 0;
                        break;
                    case 'r':
                        clear();
                        initializePlayer(player, maxX, maxY);
                        player->points = 0;
                        player->health = 3;
                        currentPanel = 1;
                     break;
                    case 'q':
                        exit(0);
                     break;
                }
                break;
        }
        drawBorder(maxX, maxY);
        refresh();
        timeout(TIMEOUT);
    }
    delwin(mainwin);
    endwin();
    return 0;
}
