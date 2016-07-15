// +------------------------------------------------------------------+
// |             ____ _               _        __  __ _  __           |
// |            / ___| |__   ___  ___| | __   |  \/  | |/ /           |
// |           | |   | '_ \ / _ \/ __| |/ /   | |\/| | ' /            |
// |           | |___| | | |  __/ (__|   <    | |  | | . \            |
// |            \____|_| |_|\___|\___|_|\_\___|_|  |_|_|\_\           |
// |                                                                  |
// | Copyright Mathias Kettner 2014             mk@mathias-kettner.de |
// +------------------------------------------------------------------+
//
// This file is part of Check_MK.
// The official homepage is at http://mathias-kettner.de/check_mk.
//
// check_mk is free software;  you can redistribute it and/or modify it
// under the  terms of the  GNU General Public License  as published by
// the Free Software Foundation in version 2.  check_mk is  distributed
// in the hope that it will be useful, but WITHOUT ANY WARRANTY;  with-
// out even the implied warranty of  MERCHANTABILITY  or  FITNESS FOR A
// PARTICULAR PURPOSE. See the  GNU General Public License for more de-
// tails. You should have  received  a copy of the  GNU  General Public
// License along with GNU Make; see the file  COPYING.  If  not,  write
// to the Free Software Foundation, Inc., 51 Franklin St,  Fifth Floor,
// Boston, MA 02110-1301 USA.

#include "HostlistColumn.h"
#include "HostlistFilter.h"
#include "Renderer.h"
#include "auth.h"
#include "opids.h"

using std::string;

hostsmember *HostlistColumn::getMembers(void *data) {
    data = shiftPointer(data);
    if (data == nullptr) {
        return nullptr;
    }

    return *reinterpret_cast<hostsmember **>(reinterpret_cast<char *>(data) +
                                             _offset);
}

void HostlistColumn::output(void *row, Renderer *renderer, contact *auth_user) {
    renderer->outputBeginList();
    hostsmember *mem = getMembers(row);

    bool first = true;
    while (mem != nullptr) {
        host *hst = mem->host_ptr;
        if ((auth_user == nullptr) ||
            is_authorized_for(auth_user, hst, nullptr)) {
            if (!first) {
                renderer->outputListSeparator();
            } else {
                first = false;
            }
            if (!_show_state) {
                renderer->outputString(hst->name);
            } else {
                renderer->outputBeginSublist();
                renderer->outputString(hst->name);
                renderer->outputSublistSeparator();
                renderer->outputInteger(hst->current_state);
                renderer->outputSublistSeparator();
                renderer->outputInteger(hst->has_been_checked);
                renderer->outputEndSublist();
            }
        }
        mem = mem->next;
    }
    renderer->outputEndList();
}

Filter *HostlistColumn::createFilter(RelationalOperator relOp,
                                     const string &value) {
    return new HostlistFilter(this, relOp, value);
}
