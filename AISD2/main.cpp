#include <iostream>
#include <cassert>
#include "Queue.h"

void testBasicOperations() {
  std::cout << "Тест 1: Базовые операции" << std::endl;
  Queue q;

  q.push(10);
  assert(q.front() == 10);
  assert(q.back() == 10);

  q.push(20);
  assert(q.front() == 10);
  assert(q.back() == 20);

  q.push(30);
  assert(q.front() == 10);
  assert(q.back() == 30);

  // Тестируем pop
  assert(q.pop() == 10);
  assert(q.front() == 20);
  assert(q.back() == 30);

  assert(q.pop() == 20);
  assert(q.front() == 30);
  assert(q.back() == 30);

  assert(q.pop() == 30);

  std::cout << "Базовые операции прошли успешно" << std::endl;
}

void testMinOperations() {
  std::cout << "Тест 2: Операции с минимумом" << std::endl;
  Queue q;

  q.push(5);
  assert(q.currentMin() == 5);

  q.push(3);
  assert(q.currentMin() == 3);

  q.push(7);
  assert(q.currentMin() == 3);

  q.push(2);
  assert(q.currentMin() == 2);

  q.push(4);
  assert(q.currentMin() == 2);

  assert(q.pop() == 5);
  assert(q.currentMin() == 2);

  assert(q.pop() == 3);
  assert(q.currentMin() == 2);

  assert(q.pop() == 7);
  assert(q.currentMin() == 2);

  assert(q.pop() == 2);
  assert(q.currentMin() == 4);

  assert(q.pop() == 4);

  std::cout << "Операции с минимумом прошли успешно" << std::endl;
}

void testEdgeCases() {
  std::cout << "Тест 3: Граничные случаи" << std::endl;
  Queue q;

  q.push(5);
  q.push(5);
  q.push(5);
  assert(q.currentMin() == 5);

  assert(q.pop() == 5);
  assert(q.currentMin() == 5);

  assert(q.pop() == 5);
  assert(q.currentMin() == 5);

  assert(q.pop() == 5);

  q.push(-1);
  q.push(0);
  q.push(-5);
  q.push(10);

  assert(q.currentMin() == -5);
  assert(q.pop() == -1);
  assert(q.currentMin() == -5);
  assert(q.pop() == 0);
  assert(q.currentMin() == -5);
  assert(q.pop() == -5);
  assert(q.currentMin() == 10);

  std::cout << "Граничные случаи прошли успешно" << std::endl;
}

void testMixedOperations() {
  std::cout << "Тест 4: Смешанные операции" << std::endl;
  Queue q;

  q.push(10);
  q.push(5);
  assert(q.currentMin() == 5);

  q.push(8);
  assert(q.currentMin() == 5);

  assert(q.pop() == 10);
  assert(q.currentMin() == 5);

  q.push(3);
  assert(q.currentMin() == 3);

  q.push(7);
  assert(q.currentMin() == 3);

  assert(q.pop() == 5);
  assert(q.currentMin() == 3);

  assert(q.pop() == 8);
  assert(q.currentMin() == 3);

  assert(q.pop() == 3);
  assert(q.currentMin() == 7);

  std::cout << "Смешанные операции прошли успешно" << std::endl;
}

void testEmptyQueueExceptions() {
  std::cout << "Тест 5: Исключения для пустой очереди" << std::endl;
  Queue q;

  try {
    q.front();
    assert(false && "Ожидалось исключение для front()");
  } catch (const std::runtime_error& e) {
    std::cout << "front() корректно выбрасывает исключение" << std::endl;
  }

  try {
    q.back();
    assert(false && "Ожидалось исключение для back()");
  } catch (const std::runtime_error& e) {
    std::cout << "back() корректно выбрасывает исключение" << std::endl;
  }

  try {
    q.pop();
    assert(false && "Ожидалось исключение для pop()");
  } catch (const std::runtime_error& e) {
    std::cout << "pop() корректно выбрасывает исключение" << std::endl;
  }

  try {
    q.currentMin();
    assert(false && "Ожидалось исключение для currentMin()");
  } catch (const std::runtime_error& e) {
    std::cout << "currentMin() корректно выбрасывает исключение" << std::endl;
  }
}

int main() {
  std::setlocale(LC_ALL, "Russian");

  try {
    testBasicOperations();
    testMinOperations();
    testEdgeCases();
    testMixedOperations();
    testEmptyQueueExceptions();

    std::cout << "\nВсе тесты пройдены успешно!" << std::endl;
    return 0;
  } catch (const std::exception& e) {
    std::cerr << "\nОшибка: " << e.what() << std::endl;
    return 1;
  } catch (...) {
    std::cerr << "\nНеизвестная ошибка" << std::endl;
    return 1;
  }
}