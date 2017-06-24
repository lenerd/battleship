#ifndef USER_INTERFACE_HPP
#define USER_INTERFACE_HPP

#include <memory>
#include <string>
#include "game/board.hpp"
#include "misc/options.hpp"
#include "misc/util.hpp"


/**
 * Interface for a user interface
 */
class UserInterface
{
public:
    /**
     * Constructor
     */
    UserInterface(Board_p board_local, Board_p board_remote);
    /**
     * Destructor
     */
    virtual ~UserInterface();
    /**
     * Start the user interface
     */
    virtual void show() = 0;
    /**
     * Ask the user to place its ships
     */
    virtual void place_ships() = 0;
    /**
     * Ask the user to select a position
     */
    virtual coords_t select_position() = 0;
    /**
     * Display a message
     */
    virtual void post_message(std::string message) = 0;
    /**
     * Wait for user input before terminating program
     */
    virtual void wait_for_quit() = 0;
protected:
    Board_p board_local_;
    Board_p board_remote_;
};

using UI_p = std::unique_ptr<UserInterface>;


class UIFactory
{
public:
    UIFactory(const Options &options);
    ~UIFactory() = default;
    UI_p make(UIType type, Board_p board_local, Board_p board_remote) const;
private:
    const Options& options_;
};

#endif // USER_INTERFACE_HPP
