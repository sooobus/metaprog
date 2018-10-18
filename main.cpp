#include <iostream>
#include <vector>
#include <memory>

/* Proxy, mediator, observer
 * О задании подробнее:
Есть два класса, А и В.
У каждого из них есть свой публичный интерфейс.
Нужно написать новый шаблонный класс С, который в зависимости от шаблонного параметра будет выполнять разные роли.
1. Прокси
Обращение объектов класса А к объектам класса В должно выполняться не напрямую,
 а через аналогичный интерфейс класса-прокси.
2. Медиатор
Класс С должен иметь возможность хранить целый набор объектов класса В(или указателей на них)
 и в случае обращения от объекта класса А автоматически выбирать объект класса В и передавать ему запрос.
3. Наблюдатель
Должна появиться возможность для объектов класса А подписываться на объекты класса В.
 Тогда при обновлении объекта класса В объект С должен уведомлять всех подписчиков этого объекта-объекты класса А

 Объект С просто должен иметь возможность узнать у объекта В, было ли какое-то изменение.
Либо можно сделать, что объект В сохраняет ссылку на С, и уведомляет его об изменении какого-то свойства.
 Далее С уведомляет тех А, кто подписан на изменение этого конкретного свойства

 */

//class C; // Forward declaration
class B;

template<size_t N>
class A;

class B {
public:
    explicit B(
            size_t id_); // B must not know anything about other classes, but this id is included to demonstrate its work.


    void Foo(); // Example function that is needed to be run from A (for proxy & mediator).
    size_t GetState(); // Example state is just a bool that shows that Foo was called at least once.

private:
    bool was_called;
    size_t id;
};

template<size_t N>
class C {
public:
    explicit C() : call_index(0) { // Number of workers (objects of class B) is known before.
        b_ptrs.reserve(N);
        for (size_t i = 0; i < N; ++i)
            b_ptrs.emplace_back(new B(i));
    }

    void BFoo(); // Used as interface for calling A.Foo.

    void AddListener(std::shared_ptr<A<N>> ptr); // Adds new A that receives notifications.
    void AskBState(size_t index); // Asks specific B about its state.

private:
    std::vector<std::unique_ptr<B>> b_ptrs;
    std::vector<std::shared_ptr<A<N>>> listeners;
    size_t call_index;
};

template<size_t N>
class A {
public:
    explicit A(C<N> *multitool_) : multitool(multitool_) {} // Uses C as proxy and / or mediator.

    void CallFoo() { // Demonstrates calling B.Foo().
        multitool->BFoo();
    }

    void GetNotification(bool x); // Receives information about states that it is subscribed to.

private:
    C<N> *multitool;
};

template<size_t N>
void A<N>::GetNotification(bool x) {
    if (x) {
        std::cout << "Received state notification from B: it was already called" << std::endl;
    } else {
        std::cout << "Received state notification from B: it has not been called yet" << std::endl;
    }
}

B::B(size_t id_) : id(id_), was_called(false) {}

size_t B::GetState() {
    return was_called;
}

void B::Foo() {
    std::cout << "Yes ma'm, my number is " << id << std::endl;
    was_called = true;
}

template<size_t N>
void C<N>::AskBState(size_t index) {
    for (const auto& listener : listeners) {
        listener->GetNotification(b_ptrs[index]->GetState());
    }
}

template<size_t N>
void C<N>::BFoo() {
    b_ptrs[call_index]->Foo();
    call_index++;
    call_index %= b_ptrs.size();
}

template<size_t N>
void C<N>::AddListener(std::shared_ptr<A<N>> ptr) {
    listeners.push_back(ptr);
}

int main() {
    C<5> c;
    A<5> a1(&c);
    A<5> a2(&c);
    A<5> a3(&c);

    c.AddListener(std::make_shared<A<5>>(a1));
    c.AddListener(std::make_shared<A<5>>(a1));

    for (size_t i = 0; i < 5; ++i)
        c.AskBState(i);

    a1.CallFoo();

    for (size_t i = 0; i < 5; ++i)
        c.AskBState(i);

    a2.CallFoo();

    for (size_t i = 0; i < 5; ++i)
        c.AskBState(i);

    a3.CallFoo();
    for (size_t i = 0; i < 5; ++i)
        c.AskBState(i);

    return 0;
}
