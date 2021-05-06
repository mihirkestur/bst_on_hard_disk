#include<stdio.h>
#include<stdlib.h>
#include "assignment_4.h"
//Name: Mihir Madhusudan Kestur   SRN: PES1UG19CS272     Section: E     Semester: 4

//from the file, to the bs_tree read the header of tree present in file
static void read_header(FILE *file_ptr, tree_t *bs_tree){
	//set pointer to the beginning
	fseek(file_ptr, 0, SEEK_SET);
	//read contents of the header represented by tree_t to bs_tree
	fread(bs_tree, sizeof(tree_t), 1, file_ptr);
}

//to the file, from the bs_tree write or update the header content
static void update_header(FILE *file_ptr, const tree_t *bs_tree){
	//set pointer to beginning
	fseek(file_ptr, 0, SEEK_SET);
	//write or update the content of the header from bs_tree 
	fwrite(bs_tree, sizeof(tree_t), 1, file_ptr);
}

//from the file, to the node, read the contents of node present at begin + offset
static void read_node(FILE *file_ptr, node_t *node, int offset){
	//set pointer to begin, then skip the header and go to postion node at offset
	fseek(file_ptr, offset * sizeof(node_t) + sizeof(tree_t), SEEK_SET);
	//read the node contents of size node_t to node
	fread(node, sizeof(node_t), 1, file_ptr);
}

//to the file, update or insert a node at the postion begin + offset
static void insert_node(FILE *file_ptr, const node_t *node, int offset){
	//set pointer to begin, then skip the header and go to postion node at offset
	fseek(file_ptr, offset * sizeof(node_t) + sizeof(tree_t), SEEK_SET);
	//write or update the contents of the node from node stucture
	fwrite(node, sizeof(node_t), 1, file_ptr);
}

//this function initialises a node 
static void initialise_node(int key, node_t *node){
	node->left_offset = -1;
	node->right_offset = -1;
	node->key = key;
}

//This function return a pointer to the file (it is created if not present)
FILE* init_tree(const char* filename){
    FILE *file_ptr;
    //open file in read and write if it is there
	file_ptr = fopen(filename, "r+"); 
    //if file is not there
	if(file_ptr == NULL){
        //create file in read and write mode
		file_ptr = fopen(filename, "w+");
        //if some error then quit
		if(file_ptr == NULL){
			exit(1);
		}
		else{
            //create a tree			
			tree_t bs_tree;
            //initialise the tree contents 
            bs_tree.root = -1;
            bs_tree.free_head = -1;  
            //write this new header to file
            update_header(file_ptr, &bs_tree);
		}
	}
	return file_ptr;
}

//close the file pointed by fp
void close_tree(FILE *fp){
    fclose(fp);
}

//recursive implementation of the inorder traversal
static void inorder_bs_tree(FILE *file_ptr, const node_t *node){
	node_t child_node;
	//if there is a left child go left
	if(node->left_offset != -1){
		//read the node at left offset
		read_node(file_ptr, &child_node, node->left_offset);
		//call recursive function
		inorder_bs_tree(file_ptr, &child_node);
	}
	//print node
	printf("%d ", node->key);
	//if there is a right child go right
	if(node->right_offset != -1){
		//read the node at right offset
		read_node(file_ptr, &child_node, node->right_offset);
		//call recursive function
		inorder_bs_tree(file_ptr, &child_node);
	}
}
// Space separated keys. Last key followed by '\n'
void display_inorder(FILE *fp){
	//read the tree header
	tree_t bs_tree;
	read_header(fp, &bs_tree);
	//if tree is empty then simply print \n and return
	if(bs_tree.root == -1){
		printf("\n");
	}
	//otherwise print bst in the format expected
	else{
		//read the root node
		node_t root_node;
		read_node(fp, &root_node, bs_tree.root);
		//call the recursive function to print tree
		inorder_bs_tree(fp, &root_node);
		printf("\n");
	}
}
//recursive implementation of the preorder traversal
static void preorder_bs_tree(FILE *file_ptr, const node_t *node){
	node_t child_node;
	//print node
	printf("%d ", node->key);
	//if there is a left child go left
	if(node->left_offset != -1){
		//read the node at left offset
		read_node(file_ptr, &child_node, node->left_offset);
		//call recursive function
		preorder_bs_tree(file_ptr, &child_node);
	}
	//if there is a right child go right
	if(node->right_offset != -1){
		//read the node at right offset
		read_node(file_ptr, &child_node, node->right_offset);
		//call recursive function
		preorder_bs_tree(file_ptr, &child_node);
	}
}
// Space separated keys. Last key followed by '\n'
void display_preorder(FILE *fp){
	//read the tree header
	tree_t bs_tree;
	read_header(fp, &bs_tree);
	//if tree is empty then simply print \n and return
	if(bs_tree.root == -1){
		printf("\n");
	}
	//otherwise print bst in the format expected
	else{
		//read the root node
		node_t root_node;
		read_node(fp, &root_node, bs_tree.root);
		//call the recursive function to print tree
		preorder_bs_tree(fp, &root_node);
		printf("\n");
	}
}

//this function inserts a key into the file, returns if it is already there
void insert_key(int key, FILE *fp){
	//read the header from the file
	tree_t bs_tree;
	read_header(fp, &bs_tree);
	//if there is no node present and tree is empty
	if(bs_tree.root == -1){
		//create new root node
		node_t new_root_node;
		int new_node_offset;
		//if free nodes (deleted nodes) are available
		if(bs_tree.free_head != -1){
			//note the offset of the first free node
			new_node_offset = bs_tree.free_head;
			//read the contents of the free node  
			read_node(fp, &new_root_node, new_node_offset);
			//change the free head in header to the next free node (if any)
			bs_tree.free_head = new_root_node.right_offset;
		}
		//if there are NO free nodes (deleted nodes) available
		else{
			//new root offset becomes 0
			new_node_offset = 0;
		}
		//set the tree's root node as the new node offset obtained
		bs_tree.root = new_node_offset;
		//initialise the root node with new key
		initialise_node(key, &new_root_node);
		//update these changes in header to the file
		update_header(fp, &bs_tree);
		//insert this root node to the tree in file
		insert_node(fp, &new_root_node, new_node_offset);
	}
	//if tree is NOT empty
	else{
		int new_node_offset, parent_offset;
		//start from root node
		int traverse_offset = bs_tree.root;
		node_t parent_node, new_node;
        //traverse the entire tree to find the correct parent 
		while(traverse_offset != -1){
            //set the parent offset to the traverse (current) offset
			parent_offset = traverse_offset;
			//read the node at the parent offset
            read_node(fp, &parent_node, parent_offset);
            //if the node has same key as key to be inserted
			if(parent_node.key == key){
				//do nothing and return
                return;
			}
			//if the parent is less than key to be inserted then go to right
			else if(parent_node.key < key){
                traverse_offset = parent_node.right_offset;
            }
			//if the parent is greater than key to be inserted then go to left
            else if(parent_node.key > key){
                traverse_offset = parent_node.left_offset;
            }
        }
		//if free nodes (deleted nodes) are available
		if(bs_tree.free_head != -1){
			//note the offset of the first free node
			new_node_offset = bs_tree.free_head; 
			//read the contents of the free node 
			read_node(fp, &new_node, new_node_offset);
			//set the free head to the next available free node
			bs_tree.free_head = new_node.right_offset; 
			//update the changes in the header to the file
			update_header(fp, &bs_tree);
			//read the content of the node at parent offset to the node
            read_node(fp, &parent_node, parent_offset);
			//if key is greater than parent insert to the right
            if(parent_node.key < key){
                parent_node.right_offset = new_node_offset;
            }
			//otherwise insert to the left
            else{
                parent_node.left_offset = new_node_offset;
            }
		}
		//if there are NO free nodes (deleted nodes) available
		else{
			//go to the end of file
            fseek(fp, 0, SEEK_END);
			/*to obtain the node at the last, obtain the number of nodes in the tree by
			subtracting the size of header and then dividing by the size of node*/
	        new_node_offset = (ftell(fp) - sizeof(tree_t))/sizeof(node_t);
			//read the node at the parent offset
            read_node(fp, &parent_node, parent_offset);
			//if key is greater than parent insert to the right
            if(parent_node.key < key){
                parent_node.right_offset = new_node_offset;
            }
			//otherwise insert to the left
            else{
                parent_node.left_offset = new_node_offset;
            }	
		}
		//update the parent node offset appropriately
        insert_node(fp, &parent_node, parent_offset);
		//initialise the node with the new key
		initialise_node(key, &new_node);
		//insert this new node at the new node offset position
		insert_node(fp, &new_node, new_node_offset);
	}
}

//this function deletes the key in the tree if it exists and adds it to the free list
void delete_key(int key, FILE *fp){
	//read the header from the file
	tree_t bs_tree;
	read_header(fp, &bs_tree);
	//initialising the traverse node to the root offset
	int traverse_node_offset = bs_tree.root;
	//initialising the parent offset and the delete node offset
	int parent_node_offset = -1, delete_node_offset = -1;
	//node to be deleted
	node_t delete_node; 
	//if the tree is empty then do nothing and return
	if(bs_tree.root == -1){
		return;
	}
	//otherwise search for the node that is to be deleted
	while(traverse_node_offset != -1){
		//read the contents of the node at traverse offset to the delete node
		read_node(fp, &delete_node, traverse_node_offset);
		//if key is found then set the offset delete node
		if(delete_node.key == key){
			delete_node_offset = traverse_node_offset;
			break;
		}
		//otherwise set the parent node offset as the traverse node offset
		parent_node_offset = traverse_node_offset;
		//if the key is greater than delete node key go to right
		if(delete_node.key < key){
			traverse_node_offset = delete_node.right_offset;
		}
		//if the key is lesser than delete node key go to left
		else if(delete_node.key > key){
			traverse_node_offset = delete_node.left_offset;
		}
	}
	//if the key to be deleted is found in the tree
	if(delete_node_offset != -1){
		/*if the node to be deleted has,
		only one subtree (i.e. left subtree OR right subtree)
		OR
		NO subtrees at all*/
		if(delete_node.right_offset == -1 || delete_node.left_offset == -1){
			//the right child or the left child becomes the successor otherwise successor remains -1 
			int successor_offset = (delete_node.left_offset == -1) ? delete_node.right_offset : delete_node.left_offset;
			//if the root node is to be deleted
			if(parent_node_offset == -1){
				//add the root node to the free node list
				delete_node.right_offset = bs_tree.free_head;
				bs_tree.free_head = bs_tree.root;
				//the new root node is the successor offset
				bs_tree.root = successor_offset;
			}
			//if the root node is NOT the node that is to be deleted
			else{
				//read the parent node from the file
				node_t parent_node;
				read_node(fp, &parent_node, parent_node_offset);
				//setting the child of the parent of the delete node appropriately
				if(parent_node.left_offset == delete_node_offset){
					parent_node.left_offset = successor_offset;
				}
				else{
					parent_node.right_offset = successor_offset;
				}
				//updating these changes to the parent node in the file
				insert_node(fp, &parent_node, parent_node_offset);
				//add the deleted node to the free node list
				delete_node.right_offset = bs_tree.free_head;
				bs_tree.free_head = delete_node_offset;
			}
			//upate changes to the deleted node
			insert_node(fp, &delete_node, delete_node_offset);
			//changes made to the header is updated to the file
			update_header(fp, &bs_tree);
		}
		//the node to be deleted has both (left and right) children
		else{
			//offset of parent of inorder successor
			int parent_offset = -1; 
			//offset of inorder successor
			int inorder_successor_offset = delete_node.right_offset; 
			//read the node at the position inorder successor
			node_t inorder_successor_node; 
			read_node(fp, &inorder_successor_node, inorder_successor_offset);
			//traverse the left subtree of the right subtree of the delete node
			while(inorder_successor_node.left_offset != -1){
				//note the parent
				parent_offset = inorder_successor_offset;
				//change the inorder successor offset and read
				inorder_successor_offset = inorder_successor_node.left_offset;
				read_node(fp, &inorder_successor_node, inorder_successor_offset);
			}
			//if the inorder successor is found to be the right child of delete node
			if(parent_offset == -1){
				//the delete node will become the inorder successor
				delete_node.right_offset = inorder_successor_node.right_offset;
			}
			//if the inorder succesor is not the right child of delete node
			else{
				//read the parent of the inorder successor
				node_t parent_node;
				read_node(fp, &parent_node, parent_offset);
				//parent will now take care of inorder successor's right children to free the inorder successor
				parent_node.left_offset = inorder_successor_node.right_offset;
				//update the parent about these changes
				insert_node(fp, &parent_node, parent_offset);
			}
			//swap the keys of the inorder successor and the delete key
			delete_node.key = inorder_successor_node.key;
			//update the delete node to be the inorder successor node
			insert_node(fp, &delete_node, delete_node_offset);
			//inorder successor node now becomes the free/deleted node
			//finally add inorder successor to free list
			inorder_successor_node.right_offset = bs_tree.free_head;
			//update the inorder successor node as the deleted node
			insert_node(fp, &inorder_successor_node, inorder_successor_offset);
			bs_tree.free_head = inorder_successor_offset;
			//update the free list in the header
			update_header(fp, &bs_tree);
		}
	}
}