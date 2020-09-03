/* Table Header */

#ifndef TABLE_H
#define TABLE_H

// C Headers
#include <stdbool.h>

typedef struct _array array_t;
struct _private;

typedef struct _table {

    int size;
    int breakc;
    char *source;

    struct _private *p;

} table_t;

extern struct _table* table_ctor(const char *hex_file);
extern void table_dtor(struct _table *this);

extern int table_step(struct _table *this);
extern int table_add_breakp(struct _table *this, const char *point);
extern int table_del_breakp(struct _table *this, const char *point);

extern void table_define(struct _table *this, const char *alias, const char *seq);

extern void table_set_tip(struct _table *this, const int line);
extern int table_get_tip(const struct _table *this);

extern void table_jmp(struct _table *this, const int exec_addr);

extern bool table_is_breakp(const struct _table *this);
extern bool table_is_sync(const struct _table *this, const int hex_addr);

extern void table_content(const struct _table *this, array_t *buffer);
extern void table_breakp(const struct _table *this, array_t *buffer);

#endif
