
#include "stdafx.h"
#include "collisionManagement.h"

const double radius = 0.5;//绳子的半径
const double safetyMargin = 0.15; //安全范围，可以控制等效摩擦力的大小。
const int number_node = 150;
int knotNum = 0;

void collisionManagement(vector<threeVector>& rope){

    extern int collisionResult[];//在这里引用全局变量：BVH树遍历测试的结果数组。

    //利用数组中的数据一对一对地进行碰撞处理。
    int i = 0;
	int count = 1;//用于计算这一次循环中碰撞对的数量
	int countCluster = knotNum ;//用于区分不同的碰撞簇用的。从1开始。如果检测到已经有结了，这个结内
	                               //所有的点的碰撞簇编号肯定都是1了，那就从2开始。

    while (collisionResult[i]!=0){



        int a1 = collisionResult[i]-1;
        int a2 = collisionResult[i];      //获取发生碰撞的a线段的左右两个点的编号。
        int b1 = collisionResult[i+1]-1;
        int b2 = collisionResult[i+1];    //获取b线段左右两个点的编号。


		//加入一个检测，如果节点本身属于同一个结，则不做相关的碰撞处理。
		//如过两段刚性绳分别属于不同的碰撞簇（已经保证了两个都是结），那么进行特殊的碰撞处理（仿照FTL，当成超级节点）
		//目前的处理是，如果两个刚性绳都是被打结的线段，那么直接跳过这个循环
		//想办法做下改进，万一有多个结呢？




		//12/4日 暂时将这里设置为原来错误的做法，下同
		if (rope[a1].isKnot() || rope[b1].isKnot() || rope[a2].isKnot() || rope[b2].isKnot()) {
			//cout << "这里有个结，当前结个数为" << knotNum << endl;
			i += 2;
			continue;
		}

/*
		if (rope[a1].isKnot() && rope[b1].isKnot() && rope[a2].isKnot() && rope[b2].isKnot()) {
			//cout << "这里有个结，当前结个数为" << knotNum << endl;
			i += 2;
			continue;//待补充，这里只是单纯的跳过，不管了。还应该加入，看看ab是不是分属不同的结。
			         //这一步，是不是可以放在构建包围盒这里，我一直在想，超级节点是什么样的存在，如果是蝴蝶结中间的结，怎么算？
		}
*/


        threeVector delta;
        delta = distanceOFSegment (rope[a1],rope[a2],rope[b1],rope[b2]);

        //cout<<delta.norm()<<endl;
        double d = delta.norm();//保存目前的距离信息。因为后面delta向量要单位化。

        if (d < 2 * radius){


			//提出到碰撞簇算法
			//如果四个都不是结，那才玩这个，不然就跳过这步
			if (!rope[a1].isKnot() && !rope[b1].isKnot() && !rope[a2].isKnot() && !rope[b2].isKnot()) {
				int finalCluster;//这个变量用于记录这四个节点最终计算得到的碰撞簇编号。
				finalCluster = checkCluster(rope[a1], rope[a2], rope[b1], rope[b2]);
				if (finalCluster == 0) {
					//说明现有的四个节点不属于任何一个组，那就给它分配新的组
					countCluster += 1;
					rope[a1].setCluster(countCluster);
					rope[a2].setCluster(countCluster);
					rope[b1].setCluster(countCluster);
					rope[b2].setCluster(countCluster);
				}
				else {
					rope[a1].setCluster(finalCluster);
					rope[a2].setCluster(finalCluster);
					rope[b1].setCluster(finalCluster);
					rope[b2].setCluster(finalCluster);
				}
			}


//推开
            double D = (2 * radius - d + safetyMargin) / 2; //再检查一遍，这一步比较关键。中文论文和英文论文的有出入，估计英文的是对的

            delta.unitlize();
            delta = delta * D;

			//最后往每个向量的缓存区存储的时候还要注意看看，是不是某个线段是属于结的
			//如果这个线段属于结，那还要把它所属结的所有向量都加入这个推开的delta
  


			rope[a1].addToDelta_Plus(delta);
			rope[a2].addToDelta_Plus(delta);
			rope[b1].addToDelta_Minus(delta);
			rope[b2].addToDelta_Minus(delta);

/*
			if (rope[a1].isKnot() || rope[a2].isKnot()) {
				int key = rope[a1].getCluster();
				plusAll(rope, key, delta);
				rope[b1].addToDelta_Minus(delta);
				rope[b2].addToDelta_Minus(delta);
			}

			if (rope[b1].isKnot() || rope[b2].isKnot()) {
				int key = rope[b1].getCluster();
				minusAll(rope, key, delta);
				rope[a1].addToDelta_Plus(delta);
				rope[a2].addToDelta_Plus(delta);
			}

			if (!rope[a1].isKnot() && !rope[b1].isKnot() && !rope[a2].isKnot() && !rope[b2].isKnot()) {
				rope[a1].addToDelta_Plus(delta);
				rope[a2].addToDelta_Plus(delta);
				rope[b1].addToDelta_Minus(delta);
				rope[b2].addToDelta_Minus(delta);
			}
*/
            /*rope[a1].setConstraint();
            rope[a2].setConstraint();
            rope[b1].setConstraint();
            rope[b2].setConstraint();*/
		}


		i += 2;

	}

	//所有的碰撞对都处理完了，则将刚刚没有处理的delta实现
	for (int i = 0; i < number_node; i++) {
		rope[i].enableDelta();
	}

	//所有的碰撞对都分好组了，开始分析他们是不是打成结了
	int cluster[10];//请教一下热播咸，这里改成vector
	//初始化
	for (int i = 0; i < 10; i++) {
		cluster[i] = 0;
	}

	for (int i = 0; i < number_node; i++) {
		int c = rope[i].getCluster();
		cluster[c] += 1;
	}

	for (int i = (knotNum + 1); i < 10; i++) {
		if (cluster[i] == 0)
			break;

		if (cluster[i] > 14) {
			fulfillCluster(rope, i);
			knotNum += 1;
			for (int j = 0; j < number_node; j++) {
				int c = rope[j].getCluster();
				if (c == i) {
					rope[j].setKnot();
					rope[j].removeConstraint();//被打结的点以后就能动了。
				} 

			}
		}
	}

	//收尾工作
	//已经把超过阈值的碰撞簇设置好了，那么重置所有其他的碰撞簇。
	for (int i = 0; i < number_node; i++) {
		rope[i].clearDelta();//顺手把每个点缓存的delta先清除了
		if (rope[i].isKnot())
			continue;
		else
			rope[i].clearCluster();
	}
}


//先留着，不知道这个方法对不对。似乎，由于前面简化的提出操作，保证了一定的检测的顺序，从前到后。
//问题是：abcd四个里边，除了0以外，会有两个不同的cluster组吗？
//这种方法有特例吗？

int checkCluster(threeVector a, threeVector b, threeVector c, threeVector d)
{
	if (a.getCluster() != 0)
		return a.getCluster();

	if (b.getCluster() != 0)
		return a.getCluster();

	if (c.getCluster() != 0)
		return a.getCluster();

	if (d.getCluster() != 0)
		return a.getCluster();

	return 0;
}


//Usless, just overlook.
bool checkConscious(vector<threeVector>& rope, int goal)
{
	vector<int> goalCluster;
	for (int i = 0; i < number_node; i++) {
		int c = rope[i].getCluster();
		if (c == goal) {
			goalCluster.push_back(i);
		}
	}
	for (int i = 0; i < (goalCluster.size()-1); i++) {
		if ((goalCluster[i] + 1) != goalCluster[i + 1])
			return false;
	}
	return true;
}

void fulfillCluster(vector<threeVector>& rope, int goal)
{
	vector<int> goalCluster;
	for (int i = 0; i < number_node; i++) {
		int c = rope[i].getCluster();
		if (c == goal) {
			goalCluster.push_back(i);
		}
	}
	for (int i = goalCluster[0]; i < goalCluster[goalCluster.size() - 1]; i++) {
		rope[i].setCluster(goal);
	}

}

void plusAll(vector<threeVector>& rope, int goal, threeVector delta){
	vector<int> goalCluster;
	for (int i = 0; i < number_node; i++) {
		int c = rope[i].getCluster();
		if (c == goal) {
			goalCluster.push_back(i);
		}
	}

	for (int i = goalCluster[0]; i <= goalCluster[goalCluster.size() - 1];i++ ) {
		rope[i].addToDelta_Plus(delta);
	}


}

void minusAll(vector<threeVector>& rope, int goal, threeVector delta) {
	vector<int> goalCluster;
	for (int i = 0; i < number_node; i++) {
		int c = rope[i].getCluster();
		if (c == goal) {
			goalCluster.push_back(i);
		}
	}

	for (int i = goalCluster[0]; i <= goalCluster[goalCluster.size() - 1]; i++) {
		rope[i].addToDelta_Minus(delta);
	}

}
