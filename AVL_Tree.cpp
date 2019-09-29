#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <time.h>

struct data{
	int number;
	int height;
	struct data *left;
	struct data *right;
};

typedef struct data dt;

void refresh(int x){
	for(int i = 0; i < x; i++){
		putchar('\n');
	}
}

void moveText(char *text){
	int x = 0;
	while(text[x] != '\0'){
		printf("%c", text[x]);
		Sleep(20);
		x++;
	}
}

dt*createNode(int number){
	dt *newNode = (dt *)malloc(sizeof(dt));
	newNode->number = number;
	newNode->height = 1;		// the height of new Node is 1 (default)
	newNode->left = NULL;
	newNode->right = NULL;
	return newNode;
}

dt *freeAll(struct data *curr){
	if(curr != NULL){
		curr->left = freeAll(curr->left);
		curr->right = freeAll(curr->right);
		free(curr);
		curr = NULL;
	}
}

void preOrder(dt *curr){
	if(curr == NULL)return;
	printf("%d ", curr->number);
	preOrder(curr->left);
	preOrder(curr->right);
}

void inOrder(dt *curr){
	if(curr == NULL)return;
	inOrder(curr->left);
	printf("%d ", curr->number);
	inOrder(curr->right);
	
}

void postOrder(dt *curr){
	if(curr == NULL)return;
	postOrder(curr->left);
	postOrder(curr->right);
	printf("%d ", curr->number);
}

int max(int heightLeft, int heightRight){
	if(heightLeft >= heightRight)return heightLeft;
	if(heightLeft < heightRight)return heightRight;
	
}

int getHeight(dt *curr){
	if(curr == NULL){
		return 0;
		
	}else{
		return curr->height;
	}
}

int getBalanceFactor(dt *curr){
	if(curr == NULL){
		return 0;
	}else{
		return (getHeight(curr->left) - getHeight(curr->right));
	}
}

dt *rightRotate(dt *x){
	dt *y, *c;
	y = x->left;
	c = y->right;
	
	// rotate
	y->right = x;
	x->left = c;
	
	x->height = 1 + max(getHeight(x->left), getHeight(x->right));
	y->height = 1 + max(getHeight(y->left), getHeight(y->right));
	
	return y;		// return new root
}
 
dt *leftRotate(dt *x){
	//right-right
	dt *y, *c;
	y = x->right;
	c = y->left;
	
	// rotate
	y->left = x;
	x->right = c;
	
	x->height = 1 + max(getHeight(x->left), getHeight(x->right));
	y->height = 1 + max(getHeight(y->left), getHeight(y->right));
	
	return y;
}

dt *push(dt *curr, int number){
	if(curr == NULL){
		curr = createNode(number);
		
	}else if(number < curr->number){
		curr->left = push(curr->left, number);
		
	}else if(number > curr->number){
		curr->right = push(curr->right, number);
	}
	
	// update
	curr->height = 1 + max(getHeight(curr->left), getHeight(curr->right));
	int balanceFactor = getBalanceFactor(curr);
	
	if(balanceFactor > 1 && number < curr->left->number){
		// left-left
		// curr = rightRotate(curr);
		return rightRotate(curr);
		
	}else if(balanceFactor > 1 && number > curr->left->number){
		// left-right
		curr->left = leftRotate(curr->left);
		// dimulai setelah root
		return rightRotate(curr);
		
	}else if(balanceFactor < -1 && number > curr->right->number){
		// right-right
		// curr = leftRotate(curr);
		return leftRotate(curr);	
		
	}else if(balanceFactor < -1 && number < curr->right->number){
		// right-left
		curr->right = rightRotate(curr->right);
		return leftRotate(curr);
	}
	return curr;
}

dt *predecessor(dt *curr){
	dt *temp = curr->left;
	while(temp->right != NULL){
		temp = temp->right;
	}
	return temp;
}

dt *succesor(dt *curr){
	dt *temp = curr->right;
	while(temp->left != NULL){
		temp = temp->left;
	}
	return temp;
}

dt *removeData(dt *curr, int num){
	if(curr == NULL){
		printf("  No data!!");
		getchar();
		return curr;
		
	}else if(num < curr->number){
		curr->left = removeData(curr->left, num);
		
	}else if(num > curr->number){
		curr->right = removeData(curr->right, num);
		
	}else{
		if(curr->left == NULL && curr->right == NULL){
			free(curr);
			curr = NULL;
			
		}else if(curr->left != NULL && curr->right == NULL){
			dt *temp = curr->left;
			*curr = *temp;
			free(temp);
			
		}else if(curr->left == NULL && curr->right != NULL){
			dt *temp = curr->right;
			*curr = *temp;
			free(temp);
			
		}else{
			dt *temp = predecessor(curr);
			curr->number = temp->number;
			curr->left = removeData(curr->left, temp->number);
			
		/*
			dt *temp = succesor(curr);
			curr->number = temp->number;
			curr->right = removeData(curr->right, temp->number);
		
		*/
		}
	}
	
	
	// update
	if(curr == NULL){
		return curr;
	}
	curr->height = 1 + max(getHeight(curr->left), getHeight(curr->right));
	int balance = getBalanceFactor(curr);
	
	//left-left
	if(balance > 1 && getBalanceFactor(curr->left) >= 0){
		return rightRotate(curr);
	
	// left-right			
	}else if(balance > 1 && getBalanceFactor(curr->left) < 0){
		curr->left = leftRotate(curr->left);
		// dimulai setelah root
		return rightRotate(curr);
	
	// right-right	
	}else if(balance < -1 && getBalanceFactor(curr->right) <= 0){
		return leftRotate(curr);
		
	// right-left	
	}else if(balance < -1 && getBalanceFactor(curr->right) > 0){
		curr->right = rightRotate(curr->right);
		// dimulai setelah root
		return leftRotate(curr);
	}
	
	return curr;
}

int main(){
	int number;
	int num = 0;
	dt *root = NULL;
	srand(time(0));
	int meet = 1;
	do{
		refresh(30);
	/*
		if(meet == 1){
			for(int i = 0 ; i < 10; i++){
				number = 0;
				number = rand() % 100;
				root = push(root, number);
			}
			meet = 0;
		}
	*/		
		
		moveText("  PreOrder >> ");
		preOrder(root);
		moveText("\n  InOrder >> ");
		inOrder(root);
		moveText("\n  PostOrder >> ");
		postOrder(root);
		
	
		printf("\n  Input number ['0' to exit] >> ");
		scanf("%d", &number);
		getchar();
		root = push(root, number);
	
	
	}while(number != 0);
	
	printf("\n  Enter number >> ");
	scanf("%d", &num);
	getchar();
	root = removeData(root, num);
	
	moveText("  PreOrder >> ");
		preOrder(root);
		moveText("\n  InOrder >> ");
		inOrder(root);
		moveText("\n  PostOrder >> ");
		postOrder(root);
printf("\n  Input number ['0' to exit] >> ");
		scanf("%d", &number);
		getchar();
		root = push(root, number);
			moveText("  PreOrder >> ");
		preOrder(root);
		moveText("\n  InOrder >> ");
		inOrder(root);
		moveText("\n  PostOrder >> ");
		postOrder(root);
 	root = freeAll(root);
	return 0;
}
