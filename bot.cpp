#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cmath>
#include <utility>

using namespace std;
using Point = pair<int, int>;

const double pi = 3.141592653589793238462643383279502884197169399375105820974944;

namespace util {
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
        return *this / magnitude();
    }

    Vector(Point point1, Point point2) {
        Point diff = util::difference(point2, point1);
        x = diff.first;
        y = diff.second;
    }

    Vector operator*(double scalar) {
        return Vector(x * scalar, y * scalar);
    }

    double operator*(Vector vec) {
        return this->x * vec.x + this->y * vec.y;
    }

    Vector operator/(double scalar) {
        return Vector(x / scalar, y / scalar);
    }

    Point operator+(Point point) {
        return Point(this->x + point.first, this->y + point.second);
    }

    string toString() {
        return to_string(x) + ";" + to_string(y);
    }

    double magnitude() {
        return sqrt(pow(x, 2) + pow(y, 2));
    }

    static double angleBetween(Vector a, Vector b) {
        return acos((a * b) / (a.magnitude() * b.magnitude()));
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
        return util::position(point1, point2, point3);
    }

};

class Racer {

    Track* track;
    Point previousPosition, currentPosition;
    Point previousCheckpoint, currentCheckpoint, nextCheckpoint;

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

    string outputSpeed(int distance, int angle, double angleToOpponent, int distanceToOpponent) {
        cerr << "distance: " << distance << endl;
        cerr << "angle: " << angle << endl;
        cerr << "angleToOpponent: " << angleToOpponent << endl;
        cerr << "distanceToOpponent: " << distanceToOpponent << endl;
        if (distanceToOpponent < 1000) {
            cerr << "!!SHIELDS!!" << endl;
                return "SHIELD";
        }
        if (angleToOpponent >  -0.1 && angleToOpponent < 0.1 && distanceToOpponent < 4000 && track->lap() > 1){
                cerr << "!!BOOST!!" << endl;
                return "BOOST";
        }
        else {
            int thrust = 70 + 30 * speedMultiplier(distance, angle);
            return to_string(thrust);
        }
    }

    string outputPosition(pair<int, int> point) {
        return to_string(point.first) + " " + to_string(point.second);
    }

    Point correctedCheckpoint(Point racerPosition, Point nextCheckpoint, double angle) {
        double moveAmount = angle / 90.0;
        cerr << "moveAmount: " << moveAmount << endl;
        Vector moveVector(racerPosition, nextCheckpoint);
        moveVector = moveVector.perpendicularVector().unitVector() * moveAmount * 1200;
        cerr << "moveVector: " << moveVector.toString() << endl;
        auto output = moveVector + nextCheckpoint;
        cerr << "nextCheckpoint + moveVector: " << output.first << ";" << output.second << endl;
        return output;
    }

    bool passed(pair<int, int> point) {
        return previousCheckpoint == point;
    }

    double angleToOpponent(Point opponentPosition) {
        Vector racerHeading(previousPosition, currentPosition);
        Vector opponentDirection(currentPosition, opponentPosition);
        double angle = Vector::angleBetween(racerHeading, opponentDirection);
        cerr << "angle: " << angle << endl;
        return angle;
    }

public:

    Racer(Track* track) {
        this->track = track;
    }


    void setNextCheckpoint(Point point) {
        if (nextCheckpoint != point) {
            previousCheckpoint = currentCheckpoint;
            currentCheckpoint = nextCheckpoint;
            nextCheckpoint = point;
            track->addCheckpoint(point);
        }
    }

    void setPosition(Point point) {
        previousPosition = currentPosition;
        currentPosition = point;
    }

    string output(int distance, int angle, Point opponentPosition) {
        return outputPosition(nextCheckpoint) + " " + outputSpeed(distance, angle, angleToOpponent(opponentPosition), util::distance(currentPosition, opponentPosition));
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

        Point nextCheckpoint(next_checkpoint_x, next_checkpoint_y);
        racer.setPosition(Point(x, y));
        racer.setNextCheckpoint(nextCheckpoint);

        cout << racer.output(next_checkpoint_dist, next_checkpoint_angle, Point(opponent_x, opponent_y)) << endl;
    }
}