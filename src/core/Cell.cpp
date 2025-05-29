#include <core/Cell.hpp>
#include <core/graphics_types.hpp>

Cell CellFactory::create(const Position& pos, CellStatus type, Color color) const {
    if (type == CellStatus::EMPTY) color = Color::from_string("white");
    return Cell{type, pos, size, std::move(color)};
}

tl::expected<Cell, std::string> CellFactory::update_cell_state(const Cell& cell,
                                                               CellStatus new_state,
                                                               Color new_color) const {
    if (!is_legal_transition(cell.type, new_state)) {
        return tl::unexpected{"illegal state transition"};
    }

    // Empty → 常に白
    if (new_state == CellStatus::EMPTY) {
        new_color = Color::from_string("white");
    }

    return Cell{new_state, cell.position, cell.size, std::move(new_color)};
}
