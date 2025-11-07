struct ListNode {
  int val;
  ListNode* next;

  explicit ListNode(const int x) : val(x), next(nullptr) {
  }
};
// x число нод перед циклом, y число нод от начала цикла до точки встречи.
// fast и slow встретились. slow: x+y, fast: 2*(x+y). Тогда x+y == 0 (mod m)
// slow на y-ой ноде цикла, если он пройдет x и p пройдет x, то они окажутся на 0 ноде цикла.
ListNode* FindCycleStart(ListNode* head) {
  ListNode* slow = head;
  ListNode* fast = head;

  while (fast != nullptr && fast->next != nullptr) {
    slow = slow->next;
    fast = fast->next->next;
    if (slow == fast) {
      break;
    }
  }

  if (fast == nullptr || fast->next == nullptr) {
    return nullptr;
  }

  ListNode* p = head;
  while (p != slow) {
    slow = slow->next;
    p = p->next;
  }
  return slow;
}


// Инвариант: dist(fast) - dist(slow) = k,
// где k - итерация цикла, dist - сколько ListNode прошел указатель

// Почему они встретятся, если цикл есть? Пусть длина цикла равна m. Пусть fast прошел 2s, а slow - s, чтобы войти в цикл.
// Разность d = 2s - s = s. Если рассматривать по модулю m, то за m (<= n) итераций цикла d примет все значений {0; m-1}.
// А d == 0 (mod m) <=> позиция(slow) == позиция(fast) (mod m), т.е. они встретятся.

// n - число вершин в списке. n = s + m
// O(n):
// Цикла нет: тело цикла выполнится floor(n/2)
// Цикл есть: s итераций, чтобы войти в цикл и m итераций, чтобы встретится. s + m = n.

// Омега(n):
// Цикла нет: тело цикла выполнится floor(n/2)
// Цикл есть: До встречи fast пройдет все вершины как минимум один раз, т.е. все еще линейно.

// => Тета(n)
bool IsCycle(ListNode* head) {
  ListNode* slow = head;
  ListNode* fast = head;

  while (fast != nullptr && fast->next != nullptr) {
    slow = slow->next;
    fast = fast->next->next;
    if (slow == fast) {
      return true;
    }
  }
  return false;
}