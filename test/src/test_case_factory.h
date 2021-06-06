
#ifndef test_case_factory_h_
#define test_case_factory_h_ 1
#include "singleton.hpp"

#include <functional>
#include <string>
#include <unordered_set>
#include <unordered_map>
#include <memory>

BEG_NSP_DDM

class itest_case {
public:
    virtual void run() = 0;
};

class test_case_factory {
public:
    inline void push_case_creator(const std::string& name, itest_case* testCase)
    {
        m_case_creator[name].push_back(testCase);
    }

    inline void insert_white_type(const std::string& name)
    {
        m_white_type.insert(name);
    }

    inline void run()
    {
        for (auto& it : m_case_creator) {
            if (m_white_type.find(it.first) != m_white_type.end()) {
                for (size_t i = 0; i < it.second.size(); ++i) {
                    itest_case* testCase = it.second[i];
                    testCase->run();
                }
            }
        }
    }

private:
    std::unordered_map<std::string, std::vector<itest_case*>> m_case_creator;
    std::unordered_set<std::string> m_white_type;
};

#define TCF singleton<test_case_factory>::get_instance()

#define TEST(ty, N) \
class ty ## N ## _test_case : public itest_case \
{ \
public: \
    ty ## N ## _test_case() \
    { \
        m_dummy; \
        TCF.push_case_creator(#ty, &m_dummy); \
    } \
    virtual void run() override; \
private: \
    static ty ## N ## _test_case m_dummy; \
}; \
ty ## N ## _test_case ty ## N ## _test_case::m_dummy; \
void ty ## N ## _test_case::run()

END_NSP_DDM
#endif // test_case_factory_h_
