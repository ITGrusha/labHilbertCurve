#include <iostream>
#include <cstdint>
#include <utility>
#include <vector>
#include "gif.h"
#include <cmath>

using namespace std;

const string VERSION = "1.0b";
const int delay = 1;
GifWriter g{};


void go(vector<uint8_t> &frame,
        int size,
        pair<int, int> &position,
        int direction = 0,
        vector<uint8_t> color = vector<uint8_t>(4, 0),
        int thickness = 2,
        int length = 10) {
    if (direction == 1 || direction == 0) {
        direction = direction * -2 + 1;
        for (int i = 0; i < length; ++i) {
            position.first += direction;
            for (int j = 0; j < thickness; ++j) {
                long fpos = 4 * (position.first * size + position.second);
                for (int k = 0; k < 4; ++k)
                    frame[fpos + k] = color[k];
                position.second += 1;
            }
            position.second -= thickness;
        }
    } else if (direction == 2 || direction == 3) {
        direction = (direction - 2) * -2 + 1;
        for (int i = 0; i < length; ++i) {
            position.second += direction;
            for (int j = 0; j < thickness; ++j) {
                long fpos = 4 * (position.first * size + position.second);
                for (int k = 0; k < 4; ++k)
                    frame[fpos + k] = color[k];
                position.first += 1;
            }
            position.first -= thickness;
        }
    }
}


/**void goUp(vector<uint8_t> &frame,
          int size,
          pair<int, int> &position,
          vector<uint8_t> color = vector<uint8_t>(4, 0),
          int thickness = 2,
          int length = 10) {
    go(frame, size, position, 0, std::move(color), thickness, length);
}


void goDown(vector<uint8_t> &frame,
            int size,
            pair<int, int> &position,
            vector<uint8_t> color = vector<uint8_t>(4, 0),
            int thickness = 2,
            int length = 10) {
    go(frame, size, position, 1, std::move(color), thickness, length);
}


void goRight(vector<uint8_t> &frame,
             int size,
             pair<int, int> &position,
             vector<uint8_t> color = vector<uint8_t>(4, 0),
             int thickness = 2,
             int length = 10) {
    go(frame, size, position, 2, std::move(color), thickness, length);
}


void goLeft(vector<uint8_t> &frame,
            int size,
            pair<int, int> &position,
            vector<uint8_t> color = vector<uint8_t>(4, 0),
            int thickness = 2,
            int length = 10) {
    go(frame, size, position, 3, std::move(color), thickness, length);
}**/


void turnRight(int &facing) {
    if (facing == 0)
        facing = 3;
    else if (facing == 1)
        facing = 2;
    else if (facing == 2)
        facing = 0;
    else if (facing == 3)
        facing = 1;
}


void turnLeft(int &facing) {
    if (facing == 0)
        facing = 2;
    else if (facing == 1)
        facing = 3;
    else if (facing == 2)
        facing = 1;
    else if (facing == 3)
        facing = 0;
}


void hilbert_a(vector<uint8_t> &,
               int,
               pair<int, int> &,
               int &,
               short,
               vector<uint8_t>,
               int,
               int);

void hilbert_b(vector<uint8_t> &frame,
               int size,
               pair<int, int> &position,
               int &facing,
               short depth = 5,
               vector<uint8_t> color = vector<uint8_t>(4, 0),
               int thickness = 2,
               int length = 10) {
    if (depth != 0) {
        turnRight(facing);
        hilbert_a(frame, size, position, facing, depth - 1, color, thickness, length);
        go(frame, size, position, facing, color, thickness, length);
        GifWriteFrame(&g, frame.data(), size, size, delay);

        turnLeft(facing);
        hilbert_b(frame, size, position, facing, depth - 1, color, thickness, length);
        go(frame, size, position, facing, color, thickness, length);
        GifWriteFrame(&g, frame.data(), size, size, delay);

        hilbert_b(frame, size, position, facing, depth - 1, color, thickness, length);
        turnLeft(facing);
        go(frame, size, position, facing, color, thickness, length);
        GifWriteFrame(&g, frame.data(), size, size, delay);

        hilbert_a(frame, size, position, facing, depth - 1, color, thickness, length);
        turnRight(facing);
    }
}


void hilbert_a(vector<uint8_t> &frame,
               int size,
               pair<int, int> &position,
               int &facing,
               short depth = 5,
               vector<uint8_t> color = vector<uint8_t>(4, 0),
               int thickness = 2,
               int length = 10) {
    if (depth != 0) {
        turnLeft(facing);
        hilbert_b(frame, size, position, facing, depth - 1, color, thickness, length);
        go(frame, size, position, facing, color, thickness, length);
        GifWriteFrame(&g, frame.data(), size, size, delay);

        turnRight(facing);
        hilbert_a(frame, size, position, facing, depth - 1, color, thickness, length);
        go(frame, size, position, facing, color, thickness, length);
        GifWriteFrame(&g, frame.data(), size, size, delay);

        hilbert_a(frame, size, position, facing, depth - 1, color, thickness, length);
        turnRight(facing);
        go(frame, size, position, facing, color, thickness, length);
        GifWriteFrame(&g, frame.data(), size, size, delay);

        hilbert_b(frame, size, position, facing, depth - 1, color, thickness, length);
        turnLeft(facing);
    }

}


int main() {
    cout << "Hello! Hilbert`s curves v" + VERSION << endl
         << "Enter number of iterations: ";
    short depth = 5;
    cin >> depth;
    int thickness = 2;
    int length = 10;
    int facing = 0;
    int canvas_size = (int) (((double) length + 0. * thickness) * (double) (pow(2, depth) - 1 + 2));
    vector<uint8_t> frame(canvas_size * canvas_size * 4, 128);
    pair<int, int> position(length, length);
    GifBegin(&g, "test.gif", canvas_size, canvas_size, delay);
    GifWriteFrame(&g, frame.data(), canvas_size, canvas_size, delay);
    hilbert_a(frame, canvas_size, position, facing, depth);
//    go(frame, canvas_size, position, facing);
//    GifWriteFrame(&g, frame.data(), canvas_size, canvas_size, delay);
//    go(frame, canvas_size, position, facing);
//    GifWriteFrame(&g, frame.data(), canvas_size, canvas_size, delay);
//    turnLeft(facing);
//    go(frame, canvas_size, position, facing);
//    GifWriteFrame(&g, frame.data(), canvas_size, canvas_size, delay);
//    turnRight(facing);
//    go(frame, canvas_size, position, facing);
//    GifWriteFrame(&g, frame.data(), canvas_size, canvas_size, delay);

    GifEnd(&g);
    return 0;
}