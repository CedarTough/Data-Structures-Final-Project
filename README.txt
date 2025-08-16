Auto Chess algorithm Decision Tree

What is Auto Chess / TFT?
This project is inspired by the Auto Chess genre and Teamfight Tactics (TFT). In these games:
- Players buy units from a randomized shop each round.  
- Units have different costs, values, and traits (melee or ranged).  
- Gold management is crucial as you can either spend on units immediately or save gold for future rounds, sometimes earning interest for saving.  
- Once units are bought, they automatically fight, and the strategy happens before the battle during the shop/decision phase.  

My algorithm focuses only on the shop decision-making phase, specifically, whether to buy a unit or skip, and what that means across multiple rounds.

----------------------------------------------------------------------------------------------------------------

Goal of the project is to demonstrate: 

    1. How trees can model branching decisions.

    2. How minimax search can simulate forward planning.

    3. How an evaluation function turns raw data (gold + units) into a decision heuristic.

    4. How this applies to real-world strategy games like Auto Chess.

----------------------------------------------------------------------------------------------------------------

Data Structure Implemented: Decision Tree

The algorithm uses a decision tree to simulate future shop states.  

- Nodes: represent game states (current gold, units owned, and shop offers).  
- Edges: represent decisions the algorithm can make:
  - Buy a unit (spend gold, add to roster).  
  - Skip (keep gold and reroll the shop in the next ply).  

Each node expands into children that reflect what the shop could look like in the next round. The depth of the tree (number of plies) controls how far ahead the algorithm “thinks.”

----------------------------------------------------------------------------------------------------------------

Algorithm Used: Minimax Search:
At the core of this project is a minimax-style recursive search.  

- Traverse: Starting at the root (the current shop), the algorithm recursively explores all possible decisions (buying different units or skipping).  
- Evaluate: Each leaf node (end of a simulation path) is scored using a heuristic function.  
- Backpropagate: The scores “bubble up” to the root, where the algorithm chooses the best action at the current shop.  

Pseudocode of the minimax function:

int minimax(Node* state, int depth) {
    if (depth == 0 || state->isTerminal()) {
        return evaluate(state);
    }
    
    int bestScore = -INF;
    for (Node* child : state->children) {
        int score = minimax(child, depth - 1);
        bestScore = max(bestScore, score);
    }
    return bestScore;
}

----------------------------------------------------------------------------------------------------------------


The evaluation function is where the algorithm's intelligence comes in. A state is scored using a weighted sum of:

Unit Strength: sum of unit values (cost as proxy for level/strength).

Gold Remalgorithmning: leftover coins add to the score (for saving strategies).

Synergy Bonus: if the roster has an unequal amount of melee and ranged units, try to even it out. (In the future there will be way more synergies. However, I only chose 
               one synergy to keep the algorithm easier to follow.

----------------------------------------------------------------------------------------------------------------

I took feedback from my proposal into my work because my proposal was poorly received (it almost dropped me a letter grade :/)

One good observation that I liked was that if one strategy buys weaker units + saves more gold and another buys stronger units + saves less, the total score could be the same.

To fix this, I messed with adjusted weights:

Unit strength weighted slightly higher than gold (e.g., 1.5× per unit cost).

Gold weighted slightly lower (e.g., 1.0× per coin).

This ensures the algorithm favors stronger units when values tie, which is kind of how real players think.

----------------------------------------------------------------------------------------------------------------

Implementation Detalgorithmls:

 - Node Payload:

     - Current gold

    - List of owned units

    - Current shop (randomized list of 3 units)

    - Synergy is calculated based on melee and ranged units (I aim for an equal amount of short and long ranged units)


 - Tree Traversal: Depth-first recursive minimax.

 - Shop Randomization: Each ply simulates a new randomized shop.

 ----------------------------------------------------------------------------------------------------------------

How to run:

It runs like a normal file in C++.

For me, I build it: 

'g++ -std=c++17 -o autochess malgorithmnTwo.cpp'

and then run it:

./shop_ai

----------------------------------------------------------------------------------------------------------------

Here's an example output + explanation:

output:

Shop at depth 1: [Mage $3 V4 Ranged] [Mage $3 V4 Ranged] [Knight $4 V6 Melee]
    (after buying Knight) Shop at depth 2: [Warrior $3 V5 Melee] [Archer $2 V3 Ranged] [Archer $2 V3 Ranged]
    (after skipping) Shop at depth 2: [Mage $3 V4 Ranged] [Archer $2 V3 Ranged] [Assassin $3 V5 Melee]

Skip -> score 10
algorithm Decision: Buy Assassin (score 13)

explanation: Here, the algorithm explores both buying and skipping, simulates the next round’s shops, evaluates scores, 
             and decides to buy the Assassin.

----------------------------------------------------------------------------------------------------------------

Future goals: 

I am actually currently working on my own game in the autochess genre. I used this project as an
opportunity to actually create an algorithm for deciding what characters to play so that one might be 
able to play against it. While the current algorithm may be lacking in more niche features, I 
believe I have built a solid groundwork for an algorithm that can play my game. In the future, I hope to
add more synergy types between characters. The algorithm is currently exponential compared to the number of
plys. As such, I want the algorithm to sometimes behave greedily and skip steps to help with runtime. In the long
term, I hope to train an actual AI trained on real players that will be able to make in-game decisions.

