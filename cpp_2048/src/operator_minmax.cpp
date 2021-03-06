#include <algorithm>
#include <cstdlib>
#include "gamecore.h"
#include "log.h"
#include "operator.h"
using namespace std;

// 声明并定义一个tmp，为old的copy
#define newBoard(tmp, old)                                                                                             \
    Board tmp {                                                                                                        \
        old[0][0], old[0][1], old[0][2], old[0][3], old[1][0], old[1][1], old[1][2], old[1][3], old[2][0], old[2][1],  \
            old[2][2], old[2][3], old[3][0], old[3][1], old[3][2], old[3][3]                                           \
    }

//  0  1  2  3
//  4  5  6  7
//  8  9 10 11
// 12 13 14 15
#define board_num(i) (*(&num[0][0] + i))

const int maxdep = 4;
const int INF = 0x7FFFFFFF;

int get_point(const Board& num) {
    int point = 0;
    // 基础计数
    int max_val = 0, cnt[20] = {};
    for (int i = 0; i < 16; i++) {
        if (board_num(i) > max_val) max_val = board_num(i);
        cnt[board_num(i)]++;
    }
    // 最大值大小
    point += (20 << max_val);
    // 最大值位于角落
    // if (board_num(0) == max_val || board_num(3) == max_val || board_num(12) == max_val || board_num(15) == max_val)
    if (board_num(0) == max_val) point += 20000;
    // 行列向角落单调
    auto num_check = [&](int x0, int y0, int dx, int dy) {
        int tmp = 0;
        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 4; j++) {
                int x = x0 + i * dx;
                int y = y0 + j * dy;
                if (x + dx >= 0 && x + dx < 4) tmp += (1 << num[x][y]) - (1 << num[x + dx][y]);
                if (y + dy >= 0 && y + dy < 4) tmp += (1 << num[x][y]) - (1 << num[x][y + dy]);
                if (x + dx >= 0 && x + dx < 4 && y + dy >= 0 && y + dy < 4)
                    tmp += ((1 << num[x][y]) - (1 << num[x + dx][y + dy])) / 10;
            }
        return tmp;
    };
    // point += max(max(num_check(0, 0, 1, 1), num_check(0, 3, 1, -1)),
    //              max(num_check(3, 0, -1, 1), num_check(3, 3, -1, -1)));

    point += num_check(0, 0, 1, 1);
    // 空余数量
    point += 100 * cnt[0];

    // 大数字每个只有一个
    for (int i = 4; i <= max_val; i++)
        if (cnt[i] <= 1) point += 10 * (10 << max_val);
    return point;
}

// guide:
// https://blog.csdn.net/weixin_42165981/article/details/103263211
// https://www.cnblogs.com/Mathics/p/4100059.html
int alpha_beta(Board& num, int depth, int alpha, int beta) {
    if (depth == 0) return get_point(num);
    if (!(depth & 1)) {
        // maxnode: move
        for (int i = 1; i <= 4; i++) {
            // move不方便回溯 创建新的copy
            newBoard(tmp, num);
            if (fast_move(tmp, i))
                alpha = max(alpha, alpha_beta(tmp, depth - 1, alpha, beta));    // 移动成功
            else
                alpha = max(alpha, get_point(tmp) / 100);    // 移动失败
            if (alpha >= beta) break;
        }
        return alpha;
    }
    else {
        // minnode: add a number
        for (int i = 0; i < 16; i++) {
            // 枚举位置
            if (board_num(i) == 0) {
                // 放置2
                board_num(i) = 2;
                beta = min(beta, alpha_beta(num, depth - 1, alpha, beta));
                board_num(i) = 0;
                if (beta <= alpha) break;

                // 放置4
                board_num(i) = 4;
                beta = min(beta, alpha_beta(num, depth - 1, alpha, beta));
                board_num(i) = 0;
                if (beta <= alpha) break;
            }
        }
        return beta;
    }
}

minmax_oper::minmax_oper(): Operator("Min-Max", NON_INTERACTIVE_OPER) {}

int minmax_oper::get_moved(const Board& num) {
    // //前面部分 加速
    // if (*max_element(&num[0][0], &num[3][3]) < 4) return rand() % 4 + 1;

    //正常MIN-MAX
    int dire = 1, alpha = -INF, beta = +INF;
    for (int i = 1; i <= 4; i++) {
        // move不方便回溯 创建新的copy
        newBoard(tmp, num);
        fast_move(tmp, i);
        int val = alpha_beta(tmp, maxdep * 2 - 1, alpha, beta);
        if (val > alpha) {
            alpha = val;
            dire = i;
        }
    }
    do {
        newBoard(tmp, num);
        if (fast_move(tmp, dire))
            break;
        else
            dire = dire % 4 + 1;
    } while (true);
    return dire;
}