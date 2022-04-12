#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;
int prevX, prevY;

bool isNewCheckpoint(int x, int y) {
    if(prevX == x && prevY == y) return false;
    return true;
}
/**
 * Auto-generated code below aims at helping you parse
 * the standard input according to the problem statement.
 **/

int main()
{
    int checkpointNr = 0;
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

        // Write an action using cout. DON'T FORGET THE "<< endl"
        // To debug: cerr << "Debug messages..." << endl;
        if( isNewCheckpoint(next_checkpoint_x, next_checkpoint_y)){
            prevX = next_checkpoint_x;
            prevY = next_checkpoint_y;
            checkpointNr++;
        }

        cerr << "current checkpoint number: " << checkpointNr << endl;

        string output;

        output+= to_string(next_checkpoint_x) + " ";
        output+= to_string(next_checkpoint_y) + " ";

        if(checkpointNr == 12 && next_checkpoint_angle == 0) {
            output+= "BOOST";
        }
        else {
            int thrust;
            if( next_checkpoint_angle > 90 || next_checkpoint_angle < -90) {
                thrust = 0;
            }
            else {
                thrust = 100;
            }
            output+= to_string(thrust);
        }
        // You have to output the target position
        // followed by the power (0 <= thrust <= 100)
        // i.e.: "x y thrust"
        cout << output << endl;
    }
}