
#include "stdafx.h"
#include "collisionManagement.h"

const double radius = 0.5;//���ӵİ뾶
const double safetyMargin = 0.15; //��ȫ��Χ�����Կ��Ƶ�ЧĦ�����Ĵ�С��
const int number_node = 150;
int knotNum = 0;

void collisionManagement(vector<threeVector>& rope){

    extern int collisionResult[];//����������ȫ�ֱ�����BVH���������ԵĽ�����顣

    //���������е�����һ��һ�Եؽ�����ײ����
    int i = 0;
	int count = 1;//���ڼ�����һ��ѭ������ײ�Ե�����
	int countCluster = knotNum ;//�������ֲ�ͬ����ײ���õġ���1��ʼ�������⵽�Ѿ��н��ˣ��������
	                               //���еĵ����ײ�ر�ſ϶�����1�ˣ��Ǿʹ�2��ʼ��

    while (collisionResult[i]!=0){



        int a1 = collisionResult[i]-1;
        int a2 = collisionResult[i];      //��ȡ������ײ��a�߶ε�����������ı�š�
        int b1 = collisionResult[i+1]-1;
        int b2 = collisionResult[i+1];    //��ȡb�߶�����������ı�š�


		//����һ����⣬����ڵ㱾������ͬһ���ᣬ������ص���ײ����
		//������θ������ֱ����ڲ�ͬ����ײ�أ��Ѿ���֤���������ǽᣩ����ô�����������ײ��������FTL�����ɳ����ڵ㣩
		//Ŀǰ�Ĵ����ǣ�����������������Ǳ������߶Σ���ôֱ���������ѭ��
		//��취���¸Ľ�����һ�ж�����أ�




		//12/4�� ��ʱ����������Ϊԭ���������������ͬ
		if (rope[a1].isKnot() || rope[b1].isKnot() || rope[a2].isKnot() || rope[b2].isKnot()) {
			//cout << "�����и��ᣬ��ǰ�����Ϊ" << knotNum << endl;
			i += 2;
			continue;
		}

/*
		if (rope[a1].isKnot() && rope[b1].isKnot() && rope[a2].isKnot() && rope[b2].isKnot()) {
			//cout << "�����и��ᣬ��ǰ�����Ϊ" << knotNum << endl;
			i += 2;
			continue;//�����䣬����ֻ�ǵ����������������ˡ���Ӧ�ü��룬����ab�ǲ��Ƿ�����ͬ�Ľᡣ
			         //��һ�����ǲ��ǿ��Է��ڹ�����Χ�������һֱ���룬�����ڵ���ʲô���Ĵ��ڣ�����Ǻ������м�Ľᣬ��ô�㣿
		}
*/


        threeVector delta;
        delta = distanceOFSegment (rope[a1],rope[a2],rope[b1],rope[b2]);

        //cout<<delta.norm()<<endl;
        double d = delta.norm();//����Ŀǰ�ľ�����Ϣ����Ϊ����delta����Ҫ��λ����

        if (d < 2 * radius){


			//�������ײ���㷨
			//����ĸ������ǽᣬ�ǲ����������Ȼ�������ⲽ
			if (!rope[a1].isKnot() && !rope[b1].isKnot() && !rope[a2].isKnot() && !rope[b2].isKnot()) {
				int finalCluster;//����������ڼ�¼���ĸ��ڵ����ռ���õ�����ײ�ر�š�
				finalCluster = checkCluster(rope[a1], rope[a2], rope[b1], rope[b2]);
				if (finalCluster == 0) {
					//˵�����е��ĸ��ڵ㲻�����κ�һ���飬�Ǿ͸��������µ���
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


//�ƿ�
            double D = (2 * radius - d + safetyMargin) / 2; //�ټ��һ�飬��һ���ȽϹؼ����������ĺ�Ӣ�����ĵ��г��룬����Ӣ�ĵ��ǶԵ�

            delta.unitlize();
            delta = delta * D;

			//�����ÿ�������Ļ������洢��ʱ��Ҫע�⿴�����ǲ���ĳ���߶������ڽ��
			//�������߶����ڽᣬ�ǻ�Ҫ�����������������������������ƿ���delta
  


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

	//���е���ײ�Զ��������ˣ��򽫸ո�û�д����deltaʵ��
	for (int i = 0; i < number_node; i++) {
		rope[i].enableDelta();
	}

	//���е���ײ�Զ��ֺ����ˣ���ʼ���������ǲ��Ǵ�ɽ���
	int cluster[10];//���һ���Ȳ��̣�����ĳ�vector
	//��ʼ��
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
					rope[j].removeConstraint();//�����ĵ��Ժ���ܶ��ˡ�
				} 

			}
		}
	}

	//��β����
	//�Ѿ��ѳ�����ֵ����ײ�����ú��ˣ���ô����������������ײ�ء�
	for (int i = 0; i < number_node; i++) {
		rope[i].clearDelta();//˳�ְ�ÿ���㻺���delta�������
		if (rope[i].isKnot())
			continue;
		else
			rope[i].clearCluster();
	}
}


//�����ţ���֪����������Բ��ԡ��ƺ�������ǰ��򻯵������������֤��һ���ļ���˳�򣬴�ǰ����
//�����ǣ�abcd�ĸ���ߣ�����0���⣬����������ͬ��cluster����
//���ַ�����������

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
