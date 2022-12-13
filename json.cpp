#include<iostream>
#include<fstream>
#include"bvh_parser.h"

using namespace std;

void dfs_jsonify(joint*& root,META meta_data,ofstream& outfile,string tablenum)
{
    tablenum+="\t";
    outfile<<tablenum<<"{"<<endl;
    tablenum+="\t";
    outfile<<tablenum<<"\"type\": \""<<root->joint_type<<"\","<<endl;
    outfile<<tablenum<<"\"name\": \""<<root->name<<"\","<<endl;
    outfile<<tablenum<<"\"offset\": ["<<root->offset_x<<", "<<root->offset_y<<", "<<root->offset_z<<"],"<<endl;
    outfile<<tablenum<<"\"channels\": [";
    for(int i=0;i<root->channels.size();i++)
    {
        outfile<<"\""<<root->channels[i]<<"\"";
        if(i!=root->channels.size()-1)
            outfile<<", ";
    }
    outfile<<"],"<<endl;
    outfile<<tablenum<<"\"motion\": ["<<endl;
    tablenum+="\t";
    for(int i=0;i<meta_data.frame;i++)
    {
        outfile<<tablenum<<"[";
        for(int j=0;j<root->channels.size();j++)
        {
            outfile<<root->motion[i][j];
            if(j!=root->channels.size()-1)
            {
                outfile<<", ";
            }
        }
        if(i!=meta_data.frame-1)
        {
            outfile<<"],"<<endl;
            //cout<<i<<endl;
        }
    }
    outfile<<"]"<<endl;
    tablenum.erase(tablenum.end()-1);
    outfile<<tablenum<<"],"<<endl;
    int children_size=root->children.size();
    outfile<<tablenum<<"\"children\": ["<<endl;
    for(int i=0;i<children_size;i++)
    {
        dfs_jsonify(root->children[i],meta_data,outfile,tablenum);
        if(i!=children_size-1)
        {
            outfile<<","<<endl;
        }
    }
    outfile<<endl;
    outfile<<tablenum<<"]"<<endl;
    tablenum.erase(tablenum.end()-1);
    outfile<<tablenum<<"}"<<endl;
}
void jsonify(joint root, META meta_data) {
    string tablenum="";
    ofstream outfile;
    outfile.open("output.json");
    outfile<<"{"<<endl;
    tablenum+="\t";//参考网上一些递归输出json文件的缩进方式
    outfile<<tablenum<<"\"frame\": "<<meta_data.frame<<","<<endl;
    outfile<<tablenum<<"\"frame_time\": "<<meta_data.frame_time<<","<<endl;
    outfile<<tablenum<<"\"joint\":"<<endl;
    tablenum+="\t";
    outfile<<tablenum<<"{"<<endl;
    tablenum+="\t";
    outfile<<tablenum<<"\"type\": \"ROOT\","<<endl;
    outfile<<tablenum<<"\"name\": \""<<root.name<<"\","<<endl;
    outfile<<tablenum<<"\"offset\": ["<<root.offset_x<<", "<<root.offset_y<<", "<<root.offset_z<<"],"<<endl;
    outfile<<tablenum<<"\"channels\": [";
    for(int i=0;i<root.channels.size();i++)
    {
        outfile<<"\""<<root.channels[i]<<"\"";
        if(i!=root.channels.size()-1)
            outfile<<", ";
    }
    outfile<<"],"<<endl;
    outfile<<tablenum<<"\"motion\": ["<<endl;
    tablenum+="\t";
    for(int i=0;i<meta_data.frame;i++)
    {
        outfile<<tablenum<<"[";
        for(int j=0;j<root.channels.size();j++)
        {
            outfile<<root.motion[i][j];
            if(j!=root.channels.size()-1)
            {
                outfile<<", ";
            }
        }
        if(i!=meta_data.frame-1)
        {
            outfile<<"],"<<endl;
            //cout<<i<<endl;
        }
    }
    outfile<<"]"<<endl;
    tablenum.erase(tablenum.end()-1);
    outfile<<tablenum<<"],"<<endl;
    int children_size=root.children.size();
    outfile<<tablenum<<"\"children\": ["<<endl;
    for(int i=0;i<children_size;i++)
    {
        dfs_jsonify(root.children[i],meta_data,outfile,tablenum);
        if(i!=children_size-1)
        {
            
            outfile<<","<<endl;
        }
    }
    outfile<<endl;
    outfile<<tablenum<<"]"<<endl;
    tablenum.erase(tablenum.end()-1);
    outfile<<tablenum<<"}"<<endl;
    outfile<<"}"<<endl;
    outfile.close();
}