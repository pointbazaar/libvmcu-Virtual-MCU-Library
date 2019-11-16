/* Implementation of Debugger Functions */

// C++ Headers
#include <iostream>
#include <iomanip>
#include <string>
#include <cstring>
#include <chrono>
#include <thread>

// Project Headers
#include "debugger.hpp"
#include "ehandling.hpp"
#include "debugparser.hpp"
#include "menus.hpp"
#include "table.hpp"
#include "sys.hpp"

#define movec(cursor, offs, range) *cursor = ((*cursor) + offs) % range

using namespace std;
using namespace std::chrono;
using namespace std::this_thread;

namespace {

    void jump_forward(Sys *sys, int cursor) {

        do {

            if(sys->table_is_break() == true)
                break;

            debug_menu(sys, cursor);
            sys->step();

            sleep_for(milliseconds(100));

        } while(sys->is_terminated() == false);
    }

};

void debug(Table *table) {

    int cursor = 0;
    string last_select, select;

    DebugParser parser;
    Sys sys(table);

    if(sys.table_has_break() == true)
        jump_forward(&sys, cursor);
	
    do {

        debug_menu(&sys, cursor);
        getline(cin, select);

        if(sys.table_size() <= 0)
            continue;
		
        if(select != "")
            last_select = select;

        switch(parser.index_of(last_select)) {

            case 0: sys.step(); break;
            case 1: movec(&cursor, +1, 4); break;
            case 2: sys.scale_data(+1); break;
            case 3: sys.scale_data(-1); break;
            case 4: jump_forward(&sys, cursor); break;
            case 5: sys.table_set_tip(0); break;

            default: /* Ignoring invalid Input */ break;
        } 
		
    } while(select != "e");
}


