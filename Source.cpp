#include <iostream>
using namespace std;

enum struct Status{
	SUCCESS, FAILURE
};

template<typename E>
struct QueueNode{
	QueueNode<E> *next;
	QueueNode<E> *parent;

	QueueNode<E>(E entry) {
		this->entry = entry;
		this->next = nullptr;
	}
	E entry;
};

template<typename E>
struct Queue {
	QueueNode<E> *front, *rear;
	Queue<E>() {
		this->front = this->rear = nullptr;
	}

	Status enqueue(const E entry) {
		QueueNode<E> *tmp = new QueueNode<E>(entry);
		if (tmp == nullptr)
			return Status::FAILURE;

		if (this->rear == nullptr) {
			this->rear = this->front = tmp;
		}
		else {
			this->rear->next = tmp;
			this->rear = tmp;
		}
		return Status::SUCCESS;
	}

	bool isEmpty() const{
		return this->front == nullptr;
	}

	Status getFront(E& val) {
		if (this->isEmpty())
			return Status::FAILURE;
		
		val = this->front->entry;
		return Status::SUCCESS;
	}

	Status dequeue() {
		if (this->isEmpty())
			return Status::FAILURE;

		QueueNode<E> *tmp = this->front;
		this->front = this->front->next;

		if (this->front == nullptr)
			this->rear = nullptr;

		delete tmp;
		return Status::SUCCESS;
	}

	bool includes(const E& val) const {
		QueueNode<E> *tmp = this->front;
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
	char configuration[8];
	int size, emptyIndex;
	FrogState *parent;

	FrogState() {
		this->parent = nullptr;
		this->size = 8;
		strcpy_s(configuration, this->size, "BBB_GGG");
		this->emptyIndex = this->size / 2 - 1;
	}

	FrogState* generateBrown(FrogState parent) {
		char newConfiguration[8];
		int index = this->emptyIndex;
		bool hasMadeMove = false;
		strcpy_s(newConfiguration, this->size, this->configuration);
		

		if (this->emptyIndex > 0 && this->configuration[emptyIndex - 1] == 'B') {
			newConfiguration[index--] = 'B';
			newConfiguration[index] = '_';
			hasMadeMove = true;
		}
		else if (this->emptyIndex > 1 && this->configuration[emptyIndex - 1] == 'G' && this->configuration[emptyIndex - 2] == 'B') {
			newConfiguration[index--] = 'B';
			newConfiguration[--index] = '_';
			hasMadeMove = true;
		}

		if (hasMadeMove) {
			FrogState *brownChild = new FrogState();
			strcpy_s(brownChild->configuration, this->size, newConfiguration);
			brownChild->emptyIndex = index;
			brownChild->parent = &parent;
			return brownChild;
		}
		else {
			return nullptr;
		}
	}

	FrogState* generateGreen(FrogState parent) {
		char newConfiguration[8];
		int index = this->emptyIndex;
		bool hasMadeMove = false;

		strcpy_s(newConfiguration, this->size, this->configuration);
		
		if (this->emptyIndex < this->size - 1 && this->configuration[this->emptyIndex + 1] == 'G') {
			newConfiguration[index++] = 'G';
			newConfiguration[index] = '_';
			hasMadeMove = true;
		}
		else if (this->emptyIndex < this->size - 2 && this->configuration[this->emptyIndex + 1] == 'B' && this->configuration[this->emptyIndex + 2] == 'G') {
			newConfiguration[index++] = 'G';
			newConfiguration[++index] = '_';
			hasMadeMove = true;
		}
		
		if (hasMadeMove) {
			FrogState *greenChild = new FrogState();
			strcpy_s(greenChild->configuration, this->size, newConfiguration);
			greenChild->emptyIndex = index;
			greenChild->parent = &parent;
			return greenChild;
		}
		else {
			return nullptr;
		}
	}

	friend bool operator==(const FrogState& lhs, const FrogState& rhs) { 
		for (int i = 0; i < lhs.size; i++) {
			if (lhs.configuration[i] != rhs.configuration[i])
				return false;
		}
		return true;
	}

	friend ostream& operator<<(ostream& out, const FrogState *state) {
		if (state == nullptr)
			return out << "\n";
		else
			return out << state->configuration;
	}

	void operator=(const FrogState* other) {
		strcpy_s(this->configuration, other->size, other->configuration);
		this->parent = other->parent;
		this->size = other->size;
		this->emptyIndex = other->emptyIndex;
	}
};

Status BFS(FrogState& solution) {
	char goalState[] = "GGG_BBB";
	FrogState *x = &solution;
	FrogState *goal = new FrogState();
	strcpy_s(goal->configuration, 8, goalState);

	Queue<FrogState> open, closed;
	open.enqueue(*x);

	while (!open.isEmpty() && x != nullptr) {
		open.getFront(*x);
		cout << "Checking : " << x << "  Parent : ";
		
		if (x->parent == nullptr)
			cout << "ROOT";
		else
			cout << x->parent;
		cout << endl;
		
		open.dequeue();

		if (x == goal) {
			solution = x;
			return Status::SUCCESS;
		}
		else {
			closed.enqueue(*x);
			FrogState *brownChild = new FrogState();
			brownChild = x->generateBrown(*x);
			FrogState *greenChild = new FrogState();
			greenChild = x->generateGreen(*x);

			if (!(open.includes(*brownChild) || closed.includes(*brownChild) || brownChild == nullptr));
			{
				open.enqueue(*brownChild);
			}
			if (!(open.includes(*greenChild) || closed.includes(*greenChild) || greenChild == nullptr));
			{
				open.enqueue(*greenChild);
			}
		}
	}
	return Status::FAILURE;
}

int main() {
	
	FrogState solution;
	BFS(solution);
	cout << "SOLUTION\n\n";
	cout << &solution;
	cin.get(); cin.get();
	return 0;
}

