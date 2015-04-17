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
	srand (2); //Это необходимо для того, чтобы в списке все элементы имели разную вероятность при каждом запуске
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

	//добавим элемент в разреженный список:
	int level_of_insert = -1;
	double current_prob = (double)(rand())/RAND_MAX ;

	while (current_prob < m_probability && level_of_insert < numLevels - 1)
	{
		level_of_insert++;
		current_prob = (double)(rand())/RAND_MAX;
			//level_of_insert == highLevel for nodeForInsert
	}
	nodeForInsert -> m_levelHighest = level_of_insert;

	TypeNode *tempNode = m_pPreHead;
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
		if (i <= nodeForInsert -> m_levelHighest){
			nodeForInsert -> m_nextjump[i] = tempNode-> m_nextjump[i];
			tempNode -> m_nextjump[i] = nodeForInsert;
		}
	}

	while (tempNode -> m_next != m_pPreHead && tempNode ->m_next -> m_key < key)
		tempNode = tempNode -> m_next;
	
	nodeForInsert -> m_next = tempNode -> m_next;
	tempNode -> m_next = nodeForInsert;
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

	if (node != m_pPreHead){
		TypeNode* tempNode;

		for (int i = node -> m_levelHighest; i >= 0; i--){
			tempNode = m_pPreHead;
			while (tempNode -> m_nextjump[i] != m_pPreHead
				&& tempNode -> m_nextjump[i] != node)
			{
				//переходим к следующему узлу на этом уровне
				tempNode = tempNode -> m_nextjump[i];
			}
			//tempNode это элемент перед node на текущем уровнe
			if (tempNode -> m_nextjump[i] != m_pPreHead) //если нода вообще не из этого списка
				tempNode -> m_nextjump[i] = node -> m_nextjump[i];
		}

		tempNode =m_pPreHead;
		while (tempNode -> m_next != m_pPreHead
			&& tempNode -> m_next != node)
		{
			tempNode = tempNode -> m_next;
		}
		tempNode -> m_next = node->m_next;

		delete node;
	}

}

//=============================================================================
//== End of: SkipList =========================================================
//=============================================================================
