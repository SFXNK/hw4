#ifndef RECCHECK
//if you want to add any #includes like <iostream> you must do them here (before the next endif)

#endif

#include "equal-paths.h"
using namespace std;


// You may add any prototypes of helper functions here
int dfs(Node * root, int depth){
  if(root->left==nullptr && root->right==nullptr){
    return depth;
  }
  int dl=0, dr=0;
  if(root->left!=nullptr)
    dl=dfs(root->left, depth+1);
  if(root->right!=nullptr)
    dr=dfs(root->right, depth+1);
  if(dl==-1 || dr==-1)
    return -1;
  if(dl!=0 && dr!=0){
    if(dl==dr)
      return dl;
    else
      return -1;
  }
  if(dl!=0){
    return dl;
  }
  return dr;
}

bool equalPaths(Node * root)
{
    // Add your code below
  if(root==nullptr)
    return true;
  return dfs(root,1)!=-1;
}

