
#define _CRT_SECURE_NO_WARNINGS
#include "test_case_factory.h"
#include "pickle/pickle.h"
#include "ipc_win/ipc_pickle.h"
#include "ddtimer.h"
#include <iostream>
#include <vector>
#include <list>
#include <map>
#include <windows.h>

BEG_NSP_DDM

void my_pickele()
{
    pickle p1;
    p1 << 1;
    p1 << (u16)3;
    p1 << (u32)4;
    p1 << (u64)5;
    p1 << 6.0f;
    p1 << (double)7.0;

    p1 << "bcd";
    p1 << "efgh";
    p1 << "ijklm";
    p1 << std::string("nopqr");
    p1 << std::vector<std::string>{"st", "uvw", "xyza", "bcdef"};
    p1 << std::map<int, int>{ {3, 4}, { 5,6 }, { 1,2 } };
    p1 << "abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz\
abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz\
abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz\
abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz\
abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz\
abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz\
abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz\
abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz\
abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz\
abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz\
abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz\
abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz\
abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz\
abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz\
abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz\
abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz\
abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz\
abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz\
abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz\
abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz\
abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz";
    //////////////////////////////////////////////////////////////////////////
    int a = 0; // 1
    u16 c = 0; // 3
    u32 d = 0; // 4
    u64 e = 0; // 5
    float f = 0.0f; // 6
    double g = 0.0; // 7
    std::string i; // "bcd"
    std::string j; // "efgh"
    std::string k; // "ijklm"
    std::string l; // "nopqr"
    std::vector<std::string> m; // { "st", "uvw", "xyza", "bcdef" }
    std::map<int, int>n; // { {3,4}, {5,6}, {1,2} }
    pickle_reader pr(&p1);
    pr >> a
        >> c
        >> d
        >> e
        >> f
        >> g
        >> i
        >> j
        >> k
        >> l
        >> m
        >> n;
    std::string o;
    pr >> o;
}

void ipc_pickle() 
{
    ipc::Pickle pi;
    pi.WriteInt(1);
    pi.WriteUInt16(3);
    pi.WriteUInt32(4);
    pi.WriteUInt64(5);
    pi.WriteFloat(6.0f);
    pi.WriteDouble(7.0);
    pi.WriteString("bcd");
    pi.WriteString("efgh");
    pi.WriteString("ijklm");
    pi.WriteString(std::string("nopqr"));
    std::vector<std::string> vec{ "st", "uvw", "xyza", "bcdef" };
    pi.WriteInt((int)vec.size());
    for (const auto& it : vec) {
        pi.WriteString(it);
    }
    std::map<int, int> mp{ {3, 4}, { 5,6 }, { 1,2 } };
    pi.WriteInt((int)mp.size());
    for (const auto& it : mp) {
        pi.WriteInt(it.first);
        pi.WriteInt(it.second);
    }
    pi.WriteString("abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz\
abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz\
abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz\
abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz\
abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz\
abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz\
abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz\
abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz\
abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz\
abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz\
abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz\
abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz\
abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz\
abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz\
abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz\
abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz\
abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz\
abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz\
abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz\
abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz\
abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz");
    /////////////////////////////////reader/////////////////////////////////////////
    ipc::PickleIterator pc(pi);
    int a = 0;
    pc.ReadInt(&a);
    u16 c;
    pc.ReadUInt16(&c);

    uint32_t d;
    pc.ReadUInt32(&d);

    uint64_t e;
    pc.ReadUInt64(&e);

    float f = 0.0f;
    pc.ReadFloat(&f);

    double g = 0.0f;
    pc.ReadDouble(&g);

    std::string h;
    pc.ReadString(&h);
    std::string i;
    pc.ReadString(&i);
    std::string j;
    pc.ReadString(&j);
    std::string k;
    pc.ReadString(&k);

    std::vector<std::string> vec1;
    int size = 0;
    pc.ReadInt(&size);
    vec1.resize(size);
    for (int ii = 0; ii < size; ++ii) {
        pc.ReadString(&vec1[ii]);
    }

    std::map<int, int> mp1;
    int sizem = 0;
    pc.ReadInt(&sizem);
    for (int ii = 0; ii < sizem; ++ii) {
        int key = 0;
        int val = 0;
        pc.ReadInt(&key);
        pc.ReadInt(&val);
        mp1[key] = val;
    }

    std::string o;
    pc.ReadString(&o);
}
// 
// TEST(test_pickle, u1)
// {
//     ddtimer timer;
//     ::Sleep(1000);
// 
//     timer.reset();
//     for (int i = 0; i < 100000; ++i) {
//         my_pickele();
//     }
//     std::cout << "my_pickele " << timer.get_time_pass() / 1000 << std::endl;
// 
//     timer.reset();
//     for (int i = 0; i < 100000; ++i) {
//         ipc_pickle();
//     }
//     std::cout << "ipc_pickle " << timer.get_time_pass() / 1000 << std::endl;
// }

struct My
{
    char name[1024];
};
TEST(test_pickle, u2)
{

    My it;
    ::strcpy(it.name, "abcdefg");

    {
        pickle p1;
        p1 << "abcd";
        p1 << L"abcd";
        p1.append_buff(it.name, sizeof(it.name));

        pickle_reader pr(&p1);
        std::string s1;
        std::wstring s2;
        pr >> s1 >> s2;
        My it1;
        pr.read_buff(it1.name, sizeof(it.name));
    }

    ipc::Pickle pi;
    pi.WriteData(it.name, sizeof(it.name));

    My it1;
    ipc::PickleIterator pc(pi);
    int nameLen = sizeof(it1.name);
    char* name = nullptr;
    pc.ReadData((const char**)(&name), &nameLen);
    ::memcpy(it1.name, name, sizeof(it1.name));
}

TEST(test_pickle, pod)
{
    struct Pod
    {
        double a;
        float b;
        u8 c;
        u16 d;
        u32 e;
        u64 f;
        s8 g;
        s16 h;
        s32 i;
        s64 j;
    };
    static_assert(std::is_pod<Pod>::value, "Pod is not pod");
    Pod pod{ 1.0, 1.0f, 1, 2, 3, 4, 5, 6, 7, 8 };
    pickle p1;
    p1 << pod;
    pickle_reader pr(&p1);
    Pod podr;
    pr >> podr;
}

struct PickleNotPod
{
    double a = 0;
    float b = 0;
    std::string c;
};

template<>
class pickle_reader_writer_helper<PickleNotPod, container_traits::container_traits_none>
{
public:
    static pickle& write(pickle& pck, const PickleNotPod& r)
    {
        pck << r.a;
        pck << r.b;
        pck << r.c;
        return pck;
    }

    static pickle_reader& read(pickle_reader& reader, PickleNotPod& r)
    {
        reader >> r.a;
        reader >> r.b;
        reader >> r.c;
        return reader;
    }
};

TEST(test_pickle, PickleNotPod)
{
    PickleNotPod s{ 1.0, 1.0f, "abc" };
    pickle p1;
    p1 << s;
    pickle_reader pr(&p1);
    PickleNotPod ss;
    pr >> ss;
}

struct PickleNotPod20
{
    float a = 1;
    float a1 = 1;
    float a2 = 1;
    float a3 = 1;
    float a4 = 1;
    float a5 = 1;
    float a6 = 1;
    float a7 = 1;
    float a8 = 1;
    float a9 = 1;
    float a10 = 1;
    float a11 = 1;
    float a12 = 1;
    float a13 = 1;
    float a14 = 1;
    float a15 = 1;
    float a16 = 1;
    float a17 = 1;
    float a18 = 1;
    float a19 = 1;
};
DD_PICKLE_TRAITS_GEN(PickleNotPod20, a, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19)
TEST(test_pickle, PickleNotPod20)
{
    PickleNotPod20 s{ 2.0, 2.0, 2.0 , 1.0 , 1.0 , 1.0 , 1.0 , 1.0 , 1.0 , 1.0 , 1.0 , 1.0 , 1.0 , 1.0 , 1.0 , 1.0 , 1.0 , 1.0 , 1.0 , 1.0 };
    pickle p1;
    p1 << s;
    pickle_reader pr(&p1);
    PickleNotPod20 ss;
    pr >> ss;
}

struct PickleNotPod25 : public PickleNotPod20
{
    char a = 1;
    char a1 = 1;
    char a2 = 1;
    char a3 = 1;
    std::string a4 = "abced";
};
DD_PICKLE_TRAITS_GEN_EX(PickleNotPod25, DD_EXPEND(PickleNotPod20), a, a1, a2, a3, a4)
TEST(test_pickle, PickleNotPod25)
{
    PickleNotPod25 s;
    s.a4 = "ddd";
    pickle p1;
    p1 << s;
    pickle_reader pr(&p1);
    PickleNotPod25 ss;
    pr >> ss;
}

struct PickleNotPod0
{
};
DD_PICKLE_TRAITS_GEN(PickleNotPod0)
TEST(test_pickle, PickleNotPod0)
{
    PickleNotPod0 s;
    pickle p1;
    p1 << s;
    pickle_reader pr(&p1);
    PickleNotPod0 ss;
    pr >> ss;
}

struct PickleNotPod0_b : public PickleNotPod20, PickleNotPod0
{
};
DD_PICKLE_TRAITS_GEN_EX(PickleNotPod0_b, DD_EXPEND(PickleNotPod20, PickleNotPod0))
TEST(test_pickle, PickleNotPod0_b)
{
    PickleNotPod0_b s;
    pickle p1;
    p1 << s;
    pickle_reader pr(&p1);
    PickleNotPod0_b ss;
    pr >> ss;
}

// DD_PICKLE_TRAITS_GEN??????20??????
// struct PickleNotPod21
// {
//     char a;
//     char a1;
//     char a2;
//     char a3;
//     char a4;
//     char a5;
//     char a6;
//     char a7;
//     char a8;
//     char a9;
//     char a10;
//     char a11;
//     char a12;
//     char a13;
//     char a14;
//     char a15;
//     char a16;
//     char a17;
//     char a18;
//     char a19;
//     char a20;
// };
// DD_PICKLE_TRAITS_GEN(PickleNotPod21, a, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20)
// TEST(test_pickle, PickleNotPod21)
// {
//     PickleNotPod20 s{ 1.0, 1.0, 1.0 , 1.0 , 1.0 , 1.0 , 1.0 , 1.0 , 1.0 , 1.0 , 1.0 , 1.0 , 1.0 , 1.0 , 1.0 , 1.0 , 1.0 , 1.0 , 1.0 , 1.0 , 1.0 };
//     pickle p1;
//     p1 << s;
//     pickle_reader pr(&p1);
//     PickleNotPod ss;
//     pr >> ss;
// }

END_NSP_DDM
