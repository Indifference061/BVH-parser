#ifndef BVH_PARSER_H
#define BVH_PARSER_H
#include<string>
#include<vector>

using std::string;
using std::vector;

struct joint {
    string name;
    double offset_x, offset_y, offset_z;
    vector<joint*> children;
    vector<string> channels;
    vector<vector<double>> motion;
    string joint_type;
    //joint():offset_x(0),offset_y(0),offset_z(0){}
};

struct META {
    int frame;
    double frame_time;
};

void jsonify (joint, META);

#endif