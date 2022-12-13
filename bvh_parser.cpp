#include<fstream>
#include<assert.h>
#include<string.h>
#include<ios>
#include<stack>
#include<iostream>
#include"bvh_parser.h"
joint* now_node=NULL,*pre_node=NULL;
META meta_data;
// a naive bvh parser
using namespace std;
void dfs(joint*& root,ifstream& file)
{
    vector<double> line;
    if(root->joint_type=="End") 
    {
        root->motion.push_back({});
        return ;
    }
    
    for(int j=0;j<root->channels.size();j++)
    {
        double temp;
        file>>temp;
        //cout<<temp<<' ';
        line.push_back(temp);
    }
    //cout<<endl;
    root->motion.push_back(line);
    for(int i=0;i<root->children.size();i++)
    {
        dfs(root->children[i],file);
    }
}
int main(int argc, char** argv) {
    joint root;
    ifstream file(argv[1]);
    if(file.is_open()==false) 
    {
        cerr<<"Fileopen error!"<<endl;
        exit(1);
    }
    stack<joint*> stk;
    while(!file.eof())
    {
        string str;//之前用的char*会报segmentation fault
        file>>str;
        //cout<<str<<endl;
        if(str=="HIERARCHY") continue;
        else if(str=="ROOT")
        {
            root.joint_type="ROOT";
            file>>str;//读入关节名
            now_node=&root;//now_node指向root的地址
            root.name=str;//记录关节名
            file>>str;//“{”
            file>>str;//offset
            double offset;
            file>>offset;
            now_node->offset_x=offset;
            file>>offset;
            now_node->offset_y=offset;  
            file>>offset;
            now_node->offset_z=offset;    
            file>>str;//channels
            int channel_num;
            file>>channel_num;
            for(int i=0;i<channel_num;i++)
            {
                file>>str;
                now_node->channels.push_back(str);
            }
            pre_node=now_node;
            stk.push(pre_node);
        }
        else if(str=="JOINT")
        {
            now_node=new joint;
            if(now_node==NULL)
            {
                cerr<<"Malloc fault!"<<endl;
                exit(1);
            }
            if(pre_node)
            {
                pre_node->children.push_back(now_node);
            }
            now_node->joint_type="JOINT";
            file>>str;//读入关节名
            now_node->name=str;//记录关节名
            file>>str;//“{”
            file>>str;//offset
            double offset;
            file>>offset;
            now_node->offset_x=offset;
            file>>offset;
            now_node->offset_y=offset;  
            file>>offset;
            now_node->offset_z=offset;    
            file>>str;//channels
            int channel_num;
            file>>channel_num;
            for(int i=0;i<channel_num;i++)
            {
                file>>str;
                now_node->channels.push_back(str);
            }
            pre_node=now_node;
            stk.push(now_node);
        }
        /*else if(str=="}")
        {
            if(!stk.empty())
            {
                pre_node=stk.top();//待修改
                stk.pop();
            }
            else pre_node=&root;
        }*/
        else if(str=="End")
        {
            now_node=new joint;
            if(now_node==NULL)
            {
                cerr<<"Malloc fault!"<<endl;
                exit(1);
            }
            if(pre_node)
            {
                pre_node->children.push_back(now_node);
            }
            now_node->joint_type="End";
            file>>str;//Site
            string end_name=pre_node->name+"_End";
            now_node->name=end_name;
            file>>str;//"{"
            file>>str;//OFFSET
            double offset;
            file>>offset;
            now_node->offset_x=offset;
            file>>offset;
            now_node->offset_y=offset;
            file>>offset;
            now_node->offset_z=offset;
            file>>str;
        }
        else if(str=="}")
        {
            stk.pop(); 
            if(!stk.empty())
            {
                pre_node=stk.top();
                
            }
            else {
                pre_node=NULL;
            }
        }
        else if(str=="MOTION")
            break;
    }
    string str1;
    file>>str1;//Frames:
    //cout<<str1<<endl;
    file>>meta_data.frame;
    file>>str1;
    file>>str1;//Frame times:
    //cout<<str1<<endl;
    file>>meta_data.frame_time;
    for(int i=0;i<meta_data.frame;i++)
    {
        vector<double> line;
        for(int j=0;j<root.channels.size();j++)
        {
            double temp;
            file>>temp;
            line.push_back(temp);
        }
        root.motion.push_back(line);
        for(int k=0;k<root.children.size();k++)
        {
            dfs(root.children[k],file);
        }
    }
    
    //cout<<str1;
    jsonify(root, meta_data);
    file.close();
    return 0;
}