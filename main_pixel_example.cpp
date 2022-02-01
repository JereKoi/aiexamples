#include <pixelgridworld/gridworld.h>
#include <functional>
#include <random> // srand, rand,
#include <time.h> // time
#include <assert.h>
#include <math.h>
#include <LinearAlgebra.h>
#include <StateMachine.h>
#include <Agent.h>
#include <map>

int main() {
    srand(time(0)); // Init rnd gen
    pixelgridworld::Window window("AI demo", 512);

    // Specify a map and some functions for it:
    core::Matrix map;

    auto setPixel = [&map](const core::Vector& pos, int color) {
        auto x = pos[0];
        auto y = pos[1];
        map[y][x] = color;
    };

    auto clear = [&map, setPixel]() {
        map = core::matN(8, 1);

        for (int i = 0; i < 5; ++i) {
            setPixel({4, 2+i}, 6);
        }
        for (int i = 0; i <3; ++i) {
            setPixel({2+i, 2}, 6);
        }
    };
    auto getPixel = [&map](const core::Vector& pos) -> int {
        auto x = pos[0];
        auto y = pos[1];
        return map[y][x];
    };

    auto isLegalState = [&map](const core::Vector & pos) -> bool {
        auto x = pos[0];
        auto y = pos[1];
        if (y >= 0 && y < map.size() && x >= 0 && x < map[y].size()) {
            return map[y][x] != 6;
        }
        return false;
    };

    ActionsVector actions;

    auto getNextLegalStates = [&isLegalState,&map,&actions](const core::Vector& currentState) -> core::Matrix {
        core::Matrix results;
        for (ActionId actionId = 0; actionId < actions.size(); ++actionId) {
            auto newState = transition(actions, currentState, actionId);
            if (isLegalState(newState)) {
                results.push_back(newState);
            }
            else {
                results.push_back(currentState);
            }
        }
        return results;
    };

    core::Vector state = { 0, 0 };
    core::Vector goalState = { 7, 7 };

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
        //auto newState = randomAction(actions, state);
        //auto newState = greedyAction(actions, state, goalState);
        /*
        if (isLegalState(newState)) {
            state = newState;
        }*/

        auto openList = breadthFirstSearch(getNextLegalStates, state, goalState);
        if (openList.size() == 0) {
            printf("Path not found!\n");
        }
        else if (openList.size() == 1) {
            printf("Path found!\n");
            auto node = openList[0];
            // take first action from plan and make a state transition according to that.
            auto actionId = getFirstAction(node);

            while (node != 0 && node->prevNode != 0) {
                setPixel(node->stateAction.state, 20);
                node = node->prevNode;
            }

            auto newState = transition(actions, state, actionId);
            if (isLegalState(newState)) {
                state = newState;
            }
        }
        else {
            for (auto node : openList) {
                setPixel(node->stateAction.state, 11);
            }
        }


        if (state == goalState) {
            printf("Goal reached!\n");
            do {
                goalState[0] = rand() % map.size();
                goalState[1] = rand() % map.size();
            } while (!isLegalState(goalState));
        }

        // Set gameobject map
        setPixel(state, 10);
        // Set goal state to map
        setPixel(goalState, 8);
        window.setMap(map);
        //window.screenshot("screenshot.png");
    }

    return 0;
}
