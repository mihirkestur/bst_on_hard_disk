# bst_on_hard_disk
Key takeaways from this assignment:
- Gained insight into how a data structure can be stored on the hard disk
- Learnt to use the offset strategy to represent nodes and links
- Learnt how to use the fseek, fwrite, and ftell functions and about their arguments

Additional notes:
- The right offset of all the free nodes (deleted nodes) is used to represent the free node list
- Insertion and deletion is implemented in an iterative manner and the inorder/preorder traversals are
implemented recursively
- A file is created if there is no file initially
- It is also worthy to note that if a file exists and it does not have the tree header structure written
into it the program does not work
- However, if a file exists and the tree structure is there then, one can alter the structure using the 
required functions
- The inorder/preorder traversals displays a newline if no elements are there
- The deleted nodes are re-used
- Duplicate insertion and deletion of non-existent is taken care of by ignoring it