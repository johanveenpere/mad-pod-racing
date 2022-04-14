#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cmath>
#include <utility>

using namespace std;

class Track {

    int lapCounter = 0;
    vector<pair<int, int>> checkpoints;
    bool allCheckpointsVisited = false; 


    bool isNewCheckpoint(pair<int, int> point) {
        if (!allCheckpointsVisited) {
            for (int i = 0; i < checkpoints.size(); i++) {
                if (checkpoints.at(i) == point) {
                    return false;
                }
            }
            return true;
        }
        return false;
    }

public:

    pair<int, int> firstCheckpoint() {
        return checkpoints.at(0);
    }


    void addCheckpoint(pair<int, int> point) {
        if (isNewCheckpoint(point)) {
            checkpoints.push_back(point);
        }
        else {
            allCheckpointsVisited = true;
        }
        if (point == firstCheckpoint()) {
            lapCounter++;
        }
    }

    int lap() {
        return lapCounter;
    }

    string toString() {
        string output;
        output += "track length: " + to_string(checkpoints.size()) + "\n";
        for (auto iter = checkpoints.begin(); iter < checkpoints.end(); iter++) {
            output += to_string(iter->first) + ";" + to_string(iter->second) + "\n";
        }
        return output;
    }

};

class Racer {

    Track* track;
    pair<int, int> previousCheckpoint, currentCheckpoint, nextCheckpoint;

    double speedAngleMultiplier(int angle) {
        double rad = angle * 3.14149 / 180;
        return (cos(rad) + 1) / 2;
    }

    double aheadOfCheckpointThrottle(int distance) {
        const int breakingDistance = 1000;
        return min(distance, breakingDistance) / breakingDistance;
    }

    double speedMultiplier(int distance, int angle) {
        return speedAngleMultiplier(angle) * aheadOfCheckpointThrottle(distance);
    }

    string outputSpeed(int distance, int angle) {
        string output;
        if (angle == 0 && distance > 700 && track->lap() == 3) {
            cerr << "!!BOOST!!" << endl;
            output = "BOOST";
        }
        else {
            int thrust = 70 + 30 * speedMultiplier(distance, angle);
            output += to_string(thrust);
        }
        return output;
    }

    string outputPosition(pair<int, int> point) {
        return to_string(point.first) + " " + to_string(point.second);
    }

public:

    Racer(Track* track) {
        this->track = track;
    }


    void setNextCheckpoint(pair<int, int> point) {
        if (nextCheckpoint != point) {
            previousCheckpoint = currentCheckpoint;
            currentCheckpoint = nextCheckpoint;
            nextCheckpoint = point;
            track->addCheckpoint(point);
        }
    }

    string output(int distance, int angle) {
        return outputPosition(nextCheckpoint) + " " + outputSpeed(distance, angle);
    }

    bool passed(pair<int, int> point) {
        return previousCheckpoint == point;
    }

};


int main()
{
    Track track;
    Racer racer(&track);



    // game loop
    while (1) {
        int x;
        int y;
        int next_checkpoint_x; // x position of the next check point
        int next_checkpoint_y; // y position of the next check point
        int next_checkpoint_dist; // distance to the next checkpoint
        int next_checkpoint_angle; // angle between your pod orientation and the direction of the next checkpoint
        cin >> x >> y >> next_checkpoint_x >> next_checkpoint_y >> next_checkpoint_dist >> next_checkpoint_angle; cin.ignore();
        int opponent_x;
        int opponent_y;
        cin >> opponent_x >> opponent_y; cin.ignore();

        pair nextCheckpoint(next_checkpoint_x, next_checkpoint_y);

        racer.setNextCheckpoint(nextCheckpoint);

        cout << racer.output(next_checkpoint_dist, next_checkpoint_angle) << endl;
    }
}