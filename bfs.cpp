#include <iomanip>
#include <iostream>
#include <queue>
#include <string>
#include <utility>
#include <vector>

/*
struct Edge {
    int to;
    int weight;
    Edge(int t, int w) : to(t), weight(w){};
};
*/

struct Path {
    std::pair<int, int> pos;
    unsigned int cost;
    int heuristic;
    std::vector<std::pair<int, int>> route;  // dist = cost
    Path(int x, int y, unsigned int cost, int heur)
        : pos(std::make_pair(x, y)), cost(cost), heuristic(heur){};
    Path(int x, int y, unsigned int cost, int heur,
         std::vector<std::pair<int, int>> route)
        : pos(std::make_pair(x, y)),
          cost(cost),
          heuristic(heur),
          route(route){};
};

auto heur = [](std::pair<int, int> pos, std::pair<int, int> dst) {
    int ret =
        std::abs(pos.first - dst.first) + std::abs(pos.second - dst.second);
    return ret;
};

int main() {
    int height, width;
    const int dx[4] = {-1, 1, 0, 0};
    const int dy[4] = {0, 0, -1, 1};  // up, down, left, right
    std::cin >> height >> width;
    // std::vector<std::vector<Edge>> Graph;
    std::vector<std::string> Map(height);
    std::vector<std::vector<bool>> seen(height,
                                        std::vector<bool>(width, false));
    for (int h = 0; h < height; ++h) {
        std::cin >> Map[h];
    }
    int sx, sy, tx, ty;
    for (int h = 0; h < height; ++h) {
        for (int w = 0; w < width; ++w) {
            if (Map[h][w] == 's') {
                sx = h;
                sy = w;
                seen[h][w] = true;
            }
            if (Map[h][w] == 't') {
                tx = h;
                ty = w;
            }
        }
    }
    // std::vector<std::vector<bool>> dist(height, std::vector<bool>(width));
    auto compare = [](Path l, Path r) {
        return (l.cost + l.heuristic) > (r.cost + r.heuristic);
    };
    std::priority_queue<Path, std::vector<Path>, decltype(compare)> que{
        compare};
    std::vector<std::pair<int, int>> p_route = {std::make_pair(sx, sy)};
    que.push(Path(sx, sy, 0,
                  heur(std::make_pair(sx, sy), std::make_pair(tx, ty)),
                  p_route));

    int count = 0;
    while (!que.empty()) {
        if (que.top().pos.first == tx && que.top().pos.second == ty) break;

        int x = que.top().pos.first;
        int y = que.top().pos.second;
        unsigned int p_cost = que.top().cost;
        p_route = que.top().route;
        que.pop();

        // 隣接頂点を探索
        for (int direction = 0; direction < 4; ++direction) {
            std::vector<std::pair<int, int>> tmp_route = p_route;
            unsigned int tmp_cost = p_cost;

            int next_x = x + dx[direction];
            int next_y = y + dy[direction];
            if (next_x < 0 || next_x >= height || next_y < 0 ||
                next_y >= width) {
                continue;  // 場外アウトならダメ
            }
            if (seen[next_x][next_y])
                continue;  // 最小コストから探索していくので探索済みであれば探索しない

            if (Map[next_x][next_y] == '#') {
                continue;  // 壁はダメ
            } else if (Map[next_x][next_y] == 'x')
                tmp_cost += 3;
            else
                tmp_cost += 1;

            count++;
            seen[next_x][next_y] = true;
            tmp_route.push_back(std::make_pair(next_x, next_y));
            que.push(Path(
                next_x, next_y, tmp_cost,
                heur(std::make_pair(next_x, next_y), std::make_pair(tx, ty)),
                tmp_route));
        }
    }
    if (que.empty()) {
        std::cout << "no path exists or recursive" << std::endl;
    } else {
        for (const auto e : que.top().route) {
            Map[e.first][e.second] = 'p';
        }
        std::cout << "cost: " << que.top().cost << ", ";
        std::cout << "search node count: " << count << std::endl;
        for (int h = 0; h < height; ++h) {
            for (int w = 0; w < width; ++w) {
                std::cout << std::setw(3) << Map[h][w];
            }
            std::cout << std::endl;
        }
    }
}