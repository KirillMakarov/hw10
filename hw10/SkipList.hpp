#include <cstdlib>
//Kirill Makarov
//=============================================================================
//== NodeSkipList =============================================================
//=============================================================================

template <class Value, class Key, int numLevels>
void NodeSkipList<Value,Key,numLevels>::clear(void)
{
	for (int i = 0; i < numLevels; ++i)
	{
		m_nextjump[i] = 0;
	}
	m_levelHighest = -1;
}

template <class Value, class Key, int numLevels>
NodeSkipList<Value, Key, numLevels>::NodeSkipList(void)
{
	clear();
}

template <class Value, class Key, int numLevels>
NodeSkipList<Value, Key, numLevels>::NodeSkipList(Key key)
{
	clear();

	m_key = key;
}

template <class Value, class Key, int numLevels>
NodeSkipList<Value, Key, numLevels>::NodeSkipList(Key key, Value value)
{
	clear();

	m_key = key;
	m_value = value;
}
//=============================================================================
//== End of: NodeSkipList =====================================================
//=============================================================================

//=============================================================================
//== SkipList =================================================================
//=============================================================================
template <class Value, class Key, int numLevels>
SkipList<Value, Key, numLevels>::SkipList(double probability)
{
	srand (time(NULL)); //Это необходимо для того, чтобы в списке все элементы имели разную вероятность при каждом запуске
	m_probability = probability;
	for (int i = 0; i < numLevels; ++i)
	{
		// Lets use m_pPreHead as a final sentinel element
		m_pPreHead->m_nextjump[i] = m_pPreHead;
	}
	m_pPreHead->m_levelHighest = numLevels-1;
}

// Put your code here

template <class Value, class Key, int numLevels>
void SkipList<Value, Key, numLevels> :: insert(Value value, Key key){
	TypeNode *nodeForInsert = new TypeNode(key, value);
	TypeNode *beforeNode = findLastLessThan(key); //за log n ищем элемент после которого вставлять

	//Включим элемент в главный список (не разреженный)
	nodeForInsert -> m_next = beforeNode -> m_next;
	beforeNode -> m_next = nodeForInsert;

	//добавим элемент в разреженный список:
	double current_prob;
	int i = 0;
	while (i < numLevels)
	{
		current_prob = (double)(rand())/RAND_MAX ;
		i++;
		if (current_prob < m_probability)
		{
			(nodeForInsert -> m_levelHighest)++;
			int curLevel = nodeForInsert ->m_levelHighest;

			//добавлением одной проверки убираем ненужные пробежки с самого начала.
			if (beforeNode -> m_levelHighest >= curLevel)
			{
				nodeForInsert -> m_nextjump[curLevel] = beforeNode -> m_nextjump[curLevel];
				beforeNode -> m_nextjump[curLevel] = nodeForInsert;
			}
			else 
			{
				/*
					обновляем элемент, который стоит до.
					здесь можно также реализовать поиск сверху до этого уровня за O(logn), 
					где n - число элементов на curLevel,
					но в задании не требуется, чтобы insert работал за O(logn). 
				*/
				beforeNode = m_pPreHead;
				while (beforeNode -> m_nextjump[curLevel] != m_pPreHead
					&& beforeNode -> m_nextjump[curLevel] -> m_key < key)
				{
					//переходим к следующему узлу на этом уровне
					beforeNode = beforeNode -> m_nextjump[curLevel];
				}
				//вставляем после tempNode
				nodeForInsert -> m_nextjump[curLevel] = beforeNode -> m_nextjump[curLevel];
				beforeNode -> m_nextjump[curLevel] = nodeForInsert;
			}
		}
		else 
		{
			break;//Если на этот уровень не добавляем, то и выше тоже.
		}
	}
}


template <class Value, class Key, int numLevels>
NodeSkipList<Value,Key, numLevels> * SkipList<Value,Key,numLevels>::findLastLessThan(Key key) const
{
	TypeNode *tempNode = m_pPreHead;
	/*
	Так как по заданию numLevels -- это количество разреженных уровне,
	то на уровне m_nextjump[0] -- могут отказаться не все узлы из первоначального списка
	и существует еще более низкий уровень, где обращение идет по m_next
	Начнем искать элемент с самого верхнего уровня, а затем еще и проверим на уровне, где есть все элементы

	m_pPreHead := Sentinel element - placed before first and after last elements
	Значит, если мы дошли до этого элемента, то мы уже вышли за пределы списка на этом уровне.
	*/
	for (int i = m_pPreHead -> m_levelHighest; i >= 0; i--){
		/*
		на каждом уровне проверяем выход за границы и также, чтобы следующий элемент
		был строго меньше, чем key
		*/
		while (tempNode -> m_nextjump[i] != m_pPreHead
			&& tempNode -> m_nextjump[i] -> m_key < key)
		{
			//переходим к следующему узлу на этом уровне
			tempNode = tempNode -> m_nextjump[i];
		}
		//tempNode это последний элемент с m_key < key на текущем уровне
	}

	//доделываем поиск на заданном (не разреженный) связном списке.
	while (tempNode -> m_next != m_pPreHead && tempNode ->m_next -> m_key < key)
	{
		tempNode = tempNode -> m_next;
	}

	return tempNode;
}



template <class Value, class Key, int numLevels>
NodeSkipList<Value,Key, numLevels> * SkipList<Value,Key,numLevels>::findFirst(Key key) const{
	/*
	Если элементов с одинаковыми ключами несколько, и допустим, что
	первый лежит на самом низком уровне, а другие элементы на уровнях выше,
	то чтобы не "проскочить" первый элемент, придется искать элементы строго меньшие по ключу,
	а затем в полном списке начать искать элемент равный по ключу с найденного элемента со строго
	меньшим ключом.

	Так что воспользуемся методом findLastLessThan, который вернет элемент перед ключевым, если элемент
	с заданным ключом существует в списке. Если же такого элемента не существует, то ключ следующего элемента
	будет отключаться от значения key или следующий будет равен m_pPreHead.
	*/
	TypeNode *tempNode = findLastLessThan(key);
	tempNode = tempNode -> m_next;
	if (tempNode != m_pPreHead && tempNode -> m_key  == key)
		return tempNode;
	else
		return nullptr;
}


template <class Value, class Key, int numLevels>
void SkipList<Value,Key,numLevels>:: remove(NodeSkipList<Value,Key, numLevels> * node) {
	if (node != m_pPreHead)
	{
		delete [] *(node -> m_nextjump);
		delete node;
	}
}

//=============================================================================
//== End of: SkipList =========================================================
//=============================================================================
