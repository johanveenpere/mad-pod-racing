#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cmath>
#include <utility>

using namespace std;
using Point = pair<int, int>;

const double pi = 3.141592653589793238462643383279502884197169399375105820974944;


// taken from stackoverflow: https://stackoverflow.com/questions/1560492/how-to-tell-whether-a-point-is-to-the-right-or-left-side-of-a-line
int position(Point a, Point b, Point c) {
    int dir = ((b.first - a.first) * (c.second - a.second) - (b.second - a.second) * (c.first - a.first));
    if (dir > 0) return 1;
    if (dir < 0) return 0;
    return 0;
}

pair<int, int> difference(Point point1, Point point2) {
    return pair(point1.first - point2.first, point1.second - point2.second);
}

double distance(Point point1, Point point2) {
    auto diff = difference(point2, point1);
    return sqrt(pow(diff.first, 2) + pow(diff.second, 2));
}

double angleFrom(Point point1, Point point2, Point point3) {
    double a = distance(point2, point3);
    double b = distance(point2, point1);
    double c = distance(point1, point3);
    return acos((pow(a, 2) + pow(b, 2) - pow(c, 2)) / 2 * a * b);
}

class Vector {
public:
    double x, y;

    Vector(double x, double y) {
        this->x = x;
        this->y = y;
    }

public:
    Vector perpendicularVector() {
        return Vector(-y, x);
    }

    Vector unitVector() {
        double magnitude = sqrt(pow(x, 2) + pow(y, 2));
        return Vector(x / magnitude, y / magnitude);
    }

    Vector(Point point1, Point point2) {
        Point diff = difference(point2, point1);
        x = diff.first;
        y = diff.second;
    }

    Vector operator*(double scalar) {
        return Vector(x * scalar, y * scalar);
    }

    Point operator+(Point point) {
        return Point(this->x + point.first, this->y + point.second);
    }
};

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

    Point getPointAfter(Point point) {
        cerr << "getPointAfter: " << point.first << ";" << point.second << endl;;
        for (auto iter = checkpoints.begin(); iter < checkpoints.end(); iter++) {
            if (*iter == point) {
                cerr << "found point" << endl;
                if (iter + 1 == checkpoints.end()) {
                    cerr << "Point is last in the list" << endl;
                    cerr << "allCheckpointsVisited: " << allCheckpointsVisited << endl;
                    // Point is the last checkpoint in the list.
                    // If we have seen all of the checkpoints then we will return the first checkpoint in the list.
                    // If we haven't seen all checkpoints then we can't know what the next point is and we will return the current point.
                    cerr << "returned point" << endl;
                    return allCheckpointsVisited ? *checkpoints.begin() : point;
                }
                cerr << "returned point" << endl;
                return *++iter;
            }
        }
    }

public:

    Point firstCheckpoint() {
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

    // 1 - turns left, 0 - doesn't turn, -1 - turns right
    int trackTurningDirection(Point point1, Point point2) {
        Point point3 = getPointAfter(point2);
        cerr << "point3: " << point3.first << ";" << point3.second << endl;
        return position(point1, point2, point3);
    }

};

class Racer {

    Track* track;
    pair<int, int> previousCheckpoint, currentCheckpoint, nextCheckpoint;

    double speedAngleMultiplier(int angle) {
        double rad = angle * pi / 180;
        auto multiplier = (cos(rad) + 1) / 2;
        cerr << "angleThrottle: " << multiplier << endl;
        return multiplier;
    }

    double aheadOfCheckpointThrottle(int distance) {
        const int breakingDistance = 1000;
        auto multiplier = (double)min(distance, breakingDistance) / breakingDistance;
        cerr << "distThrottle: " << multiplier << endl;
        return multiplier;
    }

    double speedMultiplier(int distance, int angle) {
        return speedAngleMultiplier(angle) * aheadOfCheckpointThrottle(distance);
    }

    string outputSpeed(int distance, int angle) {
        string output;
        if (angle < 1 && angle > -1 && distance > 1000 && track->lap() == 3) {
            cerr << "!!BOOST!!" << endl;
            output = "BOOST";
        }
        else {
            int thrust = 70 * aheadOfCheckpointThrottle(distance) + 30 * speedAngleMultiplier(angle);
            output += to_string(thrust);
        }
        return output;
    }

    string outputPosition(pair<int, int> point) {
        return to_string(point.first) + " " + to_string(point.second);
    }

    Point correctedCheckpoint(Point passedCheckpoint, Point nextCheckpoint) {
        Vector translationVector = Vector(passedCheckpoint, nextCheckpoint).perpendicularVector().unitVector();
        cerr << "translationVector: " << translationVector.x << ";" << translationVector.y << endl;
        int direction = track->trackTurningDirection(passedCheckpoint, nextCheckpoint);
        cerr << "track turning direction: " << direction << endl;
        return translationVector * (direction * 600.0) + nextCheckpoint;
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
        return outputPosition(correctedCheckpoint(currentCheckpoint, nextCheckpoint)) + " " + outputSpeed(distance, angle);
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