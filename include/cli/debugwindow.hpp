/* DebugWindow Header */

#ifndef DEBUGWINDOW_HPP
#define DEBUGWINDOW_HPP

#define R 0x01
#define G 0x02
#define B 0x03
#define Y 0x04
#define M 0x05
#define C 0x06
#define DEF 0x07

#define GPR_PANEL 0
#define SREG_PANEL 1
#define CODE_PANEL 2
#define DATA_PANEL 3
#define EEPROM_PANEL 4
#define OUTPUT_PANEL 5
#define SIDE_PANEL 6

#define N_PANEL 7

class Panel;
class Prompt;

class DebugWindow {

public:
    DebugWindow(const int table_size);                                          // DebugWindow Constructor
    ~DebugWindow(void);                                                         // DebugWindow Destructor

    std::string read_prompt(void);                                              // reading from prompt

    void add(const int ptype, const std::string & data, const int color);       // writing to specific panel without updating
    void write(const int ptype, const std::string & data, const int color);     // writing to specific panel and updating
    void highlight(const int ptype, const std::string & data);                  // writing to specific panel using syntax highlighter
    void clear_specific(const int ptype);                                       // clear specific panel
    void clear(void);                                                           // clear all non-static panels

    void update(const int ptype);                                               // refresh a specific panel
    void update_all(void);                                                      // refresh all panels

    void move_cursor(const int ptype, const int offs);                          // moving cursor of Panel 'ptype'
    void set_cursor(const int ptype, const int at);                             // setting cursor to 'at'
    int cursor_of(const int ptype);                                             // returning current cursor of 'ptype'

    int get_height(const int ptype);                                            // getting height of specific panel
    int get_width(const int ptype);                                             // getting width of specific panel
    int get_y(const int ptype);                                                 // getting y position of specific panel
    int get_x(const int ptype);                                                 // getting x position of specific panel

    void close_panel(const int ptype);                                          // close a specific panel

private:
    Panel **panel;                                                              // Panels of DebugWindow
    Prompt *prompt;                                                             // Prompt of DebugWindow
};

#endif