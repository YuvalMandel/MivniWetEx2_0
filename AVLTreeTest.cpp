/*
 * testAVLTree.cpp
 *
 *  Created on: 19 ���� 2013
 *      Author: shirel
 */

#include "AVL.h"
#include <vector>
#include <ostream>
#include <iostream>
#include <random>
#include <chrono>
#include <algorithm>
#include <string>

template<class Value>
std::string printAVLNode(AVLNode<Value>* avl_node,
        int layer){

    if (avl_node != nullptr){
        std::string right_print = printAVLNode(avl_node -> right_son, layer +
        1);
        std::string left_print = printAVLNode(avl_node -> left_son, layer + 1);
        return "layer: " + std::to_string(layer) +
        " value: " + std::to_string (*(avl_node -> val_ptr)) + " rank: " +
        std::to_string (avl_node -> rank) +"\n" + "left " +
        left_print + "right " + right_print;
    }

    return "NULL\n";
}

int main(){

	//Specific Cases

	/*correct output for the four next cases
	1 BF: 0 Height: 0
	2 BF: 0 Height: 1
	3 BF: 0 Height: 0 */
	
	int* vals = new int[30];
    for (int i = 0; i < 30; ++i) {
        vals[i] = i;
    }

	//basic LL root change
    AVLTree<int> tree1;
	tree1.Insert(vals + 3);
	tree1.Insert(vals + 2);
	tree1.Insert(vals + 1);
    std::cout << "tree1\n" << printAVLNode(tree1.root, 0);

    AVLTree<int> tree2;
	//basic LR root change
    tree2.Insert(vals + 3);
    tree2.Insert(vals + 1);
    tree2.Insert(vals + 2);
    std::cout << "tree2\n" << printAVLNode(tree2.root, 0);

	//basic RR root change
    AVLTree<int> tree3;
    tree3.Insert(vals + 1);
    tree3.Insert(vals + 2);
    tree3.Insert(vals + 3);
    std::cout << "tree3\n" << printAVLNode(tree3.root, 0);

	//basic RL root change
    AVLTree<int> tree4;
    tree4.Insert(vals + 1);
    tree4.Insert(vals + 3);
    tree4.Insert(vals + 2);
    std::cout << "tree4\n" << printAVLNode(tree4.root, 0);


	//basic LR not root change
    AVLTree<int> tree5;
	/*correct output
	1 BF: 0 Height: 0
	2 BF: 0 Height: 1
	3 BF: 0 Height: 0
	4 BF: 1 Height: 2
	5 BF: 0 Height: 0
	6 BF: 1 Height: 3
	7 BF: 0 Height: 0
	8 BF: 1 Height: 1 */
    tree5.Insert(vals + 6);
    tree5.Insert(vals + 4);
    tree5.Insert(vals + 8);
    tree5.Insert(vals + 3);
    tree5.Insert(vals + 5);
    tree5.Insert(vals + 7);
    tree5.Insert(vals + 1);
    tree5.Insert(vals + 2);
    std::cout << "tree5\n" << printAVLNode(tree5.root, 0);

	//basic LL not root change
    AVLTree<int> tree6;
	/*correct output
	1 BF: 0 Height: 0
	2 BF: 0 Height: 1
	3 BF: 0 Height: 0
	4 BF: 1 Height: 2
	5 BF: 0 Height: 0
	6 BF: 1 Height: 3
	7 BF: 0 Height: 0
	8 BF: 1 Height: 1 */
    tree6.Insert(vals + 6);
    tree6.Insert(vals + 4);
    tree6.Insert(vals + 8);
    tree6.Insert(vals + 3);
    tree6.Insert(vals + 5);
    tree6.Insert(vals + 7);
    tree6.Insert(vals + 2);
    tree6.Insert(vals + 1);
    std::cout << "tree6\n" << printAVLNode(tree6.root, 0);

	//basic RR not root change
    AVLTree<int> tree7;
	/*correct output
	 * 1 BF: 0 Height: 0
	2 BF: 0 Height: 1
	3 BF: 0 Height: 0
	5 BF: 1 Height: 2
	7 BF: 0 Height: 0*/
    tree7.Insert(vals + 5);
    tree7.Insert(vals + 7);
    tree7.Insert(vals + 1);
    tree7.Insert(vals + 2);
    tree7.Insert(vals + 3);
    std::cout << "tree7\n" << printAVLNode(tree7.root, 0);

	//basic RL not root change
    AVLTree<int> tree8;
	/*correct output
	 * 1 BF: 0 Height: 0
	2 BF: 0 Height: 1
	3 BF: 0 Height: 0
	5 BF: 1 Height: 2
	7 BF: 0 Height: 0*/
    tree8.Insert(vals + 5);
    tree8.Insert(vals + 7);
    tree8.Insert(vals + 1);
    tree8.Insert(vals + 3);
    tree8.Insert(vals + 2);
    std::cout << "tree8\n" << printAVLNode(tree8.root, 0);

	//root deletion no roll successor is a neighbour
    AVLTree<int> tree9;
	/*correct output
	2 BF: 0 Height: 0
	3 BF: 1 Height: 1
	5 BF: 1 Height: 2
	6 BF: 0 Height: 0*/
    tree9.Insert(vals + 4);
    tree9.Insert(vals + 5);
    tree9.Insert(vals + 3);
    tree9.Insert(vals + 2);
    tree9.Insert(vals + 6);
    std::cout << "tree9 before remove\n" << printAVLNode(tree9.root, 0);
    tree9.Remove(4);
    std::cout << "tree9\n" << printAVLNode(tree9.root, 0);

	//root deletion no roll successor is not a neighbour
    AVLTree<int> tree10;
	/*correct output
	2 BF: 0 Height: 0
	3 BF: 1 Height: 1
	5 BF: 0 Height: 2
	6 BF: 0 Height: 0
	7 BF: 0 Height: 1
	8 BF: 0 Height: 0 */
    tree10.Insert(vals + 4);
    tree10.Insert(vals + 3);
    tree10.Insert(vals + 7);
    tree10.Insert(vals + 2);
    tree10.Insert(vals + 5);
    tree10.Insert(vals + 8);
    tree10.Insert(vals + 6);
    tree10.Remove(4);
    std::cout << "tree10\n" << printAVLNode(tree10.root, 0);

	//node deletion no roll successor is a neighbour case7
    AVLTree<int> tree11;
	/*correct output
	1 BF: 0 Height: 0
	2 BF: 1 Height: 1
	4 BF: 1 Height: 2
	5 BF: 0 Height: 0
	7 BF: 1 Height: 3
	8 BF: -1 Height: 1
	9 BF: 0 Height: 0*/
    tree11.Insert(vals + 7);
    tree11.Insert(vals + 3);
    tree11.Insert(vals + 8);
    tree11.Insert(vals + 2);
    tree11.Insert(vals + 4);
    tree11.Insert(vals + 9);
    tree11.Insert(vals + 5);
    tree11.Insert(vals + 1);
    tree11.Remove(3);
    std::cout << "tree11\n" << printAVLNode(tree11.root, 0);


	//node deletion no roll successor is not a neighbour case8
    AVLTree<int> tree12;
	/*correct output
	1 BF: 0 Height: 0
	2 BF: 1 Height: 1
	5 BF: 0 Height: 2
	6 BF: 0 Height: 0
	7 BF: 0 Height: 1
	8 BF: 0 Height: 0
	9 BF: 0 Height: 3
	10 BF: 0 Height: 0
	12 BF: -1 Height: 2
	13 BF: -1 Height: 1
	14 BF: 0 Height: 0*/
    tree12.Insert(vals + 9);
    tree12.Insert(vals + 3);
    tree12.Insert(vals + 12);
    tree12.Insert(vals + 2);
    tree12.Insert(vals + 7);
    tree12.Insert(vals + 10);
    tree12.Insert(vals + 13);
    tree12.Insert(vals + 1);
    tree12.Insert(vals + 5);
    tree12.Insert(vals + 8);
    tree12.Insert(vals + 14);
    tree12.Insert(vals + 6);
    tree12.Remove(3);
    std::cout << "tree12\n" << printAVLNode(tree12.root, 0);

	//node deletion causing LR case9
    AVLTree<int> tree13;
	/*correct output
	1 BF: 0 Height: 0
	2 BF: 0 Height: 1
	3 BF: 0 Height: 0
	4 BF: 0 Height: 2
	7 BF: -1 Height: 1
	8 BF: 0 Height: 0*/
    tree13.Insert(vals + 7);
    tree13.Insert(vals + 2);
    tree13.Insert(vals + 8);
    tree13.Insert(vals + 1);
    tree13.Insert(vals + 4);
    tree13.Insert(vals + 9);
    tree13.Insert(vals + 3);
    tree13.Remove(9);
    std::cout << "tree13\n" << printAVLNode(tree13.root, 0);

	//node deletion causing LL case10
    AVLTree<int> tree14;
	/*correct output
	1 BF: 0 Height: 0
	2 BF: 1 Height: 1
	3 BF: 0 Height: 2
	4 BF: 0 Height: 0
	7 BF: 0 Height: 1
	8 BF: 0 Height: 0*/
    tree14.Insert(vals + 7);
    tree14.Insert(vals + 3);
    tree14.Insert(vals + 8);
    tree14.Insert(vals + 2);
    tree14.Insert(vals + 4);
    tree14.Insert(vals + 9);
    tree14.Insert(vals + 1);
    tree14.Remove(9);
    std::cout << "tree14\n" << printAVLNode(tree14.root, 0);

	//node deletion causing RR case11
    AVLTree<int> tree15;
	/*correct output
	2 BF: 0 Height: 0
	3 BF: 0 Height: 1
	7 BF: 0 Height: 0
	8 BF: 0 Height: 2
	9 BF: -1 Height: 1
	10 BF: 0 Height: 0*/
    tree15.Insert(vals + 3);
    tree15.Insert(vals + 2);
    tree15.Insert(vals + 8);
    tree15.Insert(vals + 7);
    tree15.Insert(vals + 1);
    tree15.Insert(vals + 9);
    tree15.Insert(vals + 10);
    tree15.Remove(1);
    std::cout << "tree15\n" << printAVLNode(tree15.root, 0);

	//node deletion causing RL case12
    AVLTree<int> tree16;
	/*correct output
	* 2 BF: 0 Height: 0
	3 BF: 1 Height: 1
	7 BF: 0 Height: 2
	10 BF: 0 Height: 0
	13 BF: 0 Height: 1
	14 BF: 0 Height: 0*/
    tree16.Insert(vals + 3);
    tree16.Insert(vals + 2);
    tree16.Insert(vals + 13);
    tree16.Insert(vals + 7);
    tree16.Insert(vals + 1);
    tree16.Insert(vals + 14);
    tree16.Insert(vals + 10);
    tree16.Remove(1);
    std::cout << "tree16\n" << printAVLNode(tree16.root, 0);

	//double rotations RL and RR case 13
    AVLTree<int> tree17;
	/*correct output
	3 BF: 0 Height: 0
	5 BF: 0 Height: 1
	7 BF: 0 Height: 0
	9 BF: 0 Height: 2
	11 BF: 0 Height: 0
	13 BF: 1 Height: 1
	15 BF: 0 Height: 3
	17 BF: 0 Height: 0
	19 BF: -1 Height: 2
	21 BF: -1 Height: 1
	23 BF: 0 Height: 0*/
    tree17.Insert(vals + 9);
    tree17.Insert(vals + 3);
    tree17.Insert(vals + 15);
    tree17.Insert(vals + 1);
    tree17.Insert(vals + 7);
    tree17.Insert(vals + 13);
    tree17.Insert(vals + 19);
    tree17.Insert(vals + 5);
    tree17.Insert(vals + 11);
    tree17.Insert(vals + 17);
    tree17.Insert(vals + 21);
    tree17.Insert(vals + 23);
    tree17.Remove(1);
    std::cout << "tree17\n" << printAVLNode(tree17.root, 0);

	//double rotations RR and RR case 14
    AVLTree<int> tree18;
	/*correct output
	3 BF: 0 Height: 0
	7 BF: 0 Height: 1
	8 BF: 0 Height: 0
	9 BF: 0 Height: 2
	11 BF: 0 Height: 0
	13 BF: 1 Height: 1
	15 BF: 0 Height: 3
	17 BF: 0 Height: 0
	19 BF: -1 Height: 2
	21 BF: -1 Height: 1
	23 BF: 0 Height: 0*/
    tree18.Insert(vals + 9);
    tree18.Insert(vals + 3);
    tree18.Insert(vals + 15);
    tree18.Insert(vals + 1);
    tree18.Insert(vals + 7);
    tree18.Insert(vals + 13);
    tree18.Insert(vals + 19);
    tree18.Insert(vals + 8);
    tree18.Insert(vals + 11);
    tree18.Insert(vals + 17);
    tree18.Insert(vals + 21);
    tree18.Insert(vals + 23);
    tree18.Remove(1);
    std::cout << "tree18\n" << printAVLNode(tree18.root, 0);

	//double rotations RL and LL case 15
    AVLTree<int> tree19;
	/*correct output
	6 BF: 0 Height: 0
	7 BF: 1 Height: 1
	8 BF: 1 Height: 2
	9 BF: 0 Height: 0
	10 BF: 0 Height: 3
	11 BF: -1 Height: 1
	12 BF: 0 Height: 0
	15 BF: 0 Height: 2
	17 BF: 0 Height: 0
	18 BF: 0 Height: 1
	20 BF: 0 Height: 0
	 */
    tree19.Insert(vals + 15);
    tree19.Insert(vals + 10);
    tree19.Insert(vals + 20);
    tree19.Insert(vals + 8);
    tree19.Insert(vals + 11);
    tree19.Insert(vals + 17);
    tree19.Insert(vals + 21);
    tree19.Insert(vals + 7);
    tree19.Insert(vals + 9);
    tree19.Insert(vals + 12);
    tree19.Insert(vals + 18);
    tree19.Insert(vals + 6);
    tree19.Remove(21);
    std::cout << "tree19\n" << printAVLNode(tree19.root, 0);
    for (int i = 0; i < 11; ++i) {
        std::cout << *((tree19.FindValueByIndex(1 + i)) -> val_ptr) << " ";
    }
    std::cout << "\n";
	//double rotations LR and LL case 16
    AVLTree<int> tree20;
	/*correct output
	6 BF: 0 Height: 0
	7 BF: 1 Height: 1
	8 BF: 1 Height: 2
	9 BF: 0 Height: 0
	10 BF: 0 Height: 3
	11 BF: -1 Height: 1
	12 BF: 0 Height: 0
	15 BF: 0 Height: 2
	20 BF: 0 Height: 0
	21 BF: 0 Height: 1
	22 BF: 0 Height: 0 */
    tree20.Insert(vals + 15);
    tree20.Insert(vals + 10);
    tree20.Insert(vals + 20);
    tree20.Insert(vals + 8);
    tree20.Insert(vals + 11);
    tree20.Insert(vals + 17);
    tree20.Insert(vals + 22);
    tree20.Insert(vals + 7);
    tree20.Insert(vals + 9);
    tree20.Insert(vals + 12);
    tree20.Insert(vals + 21);
    tree20.Insert(vals + 6);
    tree20.Remove(17);
    std::cout << "tree20\n" << printAVLNode(tree20.root, 0);

	//delete node cause LR
    AVLTree<int> tree21;
	/*correct output
	 * 2 BF: 0 Height: 0
	3 BF: -1 Height: 2
	4 BF: 0 Height: 0
	6 BF: 1 Height: 1*/

    tree21.Insert(vals + 5);
    tree21.Insert(vals + 3);
    tree21.Insert(vals + 6);
    tree21.Insert(vals + 2);
    tree21.Insert(vals + 4);
    std::cout << "tree21 before remove\n" << printAVLNode(tree21.root, 0);
    tree21.Remove(5);
    std::cout << "tree21\n" << printAVLNode(tree21.root, 0);

	//delete node cause LR
    AVLTree<int> tree22;
	/*correct output
	 * 2 BF: 0 Height: 0
	3 BF: 0 Height: 1
	6 BF: 0 Height: 0*/
    tree22.Insert(vals + 5);
    tree22.Insert(vals + 3);
    tree22.Insert(vals + 6);
    tree22.Insert(vals + 2);
    tree22.Remove(5);
    std::cout << "tree22\n" << printAVLNode(tree22.root, 0);

//	std::vector<int> vector;
//	 for (int i=1; i<=100; i++) vector.push_back(i);
//
//	 //Randomly insert and removes nodes
//	 for (int k = 0; k < 20; ++k) {
//		 unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
//		 shuffle (vector.begin(), vector.end(), std::default_random_engine(seed));
//		 for (std::vector<int>::iterator it = vector.begin() ; it != vector.end(); ++it){
//				tree.Insert(vals + *it,*it);
//
//		 }
//		 tree.printTree();
//		 shuffle (vector.begin(), vector.end(), std::default_random_engine(seed));
//		 for (std::vector<int>::iterator it = vector.begin() ; it != vector.end(); ++it){
//				tree.removeNode(*it);
//				tree.printTree();
//
//		 }
//		 tree.treeClear();
//		 tree.printTree();
//		 std::cout << '\n';
//	}

	return 0;
}

