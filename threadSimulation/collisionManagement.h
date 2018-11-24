#pragma once

#include "DistanceOFsegment.h"
#include "Nodes.h"
#include<vector>

using namespace std;

void collisionManagement(vector<threeVector>& rope);
int checkCluster(threeVector a, threeVector b, threeVector c, threeVector d);
bool checkConscious(vector<threeVector>& rope, int goal);
void fulfillCluster(vector<threeVector>& rope, int goal);
void plusAll(vector<threeVector>& rope, int goal, threeVector delta);
void minusAll(vector<threeVector>& rope, int goal, threeVector delta);


