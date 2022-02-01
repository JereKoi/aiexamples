#pragma once
#include <StateMachine.h>
#include <LinearAlgebra.h>

/// <summary>
/// Random action policy function for agent.
/// </summary>
/// <param name="actions">Agent actions</param>
/// <param name="state">Current state</param>
/// <returns>Returns new state, what is get by making a random action.</returns>
auto randomAction(const ActionsVector& actions, const core::Vector& state) {
    ActionId actionId = rand() % actions.size(); // 0 - actions.size()-1
    return transition(actions, state, actionId);
}

/// <summary>
/// Makes greedy action towards goal.
/// </summary>
/// <param name="actions">Agent actions</param>
/// <param name="state">Current state</param>
/// <param name="end">Returns new state, what is get by making a greedy action towards goal.</param>
/// <returns></returns>
auto greedyAction(const ActionsVector& actions, const core::Vector& state, const core::Vector& end) {
    ActionId bestActionId = 0;
    float bestDistance = distance(end, transition(actions, state, 0));
    for (ActionId actionId = 1; actionId < actions.size(); ++actionId) {
        auto newState = transition(actions, state, actionId);
        auto curDistance = distance(end, newState);
        if (curDistance < bestDistance) {
            bestActionId = actionId;
            bestDistance = curDistance;
        }
    }
    return transition(actions, state, bestActionId);
}




// Search node
struct SearchNode {
    SearchNode(StateAction sa, SearchNode* parentNode = 0)
        : prevNode(parentNode)
        , stateAction(sa)
        , cost(0) {
        if (parentNode) {
            cost = distance(stateAction.state, parentNode->stateAction.state);
        }
    }

    SearchNode*     prevNode;
    float           cost;
    StateAction     stateAction;
};

ActionId getFirstAction(SearchNode* node) {
    ActionId actionId = 0;
    while (node != 0 && node->prevNode != 0) {
        actionId = node->stateAction.action;
        node = node->prevNode;
    }
    return actionId;
}

template<typename GetNextStatesFunc>
auto breadthFirstSearch(GetNextStatesFunc makeAllActions, const core::Vector& initialState, const core::Vector& endState) {
    std::vector<SearchNode*> openList;
    std::map<core::Vector, bool> closedList;

    // Put first node to open list.
    openList.push_back(new SearchNode(StateAction{ initialState, 0 }));

    while (openList.size() > 0) {
        SearchNode* currentNode = openList[0];
        openList.erase(openList.begin(), openList.begin() + 1);
        closedList[currentNode->stateAction.state] = true;

        if (currentNode->stateAction.state == endState) {
            // Goal found, return current node.
            return std::vector<SearchNode*>({ currentNode });
        }

        core::Matrix neighbousStates = makeAllActions(currentNode->stateAction.state);
        for (ActionId actionId = 0; actionId < neighbousStates.size(); ++actionId) {
            auto state = neighbousStates[actionId];
            if (state == currentNode->stateAction.state) {
                continue; // Action did not has any effect to state, so we can ignroe it and move to next neighbourState.
            }
            if (closedList.find(state) != closedList.end()) {
                continue; // We have alredy visited this state, so we can ignore this state.
            }

            assert(actionId < 4);
            SearchNode* newNode = new SearchNode(StateAction{ state,actionId }, currentNode);
            openList.push_back(newNode);
        }
    }
    return openList;
}


template<typename GetNextStatesFunc, typename FCostFunc, typename GCostFunc>
auto astar(
    GetNextStatesFunc makeAllActions, FCostFunc getFCost, GCostFunc getGCost, 
    const core::Vector& initialState, const core::Vector& endState)
{
    std::vector<SearchNode*> openList;
    std::map<core::Vector, bool> closedList;

    // Put first node to open list.
    openList.push_back(new SearchNode(StateAction{ initialState, 0 }));

    while (openList.size() > 0) {
        // Find index of min f from open list an use it to advance search.
        size_t minIndex = 0;
        float minFCost = getFCost(openList[0], endState);
        for (size_t i = 0; i < openList.size(); ++i) {
            float newCost = getFCost(openList[i], endState);
            if (newCost < minFCost) {
                minIndex = i;
                minFCost = newCost;
            }
        }

        SearchNode* currentNode = openList[minIndex];
        openList.erase(openList.begin() + minIndex, openList.begin() + minIndex + 1);
        closedList[currentNode->stateAction.state] = true;

        if (currentNode->stateAction.state == endState) {
            // Goal found, return current node.
            return std::vector<SearchNode*>({ currentNode });
        }

        core::Matrix neighbousStates = makeAllActions(currentNode->stateAction.state);
        for (ActionId actionId = 0; actionId < neighbousStates.size(); ++actionId) {
            auto state = neighbousStates[actionId];
            if (state == currentNode->stateAction.state) {
                continue; // Action did not has any effect to state, so we can ignroe it and move to next neighbourState.
            }
            if (closedList.find(state) != closedList.end()) {
                continue; // We have alredy visited this state, so we can ignore this state.
            }

            assert(actionId < 4);
            SearchNode* newNode = new SearchNode(StateAction{ state,actionId }, currentNode);
            // Find state from open list.
            auto it = std::find_if(openList.begin(), openList.end(), [newNode](const SearchNode* n) -> bool {
                return n->stateAction.state == newNode->stateAction.state;
            });
            // If already at open list and if has smaller g-cost, remplace node from open list.
            if (it != openList.end()) {
                if (getGCost(*it) > getGCost(newNode)) {
                    (*it) = newNode;
                }
                else {
                    delete newNode;
                }
            }
            else {
                // Otherwise, not found from open list, so add it. 
                openList.push_back(newNode);
            }
        }
    }
    return openList;
}