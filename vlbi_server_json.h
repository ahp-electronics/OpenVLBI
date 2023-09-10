/*  OpenVLBI - Open Source Very Long Baseline Interferometry
*   Copyright © 2017-2023  Ilia Platone
*
*   This program is free software; you can redistribute it and/or modify
*   it under the terms of the GNU General Public License as published by
*   the Free Software Foundation; either version 2 of the License, or
*   (at your option) any later version.
*
*   This program is distributed in the hope that it will be useful,
*   but WITHOUT ANY WARRANTY; without even the implied warranty of
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*   GNU General Public License for more details.
*
*   You should have received a copy of the GNU General Public License along
*   with this program; if not, write to the Free Software Foundation, Inc.,
*   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/

#ifndef VLBI_SERVER_JSON_H
#define VLBI_SERVER_JSON_H

#include "vlbi_server.h"

using namespace VLBI;

class JSONServer : public VLBI::Server
{
    public:
        JSONServer();
        ~JSONServer() override;

        int Init(int argc, char** argv) override;
        void Parse() override;

    private:
        bool CheckMask(unsigned long mask, int n);
        bool nodelay;
        bool relative;
        vlbi_node node;
};

#endif //VLBI_SERVER_JSON_H
