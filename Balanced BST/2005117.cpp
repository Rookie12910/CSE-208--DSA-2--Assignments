#include <bits/stdc++.h>
using namespace std;
#define den 1000000.0

ifstream ifile;
ofstream ofile;


template <class T>
class Node
{
public:
    T value;
    Node<T> *left;
    Node<T> *right;
};

template <class T>
class AVL_BST
{
public:
    Node<T> *root;
    int height(Node<T> *node);
    int h_diff(Node<T> *node);
    Node<T> *ll_rot(Node<T> *node);
    Node<T> *lr_rot(Node<T> *node);
    Node<T> *rl_rot(Node<T> *node);
    Node<T> *rr_rot(Node<T> *node);
    Node<T> *balance(Node<T> *node);
    Node<T> *Insert(Node<T> *node, T value);
    void traverse(Node<T> *node);
    bool Find(Node<T> *node, T data);
    Node<T> *findMin(Node<T> *node);
    Node<T> *deleteNode(Node<T> *node, T data);
    void Print(Node<T> *node);
    AVL_BST()
    {
        root = NULL;
    }
};

template <class T>
int AVL_BST<T>::height(Node<T> *node)
{
    int ht = 0;
    if (node != NULL)
    {
        ht = max(height(node->left), height(node->right)) + 1;
    }
    return ht;
}

template <class T>
int AVL_BST<T>::h_diff(Node<T> *node)
{
    int bf = height(node->left) - height(node->right);
    return bf;
}

template <class T>
Node<T> *AVL_BST<T>::ll_rot(Node<T> *node)
{
    Node<T> *temp = node->left;;
    node->left = temp->right;
    temp->right = node;
    return temp;
}

template <class T>
Node<T> *AVL_BST<T>::rr_rot(Node<T> *node)
{
    Node<T> *temp = node->right;
    node->right = temp->left;
    temp->left = node;
    return temp;
}

template <class T>
Node<T> *AVL_BST<T>::lr_rot(Node<T> *node)
{
    node->left = rr_rot(node->left);
    return ll_rot(node);
}

template <class T>
Node<T> *AVL_BST<T>::rl_rot(Node<T> *node)
{
    node->right = ll_rot(node->right);
    return rr_rot(node);
}

template <class T>
Node<T> *AVL_BST<T>::balance(Node<T> *node)
{
    int bf = h_diff(node);
    if (bf < -1)
    {
        if (h_diff(node->right) > 0)
            node = rl_rot(node);
        else
            node = rr_rot(node);
    }
    else if (bf > 1)
    {
        if (h_diff(node->left) >= 0)
            node = ll_rot(node);
        else
            node = lr_rot(node);
    }
    return node;
}

template <class T>
Node<T> *AVL_BST<T>::Insert(Node<T> *node, T data)
{
    if (node == NULL)
    {
        node = new Node<T>;
        node->value = data;
        node->left = NULL;
        node->right = NULL;
        return node;
    }
    else if (data <= node->value)
    {
        node->left = Insert(node->left, data);
        return balance(node);
    }
    else
    {
        node->right = Insert(node->right, data);
        return balance(node);
    }
}

template <class T>
void AVL_BST<T>::traverse(Node<T> *node)
{
    if (node == NULL)
        return;
    traverse(node->left);
    ofile << node->value << " ";
    traverse(node->right);
}


template <class T>
bool AVL_BST<T>::Find(Node<T> *node, T value)
{
    if (node == NULL)
    {
        return false;
    }
    else if (node->value == value)
    {
        return true;
    }
    else if (value < node->value)
    {
        return Find(node->left, value);
    }
    else
    {
        return Find(node->right, value);
    }
}


template <class T>
Node<T>* AVL_BST<T>::findMin(Node<T>* node)
{
    while (node->left != NULL)
        node = node->left;
    return node;
}

template <class T>
Node<T>* AVL_BST<T>::deleteNode(Node<T>* node, T value)
{
    if (node == NULL)
        return node;

    if (value < node->value)
        node->left = deleteNode(node->left, value);
    else if (value > node->value)
        node->right = deleteNode(node->right, value);
    else
    {
        Node<T>* temp;
        if (node->left == NULL && node->right == NULL)
        {
            temp = node;
            node = NULL;
            delete temp;
        }
        else if (node->left == NULL || node->right == NULL)
        {

            if (node->left)
                temp = node->left;
            else
                temp = node->right;

            *node = *temp;
            delete temp;
        }
        else
        {
            Node<T>* temp = findMin(node->right);
            node->value = temp->value;
            node->right = deleteNode(node->right, temp->value);
        }
    }

    if (node == NULL)
        return node;

    node = balance(node);
    return node;
}



template<class T>
void AVL_BST<T>::Print(Node<T> *node)
{
    if(node==NULL) return;
    if(node!=NULL && node->left==NULL && node->right==NULL)
    {
        ofile<<node->value;
        return;
    }
    else if(node!=NULL && (node->left!=NULL || node->right!=NULL))
    {
        ofile<<node->value;
        ofile<<"(";
        Print(node->left);
        ofile<<",";
        Print(node->right);
        ofile<<")";
    }
}




int main()
{
    AVL_BST<int> Tree;
    ifile.open("in.txt");
    ofile.open("out_avl.txt");
    int64_t ti = 0, td = 0, ts = 0, tt = 0,time_taken=0;

    char operation;
    while(ifile.peek()!=EOF)
    {
        ifile>>operation;
        switch(operation)
        {
        case 'I':
        {
            int data;
            ifile>>data;
            auto t1 = std::chrono::high_resolution_clock::now();
            Tree.root = Tree.Insert(Tree.root,data);
            auto t2 = std::chrono::high_resolution_clock::now();
            time_taken = std::chrono::duration_cast<std::chrono::nanoseconds>(t2 - t1).count();
            ti+=time_taken;
            Tree.Print(Tree.root);
            ofile<<endl;
            break;
        }
        case 'F':
        {
            int data;
            ifile >> data;
            auto t1 = std::chrono::high_resolution_clock::now();
            if (Tree.Find(Tree.root,data))
                ofile << "found" << endl;
            else
                ofile << "not found" << endl;
            auto t2 = std::chrono::high_resolution_clock::now();
            time_taken = std::chrono::duration_cast<std::chrono::nanoseconds>(t2 - t1).count();
            ts+=time_taken;
            break;
        }

        case 'D':
        {
            int data;
            ifile >> data;
            auto t1 = std::chrono::high_resolution_clock::now();
            Tree.root=Tree.deleteNode(Tree.root,data);
            auto t2 = std::chrono::high_resolution_clock::now();
            time_taken = std::chrono::duration_cast<std::chrono::nanoseconds>(t2 - t1).count();
            td+=time_taken;
            Tree.Print(Tree.root);
            ofile<<endl;
            break;
        }

        case 'T':
        {
            auto t1 = std::chrono::high_resolution_clock::now();
            Tree.traverse(Tree.root);
            auto t2 = std::chrono::high_resolution_clock::now();
            time_taken = std::chrono::duration_cast<std::chrono::nanoseconds>(t2 - t1).count();
            tt+=time_taken;
            ofile<<endl;
            break;
        }
        }
    }
    ofile.close();
    ofile.open("report_avl.txt");
    ofile<<"operation time(ms)"<<fixed<<setprecision(10)<<endl<<"insert  "<<(ti/den)<<endl<<"delete  "<<(td/den)<<endl<<"search  "<<(ts/den)<<endl<<"trav  "<<(tt/den)<<endl<<"total  "<<(ti+td+ts+tt)/den<<endl;
    ifile.close();
    ofile.close();
    return 0;
}


