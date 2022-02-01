#include <pixelgridworld/gridworld.h>
#include <functional>
#include <random> // srand, rand,
#include <time.h> // time

// State Machine:
typedef std::function<core::Vector(core::Vector)> ActionFunction;
typedef std::vector<ActionFunction> ActionsVector;
typedef size_t ActionId;

// Makes state Transition to new state from actions vector using action id.
auto transition(const ActionsVector& actions, const core::Vector& state, ActionId actionId) {
    return actions[actionId](state);
}

auto randomAction(const ActionsVector& actions, const core::Vector& state) {
    ActionId actionId = rand() % actions.size(); // 0 - actions.size()-1
    return transition(actions, state, actionId);
}

int main() {
    srand(time(0)); // Init rnd gen
    pixelgridworld::Window window("AI demo", 512);

    // Specify a map and some functions for it:
    core::Matrix map;
    auto clear = [&map]() {
        map = core::matN(8,1);
    };
    auto setPixel = [&map](const core::Vector& pos, int color) {
        auto x = pos[0];
        auto y = pos[1];
        map[y][x] = color;
    };
    auto getPixel = [&map](const core::Vector& pos) -> int {
        auto x = pos[0];
        auto y = pos[1];
        return map[y][x];
    };

    auto isLegalState = [&map](const core::Vector & pos) -> bool {
        auto x = pos[0];
        auto y = pos[1];
        return y >= 0 && y < map.size() && x >= 0 && x < map[y].size(); // Check map limits
    };

    core::Vector state = { 0, 0 };

    ActionsVector actions;
    actions.push_back([](core::Vector state) -> core::Vector {
        state[0] += 1; // Walk right
        return state;
    });
    actions.push_back([](core::Vector state) -> core::Vector {
        state[0] -= 1; // Walk left
        return state;
    });
    actions.push_back([](core::Vector state) -> core::Vector {
        state[1] += 1; // Walk down
        return state;
    });
    actions.push_back([](core::Vector state) -> core::Vector {
        state[1] -= 1; // Walk up
        return state;
    });

    while(window.shouldClose() == false){
        clear();
        //auto newState = transition(actions, state, 0);
        auto newState = randomAction(actions, state);

        if (isLegalState(newState)) {
            state = newState;
        }
        // Set gameobject map
        setPixel(state, 10);
        window.setMap(map);
        //window.screenshot("screenshot.png");
    }

    return 0;
}
