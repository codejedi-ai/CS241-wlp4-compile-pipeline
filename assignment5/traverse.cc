#include <iostream>
#include <stack>
using namespace std;
struct Node
{
	int num, children;
};
int main()
{
	stack<int> childrenStack; // this contains the amount of children in the node
	stack<Node> treeStack;
	int a, b;
	while ((cin >> a) && (cin >> b))
	{
		childrenStack.push(b);
		treeStack.push({a, b});

		while (!childrenStack.empty() && !childrenStack.top()) 
		// the top is zero in the new assignemnt the zero top needs to be identified immediatelly
		{
			cout << treeStack.top().num << " " << treeStack.top().children << endl;
			childrenStack.pop();
			treeStack.pop();
			if (childrenStack.empty())
				break;
			int buf = childrenStack.top();
			childrenStack.pop();
			childrenStack.push(buf - 1);
		}
	}
}