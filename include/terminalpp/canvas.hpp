#ifndef TERMINALPP_CANVAS_HPP_
#define TERMINALPP_CANVAS_HPP_

#include "terminalpp/element.hpp"
#include "terminalpp/extent.hpp"
#include <vector>

namespace terminalpp {
    
//* =========================================================================
/// \brief A class representing a grid onto which elements can be painted.
///
/// \par Usage
/// The grid is formed from a contiguous block of memory.  Position [0,0]
/// represents the top left of the grid, and it proceeds in a left-to-right
/// manner first, then top-to-bottom.
//* =========================================================================
class canvas 
{
public :
    typedef element *iterator;
    typedef element const *const_iterator;

    //* =====================================================================
    /// \brief A proxy into a row of elements on the canvas
    //* =====================================================================
    class row_proxy
    {
    public :
        // ==================================================================
        // CONSTRUCTOR
        // ==================================================================
        row_proxy(canvas &cvs, s32 column, s32 row);

        // ==================================================================
        // OPERATOR=
        // ==================================================================
        void operator=(element const &value);

        // ==================================================================
        // CONVERSION OPERATOR: ELEMENT
        // ==================================================================
        operator element &();

    private :
        canvas &canvas_;
        s32     column_;
        s32     row_;
    };

    //* =====================================================================
    /// \brief A proxy into a column of elements on the canvas
    //* =====================================================================
    class column_proxy
    {
    public :
        // ==================================================================
        // CONSTRUCTOR
        // ==================================================================
        column_proxy(canvas &cvs, s32 column);

        // ==================================================================
        // OPERATOR[]
        // ==================================================================
        row_proxy operator[](s32 row);

    private :
        canvas &canvas_;
        s32     column_;
    };

    //* =====================================================================
    /// \brief A constant proxy into a column of elements on the canvas
    //* =====================================================================
    class const_column_proxy
    {
    public :
        // ==================================================================
        // CONSTRUCTOR
        // ==================================================================
        const_column_proxy(canvas const &cvs, s32 column);

        // ==================================================================
        // OPERATOR[]
        // ==================================================================
        element const &operator[](s32 row) const;

    private :
        canvas const &canvas_;
        s32           column_;
    };
    
    //* =====================================================================
    /// \brief Constructor
    //* =====================================================================
    canvas(extent size);
    
    //* =====================================================================
    /// \brief Returns the size of the canvas.
    //* =====================================================================
    extent size() const;
    
    //* =====================================================================
    /// \brief Returns pointer to the top-left element.
    //* =====================================================================
    iterator begin();
    
    //* =====================================================================
    /// \brief Returns pointer to the top-left element.
    //* =====================================================================
    const_iterator begin() const;

    //* =====================================================================
    /// \brief Returns one-past-the-end of the canvas.
    //* =====================================================================
    iterator end();

    //* =====================================================================
    /// \brief Returns one-past-the-end of the canvas.
    //* =====================================================================
    const_iterator end() const;
    
    //* =====================================================================
    /// \brief A subscript operator into a column
    //* =====================================================================
    column_proxy operator[](s32 column);
    
    //* =====================================================================
    /// \brief A subscript operator into a column
    //* =====================================================================
    const_column_proxy operator[](s32 column) const;

private :
    //* =====================================================================
    /// \brief Set the value of an element.
    //* =====================================================================
    void set_element(s32 column, s32 row, element const &value);
    
    //* =====================================================================
    /// \brief Get the value of an element.
    //* =====================================================================
    element &get_element(s32 column, s32 row);
    
    //* =====================================================================
    /// \brief Get the value of an element.
    //* =====================================================================
    element const &get_element(s32 column, s32 row) const;
    
    std::vector<element> grid_;
    extent               size_;
};
    
}

#endif
