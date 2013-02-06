//written by Daniel Griffin
//This program takes in data in the form of an ID number assosiated
//with a name. It then takes in parental data based off 3 ID numbers
//The first ID is the child, then the two parents
//It then calculates and outputs people with the longest family tree
//Finally it outputs the oldest ansestor

#include <iostream>
using namespace std;

const int UNKNOWN = 000;	//so that it is flexible
const int SENTINEL = -1;	//the sentinels are set

struct Person{
	int ID;
	Person *mother;
	Person *father;
	int length;		//stored individually for efficency
	string name;
	Person(){		//sets initial conditions
		length = SENTINEL;
		mother = NULL;
		father = NULL;
	}
};

struct node{			//a node to build lists
	Person *data;
	node *next;
	node(){
		next = NULL;
	}
};


node *input();
node *append(node*);
node *input_nodes(node*);
void input_data(node*);
Person *find_ID_address(node*,int);
void debug_output(node*);
node *longest(node*);
int length(Person*);
int find_max(node*);
node *max_list(node*,int); 
void length_pass(node*);
void output(node*);
void list_max(node*);
void delete_list(node*);
//void oldest_ouput(node*); if time allowed these two functions would have
//node *oldest(node*);       found the ansestors


int main(){	//as short as possible
	node *list = input();	//gathers inputs
	node *longest_list = longest(list); //gets list of longest children
	output(longest_list);	//outputs the list
	delete_list(list);	//deletes lists
	delete_list(longest_list);
	return 0;
}

//adds a new data storage point on the front of the list
node *append(node *list){
	node *next_node = new node; //creates new node
	next_node->data = new Person; //creates a new person for node
	next_node->next = list; //adds the old list to next
	return next_node;	//returns the new list
}

//gathers all ID numbers and associated names
//the input is for recursive properties
node *input_nodes(node *list){
	int current_ID;
	string current_name;
	while(true){
		cin >> current_ID;
		if (current_ID == SENTINEL){
			break;
		}
		getline(cin, current_name);
		list = append(list);		//creates new storage point
		list->data->ID = current_ID;	//stores data in struct
		list->data->name = current_name;
	}
	return list;	//returns the completed list
}

//runs all input opperations, creates original list
node *input(){
	node *list = input_nodes(NULL);//NULL because list created in input
	input_data(list);	//gathers data
	return list;
}

//finds the address of the a person with a specific id number
Person *find_ID_address(node *list,int id){
	node *hold_node = list; //prevents list from being modified
	if(id == UNKNOWN){	//if no parents, return NULL
		return NULL;
	}
	while(true){	//otherwise
		if(hold_node == NULL){ //if NULL done
			break;
		}
		if(hold_node->data->ID == id){ //if ID match return address
			return hold_node->data;
		}
		hold_node = hold_node->next; //cycle through list
	}
	return NULL; //will only reach if cycled through with no success
}

//gathers the information on parents, identifies address
//will fill in the struct data within the linked list
void input_data(node *list){
	int child,father,mother; //stores id numbers
	Person *Kid;		//stores person struct
	while( true ){
		cin >> child >> father >> mother;
		Kid = find_ID_address(list,child); //finds person struct
		Kid->mother = find_ID_address(list,mother);//struct info added
		Kid->father = find_ID_address(list,father);//struct info added

		if (cin.eof()){ //break at end of file
			break;
		}
	}
}

//performs all of the longest calculations for main
node *longest(node *list){
	length_pass(list);	//fills in all length values
	int max = find_max(list);//finds the longest length
	node *maxlist = max_list(list,max); //makes list of all longest people
	return maxlist;//returns this list
}

//cycles through list to find length of each person
void length_pass(node *list){
	node *list_holder = list; //prevents modification of list
	while(list_holder !=NULL){//go through entire list
		length(list_holder->data);//finds each length
		list_holder = list_holder->next;//cycles
	}
	
}

//if given a person, will find the length of their line and fill
//in the corresponding struct member variable
int length(Person *list){
	int len = 0; //length
	int lMother, lFather; //length mother, length father
	if(list == NULL){	//if null return length
		return len;
	}
	if(list->length == SENTINEL){	//if length is the non-value
		lMother = length(list->mother);//find mother length
		lFather = length(list->father);//find father length
		if(lMother > lFather){ //set length to the longer one
			len += lMother;
		}
		else{
			len += lFather;
		}
		len++;	//adds one to include self
		list->length = len;//sets length to the length
		return len;	//returns length for recursion
	}
	else{
		return list->length;//if length known just return length
				//helps to improve efficiency
	}
}

//this function was merely to debug, it was used to check input
void debug_output(node *list){
	if(list != NULL){	//out the list and length and parents
		cout << endl << list->data->name;
		cout <<  " length: " << list->data->length << endl;
		if(list->data->mother != NULL){
			cout << "mother: " << list->data->mother->name 
			     << endl;
		}
		if(list->data->father != NULL){
			cout << "father: " << list->data->father->name 
			     << endl;
		}
		debug_output(list->next);//go through list
	}
}

//finds the maximum length of all people in list
int find_max(node *list){
	node *node_holder = list;	//prevents modification of list
	int max = 1;
	while(node_holder != NULL){		//goes through whole list
		if(node_holder->data->length > max){//compares all lengths	
			max = node_holder->data->length;	
		}
		node_holder = node_holder->next; //cycles through
	}
	return max;//returns the largest through
}

//makes a list of all people with maximum length
node *max_list(node *list,int max){
	node *node_holder = list;//protects the list
	node *maxlist = NULL;	//starts list as NULL
	while(node_holder != NULL){	//goes through entire list
		if(node_holder->data->length == max){	//if max length
			maxlist = append(maxlist); 	//create new node
			maxlist->data = node_holder->data; //add the person
		}
		node_holder = node_holder->next; //cycle through
	}
	return maxlist;//return the created list
}

//controls all output, could also include calculating ansestor
void output(node *list){
	list_max(list);
}

//output the list of maximum length ansestories
void list_max(node *list){
	if(list != NULL){	//cycles through entire list
		cout << "length: " << list->data->length //outputs length
                << " ID: " << list->data->ID //outputs ID
                << " name: " << list->data->name <<endl; //outputs name
		list_max(list->next); //cycles through recursively
	}
}

//deletes an entire list from the heap, including the person inside the node
void delete_list(node *list){
	node *holder;	//used to hold the array while deleting first item
	while(list != NULL){
		if(list->data != NULL){//if there is a person
			delete list->data;//delete it
		}
		holder = list->next;//hold the rest of the list
		delete list; //delete the head
		list = holder; //set the list to the rest of the list
	}
}

