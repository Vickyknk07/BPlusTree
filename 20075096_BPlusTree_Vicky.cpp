#include <bits/stdc++.h>
using namespace std;

#define ll long long

class BPNode{
public:
    ll mx_sz, size;
    BPNode(ll mx_sz){
		this->mx_sz = mx_sz;
		records = new ll[mx_sz];
		bp_nodes = new BPNode *[mx_sz+1];
	}
    ll *records;
    BPNode **bp_nodes;
    ll is_leaf;
};

class BPTree{
public:
    ll order;
    BPTree(ll order){
		root = NULL;
	}
    BPNode *root;

    void insertKey(ll key, ll order){
		if(root == NULL){
			root = new BPNode(order);
			root->is_leaf = 1;
			root->records[0] = key;
			root->size = 1;
		}
		else{
			BPNode *node = root;
			BPNode *par;
			while(node->is_leaf == 0){
				par = node;
				ll pos = 0;
				while(pos < node->size){
					if(pos == node->size-1){
						node = node->bp_nodes[pos+1];
						break;
					}
					if(key < node->records[pos]){
						node = node->bp_nodes[pos];
						break;
					}
					pos++;
				}
			}
			if(node->size < order){
				ll pos1 = 0;
				while(key > node->records[pos1] && pos1 < node->size) pos1++;

				ll pos2 = node->size;
				while(pos2 > pos1){
					node->records[pos2] = node->records[pos2-1];
					pos2--;
				}

				node->size++;
				node->records[pos1] = key;
				node->bp_nodes[node->size] = node->bp_nodes[node->size-1];
				node->bp_nodes[node->size-1] = nullptr;
			}
			else{
				BPNode *new_leaf = new BPNode(order);
				ll temp[order+1];
				for(ll i=0; i<order; i++) temp[i] = node->records[i];

				ll i = 0, j;
				while(key > temp[i] && i < order) i++;

				ll pos = order+1;
				while(pos > i){
					temp[pos] = temp[pos-1];
					pos--;
				}

				new_leaf->is_leaf = 1;
				temp[i] = key;
				new_leaf->size = order+1 - (order+1) / 2;
				node->size = (order+1) / 2;
				new_leaf->bp_nodes[new_leaf->size] = node->bp_nodes[order];
				node->bp_nodes[node->size] = new_leaf;
				node->bp_nodes[order] = NULL;

				pos = 0;
				while(pos < node->size){
					node->records[pos] = temp[pos];
					pos++;
				}

				i = 0, j = node->size;
				while(i < new_leaf->size){
					new_leaf->records[i] = temp[j];
					i++; j++;
				}

				if(node == root){
					BPNode *new_root = new BPNode(order);
					new_root->bp_nodes[0] = node;
					new_root->records[0] = new_leaf->records[0];
					new_root->bp_nodes[1] = new_leaf;
					new_root->size = 1;
					new_root->is_leaf = 0;
					root = new_root;
				} 
				else insertInternal(new_leaf->records[0], par, new_leaf, order);
			}
		}
	}

	void insertInternal(ll key, BPNode *node, BPNode *child, ll order){
		if(node->size < order){
			ll i = 0;
			while(key > node->records[i] && i < node->size) i++;

			ll pos = node->size;
			while(pos > i){
				node->records[pos] = node->records[pos-1];
				pos--;
			}

			pos = node->size+1;
			while(pos > i+1){
				node->bp_nodes[pos] = node->bp_nodes[pos-1];
				pos--;
			}

			node->size++;
			node->records[i] = key;
			node->bp_nodes[i+1] = child;
		}
		else{
			BPNode *new_internal = new BPNode(order);
			ll temp[order+1];
			BPNode *temp_ptr[order+2];
			for(ll i=0; i<order; i++) temp[i] = node->records[i];

			for(ll i=0; i<order+1; i++) temp_ptr[i] = node->bp_nodes[i];

			ll i = 0, j;
			while(key > temp[i] && i < order) i++;

			ll pos = order+1;
			while(pos > i){
				temp[pos] = temp[pos-1];
				pos--;
			}

			temp[i] = key;
			pos = order+2;
			while(pos > i+1){
				temp_ptr[pos] = temp_ptr[pos-1];
				pos--;
			} 

			new_internal->is_leaf = 0;
			temp_ptr[i+1] = child;
			new_internal->size = order - (order + 1) / 2;
			node->size = (order+1) / 2;
			for(j=node->size+1, i=0; i<new_internal->size; j++, i++) new_internal->records[i] = temp[j];

			i = 0, j = node->size+1;
			while(i < new_internal->size+1){
				new_internal->bp_nodes[i] = temp_ptr[j];
				i++; j++;
			}

			if(node == root){
				BPNode *new_root = new BPNode(order);
				new_root->bp_nodes[0] = node;
				new_root->bp_nodes[1] = new_internal;
				new_root->records[0] = node->records[node->size];
				new_root->size = 1;
				new_root->is_leaf = 0;
				root = new_root;
			} 
			else insertInternal(node->records[node->size], findPar(root, node), new_internal, order);
		}
	}

	void displayTree(BPNode *node){
		if(node != NULL){
		    for(ll i=0; i<node->size; i++) cout << node->records[i] << " ";
		    
		    cout << endl;
		    if(node->is_leaf != 1){
		      for(ll i=0; i<node->size+1; i++) displayTree(node->bp_nodes[i]);
		    }
		}
	}

	BPNode *findPar(BPNode *node, BPNode *child){
		BPNode *par;
		if((node->bp_nodes[0])->is_leaf || node->is_leaf) return nullptr;
		
		ll pos = 0;
		while(pos < node->size+1){
			if(node->bp_nodes[pos] != child){
				par = findPar(node->bp_nodes[pos], child);
				if(par != NULL) return par;
			} 
			else{
				par = node;
				return par;
			}
			pos++;
		}
  		return par;
	}

	BPNode *getRoot(){
		return root;
	}
};

int main(){
    ll order;
    cout << "Enter the order of B+ Tree: ";
    cin >> order;

    cout << endl;
    ll operation;
    ll key;

    BPTree bp_tree(order);

    cout << "Enter 1 for insertion\nEnter 2 for status\nEnter 3 to Exit\n";
    cin >> operation;
    while(operation != 4){
        switch(operation){
            case 1:{
                cout << "Enter key to be inserted: ";
                cin >> key;
                bp_tree.insertKey(key, order);
                break;
            }
            case 2:{
                bp_tree.displayTree(bp_tree.getRoot());
                break;
            }
        }
        cout << "Enter 1 for insertion\nEnter 2 for status\nEnter 3 to Exit\n";
        cin >> operation;
    }
    return 0;
}