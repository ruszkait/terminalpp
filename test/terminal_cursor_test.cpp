#include "terminalpp/terminal.hpp"
#include "expect_sequence.hpp"
#include <gtest/gtest.h>
#include <string>
#include <iostream>

TEST(a_terminal_with_an_unknown_location, sends_absolute_coordinates_when_moving)
{
    terminalpp::terminal terminal;

    expect_sequence(
        std::string("\x1B[3;3H"),
        terminal.move_cursor({2, 2}));
}

TEST(a_terminal_with_an_unknown_location, omits_coordinates_when_moving_to_top_left)
{
    terminalpp::terminal terminal;

    expect_sequence(
        std::string("\x1B[H"),
        terminal.move_cursor({0, 0}));
}

TEST(a_terminal_with_a_known_location, omits_coordinates_when_moving_to_top_left)
{
    terminalpp::terminal terminal;
    terminal.move_cursor({10, 10});

    expect_sequence(
        std::string("\x1B[H"),
        terminal.move_cursor({0, 0}));
}

TEST(a_terminal_with_a_known_location, sends_nothing_when_moving_to_the_same_coordinates)
{
    terminalpp::terminal terminal;
    terminal.move_cursor({10, 10});

    expect_sequence(std::string{}, terminal.move_cursor({10, 10}));
}

TEST(a_terminal_supporting_cha_default_and_a_known_location,
     when_moving_to_the_home_column_sends_cha_with_no_argument)
{
    terminalpp::behaviour behaviour;
    behaviour.supports_cha         = true;
    behaviour.supports_cha_default = true;

    terminalpp::terminal terminal(behaviour);
    terminal.move_cursor({10, 10});

    expect_sequence(
        std::string("\x1B[G"),
        terminal.move_cursor({0, 10}));
}

TEST(a_terminal_supporting_cha_but_not_cha_default_and_a_known_location,
     when_moving_to_the_home_column_sends_cha_with_column_argument)
{
    terminalpp::behaviour behaviour;
    behaviour.supports_cha         = true;
    behaviour.supports_cha_default = false;

    terminalpp::terminal terminal(behaviour);
    terminal.move_cursor({10, 10});

    expect_sequence(
        std::string("\x1B[1G"),
        terminal.move_cursor({0, 10}));
}

TEST(a_terminal_not_supporting_cha_and_a_known_location,
     when_moving_to_the_home_column_sends_cub)
{
    terminalpp::behaviour behaviour;
    behaviour.supports_cha         = false;
    behaviour.supports_cha_default = false;

    terminalpp::terminal terminal(behaviour);
    terminal.move_cursor({10, 10});

    expect_sequence(
        std::string("\x1B[10D"),
        terminal.move_cursor({0, 10}));
}

TEST(a_terminal_with_known_location,
     when_moving_left_to_at_least_the_tenth_column_sends_cub)
{
    terminalpp::terminal terminal;
    terminal.move_cursor({20, 10});

    expect_sequence(
        std::string("\x1B[11D"),
        terminal.move_cursor({9, 10}));
}

TEST(a_terminal_with_known_location,
     when_moving_left_to_less_than_the_tenth_column_sends_cha)
{
    terminalpp::behaviour behaviour;
    behaviour.supports_cha = true;

    terminalpp::terminal terminal(behaviour);
    terminal.move_cursor({20, 10});

    expect_sequence(
        std::string("\x1B[9G"),
        terminal.move_cursor({8, 10}));
}

TEST(a_terminal_with_a_known_location_but_no_cha_support,
     when_moving_left_to_less_than_the_tenth_column_sends_cub)
{
    terminalpp::behaviour behaviour;
    behaviour.supports_cha = false;

    terminalpp::terminal terminal(behaviour);
    terminal.move_cursor({20, 10});

    expect_sequence(
        std::string("\x1B[12D"),
        terminal.move_cursor({8, 10}));
}

TEST(a_terminal_with_known_location,
     when_moving_right_to_at_least_the_tenth_column_sends_cuf)
{
    terminalpp::terminal terminal;
    terminal.move_cursor({5, 10});

    expect_sequence(
        std::string("\x1B[4C"),
        terminal.move_cursor({9, 10}));
}

TEST(a_terminal_with_known_location,
     when_moving_right_to_less_than_the_tenth_column_sends_cha)
{
    terminalpp::terminal terminal;
    terminal.move_cursor({5, 10});

    expect_sequence(
        std::string("\x1B[9G"),
        terminal.move_cursor({8, 10}));
}

TEST(a_terminal_with_known_location_but_no_cha_support,
     when_moving_right_to_less_than_the_tenth_column_sends_cha)
{
    terminalpp::behaviour behaviour;
    behaviour.supports_cha = false;

    terminalpp::terminal terminal(behaviour);
    terminal.move_cursor({5, 10});

    expect_sequence(
        std::string("\x1B[3C"),
        terminal.move_cursor({8, 10}));
}

TEST(a_terminal_with_a_known_location,
    when_moving_to_the_home_row_sends_cuu)
{
    // When moving to the origin column, we should use CUU in all
    // circumstances
    //  Note: it is possible that CUP is shorter if:
    //    o the terminal supports sending CUP with a default column argument
    //    o the step from cursor to origin is > 100.
    //    o the current row is < 10.
    //  E.g. ^[;9H is shorter than
    //       ^[100A
    //  However, with most terminals being 24<=>80 rows, this is highly
    //  unlikely, and not worth checking.
    terminalpp::terminal terminal;
    terminal.move_cursor({9, 9});

    expect_sequence(
        std::string("\x1B[8A"),
        terminal.move_cursor({9, 1}));
}

TEST(a_terminal_with_a_known_location,
    when_moving_up_one_row_sends_cuu_with_no_argument)
{
    terminalpp::terminal terminal;
    terminal.move_cursor({9, 9});

    expect_sequence(
        std::string("\x1B[A"),
        terminal.move_cursor({9, 8}));
}

TEST(a_terminal_with_a_known_location,
    when_moving_up_many_rows_sends_cuu)
{
    terminalpp::terminal terminal;
    terminal.move_cursor({9, 9});

    expect_sequence(
        std::string("\x1B[2A"),
        terminal.move_cursor({9, 7}));
}

TEST(a_terminal_with_a_known_location,
    when_moving_down_one_row_sends_cud_with_no_argument)
{
    terminalpp::terminal terminal;
    terminal.move_cursor({6, 6});

    expect_sequence(
        std::string("\x1B[B"),
        terminal.move_cursor({6, 7}));
}

TEST(a_terminal_with_a_known_location,
    when_moving_down_one_many_rows_sends_cud)
{
    terminalpp::terminal terminal;
    terminal.move_cursor({6, 6});

    expect_sequence(
        std::string("\x1B[2B"),
        terminal.move_cursor({6, 8}));
}

TEST(a_terminal_with_a_known_location,
    when_moving_in_both_axes_sends_cup)
{
    terminalpp::terminal terminal;
    terminal.move_cursor({5, 5});

    expect_sequence(
        std::string("\x1B[4;11H"),
        terminal.move_cursor({10, 3}));
}

TEST(a_terminal_with_an_unkown_cursor_state,
    when_showing_cursor_sends_show_cursor)
{
    terminalpp::terminal terminal;

    expect_sequence(
        std::string("\x1B[?25h"),
        terminal.show_cursor());
}

TEST(a_terminal_with_an_unkown_cursor_state,
    when_hiding_cursor_sends_hide_cursor)
{
    terminalpp::terminal terminal;

    expect_sequence(
        std::string("\x1B[?25l"),
        terminal.hide_cursor());
}

TEST(a_terminal_with_a_shown_cursor,
    when_showing_cursor_sends_nothing)
{
    terminalpp::terminal terminal;
    terminal.show_cursor();

    expect_sequence(
        std::string(""),
        terminal.show_cursor());
}

TEST(a_terminal_with_a_hidden_cursor,
    when_showing_cursor_sends_show_cursor)
{
    terminalpp::terminal terminal;
    terminal.show_cursor();

    expect_sequence(
        std::string("\x1B[?25l"),
        terminal.hide_cursor());
}

TEST(a_terminal_with_a_shown_cursor,
    when_hiding_cursor_sends_hide_cursor)
{
    terminalpp::terminal terminal;
    terminal.hide_cursor();

    expect_sequence(
        std::string("\x1B[?25h"),
        terminal.show_cursor());
}

TEST(a_terminal_with_a_hidden_cursor,
    when_hiding_cursor_sends_nothing)
{
    terminalpp::terminal terminal;
    terminal.hide_cursor();

    expect_sequence(
        std::string(""),
        terminal.hide_cursor());
}

TEST(saving_the_cursor_position_in_a_terminal, sends_save_position)
{
    terminalpp::terminal terminal;

    expect_sequence(
        std::string("\x1B[s"),
        terminal.save_cursor());
}

TEST(restoring_the_cursor_position_in_a_terminal, sends_restore_position)
{
    terminalpp::terminal terminal;
    terminal.move_cursor({5, 5});
    terminal.save_cursor();
    terminal.move_cursor({10, 10});

    expect_sequence(
        std::string("\x1B[u"),
        terminal.restore_cursor());

    expect_sequence(
        std::string(""),
        terminal.move_cursor({5, 5}));
}

TEST(moving_the_cursor_horizontally_to_the_home_column, sends_cha)
{
    terminalpp::terminal terminal;

    expect_sequence(
        std::string("\x1B[G"),
        terminal.move_cursor_horizontally(0));
}

TEST(moving_the_cursor_horizontally_to_a_non_home_column, sends_cha_and_coordinate)
{
    terminalpp::terminal terminal;

    expect_sequence(
        std::string("\x1B[8G"),
        terminal.move_cursor_horizontally(7));
}

TEST(moving_the_cursor_horizontally_with_an_unknown_coordinate, leaves_the_coordinate_unknown)
{
    terminalpp::terminal terminal;
    terminal.move_cursor_horizontally(6);

    // We expect that we receive a full move-to because the coordinate
    // is unknown.
    expect_sequence(
        std::string("\x1B[4;7H"),
        terminal.move_cursor({6, 3}));
}

TEST(moving_the_cursor_horizontally_from_a_known_coordinate, allows_shortcuts)
{
    terminalpp::terminal terminal;
    terminal.move_cursor({12, 2});
    terminal.move_cursor_horizontally(6);

    // Because the cursor position was known, we know that this just
    // moves the cursor down one space.
    expect_sequence(
        std::string("\x1B[B"),
        terminal.move_cursor({6, 3}));
}

TEST(moving_the_cursor_vertically_to_the_home_column, sends_vpa)
{
    terminalpp::terminal terminal;

    expect_sequence(
        std::string("\x1B[d"),
        terminal.move_cursor_vertically(0));
}

TEST(moving_the_cursor_vertically_to_a_non_home_column, sends_cha_and_coordinate)
{
    terminalpp::terminal terminal;

    expect_sequence(
        std::string("\x1B[8d"),
        terminal.move_cursor_vertically(7));
}

TEST(moving_the_cursor_vertically_with_an_unknown_coordinate, leaves_the_coordinate_unknown)
{
    terminalpp::terminal terminal;
    terminal.move_cursor_vertically(6);

    // We expect that we receive a full move-to because the coordinate
    // is unknown.
    expect_sequence(
        std::string("\x1B[4;7H"),
        terminal.move_cursor({6, 3}));
}

TEST(moving_the_cursor_vertically_from_a_known_coordinate, allows_shortcuts)
{
    terminalpp::terminal terminal;
    terminal.move_cursor({12, 2});
    terminal.move_cursor_vertically(6);

    // Because the cursor position was known, we know that this just
    // moves the cursor left one space.
    expect_sequence(
        std::string("\x1B[D"),
        terminal.move_cursor({11, 6}));
}
