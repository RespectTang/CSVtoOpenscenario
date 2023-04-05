#include <iostream>
#include <istream>
#include <streambuf>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <stdlib.h>
#include "tinyxml2.h"
#include "tinyxml.h"
using namespace tinyxml2;
using namespace std;

int cnt = 0;//生成文件版本计数器

vector<vector<double>> read_gt_file(string filename){
    ifstream inFile(filename, ios::in);
    string lineStr;
    char delim=',';
    vector<vector<double>> matrix;
    if(inFile.fail())
    {
        cout <<"Read files fail....."<<endl;
        return matrix;
    }
    std::istringstream sin;         //将整行字符串line读入到字符串istringstream中
    std::vector<std::string> words; //声明一个字符串向量
    std::string word;
    cout << "------------------------------Open CSV FIle Success!--------------------------------" << endl;
    // 读取标题行
    std::getline(inFile, lineStr);

    while (getline(inFile, lineStr)) 
    {
        stringstream ss(lineStr);
        string str;
        vector<double> lineArray;

        // cut apart by flag
        while (getline(ss, str, delim))
            lineArray.push_back(stod(str));
        matrix.push_back(lineArray);
    }
    return matrix;
}

void readXML(const char* ccXmlName,double v1,double v2,double dis)
{
    tinyxml2::XMLDocument docXml;
    XMLError errXml = docXml.LoadFile(ccXmlName);//读取模板文件
    TiXmlDocument* myDocument = new TiXmlDocument(ccXmlName);
    myDocument->LoadFile();
    
    if (XML_SUCCESS == errXml)
    {
        cout << "----------------------------------Open XML FIle Success!------------------------------------" << endl;


        //获得xml的头，即声明部分
        TiXmlDeclaration* decl = myDocument->FirstChild()->ToDeclaration();
        cout << "xml version is " << decl->Version() << endl;
        cout << "xml encoding is " << decl->Encoding() << endl;

        //获得根元素
        TiXmlElement* RootElement = myDocument->RootElement();

        //输出根元素名称
        cout << "The RootElement is "<< RootElement->Value() << endl;

        //手动遍历xml树，比for快一点
        TiXmlElement* FileHeaderElement = RootElement->FirstChildElement();
        TiXmlElement* CatalogLocationsElement = FileHeaderElement->NextSiblingElement();
        TiXmlElement* RoadNetworkElement = CatalogLocationsElement->NextSiblingElement();
        TiXmlElement* EntitiesElement = RoadNetworkElement->NextSiblingElement();
        TiXmlElement* StoryboardElement = EntitiesElement->NextSiblingElement();//进入storyboard
        cout << "StoryboardElement value is " << StoryboardElement->Value() << endl;

        TiXmlElement* InitElement = StoryboardElement->FirstChildElement();//进入init
        TiXmlElement* ActionsElement = InitElement->FirstChildElement();//进入Actions
        TiXmlElement* GlobalActionElement = ActionsElement->FirstChildElement();//进入GlobalActions
        cout << "GlobalActionElement value is " << GlobalActionElement->Value() << endl;

        //GlobalAction后面两个兄弟节点为两个Private，一个是主车的初始状态，一个是对手车的初始状态
        cout << "------------------ego state-------------------" << endl;
        // Private1设置主车ego运动状态，
        // 里面有三个PrivateAction，第一个是坐标，第二个是速度，第三个是状态
        //跟车模型里面只需要修改速度
        TiXmlElement* Private1Element = GlobalActionElement->NextSiblingElement();
        cout << "Private1Element value is " << Private1Element->Value() << endl;
        TiXmlElement* PrivateAction1Element = Private1Element-> FirstChildElement();
        cout << "PrivateAction1Element value is " << PrivateAction1Element->Value() << endl;
        TiXmlElement* PrivateAction2Element = PrivateAction1Element->NextSiblingElement();
        cout << "PrivateAction2Element value is " << PrivateAction2Element->Value() << endl;
        TiXmlElement* PrivateAction3Element = PrivateAction2Element->NextSiblingElement();
        cout << "PrivateAction3Element value is " << PrivateAction3Element->Value() << endl; 

       // egoEgoWorldPosition 获得主车坐标
        TiXmlElement* EgoWorldPositionElement = PrivateAction1Element->FirstChildElement()->FirstChildElement()->FirstChildElement();
        cout << "EgoWorldPositionElement value is " << EgoWorldPositionElement->Value() << endl;
        //egoEgoWorldPosition里面有六个属性h p r x y z
        TiXmlAttribute* EgoWP_Attr_h = EgoWorldPositionElement->FirstAttribute();//h
        cout << "EgoWP_Attr_h value is " << EgoWP_Attr_h->Value() << endl;
        TiXmlAttribute* EgoWP_Attr_x = EgoWP_Attr_h->Next()->Next()->Next();//x
        TiXmlAttribute* EgoWP_Attr_y = EgoWP_Attr_x->Next();                //y
        cout << "EgoWP_Attr_x value is " << EgoWP_Attr_x->Value() << endl;
        cout << "EgoWP_Attr_y value is " << EgoWP_Attr_y->Value() << endl;

        //PrivateAction2获得主车速度AbsoluteTargetSpeed
        TiXmlElement* AbsoluteTargetSpeedElement = PrivateAction2Element->FirstChildElement()->FirstChildElement()->FirstChildElement()
                                                                     ->NextSiblingElement()->FirstChildElement();
        TiXmlAttribute* EgoATS_Attr_Value = AbsoluteTargetSpeedElement->FirstAttribute();//value
        cout << "EgoATS_Attr_Value value is " << EgoATS_Attr_Value->Value() << endl;

        cout << "------------------Opponent car state-------------------" << endl;

       // Private2获得对手车初始状态数据
       // 里面有两个PrivateAction ,第一个是坐标，第二个是速度
        TiXmlElement* Private2Element = Private1Element->NextSiblingElement();
        cout << "Private2Element value is " << Private2Element->Value() << endl;
        TiXmlElement* PrivateAction2_1Element = Private2Element->FirstChildElement();
        cout << "PrivateAction1Element value is " << PrivateAction2_1Element->Value() << endl;
        TiXmlElement* PrivateAction2_2Element = PrivateAction2_1Element->NextSiblingElement();
        cout << "PrivateAction2Element value is " << PrivateAction2_2Element->Value() << endl;

        // OCWorldPosition 获得对手车坐标
        TiXmlElement* OCWorldPositionElement = PrivateAction2_1Element->FirstChildElement()->FirstChildElement()->FirstChildElement();
        cout << "OCWorldPositionElement value is " << OCWorldPositionElement->Value() << endl;
        //OCWorldPosition里面有六个属性h p r x y z
        TiXmlAttribute* OCWP_Attr_h = OCWorldPositionElement->FirstAttribute();//h
        cout << "OCWP_Attr_h value is " << OCWP_Attr_h->Value() << endl;
        TiXmlAttribute* OCWP_Attr_x = OCWP_Attr_h->Next()->Next()->Next();//x
        TiXmlAttribute* OCWP_Attr_y = OCWP_Attr_x->Next();                //y
        cout << "OCWP_Attr_x value is " << OCWP_Attr_x->Value() << endl;
        cout << "OCWP_Attr_y value is " << OCWP_Attr_y->Value() << endl;

        //PrivateAction2_2获得主车速度AbsoluteTargetSpeed
        TiXmlElement* OCAbsoluteTargetSpeedElement = PrivateAction2_2Element->FirstChildElement()->FirstChildElement()->FirstChildElement()
                                                                            ->NextSiblingElement()->FirstChildElement();
        TiXmlAttribute* OCATS_Attr_Value = OCAbsoluteTargetSpeedElement->FirstAttribute();//value
        cout << "OCATS_Attr_Value value is " << OCATS_Attr_Value->Value() << endl;

        //---------------修改参数-------------------
        //修改主车速度、对手车速度
        string EgoSpeed = to_string(v1);
        string OCSpeed = to_string(v2);
        char* cEgoSpeed = const_cast<char*>(EgoSpeed.c_str());
        char* cOCSpeed = const_cast<char*>(OCSpeed.c_str());
        EgoATS_Attr_Value->SetValue(cEgoSpeed);
        OCATS_Attr_Value->SetValue(cOCSpeed);
        cout << "After modification EgoATS_Attr_Value value is " << EgoATS_Attr_Value->Value() << endl;
        cout << "After modification OCATS_Attr_Value value is " << OCATS_Attr_Value->Value() << endl;
        
        //修改对手车世界坐标，dis是一维的，仅考虑对手车在主车之前的x坐标，即主车坐标x+dis=对手车坐标x
        stringstream ss;
        double ans;
        ss << EgoWP_Attr_x->Value();
        ss >> ans;
        ans = ans + dis;
        string OCWP_x = to_string(ans);
        char* cOCWP_x = const_cast<char*>(OCWP_x.c_str());
        OCWP_Attr_x->SetValue(cOCWP_x);
        cout << "After modification OCWP_Attr_x value is " << OCWP_Attr_x->Value() << endl;


        //保存文件
        cnt++;
        string path = "C:/Users/RTT/source/repos/CSVtoOpenscenario/OPenScenarioFile/";
        string FileName = "template";
        string version = "_version_"+to_string(cnt);
        string suffix = ".xosc";
        string OutPutFile = path + FileName + version + suffix;
        cout << "Out Put File Path is " << OutPutFile << endl;
        char* cOutPutFile = const_cast<char*>(OutPutFile.c_str());
        myDocument->SaveFile(cOutPutFile);
    }
    
}


int main()
{
    vector<vector<double>> csvdata;
    csvdata  = read_gt_file("C:/Users/RTT/source/repos/CSVtoOpenscenario/cutData/testvvd.csv");
    for(int i=0;i<csvdata.size();i++)
    {
        for(int j = 0;j<3;j++)
        {
            if(j==0) cout<<"v1="<< csvdata[i][j]<<" ";
            if(j==1) cout<<"v2="<< csvdata[i][j]<<" ";
            if(j==2) cout<<"v3="<< csvdata[i][j]<<" ";
        }
        cout<<endl;
    }
    for (int i = 0; i < csvdata.size(); i++)
    {
    //模板文件，vvd参数
    readXML("C:/Users/RTT/source/repos/CSVtoOpenscenario/OPenScenarioFile/template.xosc",
        double(csvdata[i][0]), double(csvdata[i][1]), double(csvdata[i][2]));
    }
    

    return 0;
}
