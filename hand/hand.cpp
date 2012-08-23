/*
 *  Copyright 2012 Alex Stefaniuc
 *
 *  This file is part of Hand.
 *
 *  Hand is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License
 *  as published by the Free Software Foundation, either version 3
 *  of the License, or (at your option) any later version.
 *
 *  Hand is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with Hand. If not, see <http://www.gnu.org/licenses/>.
 */

#include "base/handserver.h"


using namespace std;


int main(int argc, const char *argv[])
{
    HandServer* server = HandServer::GetInstance();
    if(argc == 2)
    {
        string file = argv[1];
        if(!server->Present(file))
            return -1;
    }
    // Step 3 of the 3 staged server starting process
    // Start the timer driven (callback) execution and stop the current thread
    server->Beat();
    return 0;
}
