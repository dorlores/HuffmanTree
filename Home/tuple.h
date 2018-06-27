
#ifndef TUPLE_H
#define TUPLE_H

class Tuple {
private:
	char key;
	int weight;
public:
	Tuple(char key, int weight) : key(key), weight(weight) {}
	
	char getKey() { return key; }
	int getWeight() { return weight; }
};

#endif // !TUPLE_H
