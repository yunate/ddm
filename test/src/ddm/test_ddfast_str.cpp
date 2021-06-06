
#include "test_case_factory.h"
#include "str/ddfast_str.hpp"

BEG_NSP_DDM
TEST(test_ddfast_str, trim)
{
    ddfast_str fast_str1("f   ");
    fast_str1.trim();
    DD_ASSERT(fast_str1.length() == 1);

    ddfast_str fast_str2("   f");
    fast_str2.trim();
    DD_ASSERT(fast_str2.length() == 1);

    ddfast_str fast_str3("");
    fast_str3.trim();
    DD_ASSERT(fast_str3.length() == 0);

    ddfast_str fast_str4(" f ");
    fast_str4.trim();
    DD_ASSERT(fast_str4.length() == 1);

    ddfast_str fast_str5("  ");
    fast_str5.trim();
    DD_ASSERT(fast_str5.length() == 0);
}

TEST(test_ddfast_str, beg_with)
{
    ddfast_str fast_str("abc_def_ghi_jkl_mn");
    DD_ASSERT(fast_str.beg_with(""));
    DD_ASSERT(fast_str.beg_with("abc_"));
    DD_ASSERT(!fast_str.beg_with("abcd"));

    ddfast_str fast_str1("");
    DD_ASSERT(!fast_str1.beg_with("abc_"));
    DD_ASSERT(fast_str1.beg_with(""));
}

TEST(test_ddfast_str, end_with)
{
    ddfast_str fast_str("abc_def_ghi_jkl_mn");
    DD_ASSERT(fast_str.end_with(""));
    DD_ASSERT(fast_str.end_with("mn"));
    DD_ASSERT(!fast_str.end_with("abcd"));

    ddfast_str fast_str1("");
    DD_ASSERT(!fast_str1.end_with("abc_"));
    DD_ASSERT(fast_str1.end_with(""));
}

TEST(test_ddfast_str, sub_str)
{
    {
        ddfast_str fast_str("abc_def_ghi_jkl_mn");
        ddfast_str sub = fast_str.sub_str(0, fast_str.length());
        DD_ASSERT(sub == fast_str);
    }

    {
        ddfast_str fast_str("abc_def_ghi_jkl_mn");
        ddfast_str sub = fast_str.sub_str(0);
        DD_ASSERT(sub == fast_str);
    }

    {
        ddfast_str fast_str("abc_def_ghi_jkl_mn");
        ddfast_str sub = fast_str.sub_str(fast_str.find("d"));
        DD_ASSERT(sub == "def_ghi_jkl_mn");
    }

    {
        ddfast_str fast_str("abc_def_ghi_jkl_mn");
        ddfast_str sub = fast_str.sub_str(fast_str.find("d"), 3);
        DD_ASSERT(sub == "def");
    }

    {
        ddfast_str fast_str1("");
        ddfast_str sub1 = fast_str1.sub_str(0, fast_str1.length());
        DD_ASSERT(sub1 == fast_str1);
    }
}

TEST(test_ddfast_str, to_sub_str)
{
    {
        ddfast_str fast_str("abc_def_ghi_jkl_mn");
        fast_str.to_sub_str(0, fast_str.length());
        ddfast_str sub(fast_str);
        DD_ASSERT(sub == fast_str);
    }

    {
        ddfast_str fast_str("abc_def_ghi_jkl_mn");
        fast_str.to_sub_str(0);
        ddfast_str sub(fast_str);
        DD_ASSERT(sub == fast_str);
    }

    {
        ddfast_str fast_str("abc_def_ghi_jkl_mn");
        fast_str.to_sub_str(fast_str.find("d"));
        ddfast_str sub(fast_str);
        DD_ASSERT(sub == "def_ghi_jkl_mn");
    }

    {
        ddfast_str fast_str("abc_def_ghi_jkl_mn");
        fast_str.to_sub_str(fast_str.find("d"), 3);
        ddfast_str sub(fast_str);
        DD_ASSERT(sub == "def");
    }

    {
        ddfast_str fast_str1("");
        fast_str1.to_sub_str(0, fast_str1.length());
        ddfast_str sub1(fast_str1);
        DD_ASSERT(sub1 == fast_str1);
    }
}

TEST(test_ddfast_str, find)
{
    {
        ddfast_str fast_str("abc_def_ghi_jkl_mn");
        DD_ASSERT(fast_str.find('a') == 0);
        DD_ASSERT(fast_str.find("a") == 0);
        DD_ASSERT(fast_str.find('n') == fast_str.length() - 1);
        DD_ASSERT(fast_str.find("n") == fast_str.length() - 1);
        DD_ASSERT(fast_str.find("abc") == 0);
        DD_ASSERT(fast_str.find("mn") == fast_str.length() - 2);
        DD_ASSERT(fast_str.find("abc_def_ghi_jkl_mn") == 0);
        DD_ASSERT(fast_str.find("") == 0);
        DD_ASSERT(fast_str.find("ff") == ddfast_str::xnpos);
        DD_ASSERT(fast_str.find("abc_def_ghi_jkl_mnff") == ddfast_str::xnpos);
        DD_ASSERT(fast_str.find('x') == ddfast_str::xnpos);
    }

    {
        ddfast_str fast_str("");
        DD_ASSERT(fast_str.find('a') == ddfast_str::xnpos);
        DD_ASSERT(fast_str.find("a") == ddfast_str::xnpos);
        DD_ASSERT(fast_str.find('n') == ddfast_str::xnpos);
        DD_ASSERT(fast_str.find("n") == ddfast_str::xnpos);
        DD_ASSERT(fast_str.find("abc") == ddfast_str::xnpos);
        DD_ASSERT(fast_str.find("mn") == ddfast_str::xnpos);
        DD_ASSERT(fast_str.find("abc_def_ghi_jkl_mn") == ddfast_str::xnpos);
        DD_ASSERT(fast_str.find("") == 0);
        DD_ASSERT(fast_str.find("ff") == ddfast_str::xnpos);
        DD_ASSERT(fast_str.find("abc_def_ghi_jkl_mnff") == ddfast_str::xnpos);
        DD_ASSERT(fast_str.find('x') == ddfast_str::xnpos);
    }
}

TEST(test_ddfast_str, find_last_of)
{
    {
        ddfast_str fast_str("abc_def_ghi_jkl_mn");
        DD_ASSERT(fast_str.find_last_of('a') == 0);
        DD_ASSERT(fast_str.find_last_of("a") == 0);
        DD_ASSERT(fast_str.find_last_of('n') == fast_str.length() - 1);
        DD_ASSERT(fast_str.find_last_of("n") == fast_str.length() - 1);
        DD_ASSERT(fast_str.find_last_of("abc") == 0);
        DD_ASSERT(fast_str.find_last_of("mn") == fast_str.length() - 2);
        DD_ASSERT(fast_str.find_last_of("abc_def_ghi_jkl_mn") == 0);
        DD_ASSERT(fast_str.find_last_of("") == 0);
        DD_ASSERT(fast_str.find_last_of("ff") == ddfast_str::xnpos);
        DD_ASSERT(fast_str.find_last_of("abc_def_ghi_jkl_mnff") == ddfast_str::xnpos);
        DD_ASSERT(fast_str.find_last_of('x') == ddfast_str::xnpos);
    }

    {
        ddfast_str fast_str("");
        DD_ASSERT(fast_str.find_last_of('a') == ddfast_str::xnpos);
        DD_ASSERT(fast_str.find_last_of("a") == ddfast_str::xnpos);
        DD_ASSERT(fast_str.find_last_of('n') == ddfast_str::xnpos);
        DD_ASSERT(fast_str.find_last_of("n") == ddfast_str::xnpos);
        DD_ASSERT(fast_str.find_last_of("abc") == ddfast_str::xnpos);
        DD_ASSERT(fast_str.find_last_of("mn") == ddfast_str::xnpos);
        DD_ASSERT(fast_str.find_last_of("abc_def_ghi_jkl_mn") == ddfast_str::xnpos);
        DD_ASSERT(fast_str.find_last_of("") == 0);
        DD_ASSERT(fast_str.find_last_of("ff") == ddfast_str::xnpos);
        DD_ASSERT(fast_str.find_last_of("abc_def_ghi_jkl_mnff") == ddfast_str::xnpos);
        DD_ASSERT(fast_str.find_last_of('x') == ddfast_str::xnpos);
    }
}

TEST(test_ddfast_str, split)
{
    {
        std::vector<ddfast_str> out;
        ddfast_str fast_str("abc_def_ghi_jkl_mn");
        fast_str.split("_", out);
        DD_ASSERT(out.size() == 5);

        out.clear();
        fast_str.split("__", out);
        DD_ASSERT(out.size() == 1);

        out.clear();
        fast_str.split("abc_def_ghi_jkl_mn", out);
        DD_ASSERT(out.size() == 2);

        out.clear();
        fast_str.split("abc_def_ghi_jkl_mnd", out);
        DD_ASSERT(out.size() == 1);
    }

    {
        std::vector<ddfast_str> out;
        ddfast_str fast_str("");
        fast_str.split("_", out);
        DD_ASSERT(out.size() == 1);

        out.clear();
        fast_str.split("__", out);
        DD_ASSERT(out.size() == 1);

        out.clear();
        fast_str.split("abc_def_ghi_jkl_mn", out);
        DD_ASSERT(out.size() == 1);

        out.clear();
        fast_str.split("abc_def_ghi_jkl_mnd", out);
        DD_ASSERT(out.size() == 1);
    }
}

END_NSP_DDM
