#include <iostream>
#include <string>
using namespace std;

template<typename E>
struct Node{
	Node<E> *next;

	Node<E>(E entry) {
		this->entry = entry;
		this->next = nullptr;
	}
	E entry;
};

template<typename E>
struct Queue {
	Node<E> *front, *rear;
	Queue<E>() {
		this->front = this->rear = nullptr;
	}

	void enqueue(const E entry) {
		Node<E> *tmp = new Node<E>(entry);
		if (tmp == nullptr)
			return;

		if (this->rear == nullptr) {
			this->rear = this->front = tmp;
		}
		else {
			this->rear->next = tmp;
			this->rear = tmp;
		}
	}

	bool isEmpty() const{
		return this->front == nullptr;
	}

	bool getFront(E& val) {
		if (this->isEmpty())
			return false;

		val = this->front->entry;
		return true;
	}

	void dequeue() {
		if (this->isEmpty())
			return;

		Node<E> *tmp = this->front;
		this->front = this->front->next;

		if (this->front == nullptr)
			this->rear = nullptr;

		delete tmp;
	}

	bool includes(const E& val) const {
		Node<E> *tmp = this->front;
		bool found = false;
		while (tmp != nullptr && !found) {
			if (tmp->entry == val)
				found = true;
			tmp = tmp->next;
		}
		return found;
	}
};

struct FrogState {
	string configuration, message;
	unsigned int emptyIndex;
	FrogState *parent;

	FrogState(int stateSize = 7) {
		this->parent = nullptr;
		this->configuration = string(stateSize/2,'B') + '_' + string(stateSize/2,'G');
		this->emptyIndex = this->configuration.size() / 2;
	}

	FrogState(string configuration, FrogState *parent = nullptr, int emptyIndex = -1) {
		this->parent = parent;
		this->configuration = configuration;
		this->emptyIndex = emptyIndex == -1 ? configuration.size() / 2 : emptyIndex;
	}

	bool generateBrown(FrogState& brownChild, FrogState& parent) {
		string newConfiguration = this->configuration;
		string newMessage;
		int index = this->emptyIndex;
		bool hasMadeMove = false;

		if (this->emptyIndex > 0 && this->configuration[emptyIndex - 1] == 'B') {
			newMessage = "Brown frog on place " + to_string(emptyIndex) + " moves right.";
			newConfiguration[index--] = 'B';
			newConfiguration[index] = '_';
			hasMadeMove = true;
		}
		else if (this->emptyIndex > 1 && this->configuration[emptyIndex - 1] == 'G' && this->configuration[emptyIndex - 2] == 'B') {
			newMessage = "Brown frog on place " + to_string(emptyIndex - 1) + " jumps right.";
			newConfiguration[index--] = 'B';
			newConfiguration[--index] = '_';
			hasMadeMove = true;
		}

		if (hasMadeMove) {
			brownChild.configuration = newConfiguration;
			brownChild.message = newMessage;
			brownChild.parent = &parent;
			brownChild.emptyIndex = index;
			return true;
		}
		return false;
	}

	bool generateGreen(FrogState& greenChild,FrogState &parent) {
		string newConfiguration = this->configuration;
		string newMessage;
		int index = this->emptyIndex;
		bool hasMadeMove = false;

		if (this->emptyIndex < this->configuration.size() - 1 && this->configuration[this->emptyIndex + 1] == 'G') {
			newMessage = "Green frog on place " + to_string(emptyIndex + 2) + " moves left.";
			newConfiguration[index++] = 'G';
			newConfiguration[index] = '_';
			hasMadeMove = true;
		}
		else if (this->emptyIndex < this->configuration.size() - 2 && this->configuration[this->emptyIndex + 1] == 'B' && this->configuration[this->emptyIndex + 2] == 'G') {
			newMessage = "Green frog on place " + to_string(emptyIndex + 3) + " jumps left.";
			newConfiguration[index++] = 'G';
			newConfiguration[++index] = '_';
			hasMadeMove = true;
		}
		
		if (hasMadeMove){
			greenChild.configuration = newConfiguration;
			greenChild.message = newMessage;
			greenChild.parent = &parent;
			greenChild.emptyIndex = index;
			return true;
		}
		return false;
	}

	friend bool operator==(const FrogState& lhs, const FrogState& rhs) {
		return lhs.configuration == rhs.configuration;
	}
};

FrogState BFS(int stateSize) {
	string goalState = string(stateSize / 2, 'G') + '_' + string(stateSize / 2, 'B');
	FrogState start(stateSize);
	FrogState goal(goalState);

	Queue<FrogState> open, closed;
	open.enqueue(start);

	while (!open.isEmpty()) {
		FrogState *current = new FrogState();
		open.getFront(*current);

		open.dequeue();

		if (current->configuration == goal.configuration) {
			return *current;
		}
		else {
			closed.enqueue(*current);

			FrogState brownChild, greenChild;
			bool brownSuccess = current->generateBrown(brownChild, *current);
			bool greenSuccess = current->generateGreen(greenChild, *current);
			
			if (!(open.includes(brownChild) || closed.includes(brownChild) && brownSuccess))
				open.enqueue(brownChild);
			
			if (!(open.includes(greenChild) || closed.includes(greenChild) && greenSuccess))
				open.enqueue(greenChild);
		}
	}
	return start;
}

void displaySolution(FrogState *solution) {
	if (solution->parent != nullptr) {
		displaySolution(solution->parent);
		cout << solution->message << "\tState : " << solution->configuration<<endl;
	}
	else {
		cout << "Start state : " << solution->configuration << endl << endl;
		cout << "Jumps as follows :" << endl;
	}
}

int main() {
	
	int stateSize;
	cout << "Welcome to frog game solver using BFS. For larger configurations( > 17 ) please find a better algorithm!\n";
	cout << "Enter state size (odd number) : ";
	cin >> stateSize;
	
	if (stateSize & 0)
		stateSize++;
		
	FrogState solution = BFS(stateSize);
	displaySolution(&solution);

	cin.get(); cin.get();
	return 0;
}
