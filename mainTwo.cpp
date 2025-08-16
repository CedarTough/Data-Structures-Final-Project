#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>
#include <limits>

struct Unit {
    std::string name;
    int cost;
    int baseValue; // base strategic value
    std::string rangeType; // Melee or Ranged
};

struct GameState {
    int gold;
    std::vector<Unit> owned_units;
};

std::vector<Unit> allUnits = {
    {"Warrior",   3, 5, "Melee"},
    {"Mage",      3, 4, "Ranged"},
    {"Archer",    2, 3, "Ranged"},
    {"Knight",    4, 6, "Melee"},
    {"Assassin",  3, 5, "Melee"},
    {"Paladin",   5, 7, "Melee"},
    {"Sorcerer",  4, 6, "Ranged"},
    {"Ranger",    3, 4, "Ranged"},
    {"Berserker", 6, 8, "Melee"},
    {"Necromancer", 4, 5, "Ranged"}
};

// Generate random shop of 3 units
std::vector<Unit> generateShop() {
    std::vector<Unit> shop;
    for (int i = 0; i < 3; i++) {
        shop.push_back(allUnits[rand() % allUnits.size()]);
    }
    return shop;
}

// Evaluation function with synergy
int evaluateState(const GameState& state) {
    int score = state.gold;

    int meleeCount = 0, rangedCount = 0;

    for (auto& u : state.owned_units) {
        score += u.baseValue;
        if (u.rangeType == "Melee") meleeCount++;
        else rangedCount++;
    }

    // Synergy bonus: prefer balance
    int difference = abs(meleeCount - rangedCount);
    int synergy = -difference * 2;
    score += synergy;

    return score;
}

// Recursive minimax with shop printing
int minimax(const GameState& state, int depth, int maxDepth, bool maximizingPlayer) {
    if (depth == maxDepth) {
        return evaluateState(state);
    }

    // NEW shop each ply
    std::vector<Unit> shop = generateShop();

    // Indentation for clarity
    std::string indent(depth * 2, ' ');
    std::cout << indent << "Shop at depth " << depth << ": ";
    for (auto& u : shop) {
        std::cout << "[" << u.name << " $" << u.cost << " V" << u.baseValue
                  << " " << u.rangeType << "] ";
    }
    std::cout << "\n";

    int bestScore = maximizingPlayer ? std::numeric_limits<int>::min()
                                     : std::numeric_limits<int>::max();

    for (size_t i = 0; i < shop.size() + 1; i++) {
        GameState nextState = state;

        // Option: buy unit i
        if (i < shop.size() && state.gold >= shop[i].cost) {
            nextState.gold -= shop[i].cost;
            nextState.owned_units.push_back(shop[i]);
        }

        int score = minimax(nextState, depth + 1, maxDepth, !maximizingPlayer);

        if (maximizingPlayer) {
            bestScore = std::max(bestScore, score);
        } else {
            bestScore = std::min(bestScore, score);
        }
    }

    return bestScore;
}

// First move decision
void chooseBestMove(GameState state, int maxDepth) {
    std::vector<Unit> shop = generateShop();
    std::cout << "AI Root Shop: ";
    for (auto& u : shop) {
        std::cout << "[" << u.name << " $" << u.cost << " V" << u.baseValue
                  << " " << u.rangeType << "] ";
    }
    std::cout << "\n";

    int bestScore = std::numeric_limits<int>::min();
    std::string bestMove = "Skip";

    for (size_t i = 0; i < shop.size() + 1; i++) {
        GameState nextState = state;

        std::string move;
        if (i < shop.size() && state.gold >= shop[i].cost) {
            nextState.gold -= shop[i].cost;
            nextState.owned_units.push_back(shop[i]);
            move = "Buy " + shop[i].name;
        } else {
            move = "Skip";
        }

        int score = minimax(nextState, 1, maxDepth, false);

        std::cout << move << " -> score " << score << "\n";

        if (score > bestScore) {
            bestScore = score;
            bestMove = move;
        }
    }

    std::cout << "AI Decision: " << bestMove << " (score " << bestScore << ")\n";
}

int main() {
    srand((unsigned)time(0));

    GameState startState;
    startState.gold = 10;

    int plyDepth;
    std::cout << "Enter desired ply depth: ";
    std::cin >> plyDepth;

    chooseBestMove(startState, plyDepth);

    return 0;
}
