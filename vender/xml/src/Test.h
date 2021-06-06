#pragma once
#include <iostream>

#include "tinyxml.h"
#include "tinystr.h"

using namespace std;

namespace utilstest
{
    static void Test_xml0()
    {
        TiXmlDocument doc("");
        //bool loadOk = doc.LoadFile();
        doc.Parse("<xml>"
            "<tag1 type=\"1\" />"
            "<tag2 type=\"2\" />"
            "</xml>");

        if (doc.Error())
        {
            cout << "could load:" << doc.ErrorDesc() << endl;
        }

        TiXmlPrinter printer;//提供的工具类,目的是将xml的数据按格式输出
        doc.Accept(&printer);
        cout << printer.CStr() << endl;//输出

        TiXmlElement*node = doc.FirstChildElement();//获取第一个element节点
        cout << node->Value() << endl;//输出节点的值
        string t;
        node->QueryValueAttribute("type", &t);//获取节点属性
        cout << t << endl;

        TiXmlNode* node1 = doc.FirstChild()->FirstChild();
        node1->NextSibling()->ToElement()->QueryStringAttribute("type", &t);//获取第二个子节点的数据
        cout << "2:" << t << endl;

        //使用遍历的方式进行处理
        TiXmlNode* child = nullptr;
        TiXmlElement* element = nullptr;
        TiXmlAttribute *attr = nullptr;
        int ct;
        while (child = doc.FirstChild()->IterateChildren(child))
        {
            cout << child->ValueStr() << "\t";
            ct = child->Type();
            cout << ct << "\t";
            //根据不同的节点类型做相应处理  
            switch (ct)
            {
            case TiXmlNode::TINYXML_TEXT:
                break;
            case TiXmlNode::TINYXML_ELEMENT:
                element = child->ToElement();
                attr = element->FirstAttribute();
                while (attr)
                {
                    cout << attr->NameTStr() << "=" << attr->ValueStr() << '\t';
                    attr = attr->Next();
                }
                break;
            }
        }
    }
}
