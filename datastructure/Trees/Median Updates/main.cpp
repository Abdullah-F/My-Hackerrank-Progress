#include<iostream>
#include<sstream>
using namespace std;

typedef struct node {

    node() {
        height = 0;
        tree_size = 0;
    }
    int tree_size;
    long int data;
    int height;
    node * parent;
    node * left;
    node * right;
} node;

template<typename T>
T * insert(T * root, int value) {// note : "only inserts distinct values."
    if (!root) {
        root = new T();
        root->data = value;
        root->parent = nullptr;
        root->left = nullptr;
        root->right = nullptr;
        return root;//returns the root
    }

    if (value >= root ->data) {
        if (!root->right) {
            root->right = new T();
            root->right->data = value;
            root->right->parent = root;
            root->right->left = nullptr;
            root->right->right = nullptr;
            return root; //returns the parent of the inserted node
        }
           
        return  insert(root->right, value); //just go right one more
    }   
       
        if (!root->left) {
            root->left = new T();
            root->left->data = value;
            root->left->parent = root;
            root->left->right = nullptr;
            root->left->left = nullptr;
            return root; //return the parent of the new inserted node
        }
    return insert(root->left, value); //go left one more
}

node* find(node* root, int value) {// if not found it returns a pointer to the appropriate position where it can be inserted.
    if(!root)
        return root;
    if (root->data == value)
        return root;

    if (value >= root->data && root->right)
        return find(root->right, value);
    else if (value < root->data && root->left) return find(root->left, value);

    return root;
}

node* left_descendant(node* right_node) {//used in the next function
    if (right_node->left)
        return left_descendant(right_node->left);
    return right_node;
}

node* right_ancestor(node* previous_node) {//used in the nex() function.
    node* ancestor = previous_node->parent;

    while (ancestor && previous_node->data >= ancestor->data)
        ancestor = ancestor->parent;

    return ancestor;
}


node* next_node_ptr(node* current) {// returns null if the number we search for is the largest one.

    if (current->right) {
        return left_descendant(current->right);
    } else {
        return right_ancestor(current);
    }

}



node* delete_node_avl_version(node(*(&root)),node* node_to_be_deleted) {//works fine whiout errors, but needs more
    //testing
    if (!node_to_be_deleted->right) {
        
        if(node_to_be_deleted->parent){
            
        if (node_to_be_deleted->parent->left&&
                (node_to_be_deleted->parent->left->data == 
                node_to_be_deleted->data)) {
            
            node_to_be_deleted->parent->left = node_to_be_deleted->left;
            if (node_to_be_deleted->left)
                node_to_be_deleted->left->parent = node_to_be_deleted->parent;
        } else {
            node_to_be_deleted->parent->right = node_to_be_deleted->left;
            if (node_to_be_deleted->left)
                node_to_be_deleted->left->parent = node_to_be_deleted->parent;
        }
        
        }else if(!node_to_be_deleted->parent && node_to_be_deleted->left)
        {
            root = node_to_be_deleted->left;
            root->parent = nullptr;
            return nullptr;// return the parent of the deleted node
        }
        else if(!node_to_be_deleted->parent && !node_to_be_deleted->left)
        {
            root = nullptr;
            return nullptr;//returns the parent of the deleted node  
        } 
        
        return node_to_be_deleted->parent;//returns the parent of the deleted node

    } else {
        node* next_node = next_node_ptr(node_to_be_deleted);//find next element to be deleted
        node_to_be_deleted->data = next_node->data;//replace the deleted node with its next
        //the above line is doing replacements each time. to better make it only once. 
       return  delete_node_avl_version(root,next_node);//delete next value (since it is now duplicate value.).

    }
}


void rotate_right(node(*(&root)), node *to_be_rotated) {//well tested.
    if (!to_be_rotated)
        return;
    else if (!to_be_rotated->left)
        return;


    node* parent = to_be_rotated->parent;
    node* to_be_rotated_left = to_be_rotated->left;

    node* to_be_rotated_new_left = to_be_rotated->left->right;

    to_be_rotated_left->parent = parent;

    if (parent) {
        if (to_be_rotated_left->data >= parent->data)
            parent->right = to_be_rotated_left; //if to_be_rotated_left is "equal" or larger than parent
        else
            parent->left = to_be_rotated_left;
    } else root = to_be_rotated_left;

    to_be_rotated->parent = to_be_rotated_left;
    to_be_rotated_left->right = to_be_rotated;

    if (to_be_rotated_new_left)
        to_be_rotated_new_left->parent = to_be_rotated;

    to_be_rotated->left = to_be_rotated_new_left;

}

void rotate_left(node(*(&root)), node *to_be_rotated) {//well tested
    if (!to_be_rotated)
        return;
    else if (!to_be_rotated->right)
        return;

    node* parent = to_be_rotated->parent;
    node* to_be_rotated_right = to_be_rotated->right;
    node* to_be_rotated_new_right = to_be_rotated->right->left;

    to_be_rotated_right->parent = parent;

    if (parent) {
        if (to_be_rotated_right->data >= parent->data)
            parent->right = to_be_rotated_right;
        else parent->left = to_be_rotated_right;
    } else root = to_be_rotated_right;

    to_be_rotated->right = to_be_rotated_new_right;


    if (to_be_rotated_new_right)
        to_be_rotated_new_right->parent = to_be_rotated;

    to_be_rotated->parent = to_be_rotated_right;
    to_be_rotated_right->left = to_be_rotated;
}

void adjust_height(node* to_be_height_adjusted) {//well tested

    if (to_be_height_adjusted->left && to_be_height_adjusted->right){
        to_be_height_adjusted->height = 1 + max(to_be_height_adjusted->left->height,
            to_be_height_adjusted->right->height);//adjust height
        
        to_be_height_adjusted->tree_size = 1 + to_be_height_adjusted->left->tree_size+
                to_be_height_adjusted->right->tree_size;//adjust size
    }
    else if (!to_be_height_adjusted->left && to_be_height_adjusted->right){
        to_be_height_adjusted->height = 1 + to_be_height_adjusted->right->height;//adjust height 
        to_be_height_adjusted->tree_size = to_be_height_adjusted->height;//adjust size
    }
    else if (to_be_height_adjusted->left && !to_be_height_adjusted->right){
        to_be_height_adjusted->height = 1 + to_be_height_adjusted->left->height;//adjust height
        to_be_height_adjusted->tree_size = to_be_height_adjusted->height;//adjust size
    }
    else{ 
        to_be_height_adjusted->height = 1;//set height = 1
        to_be_height_adjusted->tree_size = to_be_height_adjusted->height;//adjust size
    }
}

void rebalance_left(node(*(&root)), node* node_to_be_balanced) {

    if (node_to_be_balanced->right) {
        node* node_to_be_balanced_right = node_to_be_balanced->right;

        if (node_to_be_balanced_right->right && node_to_be_balanced_right->left) {

            if (node_to_be_balanced_right->left->height >
                    node_to_be_balanced_right->right->height) {
                rotate_right(root, node_to_be_balanced_right);
                adjust_height(node_to_be_balanced_right);
                adjust_height(node_to_be_balanced_right->parent);
            }
        } else if (!node_to_be_balanced_right->right && node_to_be_balanced_right->left) {
            
                rotate_right(root, node_to_be_balanced_right);
                adjust_height(node_to_be_balanced_right);
                adjust_height(node_to_be_balanced_right->parent);
            
        }

    }

    rotate_left(root, node_to_be_balanced);
    adjust_height(node_to_be_balanced);
    adjust_height(node_to_be_balanced->parent);
}

void rebalance_right(node(*(&root)), node* node_to_be_balanced) {

    if (node_to_be_balanced->left) {

        node* node_to_be_balanced_left = node_to_be_balanced->left;
        if (node_to_be_balanced_left->right && node_to_be_balanced_left->left) {
            if (node_to_be_balanced_left->right->height >
                    node_to_be_balanced_left->left->height) {
                rotate_left(root, node_to_be_balanced_left);

                adjust_height(node_to_be_balanced_left);
                adjust_height(node_to_be_balanced_left->parent);
            }
        } else if (node_to_be_balanced_left->right && !node_to_be_balanced_left->left) {
            
            
                rotate_left(root, node_to_be_balanced_left);
                adjust_height(node_to_be_balanced_left);
                adjust_height(node_to_be_balanced_left->parent);
        }

    }

    rotate_right(root, node_to_be_balanced);
    adjust_height(node_to_be_balanced);
    adjust_height(node_to_be_balanced->parent);
}

void rebalance(node(*(&root)), node* balancing_starting_point) {
    node* parent = balancing_starting_point->parent;
    //well tested function.
    if (balancing_starting_point->left && balancing_starting_point->right) {
        if (balancing_starting_point->left->height >
                balancing_starting_point->right->height + 1)
            rebalance_right(root, balancing_starting_point);
    } else if (balancing_starting_point->left && !balancing_starting_point->right) {
        if (balancing_starting_point->left->height > 1)
            rebalance_right(root, balancing_starting_point);
    }

    if (balancing_starting_point->left && balancing_starting_point->right) {
        if (balancing_starting_point->right->height >
                balancing_starting_point->left->height + 1)
            rebalance_left(root, balancing_starting_point);
    } else if (!balancing_starting_point->left && balancing_starting_point->right) {
        if(balancing_starting_point->right->height > 1)
        rebalance_left(root, balancing_starting_point);
    }

    adjust_height(balancing_starting_point);// to be better but it in an else statement after the above conditions.
    
    if (parent)
        rebalance(root, parent);


}

void insert_AVL(node(*(&root)), int value) {
    node* balance_starting_node;
    if (root){
     balance_starting_node = insert(root, value);
    if(value >= balance_starting_node->data)
        balance_starting_node = balance_starting_node->right;
    else 
        balance_starting_node = balance_starting_node->left;
    }
    else{
        
     root =  insert(root, value);// if tree is empty create a head .
     balance_starting_node = root;
    }
    
    rebalance(root, balance_starting_node);
}



void delete_AVL(node(*(&root)), int value)
{//produces correct outputs , still needs further testing
 //only works on distinct values input, needs small adjustment for none-distinct values.
    if(root)
    {
        node* node_to_be_deleted = find(root,value);
        if(node_to_be_deleted->data == value)
        {
            node* balance_starting_point = delete_node_avl_version(root,node_to_be_deleted);
            if(balance_starting_point){
                rebalance(root,balance_starting_point);
            }else return;
            
        }else return;
    }else return;
}

    

int select_node_by_rank(node* root , int node_rank)//well tested.
{//returns the node value at the specified rank (index).
    node_rank++;
    if(!root)
        return -1;//can't select from an empty tree.
    if(node_rank < 1 || node_rank > root->tree_size)
        return -1;//out of range exception.
    
    int node_value = -1 ,temp_rank = 0,sub_rank = 0;
    node* temp = root;
    
    while(temp)
    {
         if(temp->left)
               sub_rank += (temp->left->tree_size +1);
            else sub_rank += 1;
         temp_rank+= sub_rank;
         
        if(temp_rank == node_rank)
        {
            node_value = temp->data;
            break;
        }else if(node_rank > temp_rank)
        {
            temp = temp->right;
            sub_rank = 0;
        }else{
            temp = temp->left;
            temp_rank -= sub_rank;
            sub_rank = 0;
        }
        
        
    }
    
    
    return node_value;
}

double get_avl_median(node* root){//returns the median element of an avl tree.
    //this method does not ckeck if the incoming root is null or not.
 
   
    if(root->tree_size %2 != 0)
        return select_node_by_rank(root,root->tree_size/2);
    
        long int med1 = select_node_by_rank(root,root->tree_size/2);
        long int med2 = select_node_by_rank(root,(root->tree_size/2)-1);
        return (med1+med2)/2.0;
}

string TrimString(string str)
  {
  bool dot = false;
  for(int i = 0 ; i < str.length() ; i++ )
  {
      if(str[i] == '.'){
          dot = true;
          break;
      }
  }
  char cr ;
  if(dot)
  for(int s=str.length()-1 ; s>0; --s)
    {
      cr = str[s]; 
    if(cr == '0' || cr == '.') 
        str.erase(s,1);
    else 
        break;
    if(cr == '.')
        break;
    }
  
  return str;
  }



int main() {

    node* tree = nullptr;
    long int size, value;
    char op;
    cin >> size;
    
    while(size > 0)
    {
        cin >> op >> value;
        if(op == 'a')
        {
            insert_AVL(tree,value);
        }else if(tree){
            
            node* temp = find(tree,value);
            if(temp->data == value){
              delete_AVL(tree,value);
            }
            else{
                cout << "Wrong!" << endl;
                size--;
                continue;
            }
        }
        
        if(tree)
        {
              stringstream ss;
              string str;
                ss << std::fixed << get_avl_median(tree); 
                ss >> str;
                str = TrimString(str);
                cout << str << endl;
        }else
             cout << "Wrong!" << endl;
        
        size--;
    }
    
    return 0;
}
/*
string TrimString(string str)
  {
  bool dot = false;
  for(int i = 0 ; i < str.length() ; i++ )
  {
      if(str[i] == '.'){
          dot = true;
          break;
      }
  }
  char cr ;
  if(dot)
  for(int s=str.length()-1 ; s>0; --s)
    {
      cr = str[s]; 
    if(cr == '0' || cr == '.') 
        str.erase(s,1);
    else 
        break;
    if(cr == '.')
        break;
    }
  
  return str;
  }

int main() {
   multiset<int> mySet;
    char op;
    long int val, erased_elements, size, med1, med2, n;
    double median;
    string str;
    multiset<int>::iterator itr;
    mySet.insert(1);
   
    mySet.erase(1);
    cin >> n;
    while (n > 0) {
        cin >> op >> val;

        if (op == 'a') {
            mySet.insert(val);
            size = mySet.size();
        } else {
            erased_elements = erased_elements = mySet.size();
            if (erased_elements > 0) {

                itr = mySet.find(val);
                if (itr == mySet.end())
                    erased_elements = 0;
                else {
                    mySet.erase(itr);
                    size = mySet.size();
                    if (erased_elements != 1)
                        erased_elements = erased_elements - size;
                }

            }

        }

        if ((op == 'a' || (op == 'r' && erased_elements > 0)) && size > 0) {
            itr = mySet.begin();
            advance(itr, size / 2);

            if (size % 2 != 0) {
                cout << *itr << endl;
            } else {
                med1 = *itr;

                --itr;

                med2 = *itr;

                median = (med2 + med1) / 2.0;
                

                stringstream ss;
               
                ss << std::fixed << median; // outputs 5000000.550000
                ss >> str;
                str = TrimString(str);
                
                cout << str << endl;    
            }
        } else
            cout << "Wrong!" << endl;
        n--;
    }



    return 0;
}*/