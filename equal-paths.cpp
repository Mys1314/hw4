#ifndef RECCHECK
//if you want to add any #includes like <iostream> you must do them here (before the next endif)
#include <iostream>
#endif

#include "equal-paths.h"
using namespace std;


// You may add any prototypes of helper functions here
// a function I coded in labs
int heights(Node *root){
	if(root == NULL){
		return 0;
	}
	int lefth = heights(root -> left);
	int righth = heights(root -> right);
	if(lefth > righth){
		return lefth + 1;
	}else{
		return righth + 1;
	}
}

bool equalPaths(Node * root)
{
    if (root == NULL){
        return true;
    }
    int hr = heights(root -> right);
    int hl = heights(root -> left);
    if(hr == 0||hl == 0||hr == hl){
        return equalPaths(root -> left) && equalPaths(root -> right);
    }
    return false;

}

