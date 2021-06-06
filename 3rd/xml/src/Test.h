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

        TiXmlPrinter printer;//�ṩ�Ĺ�����,Ŀ���ǽ�xml�����ݰ���ʽ���
        doc.Accept(&printer);
        cout << printer.CStr() << endl;//���

        TiXmlElement*node = doc.FirstChildElement();//��ȡ��һ��element�ڵ�
        cout << node->Value() << endl;//����ڵ��ֵ
        string t;
        node->QueryValueAttribute("type", &t);//��ȡ�ڵ�����
        cout << t << endl;

        TiXmlNode* node1 = doc.FirstChild()->FirstChild();
        node1->NextSibling()->ToElement()->QueryStringAttribute("type", &t);//��ȡ�ڶ����ӽڵ������
        cout << "2:" << t << endl;

        //ʹ�ñ����ķ�ʽ���д���
        TiXmlNode* child = nullptr;
        TiXmlElement* element = nullptr;
        TiXmlAttribute *attr = nullptr;
        int ct;
        while (child = doc.FirstChild()->IterateChildren(child))
        {
            cout << child->ValueStr() << "\t";
            ct = child->Type();
            cout << ct << "\t";
            //���ݲ�ͬ�Ľڵ���������Ӧ����  
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
