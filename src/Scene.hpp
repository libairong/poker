#pragma once

#include <map>
#include <vector>
#include "PositionToCards.hpp"

/**
 * 当前场景类
 */
class Scene {
public:
    Scene() = default;
    void resetIsPlayerPass() {
        map<int, bool>::iterator it;
        for (it = isPlayerPass.begin(); it != isPlayerPass.end(); it++) {
            isPlayerPass[it->first] = false;
        }
    };
    int getTheFalseNumOfIsPlayerPass() {
        int num = 0;
        map<int, bool>::iterator it;
        for (it = isPlayerPass.begin(); it != isPlayerPass.end(); it++) {
            if (isPlayerPass[it->first] == false) {
                num++;
            }
        }
        return num;
    }
    map<int, bool> isPlayerActive;
    map<int, bool> isPlayerPass;
    map<int, int> numOfTheCardInPlayers;
    vector<PositionToCards>* _disposed_cards;
    int theNumberOfRemainingCards;
};

