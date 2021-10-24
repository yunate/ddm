
#include "test_case_factory.h"
#include "thread/async/sync_n_buff.h"
#include <thread>
#include <iostream>
BEG_NSP_DDM
TEST(test_sync_n_buff, test_sync_n_buff_queue)
{
    sync_n_queue<int> xx(90);
    xx.push(1);
    xx.push(2);
    xx.push(3);
    bool l = xx.push(4, 10); l;
    std::queue<int> que;
    int yy = xx.pop(); yy;
}

TEST(test_sync_n_buff, test_sync_n_buff_stack)
{
    // sync_n_stack
    sync_n_stack<int> xx;
    xx.push(1);
    xx.push(2);
    xx.push(3);
    xx.push(4);
    std::queue<int> que;
    int yy = xx.pop(); yy;
}
END_NSP_DDM
