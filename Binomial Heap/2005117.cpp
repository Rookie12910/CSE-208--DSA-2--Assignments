#include<bits/stdc++.h>
using namespace std;

ifstream ifile;
ofstream ofile;

class Node
{
public:
    int key;
    int degree;
    Node *parent;
    Node *lmChild; //Left Most Child
    Node *rSibling; //Right Sibling
    Node(int key,int degree)
    {
        this->key = key;
        this->degree = degree;
        parent = nullptr;
        lmChild = nullptr;
        rSibling = nullptr;
    }
};

class BinomialHeap
{
public:
     Node *head;

    BinomialHeap()
    {
        head = nullptr;
    }
    BinomialHeap(Node *node)
    {
        head = node;
    }
    void linkTrees(Node *n1, Node *n2);
    void Insert (int data);
    void Merge(BinomialHeap h);
    void Union (BinomialHeap h);
    Node* Find_Min();
    Node* Extract_Min();
    void Print();
};

void BinomialHeap::Insert(int key)
{
    Node *node = new Node(key,0);
    BinomialHeap h(node);
    Union(h);
}


void BinomialHeap::linkTrees(Node* n1, Node* n2)
{
		n2->parent = n1;
		n2->rSibling = n1->lmChild;
		n1->lmChild = n2;
		n1->degree++;
}
void BinomialHeap::Merge(BinomialHeap h)
{
    if(this->head == nullptr)
    {
        this->head = h.head;
        return;
    }
    if(h.head == nullptr) return;

    Node *curr1 = this->head;
    Node *curr2 = h.head;
    Node *curr = nullptr;
    if(curr1->degree<=curr2->degree)
    {
        curr = curr1;
        curr1 = curr1->rSibling;
    }
    else
    {
        curr = curr2;
        curr2 = curr2->rSibling;
    }
    Node *start = curr;
    while(curr1!=nullptr && curr2!=nullptr)
    {
        if(curr1->degree<=curr2->degree)
        {
            curr->rSibling = curr1;
            curr1 = curr1->rSibling;
        }
        else
        {
            curr->rSibling = curr2;
            curr2 = curr2->rSibling;
        }
        curr = curr->rSibling;
    }
    if(curr2==nullptr)
    {
        curr->rSibling = curr1;
    }
    else
    {
        curr->rSibling = curr2;
    }
    this->head = start;
}

void BinomialHeap::Union(BinomialHeap h)
{
    this->Merge(h);
    if(this->head==nullptr) return;
    Node *prev = nullptr;
    Node *curr = this->head;
    Node *next = curr->rSibling;
    while(next!=nullptr)
    {
        if((curr->degree!=next->degree)||(next->rSibling!=nullptr && curr->degree==next->rSibling->degree))
        {
            prev = curr;
            curr = next;
        }
        else
        {
            if(curr->key<=next->key)
            {
                curr->rSibling = next->rSibling;
                linkTrees(curr,next);
            }
            else
            {
                if(prev==nullptr)
                {
                    this->head = next;
                }
                else
                {
                    prev->rSibling = next;
                }
                linkTrees(next,curr);
                curr = next;
            }
        }
        next = curr->rSibling;
    }

}

Node* BinomialHeap::Find_Min()
{
    int Min = INT_MAX;
    Node *curr = this->head;
    Node *Min_node = nullptr;
    while(curr!=nullptr)
    {
        if(curr->key<Min)
        {
            Min = curr->key;
            Min_node = curr;
        }
        curr = curr->rSibling;
    }
    return Min_node;
}

Node* BinomialHeap::Extract_Min()
{
    Node *Min_node = Find_Min();
    if(Min_node==nullptr) return nullptr;
    Node *prev = this->head;
    if(this->head==Min_node)
    {
       prev = nullptr;
       head = head->rSibling;
    }
    else
    {
        while(prev->rSibling!=Min_node)
        {
            prev = prev->rSibling;
        }
        prev->rSibling = Min_node->rSibling;
    }
    stack<Node*> stk;
    Node *node = Min_node->lmChild;
    if(node==nullptr) return Min_node;
    node->parent = NULL;
    while(node!=nullptr)
    {
        stk.push(node);
        node = node->rSibling;
    }

    BinomialHeap h(stk.top());
    Node *curr = h.head;
    while(!stk.empty())
    {
        curr->rSibling = stk.top();
        stk.pop();
        curr = curr->rSibling;
    }
    curr->rSibling = nullptr;
    Union(h);
    return Min_node;
}

void BinomialHeap::Print()
    {
        ofile<<"Printing Binomial Heap..."<<endl;
        Node* n = head;
        while(n != nullptr)
        {
            ofile<<"Binomial Tree, B"<<n->degree<<endl;
            int level = 0;
            queue <Node*> q;
            q.push(n);
            while(!q.empty())
            {
                int currSize = q.size();
                ofile<<"Level "<<level<<" : ";
                for(int i =0;i<currSize;i++)
                {
                    Node* curr = q.front();
                    q.pop();
                    ofile<<curr->key<<" ";
                if(curr->lmChild != nullptr)
                {
                    q.push(curr->lmChild);
                    Node* sib = curr->lmChild->rSibling;
                    while(sib != nullptr)
                    {
                        q.push(sib);
                        sib = sib->rSibling;
                    }
                }
                }
                level++;
                ofile<<endl;
            }
            n = n->rSibling;
        }
    }


int main()
{
    BinomialHeap H;
    bool flag = true;
    ifile.open("input.txt");
    ofile.open("output.txt");
    char operation;
    while(ifile.peek()!=EOF)
    {
        if(flag) ifile>>operation;
        else flag = true;
        if(ifile.peek()==EOF)
            break;
        switch(operation)
        {
        case 'I':
        {
            int data;
            ifile>>data;
            H.Insert(data);
            break;
        }
        case 'P':
        {
            H.Print();
            break;
        }

        case 'F':
        {
            Node *minNode = H.Find_Min();
            if(minNode==nullptr) ofile<<"Find-Min returned "<<endl;
            else
            {
              int Min = minNode->key;
              ofile<<"Find-Min returned "<<Min<<endl;
            }
            break;
        }

        case 'E':
        {
            Node *minNode = H.Extract_Min();
            if(minNode==nullptr) ofile<<"Extract-Min returned "<<endl;
            else
            {
              int Min = minNode->key;
              ofile<<"Extract-Min returned "<<Min<<endl;
            }
            break;
        }
        case 'U':
            {
                flag = false;
                BinomialHeap h;
                string s;
                while (ifile >> s) {
                    try {
                        int data = stoi(s);
                        h.Insert(data);
                    } catch (const std::invalid_argument& e) {
                        operation=s[0];
                        break;
                    }
                }
                H.Union(h);
                break;
            }
        }
    }
    return 0;
}
