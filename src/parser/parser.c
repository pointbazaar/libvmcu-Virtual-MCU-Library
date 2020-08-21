/* Parser Implementation */

// C Headers
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// Project Headers
#include "parser/parser.h"
#include "parser/commands.h"
#include "collections/list.h"
#include "collections/map.h"
#include "misc/ehandling.h" 
#include "misc/filemanip.h"
#include "misc/stringmanip.h"

struct _private {

    map_t *map;
};

/* Forward Declaration of private Functions */

static bool parser_parse_arg(const struct _parser *this, list_t *items);
static bool parser_parse_debug(const struct _parser *this, list_t *items);

/* --- Public --- */

struct _parser* parser_ctor(CONTEXT context) {

    struct _parser *parser;

    if((parser = malloc(sizeof(struct _parser))) == NULL)
        return NULL;

    if((parser->p = malloc(sizeof(struct _private))) == NULL) {

        free(parser);
        return NULL;
    }

    parser->p->map = map_ctor(ncmd[context]);

    for(int i = 0; i < ncmd[context]; i++)
        map_put(parser->p->map, commands[context][i], i);

    parser->context = context;
    return parser;
}

void parser_dtor(struct _parser *this) {

    map_dtor(this->p->map);

    free(this->p);
    free(this);
}

int parser_eval(const struct _parser *this, const char *cmd) {

    list_t *items = ls_ctor(NULL, NULL);
    split(cmd, ' ', items);

    const char *keyword = (char*) ls_at(items, 0);

    if(map_get(this->p->map, keyword) < 0) {

        ls_dtor(items);
        return -1;
    }

    bool passed = false;

    switch(this->context) {

        case ARG: passed = parser_parse_arg(this, items); break;
        case DEBUG: passed = parser_parse_debug(this, items); break;

        default: break;
    }

    const int res = (passed ? map_get(this->p->map, keyword) : -1);
    ls_dtor(items);

    return res;
}

/* --- Private --- */

static bool parser_parse_arg(const struct _parser *this, list_t *items) {

    const char *keyword = ls_at(items, 0);

    const int index = map_get(this->p->map, keyword);
    const int argc = items->size - 1;

    switch(index) {

        case 0: case 1:

            if(argc != 1)
                print_status("Missing File.", true);

            const char *param = ls_at(items, 1);

            if(file_exists(param) == false)
                print_status("File does not exist.", true);

        break;

        case 2:

            if(argc != 0)
                print_status("Too many arguments.", true);

        break;

        default: return false;
    }

    return true;
}

static bool parser_parse_debug(const struct _parser *this, list_t *items) {

    const char *keyword = ls_at(items, 0);

    const int index = map_get(this->p->map, keyword);
    const int argc = items->size - 1;

    switch(index) {

        case 0: case 1: case 2: case 3: case 4:

            if(argc != 0)
                return false;

        break;

        case 5:

            if(argc != 0)
                return false;

        break;

        case 6:

            if(argc != 1)
                return false;

        break;

        case 7: case 8:

            if(argc != 0)
                return false;

        break;
        
        case 9: case 10:

            if(argc != 1)
                return false;

        break;

        case 11: case 12:

            if(argc != 2)
                return false;

        break;

        case 13:

            if(argc != 1)
                return false;

        break;

        case 14: case 15: case 16:

            if(argc != 0)
                return false;

        break;

        case 17: case 18:
        
            if(argc != 1)
                return false;

            if(get_int((char*) ls_at(items, 1)) < 0)
                return false;
        
        break;

        case 19:

            if(argc != 2)
                return false;

        break;
        
        case 20: case 21:
        
            if(argc != 0)
                return false;
        
        break;
        
        default: print_status("Could not parse line.", true); break;
    }
        
    return true;
}