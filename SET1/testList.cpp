#include <iostream>
#include <vector>
#include <cassert>
#include "list.h"

// Ваш код List здесь...

class ListTest {
public:
  static void runAllTests() {
    std::cout << "Запуск всех тестов..." << std::endl;

    testDefaultConstructor();
    testVectorConstructor();
    testCopyConstructor();
    testPushFront();
    testPushBack();
    testPopFront();
    testPopBack();
    testFrontBack();
    testInsert();
    testErase();
    testClear();
    testReverse();
    testRemoveDuplicates();
    testReplace();
    testCopy();
    testMerge();
    testCheckCycle();
    testSizeEmpty();
    testEdgeCases();

    std::cout << "Все тесты пройдены успешно!" << std::endl;
  }

private:
  static void testDefaultConstructor() {
    std::cout << "Тест конструктора по умолчанию... ";
    List list;
    assert(list.size() == 0);
    assert(list.empty());
    assert(list.head == nullptr);
    assert(list.tail == nullptr);
    std::cout << "OK" << std::endl;
  }

  static void testVectorConstructor() {
    std::cout << "Тест конструктора из вектора... ";

    // Пустой вектор
    List list1(std::vector<int>{});
    assert(list1.size() == 0);
    assert(list1.empty());

    // Один элемент
    List list2(std::vector<int>{42});
    assert(list2.size() == 1);
    assert(!list2.empty());
    assert(list2.head->value == 42);
    assert(list2.tail->value == 42);

    // Несколько элементов
    List list3(std::vector<int>{1, 2, 3, 4, 5});
    assert(list3.size() == 5);
    assert(list3.head->value == 1);
    assert(list3.tail->value == 5);

    std::cout << "OK" << std::endl;
  }

  static void testCopyConstructor() {
    std::cout << "Тест конструктора копирования... ";

    // Копирование пустого списка
    List emptyList;
    List copyEmpty(emptyList);
    assert(copyEmpty.size() == 0);
    assert(copyEmpty.empty());

    // Копирование непустого списка
    List original(std::vector<int>{1, 2, 3});
    List copy(original);
    assert(copy.size() == 3);
    assert(copy.head->value == 1);
    assert(copy.tail->value == 3);

    // Проверка, что это глубокое копирование
    original.push_front(0);
    assert(original.size() == 4);
    assert(copy.size() == 3); // Копия не должна измениться

    std::cout << "OK" << std::endl;
  }

  static void testPushFront() {
    std::cout << "Тест push_front... ";
    List list;

    // Добавление в пустой список
    list.push_front(1);
    assert(list.size() == 1);
    assert(list.head->value == 1);
    assert(list.tail->value == 1);

    // Добавление нескольких элементов
    list.push_front(2);
    list.push_front(3);
    assert(list.size() == 3);
    assert(list.head->value == 3);
    assert(list.head->next->value == 2);
    assert(list.tail->value == 1);

    std::cout << "OK" << std::endl;
  }

  static void testPushBack() {
    std::cout << "Тест push_back... ";
    List list;

    // Добавление в пустой список
    list.push_back(1);
    assert(list.size() == 1);
    assert(list.head->value == 1);
    assert(list.tail->value == 1);

    // Добавление нескольких элементов
    list.push_back(2);
    list.push_back(3);
    assert(list.size() == 3);
    assert(list.head->value == 1);
    assert(list.head->next->value == 2);
    assert(list.tail->value == 3);

    std::cout << "OK" << std::endl;
  }

  static void testPopFront() {
    std::cout << "Тест pop_front... ";
    List list(std::vector<int>{1, 2, 3});

    // Удаление из списка с несколькими элементами
    list.pop_front();
    assert(list.size() == 2);
    assert(list.head->value == 2);
    assert(list.tail->value == 3);

    // Удаление до одного элемента
    list.pop_front();
    assert(list.size() == 1);
    assert(list.head->value == 3);
    assert(list.tail->value == 3);

    // Удаление последнего элемента
    list.pop_front();
    assert(list.size() == 0);
    assert(list.empty());
    assert(list.head == nullptr);
    assert(list.tail == nullptr);

    std::cout << "OK" << std::endl;
  }

  static void testPopBack() {
    std::cout << "Тест pop_back... ";
    List list(std::vector<int>{1, 2, 3});

    // Удаление из списка с несколькими элементами
    list.pop_back();
    assert(list.size() == 2);
    assert(list.head->value == 1);
    assert(list.tail->value == 2);

    // Удаление до одного элемента
    list.pop_back();
    assert(list.size() == 1);
    assert(list.head->value == 1);
    assert(list.tail->value == 1);

    // Удаление последнего элемента
    list.pop_back();
    assert(list.size() == 0);
    assert(list.empty());
    assert(list.head == nullptr);
    assert(list.tail == nullptr);

    std::cout << "OK" << std::endl;
  }

  static void testFrontBack() {
    std::cout << "Тест front/back... ";
    List list(std::vector<int>{10, 20, 30});

    assert(list.front() == 10);
    assert(list.back() == 30);

    // После изменений
    list.push_front(5);
    list.push_back(40);
    assert(list.front() == 5);
    assert(list.back() == 40);

    std::cout << "OK" << std::endl;
  }

  static void testInsert() {
    std::cout << "Тест insert... ";
    List list(std::vector<int>{1, 3, 4});

    // Вставка в начало
    list.insert(list.head, 0);
    assert(list.size() == 4);
    assert(list.head->next->value == 0);

    // Вставка в середину
    list.insert(list.head->next->next, 2); // Между 1 и 3
    assert(list.size() == 5);

    // Вставка в конец (через tail)
    list.insert(list.tail, 5);
    assert(list.size() == 6);
    assert(list.tail->value == 5);

    std::cout << "OK" << std::endl;
  }

  static void testErase() {
    std::cout << "Тест erase... ";
    List list(std::vector<int>{1, 2, 3, 4, 5});

    // Удаление из середины
    list.erase(list.head->next); // Удаляем 2
    assert(list.size() == 4);
    assert(list.head->next->value == 3);

    // Удаление головы
    list.erase(list.head); // Удаляем 1
    assert(list.size() == 3);
    assert(list.head->value == 3);

    // Удаление хвоста
    list.erase(list.tail); // Удаляем 5
    assert(list.size() == 2);
    assert(list.tail->value == 4);

    // Удаление единственного элемента
    List single(std::vector<int>{42});
    single.erase(single.head);
    assert(single.size() == 0);
    assert(single.empty());

    std::cout << "OK" << std::endl;
  }

  static void testClear() {
    std::cout << "Тест clear... ";
    List list(std::vector<int>{1, 2, 3, 4, 5});

    list.clear();
    assert(list.size() == 0);
    assert(list.empty());
    assert(list.head == nullptr);
    assert(list.tail == nullptr);

    // Очистка пустого списка
    List empty;
    empty.clear();
    assert(empty.size() == 0);

    std::cout << "OK" << std::endl;
  }

  static void testReverse() {
    std::cout << "Тест reverse... ";

    // Пустой список
    List empty;
    empty.reverse();
    assert(empty.empty());

    // Один элемент
    List single(std::vector<int>{42});
    single.reverse();
    assert(single.head->value == 42);
    assert(single.tail->value == 42);

    // Несколько элементов
    List list(std::vector<int>{1, 2, 3, 4, 5});
    list.reverse();
    assert(list.head->value == 5);
    assert(list.tail->value == 1);
    assert(list.head->next->value == 4);
    assert(list.head->next->next->value == 3);

    std::cout << "OK" << std::endl;
  }

  static void testRemoveDuplicates() {
    std::cout << "Тест remove_duplicates... ";

    // Без дубликатов
    List list1(std::vector<int>{1, 2, 3});
    list1.remove_duplicates();
    assert(list1.size() == 3);

    // С дубликатами
    List list2(std::vector<int>{1, 2, 2, 3, 3, 3, 4});
    list2.remove_duplicates();
    assert(list2.size() == 4);
    assert(list2.head->value == 1);
    assert(list2.head->next->value == 2);
    assert(list2.head->next->next->value == 3);
    assert(list2.tail->value == 4);

    // Все элементы одинаковые
    List list3(std::vector<int>{5, 5, 5, 5});
    list3.remove_duplicates();
    assert(list3.size() == 1);
    assert(list3.head->value == 5);

    std::cout << "OK" << std::endl;
  }

  static void testReplace() {
    std::cout << "Тест replace... ";
    List list(std::vector<int>{1, 2, 3, 2, 4, 2});

    // Замена существующих значений
    list.replace(2, 99);
    assert(list.head->next->value == 99); // Бывшая 2
    assert(list.head->next->next->next->value == 99); // Бывшая 2
    assert(list.tail->value == 99); // Бывшая 2

    // Замена несуществующего значения
    list.replace(100, 200);
    assert(list.size() == 6); // Размер не должен измениться

    std::cout << "OK" << std::endl;
  }

  static void testCopy() {
    std::cout << "Тест copy... ";
    List original(std::vector<int>{1, 2, 3});
    List copy;

    copy.copy(original);
    assert(copy.size() == 3);
    assert(copy.head->value == 1);
    assert(copy.tail->value == 3);

    // Копирование в непустой список (должно очистить сначала)
    List existing(std::vector<int>{10, 20});
    existing.copy(original);
    assert(existing.size() == 3);
    assert(existing.head->value == 1);

    // Копирование пустого списка
    List empty;
    List target(std::vector<int>{1, 2});
    target.copy(empty);
    assert(target.empty());

    std::cout << "OK" << std::endl;
  }

  static void testMerge() {
    std::cout << "Тест merge... ";
    List list1(std::vector<int>{1, 2, 3});
    List list2(std::vector<int>{4, 5, 6});

    list1.merge(list2);
    assert(list1.size() == 6);

    // Проверка порядка элементов
    Node* current = list1.head;
    for (int i = 1; i <= 6; i++) {
      assert(current->value == i);
      current = current->next;
    }

    // Слияние с пустым списком
    List nonEmpty(std::vector<int>{1, 2});
    List empty;
    nonEmpty.merge(empty);
    assert(nonEmpty.size() == 2);

    std::cout << "OK" << std::endl;
  }

  static void testCheckCycle() {
    std::cout << "Тест check_cycle... ";

    // Обычный список без цикла
    List normal(std::vector<int>{1, 2, 3});
    assert(!normal.check_cycle());

    // Пустой список
    List empty;
    assert(!empty.check_cycle());

    // Один элемент
    List single(std::vector<int>{42});
    assert(!single.check_cycle());

    // Создаем цикл вручную для тестирования
    List cyclic;
    cyclic.push_back(1);
    cyclic.push_back(2);
    cyclic.push_back(3);
    cyclic.tail->next = cyclic.head; // Создаем цикл

    assert(cyclic.check_cycle());

    std::cout << "OK" << std::endl;
  }

  static void testSizeEmpty() {
    std::cout << "Тест size/empty... ";
    List list;

    assert(list.size() == 0);
    assert(list.empty());

    list.push_back(1);
    assert(list.size() == 1);
    assert(!list.empty());

    list.push_front(2);
    assert(list.size() == 2);
    assert(!list.empty());

    list.clear();
    assert(list.size() == 0);
    assert(list.empty());

    std::cout << "OK" << std::endl;
  }

  static void testEdgeCases() {
    std::cout << "Тест граничных случаев... ";

    // Многократное добавление/удаление
    List list;
    for (int i = 0; i < 1000; i++) {
      list.push_back(i);
    }
    assert(list.size() == 1000);

    for (int i = 0; i < 1000; i++) {
      list.pop_front();
    }
    assert(list.empty());

    // Большие значения
    list.push_front(INT_MAX);
    list.push_back(INT_MIN);
    assert(list.front() == INT_MAX);
    assert(list.back() == INT_MIN);

    // Отрицательные значения
    List negatives(std::vector<int>{-1, -2, -3});
    assert(negatives.size() == 3);
    assert(negatives.front() == -1);
    assert(negatives.back() == -3);

    std::cout << "OK" << std::endl;
  }
};

int main() {
  std::setlocale(LC_ALL, "Russian");
  try {
    ListTest::runAllTests();
    std::cout << "\n=== ВСЕ ТЕСТЫ ПРОЙДЕНЫ УСПЕШНО ===" << std::endl;
    return 0;
  } catch (const std::exception& e) {
    std::cerr << "Тест провален: " << e.what() << std::endl;
    return 1;
  }
}