#include <core/Cell.hpp>

Cell CellFactory::create(const Position& pos, CellStatus type, std::string color) const {
    if (type == CellStatus::EMPTY) color = "white";
    return Cell{type, pos, size_, std::move(color)};
}

tl::expected<Cell, std::string> CellFactory::update_cell_state(const Cell& cell,
                                                               CellStatus new_state,
                                                               std::string new_color) {
    if (!is_legal_transition(cell.type, new_state)) {
        return tl::unexpected{"illegal state transition"};
    }

    // Empty → 常に白
    if (new_state == CellStatus::EMPTY) new_color = "white";

    return Cell{new_state, cell.position, cell.size, std::move(new_color)};
}
