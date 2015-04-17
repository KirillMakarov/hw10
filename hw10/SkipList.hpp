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
	TypeNode *beforeNode = findLastLessThan(key); //за log n ищем элемент после которого вставлять

	/*
	В задании не указано нужно ли сохранять устойчивость (если есть несколько элементов
	с одинаковым ключом, то последний вставленный будет в конце).

	Исходя из условия решаемой задачи нам совершенно неважно, какой элемент мы раньше добавили
	в список,
	если у двух запросов время совпадает с точностью до секунды.

	Так как порядок элементов с одинаковыми key не важен, то не будем его поддерживать.

	Гарантируется лишь то, что все элементы будут добавлены в список по порядку и элементы с одинаковыми
	key будут рядом.
	*/

	//Включим элемент в главный список (не разреженный)
	nodeForInsert -> m_next = beforeNode -> m_next;
	beforeNode -> m_next = nodeForInsert;

	//добавим элемент в разреженный список:
	double current_prob;
	int j = 0;
	while (j < numLevels)
	{
		current_prob = (double)(rand())/RAND_MAX ;
		j++;
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
				реализуем поиск сверху до этого уровня за O(logn) (log будет когда numLevel подобрано верно), 
				где n - число элементов на curLevel,
				*/
				beforeNode = m_pPreHead;
				for (int i = m_pPreHead -> m_levelHighest; i >= curLevel; i--){
					/*
					на каждом уровне проверяем выход за границы и также, чтобы следующий элемент
					был строго меньше, чем key
					*/
					while (beforeNode -> m_nextjump[i] != m_pPreHead
						&& beforeNode -> m_nextjump[i] -> m_key < key)
					{
						//переходим к следующему узлу на этом уровне
						beforeNode = beforeNode -> m_nextjump[i];
					}
					//beforeNode это последний элемент с m_key < key на текущем уровне
				}
				//beforeNode это последний элемент с m_key < key на уровне curLevel
				nodeForInsert -> m_nextjump[curLevel] = beforeNode -> m_nextjump[curLevel];
				beforeNode -> m_nextjump[curLevel] = nodeForInsert;
			}
		}
		else 
		{
			break;//Если на этот уровень не добавляем, то и выше тоже.
		}
	}
	//добавление элемента в худшем случае выполнится за O(log n * numLevels). logn - поиск позиции на текущем левеле
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
