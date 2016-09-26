#include <stdio.h>
#include "aae_utils/aae_linked_list_type_generation.h"



//extern variable from aae_memory_allocator.h
aae_allocator aae_mallocator;




struct tree
{


	struct node
	{	
		enum {black, red} color;
		int key;
		node *parent, *left, *right;
	} *root, *nil;



	tree(int root_key)
	{
		

		nil = AAE_NEW(node, aae_mallocator);
		nil->parent = nil->left = nil->right = nil;
		nil->color = node::black;
		nil->key = root_key;


		root = AAE_NEW(node, aae_mallocator);
		root->parent = root->left = root->right = nil;
		root->color = node::black;
		root->key = root_key;


	}


};




tree::node* insert_node(tree * t, tree::node * parent, tree::node *& root, int key)
{ 



	if (root == t->nil)
	{
		
		root = AAE_NEW(tree::node, aae_mallocator);
		root->key = key;
		root->left = root->right = t->nil;
		root->parent = parent;
		return root;

	}
	else
	{

		if (key < root->key)
			return insert_node(t, root, root->left, key);
		else
			return insert_node(t, root, root->right, key);

	}
	

}




void rb_left_rotate(tree * t, tree::node * x)
{


	tree::node * y = x->right;
	x->right = y->left;



	if (y->left != t->nil)
		y->left->parent = x;



	y->parent = x->parent;
	if (x->parent == t->nil)
		t->root = y;
	else
	{

		if (x == x->parent->left)
			x->parent->left = y;
		else
			x->parent->right = y;
	}



	y->left = x;
	x->parent = y;

}



void rb_right_rotate(tree * t, tree::node * y)
{


	tree::node * x = y->left;
	y->left = x->right;



	if (x->right != t->nil)
		x->right->parent = y;



	x->parent = y->parent;
	if (y->parent == t->nil) 
		t->root = x;
	else
	{
		if (y == y->parent->left)
			y->parent->left = x;
		else
			y->parent->right = x;
	}



	x->right = y;
	y->parent = x;

}



void rb_insert_node(tree * t, int key)
{



	tree::node * new_node = insert_node(t, t->nil, t->root,  key);
	new_node->color = tree::node::red;


	while (new_node->parent->color == tree::node::red)
	{


		if (new_node->parent == new_node->parent->parent->left)
		{


			tree::node * right_uncle = new_node->parent->parent->right;
			if (right_uncle->color == tree::node::red)
			{

				right_uncle->color = tree::node::black;
				new_node->parent->color = tree::node::black;
				new_node->parent->parent->color = tree::node::red;
				new_node = new_node->parent->parent;

			}
			else
			{

				if (new_node == new_node->parent->right)
				{

					new_node = new_node->parent;
					rb_left_rotate(t, new_node);
				}
				
				new_node->parent->color = tree::node::black;
				new_node->parent->parent->color = tree::node::red;
				rb_right_rotate(t, new_node->parent->parent);

			}

		}
		else
		{

			tree::node * left_uncle = new_node->parent->parent->left;
			if (left_uncle->color == tree::node::red)
			{

				left_uncle->color = tree::node::black;
				new_node->parent->color = tree::node::black;
				new_node->parent->parent->color = tree::node::red;
				new_node = new_node->parent->parent;

			}
			else
			{

				if (new_node == new_node->parent->left)
				{

					new_node = new_node->parent;
					rb_right_rotate(t, new_node);
				}
				
				new_node->parent->color = tree::node::black;
				new_node->parent->parent->color = tree::node::red;
				rb_left_rotate(t, new_node->parent->parent);

			}

			

		}

	}
	t->root->color = tree::node::black;


}




void print_node(tree * t, tree::node *root)
{


	int parent = (root->parent != t->nil) ? root->parent->key : -1;
	int left = (root->left != t->nil) ? root->left->key : -1;
	int right = (root->right != t->nil) ? root->right->key : -1;
	printf("Key:%d,Color:%s,Parent:%d,Left:%d,Right:%d\n", root->key, root->color ? "Red" : "Black", parent, left, right);


	if (left != -1)
		print_node(t, root->left);
	if (right != -1)
		print_node(t, root->right);


}




int main(
    int     argc,
    char   *argv[],
    char   *envp[]
    )
{


	tree * test = AAE_NEW(tree, aae_mallocator)(25);
	rb_insert_node(test, 28); 
	rb_insert_node(test, 27);
	rb_insert_node(test, 29);
	rb_insert_node(test, 23);
	rb_insert_node(test, 24);
	rb_insert_node(test, 30);
	print_node(test, test->root);


	return 0;


}
