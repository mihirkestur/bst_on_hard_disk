#include <stdio.h>
#include <stdlib.h>
#include "header.h"
int main(){
	FILE *file_ptr = init_tree("test.dat");
	int key, choice; 
    printf("1 Insert, 2 Inorder, 3 Preorder, 4 Delete :");
	scanf("%d", &choice);
	while(choice){
		switch(choice){
			case 1: 
				printf("key to insert: ");
				scanf("%d", &key);
				insert_key(key, file_ptr);
				break;
			case 2: 
				display_inorder(file_ptr);
				break;
			case 3: 
				display_preorder(file_ptr);
				break;
            case 4: 
				printf("key to delete: ");
				scanf("%d", &key);
				delete_key(key, file_ptr);
				break;  
		}
		printf("Enter choice: ");
		scanf("%d", &choice);
	}
	close_tree(file_ptr);
	return 0;
}