#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cmath>

using namespace std;
int prevX, prevY;

bool isNewCheckpoint(int x, int y) {
    if (prevX == x && prevY == y) return false;
    return true;
}

string outputPosition(int nextCheckpointX, int nextCheckpointY) {
    return to_string(nextCheckpointX) + " " + to_string(nextCheckpointY);
}

double speedAngleMultiplier(int angle) {
    double rad = angle * 3.14149 / 180;
    return (cos(rad) + 1) / 2;
}

double aheadOfCheckpointThrottle(int distance) {
    const int breakingDistance = 10;
    return min(distance, breakingDistance) / breakingDistance;
}

double speedMultiplier(int distance, int angle) {
    return speedAngleMultiplier(angle) * aheadOfCheckpointThrottle(distance);
}

string outputSpeed(int distance, int angle, int checkpointNr) {
    string output;
    if (checkpointNr == 12 && angle == 0) {
        output += "BOOST";
    }
    else {
        int thrust = 10 + 90 * speedMultiplier(distance, angle);
        output += to_string(thrust);
    }
    return output;
}

int main()
{
    int checkpointNr = 0;
    // game loop

    int x = 0;
    int y = 0;
    int next_checkpoint_x = 1000; // x position of the next check point
    int next_checkpoint_y = 1000; // y position of the next check point
    int next_checkpoint_dist = 1414; // distance to the next checkpoint
    int next_checkpoint_angle = 0; // angle between your pod orientation and the direction of the next checkpoint
    //cin >> x >> y >> next_checkpoint_x >> next_checkpoint_y >> next_checkpoint_dist >> next_checkpoint_angle; cin.ignore();
    int opponent_x;
    int opponent_y;
    //cin >> opponent_x >> opponent_y; cin.ignore();

    if (isNewCheckpoint(next_checkpoint_x, next_checkpoint_y)) {
        prevX = next_checkpoint_x;
        prevY = next_checkpoint_y;
        checkpointNr++;
    }

    cerr << "current checkpoint number: " << checkpointNr << endl;

    cout << outputPosition(next_checkpoint_x, next_checkpoint_y) << " " << outputSpeed(next_checkpoint_dist, next_checkpoint_angle, checkpointNr) << endl;
}