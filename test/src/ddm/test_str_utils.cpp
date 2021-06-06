
#include "str/str_utils.h"
#include "str/url_parser.h"
#include "color_print.h"

#include "test_case_factory.h"

BEG_NSP_DDM

TEST(test_str_utils, uft16_uft8)
{
    ddstrw str = L"hello test";
    ddstra utf8Str;
    str_utils::uft16_uft8(str, utf8Str);
}

TEST(test_str_utils, url_parser)
{
    std::vector<std::string> urls;
    urls.push_back("http://www.baidu.com:20/");
    urls.push_back("http://www.baidu.com:20/?#aa");
    urls.push_back("http:\\\\FF:123@www.baidu.com:80\\wget.apx\\/dd\\/?name=ydh#anchor");
    urls.push_back("http:\\\\:123@www.baidu.com:83\\wget.apx//dd/?name=ydh#anchor");
    urls.push_back("http:\\\\FF:@www.baidu.com:82\\wget.apx\\\\dd/?name=ydh#anchor");
    urls.push_back("http:\\\\FF@www.baidu.com:81\\wget.apx/dd\\\\?name=ydh#anchor");
    urls.push_back("http:\\\\FF@www.baidu.com:\\wget.apx/dd//?name=ydh#anchor");
    urls.push_back("http:\\\\FF@www.baidu.com\\wget.apx/dd/\\?name=ydh#anchor");
    urls.push_back("http:\\\\FF@\\wget.apx/dd/?name=ydh#anchor");
    urls.push_back("http://www.baidu.com/wget.apx/dd/?name=ydh#anchor");
    urls.push_back("http://www.baidu.com#anchor?name=ydh/wget.apx/dd/");
    urls.push_back("http://www.baidu.com/wget.apx/#anchor");
    urls.push_back("http://www.baidu.com/wget.apx/dd/?name=ydh#anchor");
    urls.push_back("http://www.baidu.com/wget.apx/dd/?name=ydh#anchor");
    urls.push_back("http://www.baidu.com/wget.apx/dd?name=ydh#");
    urls.push_back("http://www.baidu.com/wget.apx/dd?name=ydh");
    urls.push_back("http://www.baidu.com/wget.apx/dd/cc?");
    urls.push_back("http://www.baidu.com/wget.apx/");
    urls.push_back("http://ydh:123@w/wget.apx");
    urls.push_back("http:///wget.apx");
    urls.push_back("http://@/wget.apx");
    urls.push_back("http://:@/wget.apx");
    urls.push_back("http://:@:/wget.apx");
    urls.push_back("http://:/wget.apx");
    urls.push_back("http://www.baidu.com:20/");
    urls.push_back("http://www.baidu.com:20");
    urls.push_back("http://www.baidu.com:");
    urls.push_back("http://www.baidu.com/");
    urls.push_back("http:///?#");
    urls.push_back("http://w/?dada#");
    urls.push_back("http://w/?dada#daada");
    urls.push_back("http://w/#dada?daada");
    urls.push_back("http://w/#dada#ab");
    urls.push_back("http://");
    urls.push_back("http:/");

    for (auto& it : urls)
    {
        color_printf(Gray, "\r\n");
        color_printf(Gray, it.c_str());
        color_printf(Gray, "\r\n");
        dd_url url;
        parse_url(it, url);
        color_printf(Green, "norml:");
        color_printf(Green, url.get_formated_url().c_str());

        dd_url urlEx;
        parse_url_regex(it, urlEx);
        color_printf(Green, "\r\nregex:");
        color_printf(Green, urlEx.get_formated_url().c_str());
        color_printf(Green, "\r\n");
    }
}


END_NSP_DDM
