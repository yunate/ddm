#define  _CRT_SECURE_NO_WARNINGS

#include "cmd_line_util.h"

#include <windows.h>
#include <shellapi.h>
BEG_NSP_DDM
void cmd_line_util::get_cmds(std::vector<ddstr>& cmds)
{
    ddstr cmd = ::GetCommandLine();

    // ���� ���� �ո�
    if (cmd.length() < 3) {
        return;
    }

    size_t index = 1;

    for (; index < cmd.length(); ++index) {
        if (cmd[index] == _DDT('\"')) {
            cmds.push_back(cmd.substr(0, index + 1));
            break;
        }
    }

    ++index;
    size_t beg = index;

    for (; index < cmd.length(); ++index) {
        if (cmd[index] == _DDT(' ')) {
            if (beg < index) {
                cmds.push_back(cmd.substr(beg, index - beg));
            }

            beg = index + 1;
        }
    }

    if (beg < cmd.length()) {
        cmds.push_back(cmd.substr(beg));
    }
}

END_NSP_DDM

