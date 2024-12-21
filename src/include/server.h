#ifndef SERVER_H
#define SERVER_H

#include <cstdlib>
#include <iostream>

/*
 * You may need to define some global variables for the information of the game map here.
 * Although we don't encourage to use global variables in real cpp projects, you may have to use them because the use of
 * class is not taught yet. However, if you are member of A-class or have learnt the use of cpp class, member functions,
 * etc., you're free to modify this structure.
 */
char complete_map[31][31];
char displayed_map[31][31];
int visit_count = 0;
int marked_mine_count = 0;
int rows;         // The count of rows of the game map. You MUST NOT modify its name.
int columns;      // The count of columns of the game map. You MUST NOT modify its name.
int total_mines;  // The count of mines of the game map. You MUST NOT modify its name. You should initialize this
                  // variable in function InitMap. It will be used in the advanced task.
int game_state;  // The state of the game, 0 for continuing, 1 for winning, -1 for losing. You MUST NOT modify its name.

/**
 * @brief The definition of function InitMap()
 *
 * @details This function is designed to read the initial map from stdin. For example, if there is a 3 * 3 map in which
 * mines are located at (0, 1) and (1, 2) (0-based), the stdin would be
 *     3 3
 *     .X.
 *     ...
 *     ..X
 * where X stands for a mine block and . stands for a normal block. After executing this function, your game map
 * would be initialized, with all the blocks unvisited.
 */
void InitMap() {
  std::cin >> rows >> columns;
  // TODO (student): Implement me!
  total_mines = 0;
  int surrounding_mines = 0;
  for (int i = 0; i < rows; ++i) {
    std::cin >> complete_map[i];
  }
  for (int i = 0; i < rows; ++i) {
    for (int j = 0; j < columns; ++j) {
      //初始化displayed_map
      displayed_map[i][j] = '?';
      //初始化地雷总数
      if (complete_map[i][j] == 'X') {
        ++total_mines;
      }
      //初始化complete_map
      if (complete_map[i][j] == '.') {
        surrounding_mines = 0;
        for (int k = (i == 0 ? i : i - 1); k <= (i == rows - 1 ? i : i + 1); ++k) {
          for (int l = (j == 0 ? j : j - 1); l <= (j == columns - 1 ? j : j + 1); ++l) {
            if (complete_map[k][l] == 'X') {
              ++surrounding_mines;
            }
          }
        }
        complete_map[i][j] = surrounding_mines + '0';
      }
    }
  }
}

/**
 * @brief The definition of function VisitBlock(int, int)
 *
 * @details This function is designed to visit a block in the game map. We take the 3 * 3 game map above as an example.
 * At the beginning, if you call VisitBlock(0, 0), the return value would be 0 (game continues), and the game map would
 * be
 *     1??
 *     ???
 *     ???
 * If you call VisitBlock(0, 1) after that, the return value would be -1 (game ends and the players loses) , and the
 * game map would be
 *     1X?
 *     ???
 *     ???
 * If you call VisitBlock(0, 2), VisitBlock(2, 0), VisitBlock(1, 2) instead, the return value of the last operation
 * would be 1 (game ends and the player wins), and the game map would be
 *     1@1
 *     122
 *     01@
 *
 * @param r The row coordinate (0-based) of the block to be visited.
 * @param c The column coordinate (0-based) of the block to be visited.
 *
 * @note You should edit the value of game_state in this function. Precisely, edit it to
 *    0  if the game continues after visit that block, or that block has already been visited before.
 *    1  if the game ends and the player wins.
 *    -1 if the game ends and the player loses.
 *
 * @note For invalid operation, you should not do anything.
 */
void VisitBlock(int r, int c) {
  // TODO (student): Implement me!
  if (displayed_map[r][c] != '?') {//访问的格子状态已经访问或标记为雷
    game_state = 0;
  }else if(complete_map[r][c] == 'X') {//访问到地雷
    displayed_map[r][c] = 'X';
    game_state = -1;
  }else {//访问到非地雷
    displayed_map[r][c] = complete_map[r][c];
    ++visit_count;
    if (complete_map[r][c] == '0') {//访问到计数为0的格子
      for (int k = (r == 0 ? r : r - 1); k <= (r == rows - 1 ? r : r + 1); ++k) {
        for (int l = (c == 0 ? c : c - 1); l <= (c == columns - 1 ? c : c + 1); ++l) {
          if (complete_map[k][l] != 'X') {
            VisitBlock(k, l);
          }
        }
      }
    }
    if(visit_count + total_mines == rows * columns) {
      game_state = 1;
      marked_mine_count = total_mines;
      for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < columns; ++j) {
          if (displayed_map[i][j] == '?') {
            displayed_map[i][j] = '@';
          }
        }
      }
    }else {
      game_state = 0;
    }
  }
}

/**
 * @brief The definition of function MarkMine(int, int)
 *
 * @details This function is designed to mark a mine in the game map.
 * If the block being marked is a mine, show it as "@".
 * If the block being marked isn't a mine, END THE GAME immediately. (NOTE: This is not the same rule as the real
 * game) And you don't need to
 *
 * For example, if we use the same map as before, and the current state is:
 *     1?1
 *     ???
 *     ???
 * If you call MarkMine(0, 1), you marked the right mine. Then the resulting game map is:
 *     1@1
 *     ???
 *     ???
 * If you call MarkMine(1, 0), you marked the wrong mine(There's no mine in grid (1, 0)).
 * The game_state would be -1 and game ends immediately. The game map would be:
 *     1?1
 *     X??
 *     ???
 * This is different from the Minesweeper you've played. You should beware of that.
 *
 * @param r The row coordinate (0-based) of the block to be marked.
 * @param c The column coordinate (0-based) of the block to be marked.
 *
 * @note You should edit the value of game_state in this function. Precisely, edit it to
 *    0  if the game continues after visit that block, or that block has already been visited before.
 *    1  if the game ends and the player wins.
 *    -1 if the game ends and the player loses.
 *
 * @note For invalid operation, you should not do anything.
 */
void MarkMine(int r, int c) {
  // TODO (student): Implement me!
  if (displayed_map[r][c] != '?') {//标记的格子状态已经访问或标记为雷
    game_state = 0;
  }else if(complete_map[r][c] == 'X') {//标记的格子为地雷
    displayed_map[r][c] = '@';
    game_state = 0;
    ++marked_mine_count;
  }else {//标记的格子为非地雷
    displayed_map[r][c] = 'X';
    game_state = -1;
  }
}

/**
 * @brief The definition of function AutoExplore(int, int)
 *
 * @details This function is designed to auto-visit adjacent blocks of a certain block.
 * See README.md for more information
 *
 * For example, if we use the same map as before, and the current map is:
 *     ?@?
 *     ?2?
 *     ??@
 * Then auto explore is available only for block (1, 1). If you call AutoExplore(1, 1), the resulting map will be:
 *     1@1
 *     122
 *     01@
 * And the game ends (and player wins).
 */
void AutoExplore(int r, int c) {
  // TODO (student): Implement me!
  if (displayed_map[r][c] > '0' && displayed_map[r][c] < '8') {
    int surrounding_mines = 0;
    for (int k = (r == 0 ? r : r - 1); k <= (r == rows - 1 ? r : r + 1); ++k) {
      for (int l = (c == 0 ? c : c - 1); l <= (c == columns - 1 ? c : c + 1); ++l) {
        if (displayed_map[k][l] == '@') {
          ++surrounding_mines;
        }
      }
    }
    if (surrounding_mines + '0' == displayed_map[r][c]) {//自动访问
      for (int k = (r == 0 ? r : r - 1); k <= (r == rows - 1 ? r : r + 1); ++k) {
        for (int l = (c == 0 ? c : c - 1); l <= (c == columns - 1 ? c : c + 1); ++l) {
          if (displayed_map[k][l] == '?') {
            VisitBlock(k, l);
          }
        }
      }
    }else {//无法自动访问
      game_state = 0;
    }
  }else {//访问无效格子
    game_state = 0;
  }
}

/**
 * @brief The definition of function ExitGame()
 *
 * @details This function is designed to exit the game.
 * It outputs a line according to the result, and a line of two integers, visit_count and marked_mine_count,
 * representing the number of blocks visited and the number of marked mines taken respectively.
 *
 * @note If the player wins, we consider that ALL mines are correctly marked.
 */
void ExitGame() {
  // TODO (student): Implement me!
  if (game_state == 1) {
    std::cout << "YOU WIN!" << std::endl << visit_count << ' ' << marked_mine_count;
  }else if(game_state == -1) {
    std::cout << "GAME OVER!" << std::endl << visit_count << ' ' << marked_mine_count;
  }
  exit(0);  // Exit the game immediately
}

/**
 * @brief The definition of function PrintMap()
 *
 * @details This function is designed to print the game map to stdout. We take the 3 * 3 game map above as an example.
 * At the beginning, if you call PrintMap(), the stdout would be
 *    ???
 *    ???
 *    ???
 * If you call VisitBlock(2, 0) and PrintMap() after that, the stdout would be
 *    ???
 *    12?
 *    01?
 * If you call VisitBlock(0, 1) and PrintMap() after that, the stdout would be
 *    ?X?
 *    12?
 *    01?
 * If the player visits all blocks without mine and call PrintMap() after that, the stdout would be
 *    1@1
 *    122
 *    01@
 * (You may find the global variable game_state useful when implementing this function.)
 *
 * @note Use std::cout to print the game map, especially when you want to try the advanced task!!!
 */
void PrintMap() {
  // TODO (student): Implement me!
  for (int i = 0; i < rows; ++i) {
    std::cout << displayed_map[i] << std::endl;
  }
}

#endif
